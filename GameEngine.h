#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

#include <iostream>
#include <fstream>
#include "Player.h"
#include <vector>

#define EMPTY 0
#define OTHER 1
#define MIN_BOARD_X_DIMS 0
#define MIN_BOARD_Y_DIMS 0
#define MAX_BOARD_X_DIMS 26
#define MAX_BOARD_Y_DIMS 26
#define ROWS 6
#define MIN_PLAYER_COUNT 0
#define MAX_PLAYER_COUNT 2

class GameEngine {
    public:
        GameEngine();
        GameEngine(std::vector<std::vector<std::string> > board);
        ~GameEngine();
        
        std::shared_ptr<Player> getPlayer(int playerIndex);
        void addPlayer(std::shared_ptr<Player> newPlayer);
        void setTileBag(std::shared_ptr<LinkedList> tileBag);
        void playGame(std::shared_ptr<Player> currPlayer);

    private:
        std::vector<std::vector<std::string> > board;
        std::shared_ptr<Player> players[2];
        std::shared_ptr<Player> currPlayer;
        std::shared_ptr<LinkedList> tileBag;
        int playerCount;
        bool firstMove;
        bool savedGame;
        bool replaceTile(std::shared_ptr<Player> currPlayer,std::string tileValue, bool checkReplace);
        bool placeTile(std::string tileValue, int boardRow, int boardCol);
        bool checkTilePlacement(std::string tileValue, int boardRow, int boardCol);
        void displayCurrentBoard(std::vector<std::vector<std::string> > board);
        void displayStatus(std::shared_ptr<Player> currPlayer, std::vector<std::vector<std::string> > board);
        bool readInput(std::shared_ptr<Player> currPlayer, std::string input);
        void addScore(std::shared_ptr<Player> currPlayer, int boardRow, int boardCol);
};

#endif // ASSIGN2_GAMEENGINE_H