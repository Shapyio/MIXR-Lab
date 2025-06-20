#include "Boid.h"
#include "mixr/base/units/Angles.hpp"

#include <cmath>

IMPLEMENT_SUBCLASS(Boid, "Boid")
EMPTY_DELETEDATA(Boid)

BEGIN_SLOTTABLE(Boid)
"speed",            // 1: speed
"startAngle",       // 2: starting angle (off X axis)
END_SLOTTABLE(Boid)

BEGIN_SLOT_MAP(Boid)
  ON_SLOT(1, setSlotSpeed, mixr::base::Number)
  ON_SLOT(2, setSlotAngle, mixr::base::Angle)
  ON_SLOT(2, setSlotAngle, mixr::base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Boid)
END_EVENT_HANDLER()

Boid::Boid()
{
  STANDARD_CONSTRUCTOR()
    setSpeed(10.0);
}

void Boid::copyData(const Boid& org, const bool)
{
  BaseClass::copyData(org);

  if (iangle != nullptr) { iangle->unref(); iangle = nullptr; }
  if (org.iangle != nullptr) iangle = org.iangle->clone();

  left = org.left;
  right = org.right;
  bottom = org.bottom;
  top = org.top;
  xPos = org.xPos;
  yPos = org.yPos;
  xOld = org.xOld;
  yOld = org.yOld;
  dx = org.dx;
  dy = org.dy;
  speed = org.speed;
  sangle = org.sangle;
  nTrails = org.nTrails;
  index = org.index;

  for (int i = 0; i < nTrails; i++) {
    trail[i] = org.trail[i];
  }
}

void Boid::reset()
{
  BaseClass::reset();

  xPos = 0;
  yPos = 0;
  nTrails = 0;
  if (iangle != nullptr) {
    mixr::base::Radians radians;
    setStartAngle(static_cast<double>(radians.convert(*iangle)));
  }
}

void Boid::setStartAngle(const double radians)
{
  sangle = radians;
  dx = std::cos(sangle) * speed;
  dy = std::sin(sangle) * speed;
}

void Boid::setSpeed(const double xx)
{
  speed = xx;
  dx = std::cos(sangle) * speed;
  dy = std::sin(sangle) * speed;
}

void Boid::updateTC(const double dt)
{
  // Update base classes stuff
  BaseClass::updateTC(dt);

  // ---
  // Move
  // ---

  xPos += dx * dt;
  if (xPos > right) {
    xPos = right - (xPos - right);
    dx = -dx;
  }
  else if (xPos < left) {
    xPos = left - (xPos - left);
    dx = -dx;
  }

  yPos += dy * dt;
  if (yPos > top) {
    yPos = top - (yPos - top);
    dy = -dy;
  }
  else if (yPos < bottom) {
    yPos = bottom - (yPos - bottom);
    dy = -dy;
  }
}

void Boid::updateData(const double dt)
{
  // Update base classes stuff
  BaseClass::updateData(dt);

  // ---
  // Save the point every pass (if it has changed)
  // ---
  if (nTrails == 0 || xPos != xOld || yPos != yOld) {
    trail[index++].set(xPos, yPos);
    if (index >= MAX_HIST) index = 0;
    if (nTrails < MAX_HIST) nTrails++;
    xOld = xPos;
    yOld = yPos;
  }
}

void Boid::drawFunc()
{
  glBegin(GL_LINE_STRIP);
  int idx{ index - 1 };
  for (int i = 0; i < nTrails; i++) {
    if (idx < 0) idx = MAX_HIST - 1;
    lcVertex2v(trail[idx--].ptr());
  }
  glEnd();
}

bool Boid::setSlotSpeed(const mixr::base::Number* const rsobj)
{
  bool ok{};
  if (rsobj != nullptr) {
    setSpeed(rsobj->getReal());
    ok = true;
  }
  return ok;
}

bool Boid::setSlotAngle(const mixr::base::Angle* const saobj)
{
  bool ok{};
  if (saobj != nullptr) {
    mixr::base::Radians radians;
    setStartAngle(static_cast<double>(radians.convert(*saobj)));
    iangle = saobj;
    iangle->ref();
    ok = true;
  }
  return ok;
}

bool Boid::setSlotAngle(const mixr::base::Number* const saobj)
{
  bool ok{};
  if (saobj != nullptr) {
    setStartAngle(saobj->getReal());
    ok = true;
  }
  return ok;
}
