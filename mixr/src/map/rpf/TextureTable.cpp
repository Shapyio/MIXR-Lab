
#include "mixr/map/rpf/TextureTable.hpp"
#include "mixr/graphics/Texture.hpp"
#include <iostream>

namespace mixr {
namespace rpf {

IMPLEMENT_SUBCLASS(TextureTable, "TextureTable")
EMPTY_SLOTTABLE(TextureTable)

TextureTable::TextureTable()
{
    STANDARD_CONSTRUCTOR()
}

void TextureTable::copyData(const TextureTable& org, const bool)
{
    BaseClass::copyData(org);

    maxTableSize = org.maxTableSize;
    centerTablePos = org.centerTablePos;
    lowerBound = org.lowerBound;
    upperBound = org.upperBound;
    row = org.row;
    col = org.col;
    size = org.size;

    for (int i = 0; i < maxTableSize; i++) {
        for (int j = 0; j < maxTableSize; j++) {
            if (texes[i][j] != nullptr) {
                texes[i][j]->unref();
                texes[i][j] = nullptr;
            }
            if (org.texes[i][j] != nullptr) {
                texes[i][j] = org.texes[i][j];
                texes[i][j]->ref();
            }

        }
    }
}

void TextureTable::deleteData()
{
    for (int i = 0; i < maxTableSize; i++) {
        for (int j = 0; j < maxTableSize; j++) {
            if (texes[i][j] != nullptr) texes[i][j]->unref();
            texes[i][j] = nullptr;
        }
    }
}

//------------------------------------------------------------------------------
// setCenterRowTexture() - Sets our center row.
//------------------------------------------------------------------------------
bool TextureTable::setCenterRowTexture(const int x)
{
    row = x;
    return true;
}

//------------------------------------------------------------------------------
// setCenterColumnTexture() - Sets our center column.
//------------------------------------------------------------------------------
bool TextureTable::setCenterColumnTexture(const int x)
{
    col = x;
    return true;
}

//------------------------------------------------------------------------------
// isInBounds() - Are we in the texture boundaries?
//------------------------------------------------------------------------------
bool TextureTable::isInBounds(int row, int col)
{
    if (row >= lowerBound && row <= upperBound && col >= lowerBound && col <= upperBound) return true;
    else return false;
}

// -------------------------------------------------------------------------
// setSize() - Sets the size of our table, and clears our texture object array
// NOTE: This will work correctly with only odd number sizes, which will
// split the table evenly.
// -------------------------------------------------------------------------
bool TextureTable::setSize(const int newSize)
{
    // Force our size to be less than our max size
    if (newSize > MAX_TABLE_SIZE) {
        size = MAX_TABLE_SIZE;
        std::cout << "Size " << newSize << " is greater than the max size of " << MAX_TABLE_SIZE << "!  Setting to MAX_TABLE_SIZE" << std::endl;
    }
    else size = newSize;

    // Set our new table size
    maxTableSize = size;

    centerTablePos = static_cast<int>((size / 2));
    lowerBound = -centerTablePos;
    upperBound = centerTablePos;

    // Clear out our texture objects
    for (int i = 0; i < maxTableSize; i++) {
        for (int j = 0; j < maxTableSize; j++) {
            if (texes[i][j] != nullptr) texes[i][j]->unref();
            texes[i][j] = nullptr;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
// setTextureObject() - Sets the texture at the given row, column.
//------------------------------------------------------------------------------
bool TextureTable::setTextureObject(int row, int col, graphics::Texture* newObj)
{
    const int x {row + centerTablePos};
    const int y {col + centerTablePos};

    if (texes[x][y] != nullptr) {
        texes[x][y]->unref();
        texes[x][y] = nullptr;
    }

    if (newObj != nullptr) {
        texes[x][y] = newObj;
        texes[x][y]->ref();
    }

    return true;
}

//------------------------------------------------------------------------------
// getTexture() - Returns the texture object at the given row / column.
//------------------------------------------------------------------------------
graphics::Texture* TextureTable::getTexture(int row, int col)
{
    return texes[row + centerTablePos][col + centerTablePos];
}

}
}

