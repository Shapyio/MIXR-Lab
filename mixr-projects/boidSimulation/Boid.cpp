#include "Boid.hpp"

#include "mixr/base/units/Angles.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/colors/Color.hpp"

// Boids call its parent (page) to recognize other objects nearby (TODO: Replace with controller)
#include "mixr/graphics/Page.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"

#include <GL/glut.h>
#include <cmath>

// PI global variable
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

IMPLEMENT_SUBCLASS(Boid, "Boid")
EMPTY_DELETEDATA(Boid)

// --- Slottable ---
BEGIN_SLOTTABLE(Boid)
"speed",                  // 1
"position",               // 2
"startAngle",             // 3
"separation",             // 4
"alignment",              // 5
"cohesion",               // 6
"separationMinDist",      // 7
"color",                  // 8
"detectionRadius",        // 9
END_SLOTTABLE(Boid)

BEGIN_SLOT_MAP(Boid)
  ON_SLOT(1, setSlotSpeed, mixr::base::Number)
  ON_SLOT(2, setSlotPosition, mixr::base::List)
  ON_SLOT(3, setSlotAngle, mixr::base::Angle)
  ON_SLOT(3, setSlotAngle, mixr::base::Number)
  ON_SLOT(4, setSlotSeparationWeight, mixr::base::Number)
  ON_SLOT(5, setSlotAlignmentWeight, mixr::base::Number)
  ON_SLOT(6, setSlotCohesionWeight, mixr::base::Number)
  ON_SLOT(7, setSlotSeparationMinDistance, mixr::base::Number)
  ON_SLOT(8, setSlotColor, mixr::base::Color)
  ON_SLOT(9, setSlotDetectionRadius, mixr::base::Number)
END_SLOT_MAP()

// --- Events ---
BEGIN_EVENT_HANDLER(Boid)
END_EVENT_HANDLER()

// Constructor
Boid::Boid()
{
  STANDARD_CONSTRUCTOR()
}

void Boid::copyData(const Boid& org, const bool)
{
  BaseClass::copyData(org);

  // Unref/ref color
  if (color != nullptr) { color->unref(); color = nullptr; }
  if (org.color != nullptr) {
    color = org.color;
    color->ref();
  }

  // Unref/ref input angle
  if (iangle != nullptr) { iangle->unref(); iangle = nullptr; }
  if (org.iangle != nullptr) {
    iangle = org.iangle;
    iangle->ref();
  }

  // Scalar data
  speed = org.speed;
  sangle = org.sangle;

  xPos = org.xPos;
  yPos = org.yPos;
  dx = org.dx;
  dy = org.dy;

  left = org.left;
  right = org.right;
  top = org.top;
  bottom = org.bottom;

  separationWeight = org.separationWeight;
  alignmentWeight = org.alignmentWeight;
  cohesionWeight = org.cohesionWeight;
  separationMinDistance = org.separationMinDistance;

  // Copy neighbors (shallow copy of pointers)
  neighbors = org.neighbors;
}

void Boid::setStartAngle(const double deg)
{
  sangle = deg * (M_PI / 180.0);
  dx = std::cos(sangle) * speed;
  dy = std::sin(sangle) * speed;
}

void Boid::setSpeed(const double s)
{
  speed = s;
  dx = std::cos(sangle) * speed;
  dy = std::sin(sangle) * speed;
}

// Graphics (Draw)
void Boid::drawFunc()
{
  glPushMatrix();

  // Translate
  glTranslated(xPos, yPos, 0);

  // Rotate
  double angleDeg = std::atan2(dy, dx) * (180.0 / M_PI);
  glRotated(angleDeg, 0, 0, 1);

  // Optional color
  if (color != nullptr) {
    glColor4f(color->red(), color->green(), color->blue(), color->alpha());
  }

  // Triangle
  glBegin(GL_TRIANGLES);
  glVertex2d(0.5, 0.0);
  glVertex2d(-0.3, 0.2);
  glVertex2d(-0.3, -0.2);
  glEnd();

  glPopMatrix();
}

