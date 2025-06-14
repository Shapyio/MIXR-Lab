
#ifndef __mixr_instruments_TickMarks_H__
#define __mixr_instruments_TickMarks_H__

#include "mixr/instruments/gauges/AnalogGauge.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: TickMarks
//
// Description: Uses tick marks in a linear fashion, either horizontally or
// vertically, and draws them accordingly.  This is very similar to dial
// tick marks, only it doesn't rotate and doesn't draw around the circle.
//------------------------------------------------------------------------------
class TickMarks : public AnalogGauge
{
    DECLARE_SUBCLASS(TickMarks,AnalogGauge)

public:
    TickMarks();

    virtual bool setTickMarkLength(const double newLength);
    virtual bool setQuantity(const int newQ);
    virtual bool setGaugeLength(const double newL);
    virtual bool setFlip(const bool x);
    virtual bool setTickGraphic(const graphics::Graphic* const newGraphic);

    double getTickMarkLength() const { return lengthTM; }
    int  getQuantity() const         { return quantity; }

    void drawFunc() override;

private:
    double lengthTM {1.0};           // tick mark length (if not a graphic)
    int    quantity {1};             // how many tick marks will we have?
    double gaugeLength {};           // length we are spanning our tick marks over
    bool   flip {};                  // our flip variable
    graphics::Graphic* myGraphic {}; // our graphic (if we choose to use one for a tick mark)

private:
    // slot table helper methods
    bool setSlotTickMarkLength(const base::Number* const);
    bool setSlotQuantity(const base::Number* const);
    bool setSlotGaugeLength(const base::Number* const);
    bool setSlotFlip(const base::Number* const);
};

}
}

#endif
