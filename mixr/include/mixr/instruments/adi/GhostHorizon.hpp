
#ifndef __mixr_instruments_GhostHorizon_H__
#define __mixr_instruments_GhostHorizon_H__

#include "mixr/instruments/adi/Adi.hpp"

namespace mixr {
namespace base { class Color; class String; class Number; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: GhostHorizon
//
// Description: Takes a pitch and roll in and determines when and where to draw the
// ghost horizon graphic
//------------------------------------------------------------------------------
class GhostHorizon : public Adi
{
    DECLARE_SUBCLASS(GhostHorizon, Adi)

public:
    GhostHorizon();

    virtual bool setWidth(const double x)           { width = x; return true; }
    virtual bool setHeight(const double x)          { height = x; return true; }

    void drawFunc() override;
    void updateData(const double dt = 0.0) override;

private:
    base::Vec3d skyColor;         // color of our sky
    base::Vec3d groundColor;      // color of our ground
    base::String* gColorName {};  // ground color name
    base::String* sColorName {};  // sky color name
    double width {20.0};          // width of our polygon
    double height {20.0};         // height of our polygon

private:
    // slot table helper methods   
    bool setSlotSkyColor(const base::Color* const);
    bool setSlotSkyColor(const base::String* const);
    bool setSlotGroundColor(const base::String* const);
    bool setSlotGroundColor(const base::Color* const);
    bool setSlotWidth(const base::Number* const);
    bool setSlotHeight(const base::Number* const);
};

}
}

#endif
