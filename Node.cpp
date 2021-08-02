#include "Node.h"

// Default Constructor of Node.
Node::Node() {
   this->tile = DEFAULT_TILE;
   this->nextNode = DEFAULT_NEXT_NODE;
}

// Custom Constructor of Node.
Node::Node(std::shared_ptr<Tile> tile, std::shared_ptr<Node> nextNode) :
   tile(tile),
   nextNode(nextNode)
{}

// Deep Copy Constructor for otherNode.
Node::Node(const Node& otherNode) {
   std::shared_ptr<Tile> newTile = 
         std::make_shared<Tile>(*otherNode.getTile());
   this->tile = newTile;

   std::shared_ptr<Node> newNextNode = 
         std::make_shared<Node>(*otherNode.getNext());
   this->nextNode = newNextNode;
}

// Deconstructor of Node.
Node::~Node() {

}

// Return a Pointer to next Node.
std::shared_ptr<Node> Node::getNext() const {
   std::shared_ptr<Node> nextNodePtr = this->nextNode;

   return nextNodePtr;
}

// Set Pointer to next Node.
void Node::setNext(std::shared_ptr<Node> nextNode) {
   this->nextNode = nextNode;
}

// Return Pointer to Tile.
std::shared_ptr<Tile> Node::getTile() const {
   std::shared_ptr<Tile> tilePtr = this->tile;
   
   return tilePtr;
}