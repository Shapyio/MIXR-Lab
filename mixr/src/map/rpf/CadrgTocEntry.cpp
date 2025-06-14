
#include "mixr/map/rpf/CadrgTocEntry.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace mixr {
namespace rpf {

IMPLEMENT_SUBCLASS(CadrgTocEntry, "CadrgTocEntry")
EMPTY_SLOTTABLE(CadrgTocEntry)

CadrgTocEntry::CadrgTocEntry()
{
    STANDARD_CONSTRUCTOR()
}

void CadrgTocEntry::copyData(const CadrgTocEntry& org, const bool)
{
    BaseClass::copyData(org);

    nwLat = org.nwLat;
    nwLon = org.nwLon;
    seLat = org.seLat;
    seLon = org.seLon;
    swLat = org.swLat;
    swLon = org.swLon;
    neLat = org.neLat;
    neLon = org.neLon;
    vertInterval = org.vertInterval;
    horizInterval = org.horizInterval;
    vertResolution = org.vertResolution;
    horizResolution = org.horizResolution;
    horizFrames = org.horizFrames;
    vertFrames = org.vertFrames;
    mapImage = org.mapImage;
    mapIndex = org.mapIndex;
}

void CadrgTocEntry::deleteData()
{
    // Delete our frames properly
    for (int i = 0; i < horizFrames; i++) {
        for (int j = 0; j < vertFrames; j++) {
            CadrgFrameEntry* p = &frames[i][j];
            if (p != nullptr) {
                p->unref();
                p = nullptr;
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------
// getFrameEntry() - Return the frame at the given row, column.
// ---------------------------------------------------------------------------------------------------
CadrgFrameEntry* CadrgTocEntry::getFrameEntry(const int v, const int h)
{
    CadrgFrameEntry* p {&frames[v][h]};
    if (p != nullptr) {
       return p;
    }
    return nullptr;
}

// -----------------------------------------------------------------------------------------
// generateItems() - Simply generate the name of the file we are using for our image.
// -----------------------------------------------------------------------------------------
void CadrgTocEntry::generateItems()
{
    const auto tempTitle = new char[100];

    // Terminate our scale, if we haven't yet.
    for (int j = 10; j > 0; j--) {
        if (scale[j] == ' ') scale[j] = '\0';
        else break;
    }

    mapImage = 0;
    // Check for overview image
    if ((std::strncmp(type, "OVERVIEW", 8) == 0) || (std::strncmp(scale, "OVERVIEW", 8) == 0)) {
        std::sprintf(tempTitle, "%s %s", type, scale);
    }
    // Image has a nwLat of ~0
    else if (nwLat == (unsigned int) ~0) std::sprintf(title, "%s %s", type, scale);
    else {
        std::sprintf(tempTitle, "%s %s Zone%s: %6.3f,%6.3f / %6.3f,%6.3f", type, scale, zone, nwLat, nwLon, seLat, seLon);
        mapImage = 1;
    }
    std::sprintf(title, "%s", tempTitle);

    delete[] tempTitle;
}

// Set functions
// -----------------------------------------------------------------------------------------
// setType() - Sets our entry type (ie... "CADRG").
// -----------------------------------------------------------------------------------------
void CadrgTocEntry::setType(const char* x, const int size)
{
    base::utStrcpy(type, size, x);
    type[size] = 0;
}

// -----------------------------------------------------------------------------------------
// setZone() - Sets the zone this entry lies in.
// -----------------------------------------------------------------------------------------
void CadrgTocEntry::setZone(const char* x, const int size)
{
    std::strncpy(zone, x, size);
    zone[size] = 0;
}

// -----------------------------------------------------------------------------------------
// setScale() - Sets the scale of this entry (ie... "1:500K").
// -----------------------------------------------------------------------------------------
void CadrgTocEntry::setScale(const char* x, const int size)
{
    std::strncpy(scale, x, size);
    scale[size] = 0;
}

// ---------------------------------------------------------------------------------------------------
// isInZone() - Takes in a lat lon and returns true if that point falls in our entry.
// ---------------------------------------------------------------------------------------------------
bool CadrgTocEntry::isInZone(double lat, double lon)
{
    bool ok {(lat >= swLat && lon >= swLon && lat <= neLat && lon <= neLon)};
    return ok;
}

}
}