// -- Update Function --- (Life of the Boid)
void Boid::updateTC(const double dt)
{
  // Update base class data
  BaseClass::updateTC(dt);

  updateVision();  // detect all nearby objects

  // Calculate boid neighbors
  neighbors.clear();
  for (auto* obj : visibleObjects) {
    auto* b = dynamic_cast<Boid*>(obj);
    if (b != nullptr && b != this) {
      neighbors.push_back(b);
    }
  }

  // Calculate boid velocity
  computeBoid(dt);

  // Update velocity
  xPos += dx * dt;
  yPos += dy * dt;

  // Clamp to screen bounds (optional: allow small margin)
  const double margin = 1.0;
  if (xPos < left + margin) xPos = left + margin;
  if (xPos > right - margin) xPos = right - margin;
  if (yPos < bottom + margin) yPos = bottom + margin;
  if (yPos > top - margin) yPos = top - margin;
}

void Boid::updateData(const double dt)
{
  BaseClass::updateData(dt);
}

void Boid::reset()
{
  BaseClass::reset();

  // reset position
  xPos = 0;
  yPos = 0;

  // If angle is set via slot, use it
  if (iangle != nullptr) {
    mixr::base::Radians radians;
    setStartAngle(static_cast<double>(radians.convert(*iangle)));
  }
  else {
    // Otherwise just reapply current angle
    setStartAngle(sangle * (180.0 / M_PI));  // convert rad -> deg
  }

  neighbors.clear();
}

// --- Boid Behavior ---
mixr::base::Vec2d Boid::computeSeparation(const std::vector<Boid*>& neighbors)
{
  mixr::base::Vec2d steer;
  for (const Boid* b : neighbors) {
    const double dx = xPos - b->xPos;
    const double dy = yPos - b->yPos;
    const double dist = std::sqrt(dx * dx + dy * dy);
    if (dist > 0 && dist < separationMinDistance) {
      steer += mixr::base::Vec2d(dx / dist, dy / dist);
    }
  }
  return steer;
}

mixr::base::Vec2d Boid::computeAlignment(const std::vector<Boid*>& neighbors)
{
  if (neighbors.empty()) return {};
  double sumDx = 0.0, sumDy = 0.0;
  for (const Boid* b : neighbors) {
    sumDx += b->dx;
    sumDy += b->dy;
  }
  const double n = static_cast<double>(neighbors.size());
  const double avgDx = sumDx / n;
  const double avgDy = sumDy / n;
  return mixr::base::Vec2d(avgDx - dx, avgDy - dy);
}

mixr::base::Vec2d Boid::computeCohesion(const std::vector<Boid*>& neighbors)
{
  if (neighbors.empty()) return {};
  double sumX = 0.0, sumY = 0.0;
  for (const Boid* b : neighbors) {
    sumX += b->xPos;
    sumY += b->yPos;
  }
  const double n = static_cast<double>(neighbors.size());
  const double centerX = sumX / n;
  const double centerY = sumY / n;
  return mixr::base::Vec2d(centerX - xPos, centerY - yPos);
}

void Boid::computeBoid(const double dt)
{
  if (neighbors.empty()) return;

  // Boid rules
  auto sep = computeSeparation(neighbors);
  auto ali = computeAlignment(neighbors);
  auto coh = computeCohesion(neighbors);

  // Combine rule-based steering
  double steerX = sep.x() * separationWeight + ali.x() * alignmentWeight + coh.x() * cohesionWeight;
  double steerY = sep.y() * separationWeight + ali.y() * alignmentWeight + coh.y() * cohesionWeight;

  // Wall avoidance 
  mixr::base::Vec2d wallForce;
  const double avoidDist = 10.0;       // Distance threshold to start avoiding walls
  const double wallStrength = 30;     // Strength of wall steering

  // Left wall
  if (xPos - left < avoidDist) wallForce += mixr::base::Vec2d(1.0, 0.0) * (1.0 - (xPos - left) / avoidDist);
  // Right wall
  if (right - xPos < avoidDist) wallForce += mixr::base::Vec2d(-1.0, 0.0) * (1.0 - (right - xPos) / avoidDist);
  // Bottom wall
  if (yPos - bottom < avoidDist) wallForce += mixr::base::Vec2d(0.0, 1.0) * (1.0 - (yPos - bottom) / avoidDist);
  // Top wall
  if (top - yPos < avoidDist) wallForce += mixr::base::Vec2d(0.0, -1.0) * (1.0 - (top - yPos) / avoidDist);

  // Apply wall steering to main steering vector
  steerX += wallForce.x() * wallStrength;
  steerY += wallForce.y() * wallStrength;

  // Normalize steering direction
  double steerMag = std::sqrt(steerX * steerX + steerY * steerY);
  if (steerMag > 0.0001) {
    steerX /= steerMag;
    steerY /= steerMag;
  }

  // Normalize current velocity
  const double currMag = std::sqrt(dx * dx + dy * dy);
  if (currMag == 0.0) return;

  double ndx = dx / currMag;
  double ndy = dy / currMag;

  // Smooth turning toward steer direction
  const double steeringStrength = 0.1;  // Lower value = smoother
  ndx += steerX * steeringStrength;
  ndy += steerY * steeringStrength;

  // Re-normalize to maintain constant speed
  const double newMag = std::sqrt(ndx * ndx + ndy * ndy);
  dx = (ndx / newMag) * speed;
  dy = (ndy / newMag) * speed;
}


