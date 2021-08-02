#include "GameEngine.h"

// Default Constructor for GameEngine.
GameEngine::GameEngine() {
    this -> playerCount = MIN_PLAYER_COUNT;

    // Create new players structure
    for (int i = START; i < MAX_PLAYER_COUNT; i++) {
        this -> players[i] = nullptr;
    }

    // Initialise Tile Bag LinkedList.
    this -> tileBag = nullptr;

    this -> firstMove = true;
    this -> savedGame = false;
}

// Custom Constructor for GameEngine.
GameEngine::GameEngine(std::vector < std::vector < std::string > > board) {
    this -> playerCount = MIN_PLAYER_COUNT;

    // Create new players structure
    for (int i = START; i < MAX_PLAYER_COUNT; i++) {
        this -> players[i] = nullptr;
    }

    // Create new board structure
    this -> board = board;

    // Initialise Tile Bag LinkedList.
    this -> tileBag = nullptr;

    this -> firstMove = true;
    this -> savedGame = false;
}

// Deconstructor for GameEngine.
GameEngine::~GameEngine() {

}

// Main Game Loop
void GameEngine::playGame(std::shared_ptr < Player > loadedCurrPlayer) {
    //starting game play
    bool running = true;
    this -> currPlayer = loadedCurrPlayer;
    bool inputSuccess = false;

    while (running) {
        displayStatus(this -> currPlayer, this -> board);
        std::cout << std::endl;

        // Display current players hand
        std::cout << "Your hand is" << std::endl;

        std::shared_ptr < LinkedList > currPlayerHand = this -> currPlayer -> getPlayerHand();

        for (int i = START; i < currPlayerHand -> getSize(); ++i) {
            std::cout << currPlayerHand -> getTile(i) -> toString();

            if (i != currPlayerHand -> getSize() - 1) {
                std::cout << ",";
            }
        }

        std::cout << std::endl;
        std::cout << std::endl;

        // Get current players input
        std::string input = "";
        std::cout << "> ";
        std::getline(std::cin >> std::ws, input);

        inputSuccess = readInput(currPlayer, input);

        if (inputSuccess) {
            std::cout << "You entered: " << input << std::endl;
            // End of turn
            currPlayer = currPlayer -> equals(players[START]) ? players[OTHER] : players[START];
        }

        if (this -> savedGame) {
            running = false;

            std::cout << std::endl;
            std::cout << "Game has been succesfully saved." << std::endl;
            std::cout << std::endl;
        } else if (tileBag -> getSize() == EMPTY) {
            bool hasPlayerFinished = false;

            for (int i = START; i < this -> playerCount; ++i) {
                if (this -> players[i] -> getPlayerHand() -> getSize() == EMPTY)
                    hasPlayerFinished = true;
            }

            if (hasPlayerFinished)
                running = false;
        }
    }
}

void GameEngine::setTileBag(std::shared_ptr < LinkedList > tileBag) {
    this -> tileBag = tileBag;
}

// Add a new player to the players structure. Only up to two players.
void GameEngine::addPlayer(std::shared_ptr < Player > newPlayer) {
    //adding player
    if (playerCount >= MIN_PLAYER_COUNT && playerCount < MAX_PLAYER_COUNT) {
        players[playerCount] = newPlayer;
        ++playerCount;
    }
}

std::shared_ptr < Player > GameEngine::getPlayer(int playerIndex) {
    std::shared_ptr < Player > outputPlayer = nullptr;

    if (playerIndex >= START && playerIndex < playerCount) {
        outputPlayer = players[playerIndex];
    }

    return outputPlayer;
}

bool GameEngine::placeTile(std::string tileValue, int boardRow, int boardCol) {
    //placing tile on
    bool successfulPlacement = false;
    // if game rule allows placement 
    if (checkTilePlacement(tileValue, boardRow, boardCol)) {
        if (board.at(boardRow).at(boardCol) == "  ") {
            board.at(boardRow).at(boardCol) = tileValue;
            successfulPlacement = true;
        } else {
            std::cout << std::endl;
            std::cout << "Position is occupied. Please try again." << std::endl;
            std::cout << std::endl;
        }
    } else {
        std::cout << "Invalid position. Please try again" << std::endl;
    }

    return successfulPlacement;
}

