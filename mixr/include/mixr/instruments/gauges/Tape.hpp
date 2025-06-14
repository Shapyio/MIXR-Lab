
#ifndef __mixr_instruments_Tape_H__
#define __mixr_instruments_Tape_H__

#include "mixr/instruments/Instrument.hpp"
#include <array>

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Tape
// Description: Dynamic class that renders only the portion of the tape
//              that is needed on the screen
//------------------------------------------------------------------------------
class Tape : public Instrument
{
    DECLARE_SUBCLASS(Tape, Instrument)

public:
    Tape();

    void updateData(const double dt = 0.0) override;

    virtual bool setRange(const int x)                  { range = x; return true;     }
    virtual bool setHeight(const double x)              { height = x; return true;    }
    virtual bool setIncrement(const int x)              { increment = x; return true; }
    virtual bool setVertical(const bool x)              { vertical = x; return true;  }
    virtual bool setMaxNumber(const double x)           { maxNum = x; return true;    }
    virtual bool setMinNumber(const double x)           { minNum = x; return true;    }
    virtual bool setConvert(const bool x)               { convert = x; return true;   }

private:
    SendData transTapeGraphicSD;    // our translation of our tape graphic
    SendData transTapeGraphicVSD;   // vertical

    static const int MAX_NUMBERS = 25;
    std::array<int, MAX_NUMBERS> numberValsHunds {};
    std::array<int, MAX_NUMBERS> numberValsHundsVis {};
    std::array<int, MAX_NUMBERS> numberValsThous {};
    std::array<int, MAX_NUMBERS> numberValsThousVis {};
    std::array<SendData, MAX_NUMBERS> numberValsHundsSD {};
    std::array<SendData, MAX_NUMBERS> numberValsThousSD {};
    std::array<SendData, MAX_NUMBERS> numberValsHundsVisSD {};
    std::array<SendData, MAX_NUMBERS> numberValsThousVisSD {};
    int range {};
    double height {};
    int increment {1};
    bool vertical {true};
    double maxNum {-1.0};
    double minNum {-1.0};
    bool convert {};

private:
    // slot table helper methods
    bool setSlotRange(const base::Number* const);
    bool setSlotHeight(const base::Number* const);
    bool setSlotIncrement(const base::Number* const);
    bool setSlotVertical(const base::Number* const);
    bool setSlotMaxNum(const base::Number* const);
    bool setSlotMinNum(const base::Number* const);
    bool setSlotConvert(const base::Number* const);
};

}
}

#endif