void Boid::updateVision()
{
  visibleObjects.clear();

  // Use getParent(), then cast to Page
  auto* parentPage = dynamic_cast<mixr::graphics::Page*>(this->container());
  if (parentPage == nullptr) {
    std::cerr << "Boid @" << this << " has no valid parent Page!" << std::endl;
    return;
  }

  const auto* comps = parentPage->getComponents();
  if (comps == nullptr) return;

  for (const auto* item = comps->getFirstItem(); item != nullptr; item = item->getNext()) {
    const auto* pair = static_cast<const mixr::base::Pair*>(item->getValue());
    auto* obj = const_cast<mixr::base::Object*>(pair->object());

    if (obj == this) continue; // skip self

    if (auto* b = dynamic_cast<Boid*>(obj)) {
      if (b == this) continue; // Skip self
      const double dx = b->xPos - xPos;
      const double dy = b->yPos - yPos;
      const double distSq = dx * dx + dy * dy;

      if (distSq <= detectionRadius * detectionRadius) {
        visibleObjects.push_back(b);
      }
    }
  }
}

// --- Slot Setters ---
bool Boid::setSlotSpeed(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    setSpeed(num->getReal());
    return true;
  }
  return false;
}

bool Boid::setSlotPosition(const mixr::base::List* const list)
{
  if (list == nullptr || list->entries() < 2) return false;

  double x{}, y{};
  const auto* px = dynamic_cast<const mixr::base::Number*>(list->getPosition(1));
  const auto* py = dynamic_cast<const mixr::base::Number*>(list->getPosition(2));
  if (px && py) {
    x = px->getReal();
    y = py->getReal();
    xPos = x;
    yPos = y;
    return true;
  }
  return false;
}

bool Boid::setSlotAngle(const mixr::base::Angle* const saobj)
{
  if (saobj != nullptr) {
    mixr::base::Radians radians;
    setStartAngle(static_cast<double>(radians.convert(*saobj)));
    if (iangle != nullptr) { iangle->unref(); iangle = nullptr; }
    iangle = saobj;
    iangle->ref();
    return true;
  }
  return false;
}

bool Boid::setSlotAngle(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    setStartAngle(num->getReal());
    return true;
  }
  return false;
}

bool Boid::setSlotSeparationWeight(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    separationWeight = num->getReal();
    return true;
  }
  return false;
}

bool Boid::setSlotAlignmentWeight(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    alignmentWeight = num->getReal();
    return true;
  }
  return false;
}

bool Boid::setSlotCohesionWeight(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    cohesionWeight = num->getReal();
    return true;
  }
  return false;
}

bool Boid::setSlotSeparationMinDistance(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    separationMinDistance = num->getReal();
    return true;
  }
  return false;
}

bool Boid::setSlotColor(const mixr::base::Color* const c)
{
  if (c != nullptr) {
    if (color != nullptr) { color->unref(); color = nullptr; }
    color = const_cast<mixr::base::Color*>(c);
    color->ref();
    return true;
  }
  return false;
}

bool Boid::setSlotDetectionRadius(const mixr::base::Number* const num)
{
  if (num != nullptr) {
    detectionRadius = num->getReal();
    return true;
  }
  return false;
}