bool GameEngine::checkTilePlacement(std::string tileValue, int boardRowInt, int boardColInt) {
    bool positionValid = false;
    unsigned int boardRow;
    boardRow = boardRowInt;
    unsigned int boardCol;
    boardCol = boardColInt;

    if (boardRow >= MIN_BOARD_Y_DIMS && boardRow < board.size() &&
        boardCol >= MIN_BOARD_Y_DIMS && boardCol < board[START].size()) {
        if (this -> firstMove) {
            positionValid = true;
            this -> firstMove = false;
        }

        // if tile not already occupied
        else if (board.at(boardRow).at(boardCol) == "  ") {
            // Check if attribute matches
            // Check if line contains max tiles
            // Check if duplicate tile
            std::string tileColour = tileValue.substr(0, 1);
            std::string tileShape = tileValue.substr(1, 1);

            int lineCount = 0;
            bool duplicateTile = false;
            bool matchingAttribute = false;

            // Search South
            if (boardRow < board.size() - 1) {
                std::string southTileValue = board.at(boardRow + 1).at(boardCol);
                std::string southTileColour = southTileValue.substr(0, 1);
                std::string southTileShape = southTileValue.substr(1, 1);

                // Check attribute
                if (southTileColour == tileColour ||
                    southTileShape == tileShape) {
                    matchingAttribute = true;
                    std::string matchingAttributeValue = southTileColour == tileColour ? tileColour : tileShape;

                    bool withinBounds = true;
                    unsigned int currBoardRow;
                    currBoardRow = boardRow + 1;
                    ++lineCount;

                    // Check line length
                    while (withinBounds) {
                        std::string currBoardTileValue = board.at(currBoardRow).at(boardCol);
                        std::string currBoardTileColour = currBoardTileValue.substr(0, 1);
                        std::string currBoardTileShape = currBoardTileValue.substr(1, 1);

                        // Check if duplicate tile
                        if (board.at(currBoardRow).at(boardCol) == tileValue)
                            duplicateTile = true;

                        // Check if next tile is within the board
                        // And if it's not end of line

                        if (currBoardRow + 1 < board.size() &&
                            board.at(currBoardRow + 1).at(boardCol) != "  ") {
                            if ((matchingAttributeValue == tileColour &&
                                    currBoardTileColour != tileColour) ||
                                (matchingAttributeValue == tileShape &&
                                    currBoardTileShape != tileShape)) {
                                matchingAttribute = false;
                            }

                            ++currBoardRow;
                            ++lineCount;
                        } else {
                            withinBounds = false;
                        }
                    }
                }
            }

            // Search West
            if (boardCol > MIN_BOARD_X_DIMS) {
                std::string westTileValue = board.at(boardRow).at(boardCol - 1);
                std::string westTileColour = westTileValue.substr(0, 1);
                std::string westTileShape = westTileValue.substr(1, 1);

                // Check attribute
                if (westTileColour == tileColour ||
                    westTileShape == tileShape) {
                    matchingAttribute = true;
                    std::string matchingAttributeValue = westTileColour == tileColour ? tileColour : tileShape;

                    bool withinBounds = true;
                    int currBoardCol = boardCol - 1;
                    ++lineCount;

                    // Check line length
                    while (withinBounds) {
                        std::string currBoardTileValue = board.at(boardRow).at(currBoardCol);
                        std::string currBoardTileColour = currBoardTileValue.substr(0, 1);
                        std::string currBoardTileShape = currBoardTileValue.substr(1, 1);

                        // Check if duplicate tile
                        if (board.at(boardRow).at(currBoardCol) == tileValue)
                            duplicateTile = true;

                        // Check if next tile is within the board
                        // And if it's not end of line
                        if (currBoardCol - 1 >= MIN_BOARD_X_DIMS &&
                            board.at(boardRow).at(currBoardCol - 1) != "  ") {
                            if ((matchingAttributeValue == tileColour &&
                                    currBoardTileColour != tileColour) ||
                                (matchingAttributeValue == tileShape &&
                                    currBoardTileShape != tileShape)) {
                                matchingAttribute = false;
                            }

                            --currBoardCol;
                            ++lineCount;
                        } else {
                            withinBounds = false;
                        }
                    }
                }
            }

            // Search North
            if (boardRow > MIN_BOARD_Y_DIMS) {
                std::string northTileValue = board.at(boardRow - 1).at(boardCol);
                std::string northTileColour = northTileValue.substr(0, 1);
                std::string northTileShape = northTileValue.substr(1, 1);

                // Check attribute
                if (northTileColour == tileColour ||
                    northTileShape == tileShape) {
                    matchingAttribute = true;
                    std::string matchingAttributeValue = northTileColour == tileColour ? tileColour : tileShape;

                    bool withinBounds = true;
                    int currBoardRow = boardRow - 1;
                    ++lineCount;

                    // Check line length
                    while (withinBounds) {
                        std::string currBoardTileValue = board.at(currBoardRow).at(boardCol);
                        std::string currBoardTileColour = currBoardTileValue.substr(0, 1);
                        std::string currBoardTileShape = currBoardTileValue.substr(1, 1);

                        // Check if duplicate tile
                        if (board.at(currBoardRow).at(boardCol) == tileValue)
                            duplicateTile = true;

                        // Check if next tile is within the board
                        // And if it's not end of line
                        if (currBoardRow - 1 >= MIN_BOARD_Y_DIMS &&
                            board.at(currBoardRow).at(boardCol) != "  ") {
                            if ((matchingAttributeValue == tileColour &&
                                    currBoardTileColour != tileColour) ||
                                (matchingAttributeValue == tileShape &&
                                    currBoardTileShape != tileShape)) {
                                matchingAttribute = false;
                            }

                            --currBoardRow;
                            ++lineCount;
                        } else {
                            withinBounds = false;
                        }
                    }
                }
            }

            // Search East
            if (boardCol < board[START].size() - 1) {
                std::string eastTileValue = board.at(boardRow).at(boardCol + 1);
                std::string eastTileColour = eastTileValue.substr(0, 1);
                std::string eastTileShape = eastTileValue.substr(1, 1);

                // Check attribute
                if (eastTileColour == tileColour ||
                    eastTileShape == tileShape) {
                    matchingAttribute = true;
                    std::string matchingAttributeValue = eastTileColour == tileColour ? tileColour : tileShape;

                    bool withinBounds = true;
                    unsigned int currBoardCol;
                    currBoardCol = boardCol + 1;
                    ++lineCount;

                    // Check line length
                    while (withinBounds) {
                        std::string currBoardTileValue = board.at(boardRow).at(currBoardCol);
                        std::string currBoardTileColour = currBoardTileValue.substr(0, 1);
                        std::string currBoardTileShape = currBoardTileValue.substr(1, 1);

                        // Check if duplicate tile
                        if (currBoardTileValue == tileValue)
                            duplicateTile = true;

                        // Check if next tile is within the board
                        // And if it's not end of line
                        if (currBoardCol + 1 < board[START].size() &&
                            currBoardTileValue != "  ") {
                            if ((matchingAttributeValue == tileColour &&
                                    currBoardTileColour != tileColour) ||
                                (matchingAttributeValue == tileShape &&
                                    currBoardTileShape != tileShape)) {
                                matchingAttribute = false;
                            }

                            ++currBoardCol;
                            ++lineCount;
                        } else {
                            withinBounds = false;
                        }
                    }
                }
            }

            if (lineCount < 6 && !duplicateTile && matchingAttribute) {
                positionValid = true;
            }
        }
    }

    return positionValid;
}

