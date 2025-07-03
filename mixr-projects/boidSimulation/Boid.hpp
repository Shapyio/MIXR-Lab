#ifndef __Boid_H__
#define __Boid_H__

#include "mixr/graphics/Graphic.hpp"
#include "vector"

namespace mixr {
  namespace base { class Angle; class Number; class Color; }
}

class Boid final : public mixr::graphics::Graphic
{
  DECLARE_SUBCLASS(Boid, mixr::graphics::Graphic)

public:
  Boid();

  void getPosition(double& xx, double& yy) const { xx = xPos; yy = yPos; }
  void setPosition(const double xx, const double yy) { xPos = xx; yPos = yy; }

  void setStartAngle(const double deg);
  void setSpeed(const double s);
  void setAllBoids(const std::vector<Boid*>* list) { allBoids = list; } // set list of boids

  void drawFunc() final;
  void updateTC(const double dt = 0.0) final;
  void updateData(const double dt = 0.0) final;
  bool event(const int event, mixr::base::Object* const obj = nullptr) final;
  void reset() final;

  // function for controller to set boid list
  void setAllBoids(std::vector<Boid*>* list) { allBoids = list; }

private:
  double left{ -10.0 }, right{ 10.0 };
  double bottom{ -10.0 }, top{ 10.0 };
  double xPos{}, yPos{};
  double dx{}, dy{};
  double speed{ 10.0 };
  double sangle{};
  double separationWeight{ 1.5 };
  double alignmentWeight{ 1.0 };
  double cohesionWeight{ 1.0 };
  double separationMinDistance{ 0.1 };

  const std::vector<Boid*>* allBoids{};  // Pointer to global list of all boids
  std::vector<Boid*> neighbors;           // Neighbors for this boid

  const mixr::base::Angle* iangle{};  // for cloneable slot storage

  // Color
  mixr::base::Color* color{};  // Optional

  // Slot table helpers
  bool setSlotSpeed(const mixr::base::Number* const);
  bool setSlotAngle(const mixr::base::Angle* const);
  bool setSlotAngle(const mixr::base::Number* const);
  bool setSlotSeparationWeight(const mixr::base::Number* const);
  bool setSlotAlignmentWeight(const mixr::base::Number* const);
  bool setSlotCohesionWeight(const mixr::base::Number* const);
  bool setSlotSeparationMinDistance(const mixr::base::Number* const);
  bool setSlotColor(const mixr::base::Color* const);

  // Boid behavior functions
  void computeBoid(const double dt);
  std::vector<Boid*> getNeighbors(const std::vector<Boid*>& allBoids, double radius = 5.0);
  mixr::base::Vec2d computeSeparation(const std::vector<Boid*>& neighbors);
  mixr::base::Vec2d computeAlignment(const std::vector<Boid*>& neighbors);
  mixr::base::Vec2d computeCohesion(const std::vector<Boid*>& neighbors);

};

#endif
