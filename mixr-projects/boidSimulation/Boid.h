#ifndef __Boid_H__
#define __Boid_H__

#include "mixr/graphics/Graphic.hpp"
#include <vector>

namespace mixr {
  namespace base {
    class Angle;
    class Number;
  }
}

//------------------------------------------------------------------------------
// Class: Boid
//
// Description: Simulates a single boid using 3 behavioral rules:
//  - Separation: avoid crowding neighbors
//  - Alignment: steer towards average heading of neighbors
//  - Cohesion: steer towards average position of neighbors
//
// Position and velocity are updated using these rules.
// 
// Note: Framework from Worm.hpp from mixr-examples/tutorial07
//
//------------------------------------------------------------------------------
class Boid final : public mixr::graphics::Graphic
{
  DECLARE_SUBCLASS(Boid, mixr::graphics::Graphic)

public:
  Boid();

  // Position
  void getPosition(double& xx, double& yy) const { xx = pos.x(); yy = pos.y(); }
  void setPosition(const double xx, const double yy) { pos.set(xx, yy); }

  // Velocity
  mixr::base::Vec2d getVelocity() const { return velocity; }
  void setVelocity(const mixr::base::Vec2d& v) { velocity = v; }

  // Boundary box
  void setBounds(double l, double r, double b, double t) {
    left = l; right = r; bottom = b; top = t;
  }

  // Dynamic weights
  void setSeparationWeight(double w) { separationWeight = w; }
  void setAlignmentWeight(double w) { alignmentWeight = w; }
  void setCohesionWeight(double w) { cohesionWeight = w; }

  // Speed
  void setSpeed(const double s) { speed = s; }
  void setStartAngle(const double deg);

  // Rendering
  void drawFunc() final;

  // Simulation steps
  void updateTC(const double dt = 0.0) final;
  void updateData(const double dt = 0.0) final;
  void reset() final;
  bool event(const int event, mixr::base::Object* const obj = nullptr) final;

  // Behavior evaluation interface (called by controller)
  void computeBoidBehavior(const std::vector<Boid*>& allBoids, const double dt);

private: // variables
  // Simulation bounds
  double left{ -10.0 }, right{ 10.0 };
  double bottom{ -10.0 }, top{ 10.0 };

  // Motion state
  mixr::base::Vec2d pos{};               // Current position, saved in Vec2d instead of 2 separate variables
  mixr::base::Vec2d velocity{};          // Current velocity
  double speed{ 10.0 };                    // Nominal speed
  double sangle{ 0.0 };                    // Start angle (radians)

  // Weights (modifiable at runtime)
  double separationWeight{ 1.5 };
  double alignmentWeight{ 1.0 };
  double cohesionWeight{ 1.0 };
  double separationMinDistance{ 0.1 };

  const mixr::base::Angle* iangle{};     // Optional input slot for angle

private: //methods
  // Internal behavior methods (helpers for computeBoidBehavior)
  mixr::base::Vec2d computeSeparation(const std::vector<Boid*>& neighbors);
  mixr::base::Vec2d computeAlignment(const std::vector<Boid*>& neighbors);
  mixr::base::Vec2d computeCohesion(const std::vector<Boid*>& neighbors);

  std::vector<Boid*> getNeighbors(const std::vector<Boid*>& allBoids, double radius = 5.0);

  // Slot table helper methods
  bool setSlotSpeed(const mixr::base::Number* const);
  bool setSlotAngle(const mixr::base::Angle* const);
  bool setSlotAngle(const mixr::base::Number* const);
  bool setSlotSeparationWeight(const mixr::base::Number* const);
  bool setSlotAlignmentWeight(const mixr::base::Number* const);
  bool setSlotCohesionWeight(const mixr::base::Number* const);
  bool setSlotSeparationMinDistance(const mixr::base::Number* const);
};

#endif