// Add score to each player after each round
void GameEngine::addScore(std::shared_ptr < Player > currPlayer, int boardRow, int boardCol) {

    std::string tileColour = board[boardRow][boardCol].substr(0, 1);
    std::string tileShape = board[boardRow][boardCol].substr(1, 1);
    int countRow = 0;
    int countCol = 0;

    for (int i = 0; i < 13; ++i) {
        if (board[boardRow][i] != "  " && i != boardCol) {
            currPlayer -> setScore(currPlayer -> getScore() + 1);
        }

        if (board[boardRow][i].substr(0, 1) == tileColour ||
            board[boardRow][i].substr(1, 1) == tileShape) {
            countRow++;
        }
    }

    for (int i = 0; i < 13; ++i) {
        if (board[i][boardCol] != "  " && i != boardRow) {
            currPlayer -> setScore(currPlayer -> getScore() + 1);
        }
        if (board[i][boardCol].substr(0, 1) == tileColour ||
            board[i][boardCol].substr(1, 1) == tileShape) {
            countCol++;
        }
    }

    // Qwirkle!!!
    if (countCol == 6) {
        currPlayer -> setScore(currPlayer -> getScore() + 6);
    }

    if (countRow == 6) {
        currPlayer -> setScore(currPlayer -> getScore() + 6);
    }
    if (countCol == 6 || countRow == 6) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "QWIRKLE!!!" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    currPlayer -> setScore(currPlayer -> getScore() + 1);
}

