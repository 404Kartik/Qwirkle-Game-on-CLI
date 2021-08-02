#include "Tile.h"

// Default Constructor of Tile.
Tile::Tile() {
    this->colour = DEFAULT_COLOUR;
    this->shape = DEFAULT_SHAPE;
}

// Constructor of Tile.
Tile::Tile(Colour colour, Shape shape) :
    colour(colour),
    shape(shape)
{}

// Deep Constructor of otherTile.
Tile::Tile(Tile& otherTile) :
    colour(otherTile.getColour()),
    shape(otherTile.getShape())
{}

// Deconstructor of Tile.
Tile::~Tile() {
    //TODO
}

// Return Tile's colour value.
Colour Tile::getColour() {
    return this->colour;
}

// Return Tile's Shape value.
Shape Tile::getShape() {
    return this->shape;
}

// Return string of Tile attributes.
std::string Tile::toString() {
    std::string stringVal = "";

    // Concatenate both attributes of Tile.
    stringVal += this->colour;
    stringVal += std::to_string(this->shape);

    return stringVal;
}