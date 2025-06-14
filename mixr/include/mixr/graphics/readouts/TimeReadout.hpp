
#ifndef __mixr_graphics_TimeReadout_H__
#define __mixr_graphics_TimeReadout_H__

#include "NumericReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: TimeReadout
//
// Factory name: TimeReadout
//
// Example formats:
//
//    HH:MM:SS      // Hours, minutes and seconds
//    0H:MM:SS.S    // Hours (w/leading zero), minutes and seconds decimal
//   +HH:MM         // Hours (w/sign) and minutes
//    HHMM.M        // Hours (w/sign) and minutes decimal
//   +0HMM:SS.S     // Hours (w/signed and leading zero), minutes
//                  //   and seconds decimal
//    MMSS.S        // Minutes and seconds decimal
//    SSSS          // Seconds
//
// Notes: The seconds and third numbers always have leading zeros.  The first
// number requires a zero, '0', to have leading zeros.
//
//------------------------------------------------------------------------------
class TimeReadout : public NumericReadout
{
   DECLARE_SUBCLASS(TimeReadout, NumericReadout)

public:
   enum class TimeMode { invalid, hhmmss, hhmm, hh, mmss, mm, ss };
   TimeReadout();
   char filterInputEvent(const int event, const int tc) override;
   double getInputValue() const override;

protected:
   void makeText() override;
   void reformat(const char* const example) override;
   TimeMode tmode {TimeMode::hhmmss};
};

}
}

#endif
