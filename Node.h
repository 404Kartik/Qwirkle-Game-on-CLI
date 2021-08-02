#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"
#include <memory>

#define DEFAULT_TILE nullptr
#define DEFAULT_NEXT_NODE nullptr

class Node {
   public:
      Node();
      Node(std::shared_ptr<Tile> tile, std::shared_ptr<Node> nextNode);
      Node(const Node& other);
      ~Node();
      std::shared_ptr<Node> getNext() const;
      std::shared_ptr<Tile> getTile() const;
      void setNext(std::shared_ptr<Node> nextNode);

   private:
      std::shared_ptr<Tile> tile;
      std::shared_ptr<Node> nextNode;
};

#endif // ASSIGN2_NODE_H