void GameEngine::displayCurrentBoard(std::vector < std::vector < std::string > > board) {
    // Print Column Headers
    std::cout << " ";
    for (unsigned int i = START; i < board[START].size(); ++i) {
        std::cout << "  " << i;
    }
    std::cout << std::endl;

    // Print Column Divider
    std::cout << "   ";
    for (unsigned int i = START; i < board[START].size(); ++i) {
        std::cout << "---";
    }
    std::cout << std::endl;

    // Print Row Headers
    char currChar = 'A';
    int unicode_array[] = {
        31,
        42,
        93,
        92,
        94,
        95
    };
    char colours[] = {
        'R',
        'O',
        'Y',
        'G',
        'B',
        'P'
    };

    for (unsigned int i = START; i < board.size(); ++i) {
        std::cout << currChar << " |";

        for (unsigned int j = START; j < board[START].size(); ++j) {
            int stored_index = -1;
            for (int index = START; index < sizeof(colours) / sizeof(colours[0]); index++) {
                if (board[i][j].at(0) == colours[index]) {
                    stored_index = index;

                }
            }
            if (stored_index != -1) {
                if(stored_index!=1)
                {
                    std::string s = board[i][j];
                std::string color_Code = std::to_string(unicode_array[stored_index]);
                std::cout << "\033[1;" << color_Code << "m" << s << "\033[0m" << "|";
                }
                else
                {
                    
                    
                    
                     std::cout << "\033[38;5;202m"<<board[i][j]<<"\033[m"<< "|";
                }
                
            } else {
                std::cout << board[i][j] << "|";

            }

            //  else
            //  {
            //           std::string s="\033[1;31";
            // s=s+board[i][j];
            // s=s+"\033[0m"+"  ";
            // std::cout << s << "|";

            //  }

        }

        std::cout << std::endl;
        ++currChar;
    }
}

// Display status of currPlayer for each round
void GameEngine::displayStatus(std::shared_ptr < Player > currPlayer, std::vector < std::vector < std::string > > board) {
    std::cout << std::endl;
    std::cout << currPlayer -> getName() << ", it's your turn" << std::endl;

    std::cout << "Score for " << players[START] -> getName() <<
        ": " << players[START] -> getScore() << std::endl;
    std::cout << "Score for " << players[OTHER] -> getName() <<
        ": " << players[OTHER] -> getScore() << std::endl;

    this -> displayCurrentBoard(board);
}

