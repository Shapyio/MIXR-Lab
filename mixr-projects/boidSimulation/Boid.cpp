#include "Boid.hpp"

#include "mixr/base/units/Angles.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/colors/Color.hpp"

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
"startAngle",             // 2
"separation",             // 3
"alignment",              // 4
"cohesion",               // 5
"separationMinDist",      // 6
"color",                  // 7
END_SLOTTABLE(Boid)

BEGIN_SLOT_MAP(Boid)
  ON_SLOT(1, setSlotSpeed, mixr::base::Number)
  ON_SLOT(2, setSlotAngle, mixr::base::Angle)
  ON_SLOT(2, setSlotAngle, mixr::base::Number)
  ON_SLOT(3, setSlotSeparationWeight, mixr::base::Number)
  ON_SLOT(4, setSlotAlignmentWeight, mixr::base::Number)
  ON_SLOT(5, setSlotCohesionWeight, mixr::base::Number)
  ON_SLOT(6, setSlotSeparationMinDistance, mixr::base::Number)
  ON_SLOT(7, setSlotColor, mixr::base::Color)
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

  // allBoids is not copied (to avoid cross-instance issues)
  allBoids = nullptr;
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

  // Calculate boid neighbors
  // Discover neighbors (within radius, but can implement k-nearest in future)
  if (allBoids != nullptr) {
    neighbors = getNeighbors(*allBoids, 5.0);  // Radius = 5.0 units
  }

  // Calculate boid velocity
  computeBoid(dt);

  // Update velocity
  xPos += dx * dt;
  yPos += dy * dt;

  // Wrap around
  if (xPos > right) xPos = left;
  else if (xPos < left) xPos = right;
  if (yPos > top) yPos = bottom;
  else if (yPos < bottom) yPos = top;
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

  auto sep = computeSeparation(neighbors);
  auto ali = computeAlignment(neighbors);
  auto coh = computeCohesion(neighbors);

  dx = sep.x() * separationWeight + ali.x() * alignmentWeight + coh.x() * cohesionWeight;
  dy = sep.y() * separationWeight + ali.y() * alignmentWeight + coh.y() * cohesionWeight;

  // Normalize to speed
  const double mag = std::sqrt(dx * dx + dy * dy);
  if (mag > 0) {
    dx = (dx / mag) * speed;
    dy = (dy / mag) * speed;
  }
}

std::vector<Boid*> Boid::getNeighbors(const std::vector<Boid*>& allBoids, double radius)
{
  std::vector<Boid*> neighbors;
  for (Boid* b : allBoids) {
    if (b == this) continue;
    const double dx = b->xPos - xPos;
    const double dy = b->yPos - yPos;
    const double distSq = dx * dx + dy * dy;
    if (distSq < radius * radius) {
      neighbors.push_back(b);
    }
  }
  return neighbors;
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
