
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <iostream>

#define DEFAULT_COLOUR '0'
#define DEFAULT_SHAPE 0

// Define a Colour type
typedef char Colour;

// Define a Shape type
typedef int Shape;

class Tile {
   public:
      Tile();
      Tile(Colour colour, Shape shape);
      Tile(Tile& otherTile);
      ~Tile();                                                                                                                                                                                                                   
      Colour getColour();
      Shape getShape();
      std::string toString();

   private:
      Colour colour;
      Shape shape;
};

#endif // ASSIGN2_TILE_H
