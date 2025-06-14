
#include "mixr/instruments/dials/AltitudeDial.hpp"
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(AltitudeDial, "AltitudeDial")
EMPTY_SLOTTABLE(AltitudeDial)
EMPTY_DELETEDATA(AltitudeDial)

AltitudeDial::AltitudeDial()
{
    STANDARD_CONSTRUCTOR()

    dialAltSD.empty();
    altHundredsSD.empty();
    altOneThousandsSD.empty();
    altTenThousandsSD.empty();
}

void AltitudeDial::copyData(const AltitudeDial& org, const bool)
{
    BaseClass::copyData(org);

    dialAltSD.empty();
    altHundredsSD.empty();
    altOneThousandsSD.empty();
    altTenThousandsSD.empty();
}

//------------------------------------------------------------------------------
// updateData() - update non time-critical threads here
//------------------------------------------------------------------------------
void AltitudeDial::updateData(const double dt)
{
    // update base class first
    BaseClass::updateData(dt);

    // figure our rotation with the new "raw" instrument value
    double alt = getInstValue();

    // send that data to the tape gauge
    double altTens = ((alt/100) - static_cast<int>(alt/100)) * 10;
    double altHundreds = ((alt/1000) - static_cast<int>(alt/1000)) * 10;
    // now figure out our ONE thousands!
    double altOneThousands =  alt / 1000;
    // figure alt TEN thousands!
    double altTenThousands = alt / 10000;
    if (altOneThousands >= 10) altOneThousands = altOneThousands - static_cast<int>(altTenThousands) * 10;
    // now figure the rest of the number
//    int altRest = int(alt/99.9999);

    if (altTens < 9) altHundreds = static_cast<double>(static_cast<int>(altHundreds));
    else {
        double x = altTens - static_cast<int>(altTens);
        altHundreds = static_cast<int>(altHundreds) + x;
    }
    if (altHundreds < 9) altOneThousands = static_cast<double>(static_cast<int>(altOneThousands));
    else {
        // scale our hundreds value to lock with the tens
        double x = altHundreds - static_cast<int>(altHundreds);
        altOneThousands = static_cast<int>(altOneThousands) + x;
    }
    if (altOneThousands < 9) altTenThousands = static_cast<double>(static_cast<int>(altTenThousands));
    else {
        // scale our hundreds value to lock with the tens
        double x = altOneThousands - static_cast<int>(altOneThousands);
        altTenThousands = static_cast<int>(altTenThousands) + x;
    }

    // send our data to our components
    send("dialalt", UPDATE_INSTRUMENTS, alt, dialAltSD);
    send("hundreds", UPDATE_INSTRUMENTS, altHundreds, altHundredsSD);
    send("onethousands", UPDATE_INSTRUMENTS, altOneThousands, altOneThousandsSD);
    send("tenthousands", UPDATE_INSTRUMENTS, altTenThousands, altTenThousandsSD);
}

//------------------------------------------------------------------------------
// figureRotation() - figures out where to rotate our gauge
// and translate our tape based on our altitude
//------------------------------------------------------------------------------
bool AltitudeDial::figureRotation(const double thisAlt)
{
    // break down our number to tenThousands, thousands, and hundreds
//    double hundreds = 0;
//    double thousands = 0;
//    double tenThousands = 0;

//    tenThousands = thisAlt/10000;
//    thousands = (thisAlt/1000) - (static_cast<int>(thisAlt/10000)* 10);
//    hundreds = ((thisAlt - (static_cast<int>(thisAlt/1000) * 1000)) / 100);

    return true;
}

}
}

