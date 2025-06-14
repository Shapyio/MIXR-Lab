
#ifndef __mixr_models_Ins_H__
#define __mixr_models_Ins_H__

#include "mixr/models/navigation/Navigation.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Classes: Ins
// Description: Generic Inerial Navigation System (INS) Data
// Factory name: Ins
//------------------------------------------------------------------------------
class Ins : public Navigation
{
    DECLARE_SUBCLASS(Ins, Navigation)

public:
    // Alignment modes
    enum AlignMode { PREC = 1, SHDG = 2, AIR = 3, ATT = 4 };

public:
    Ins();

    const base::Vec3d& getGyroBias() const         { return gyroBias; }
    const base::Vec3d& getAccelBias() const        { return accelBias; }
    double getWanderAngle() const                  { return wander; }
    double getDopplerBias() const                  { return dBias; }
    double getDopplerScaleFactor() const           { return dSFact; }
    AlignMode getAlignmentMode() const             { return alignMode; }
    double getAlignmentTTG() const                 { return alignTTG; }
    double getQuality() const                      { return quality; }

    virtual void setDopplerBias(const double dBias);
    virtual void setDopplerScaleFactor(const double dSFact);
    virtual void setAlignmentMode(const AlignMode mode);

protected:
    virtual void setGyroBias(const base::Vec3d* const p);
    virtual void setAccelBias(const base::Vec3d* const p);
    virtual void setWanderAngle(const double v);
    virtual void setAlignmentTTG(const double v);
    virtual void setQuality(const double v);

private:
    base::Vec3d gyroBias;    // Gyro Bias
    base::Vec3d accelBias;   // Acceleration Bias
    double wander {};        // Wander angle (degs)
    double dBias {};         // Doppler bias
    double dSFact {};        // Doppler Scale Factor

    // Alignment/Kalman filter data
    AlignMode alignMode {SHDG};   // Alignment mode
    double alignTTG {};           // Alignment Time to Go (sec)
    double quality {};            // Quality
};

}
}

#endif