// Return boolean of whether currPlayer made valid command.
bool GameEngine::readInput(std::shared_ptr < Player > currPlayer, std::string input) {
    bool successfulRead = false;
    std::vector < std::string > inputs;
    int begin = 0;

    for (unsigned int i = 0; i < input.size(); ++i) {
        if (input[i] == ' ' || i == input.size() - 1) {
            int wordLength = i == input.size() - 1 ? i - begin + 1 : i - begin;
            inputs.push_back(input.substr(begin, wordLength));
            begin = i + 1;
        }
    }

    if (inputs.empty()) {
        // Invalid input
        std::cout << "Invalid input. Please try again." << std::endl;
    } else if (inputs.size() >= 1) {

        if (inputs[0] == "help") {
            std::cout << "<===================================================================================================>\n"
            "\n\t\033[1;94m Introduction \033[0m\n\n"
            "The abstract game of Qwirkle consists of 108 wooden blocks with \n"
            "six different shapes in six different colors. \n"
            "There is no board, players simply use an available flat surface. \n"
            "Players begin the game with six blocks. The start player places blocks of a single \n"
            "matching attribute (color or shape but not both) on the table. Thereafter, \n"
            "a player adds blocks adjacent to at least one previously played block. \n"
            "The blocks must all be played in a line and match, without duplicates, \n"
            "either the color or shape of the previous block."
            "\n \n" 
            "To place a single tile use the following syntax : \n"
            "\t \033[38:5:202mplace <tile> at <grid location> \033[m \n"
             "Note : tile name and grid location must be in Uppercase\n" 
            "\n"
            "To place multiple tiles use the following syntax : \n"
            "\t \033[38:5:202mplace <tile> <tile> <tile> ... at <grid location> <grid location> <grid location>... \n\033[m"
            "Note : The tile location must map with the grid location & maximum 6 tiles can be placed at once\n"


            "\n\t\033[1;94m Adding Tiles \033[0m\n\n"
            "All tiles must connect to the grid and form a line of two or more tiles.\n"
            "A line is either all one shape or all one color.\n"
            "Tiles that are added to a line must share the same attribute as the tiles that are already in that line.\n"
            "You may play multiple tiles on your turn as long as all tiles played are\n"
            "the same color or the same shape and are placed in the same line.\n" 
            "You cannot play two tiles that are exactly the same.\n"
            "At least one of the tiles you play must touch (side to side) a tile\n" 
            "that has already been played and match the tile in color or shape.\n"
            "Any tiles that touch each other are part of a line.\n" 
            "Lines are either all one shape or all one color, without any duplicates.\n"

             "\n\t\033[1;94m Replace tile \033[0m\n\n"
             "To replace a tile follow the command:\n"
              "\t \033[38:5:202mreplace <tile> \033[m \n"

              "\n\t\033[1;94m Saving a file\033[0m\n\n"
             "To Save a file follow the command:\n"
              "\t \033[38:5:202msave <filename> \033[m \n"
              "Note: You dont have to put any extensions"



            "\n\t\033[1;94m Scoring \033[0m\n\n"
            "When you create a line, you score one point for each tile in that\n"
            "line (including the tiles that already existed on the grid).\n "
            "When a tile is part of two different lines, it will score 2 points.\n"
            "You score 6 bonus points whenever you complete a line of 6 tiles, which is called a Qwirkle.\n"
           "<===================================================================================================>"
            
            << std::endl;
            successfulRead = false;
        } else if (inputs[0] == "place") {
            // Place tile at ...
            if (inputs.size() >= 6) {

                std::vector < std::string > multiple_inputs;
                std::vector < std::string > multiple_positions;
                for (unsigned int i = START + 1; i < inputs.size() / 2; i++) {
                    multiple_inputs.push_back(inputs[i]);
                }

                for (unsigned int i = (inputs.size() / 2) + 1; i < inputs.size(); i++) {
                    multiple_positions.push_back(inputs[i]);
                }
                if (multiple_inputs.size() == multiple_positions.size()) {
                    for (unsigned i = START; i < multiple_inputs.size(); i++) {
                        if (currPlayer -> getPlayerHand() -> search(multiple_inputs[i]) != nullptr) {
                            std::string rowPositions[26];

                            char c = 'A';

                            // Get Row Position
                            for (unsigned int j = START; j < board.size(); ++j) {
                                rowPositions[j] = c;
                                ++c;
                            }

                            // Check if placement was successful and update score if so.
                            for (unsigned int j = START; j < board.size(); ++j) {
                                if (rowPositions[j].compare(multiple_positions[i].substr(0, 1)) == 0) {
                                    successfulRead = placeTile(multiple_inputs[i], j, std::stoi(multiple_positions[i].substr(1)));
                                    if (successfulRead) {
                                        successfulRead = replaceTile(currPlayer, multiple_inputs[i], false);
                                        addScore(currPlayer, j, std::stoi(multiple_positions[i].substr(1)));
                                    }
                                }
                            }
                        }
                        else {
                    std::cout << "You do not have this tile in your hand." << std::endl;
                    std::cout << std::endl;
                }


                    }

                }

            } else if (inputs.size() == 4 && inputs[3].size() >= 2) {
                if (currPlayer -> getPlayerHand() -> search(inputs[1]) != nullptr) {
                    std::string rowPositions[26];

                    char c = 'A';

                    // Get Row Position
                    for (unsigned int i = START; i < board.size(); ++i) {
                        rowPositions[i] = c;
                        ++c;
                    }

                    // Check if placement was successful and update score if so.
                    for (unsigned int i = START; i < board.size(); ++i) {
                        if (rowPositions[i].compare(inputs[3].substr(0, 1)) == 0) {
                            successfulRead = placeTile(inputs[1], i, std::stoi(inputs[3].substr(1)));
                            if (successfulRead) {
                                successfulRead = replaceTile(currPlayer, inputs[1], false);
                                addScore(currPlayer, i, std::stoi(inputs[3].substr(1)));
                            }
                        }
                    }
                } else {
                    std::cout << "You do not have this tile in your hand." << std::endl;
                    std::cout << std::endl;
                }
            } else {
                std::cout << std::endl;
                std::cout << "Invalid input. Please try again." << std::endl;
                std::cout << std::endl;
            }
        } else if (inputs[0] == "replace") {
            // Replace tile in player hand
            if (currPlayer -> getPlayerHand() -> search(inputs[1]) != nullptr) {
                successfulRead = replaceTile(currPlayer, inputs[1], true);
            } else {
                std::cout << "You do not have this tile in your hand." << std::endl;
                std::cout << std::endl;
            }
        } else if (inputs[0] == "save" && inputs.size() == 2) {
            // save the game
            std::ofstream file(inputs[1]);

            // Read in Player Info
            for (int i = START; i < playerCount; ++i) {
                file << players[i] -> getName() << '\n';
                file << players[i] -> getScore() << '\n';

                std::shared_ptr < LinkedList > playerHand = players[i] -> getPlayerHand();
                for (int j = START; j < playerHand -> getSize(); ++j) {
                    file << playerHand -> getTile(j) -> toString() << ", ";
                }

                file << '\n';
            }

            // Read in Current Board Shape
            file << board.size() << ", " << board[START].size() << '\n';

            // Read in Current Board State
            char boardRowHeader = 'A';

            for (unsigned int i = START; i < board.size(); ++i) {

                for (unsigned int j = START; j < board[START].size(); ++j) {
                    if (board[i][j] != "  ") {
                        file << board[i][j] << "@" << boardRowHeader << j << ", ";
                    }
                }

                ++boardRowHeader;
            }

            file << '\n';

            // Read in Tile Bag
            for (int i = START; i < tileBag -> getSize(); ++i) {
                file << tileBag -> getTile(i) -> toString() << ", ";
            }

            file << '\n';

            // Read in Current Player Name
            file << currPlayer -> getName() << '\n';
            file.close();

            this -> savedGame = true;
        } else {
            // Invalid input
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    return successfulRead;
}

// Return boolean if currPlayer can replace Tile in playerHand.
bool GameEngine::replaceTile(std::shared_ptr < Player > currPlayer,
    std::string tileValue, bool checkReplace) {
    std::shared_ptr < LinkedList > currPlayerHand = currPlayer -> getPlayerHand();

    bool handTileReplaced = false;

    // Check if currPlayer contains Tile in playerHand.
    for (int i = START; i < currPlayerHand -> getSize(); ++i) {
        if (currPlayerHand -> getTile(i) -> toString() == tileValue) {
            // Remove Tile from playerHand and place into Tile Bag
            // Draw Tile and place into playerHand.
            if (checkReplace) {
                tileBag -> addTile(currPlayerHand -> getTile(i));
            }

            currPlayerHand -> addTile(tileBag -> getTile(START));
            tileBag -> removeTile(START);
            currPlayerHand -> removeTile(i);
            handTileReplaced = true;
        }
    }

    return handTileReplaced;
}