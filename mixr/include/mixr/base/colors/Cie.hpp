
#ifndef __mixr_base_colors_Cie_H__
#define __mixr_base_colors_Cie_H__

#include "mixr/base/colors/Color.hpp"
#include "mixr/base/safe_ptr.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace base {
class Vec4d;
class MonitorMetrics;
class Table1;

//------------------------------------------------------------------------------
// Class: Cie
//
// Description:  Defines a color by Luminance, X, Y, and monitor-specific data.
//
// Factory name: cie
// Slots:
//     luminance <Number>         ! Luminance component (0.0 to 1.0) (default: 0)
//     x         <Number>         ! Green component     (0.0 to 1.0) (default: 0)
//     y         <Number>         ! Blue component      (0.0 to 1.0, x+y<=1) (default: 0)
//     monitor   <MonitorMetrics> ! Monitor characteristics
//
//
// Public methods: Base class public methods, plus ...
//
//     Cie(const MonitorMetrics* m, const double l, const double x, const double y);
//         Special constructor to initialize the CIE color with the given values.
//
//     double luminance()
//     double x()
//     double y()
//         Data access routines.  Returns the CIE component.
//
//     getCIE(Vec3d& cie)
//         Returns the CIE components in a vector.
//
//     static void cie2rgb(Vec4d& rgba, const Vec3d& cie, const MonitorMetrics* m)
//         Static function to convert a CIE color into RGB.
//
// Enumerated:
//     { LUMINANCE, X, Y }
//         Used to index the CIE color vector.
//
//
// Note:  The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        CIE color vector.
//------------------------------------------------------------------------------
class Cie : public Color
{
    DECLARE_SUBCLASS(Cie, Color)

public:
    // components of CIE color
    enum { LUMINANCE, X, Y };

public:
    Cie(const MonitorMetrics* m, const double l, const double x, const double y);
    Cie();

    double luminance() const;
    double x() const;
    double y() const;
    void getCIE(Vec3d& cie) const;

    static void cie2rgb(Vec4d& rgba, const Vec3d& cie, const MonitorMetrics* m);

protected:
    Vec3d cie;
    safe_ptr<const MonitorMetrics> monitor;

private:
   // slot table helper methods
   bool setSlotMonitor(const MonitorMetrics* const);
   bool setSlotLuminance(const Number* const);
   bool setSlotX(const Number* const);
   bool setSlotY(const Number* const);
};

}
}

#endif

