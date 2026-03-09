#ifndef ZCHESS_HPP
#define ZCHESS_HPP

#include <iostream>
#include <string.h>
#include <vector>

#include "zengine.hpp"
#include "zwindow.hpp"

#define BOARD_SIZE 8 /* MIN BOARD SIZE: 8 | MAX BOARD SIZE: 255 */
#define BOARD_SQUARES BOARD_SIZE * BOARD_SIZE

unsigned char board[BOARD_SQUARES]      = {0}; /* create board */
bool blackPeice[BOARD_SQUARES]          = {0}; /* black peice positions */
unsigned long long lastMove             =  0 ; /* keep track of the last move played [(first 32 bits - where the peice was) | (second 32 bits - where the peice moved)]*/
std::vector<unsigned int> movements;           /* all possible movements for selected peice*/
bool movingPeice                        =  0 ; /* flag to know if your mvoing a peice */
bool kingMoved[2]                       = {0}; /* keep track if kings have moved, 0 = white, 1 = black */

long long peice  = 0  ;
unsigned int to  = 0  ;
unsigned int id  = 0  ;
float posx       = 0.f;
float posy       = 0.f;

/* PEICES
0 - empty
1 - pawn
2 - rook
3 - knight
4 - bishop
5 - queen
6 - king
*/

/* BOARD (BOARD_SIZE = 8)
0  1  2  3  4  5  6  7
8  9  10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
*/

struct ZChess {

public:

ZChess(ZWindow& zwindow) : zwindow(zwindow) {
    memset(&board[BOARD_SIZE], 1, BOARD_SIZE); /* create black pawns */
    memset(&board[BOARD_SQUARES - BOARD_SIZE * 2], 1, BOARD_SIZE); /* create white pawns*/
    board[0] = board[BOARD_SIZE - 1] = board[BOARD_SQUARES - BOARD_SIZE] = board[BOARD_SQUARES - 1] = 2; /* create rooks */
    board[1] = board[BOARD_SIZE - 2] = board[BOARD_SQUARES - BOARD_SIZE + 1] = board[BOARD_SQUARES - 2] = 3; /* create knights */
    board[2] = board[BOARD_SIZE - 3] = board[BOARD_SQUARES - BOARD_SIZE + 2] = board[BOARD_SQUARES - 3] = 4; /* create bishops */
    board[3] = board[BOARD_SQUARES - 5] = 5; /* create queens */
    board[4] = board[BOARD_SQUARES - 4] = 6; /* create kings*/
    memset(blackPeice, 1, BOARD_SQUARES / 2 - 1); /* set black peices */

    /* load sprites */
    id = 0;
    deleteSprite(id);
    id = zwindow.getSizeX() > zwindow.getSizeY() ? zwindow.getSizeY() : zwindow.getSizeX();
    float x = -1.f - (float)(id / BOARD_SIZE) / id;
    float y = -.88f;
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            x += ((float)(id / BOARD_SIZE) / id) * 2.f;
            createSprite(squareModel, board[i * BOARD_SIZE + j] + (isPeiceBlack(i * BOARD_SIZE + j) ? 6 : 0), x, y, .15f, .23f, 0.f);
            sprites[spritesSize - 1].setRotationMatrix();
        }
        x = -1.f - (float)(id / BOARD_SIZE) / id;
        y += ((float)(id / BOARD_SIZE) / id) * 2.f;
    }

    sprites[0].setTexture(std::make_unique<Texture>("brook.png"));
    sprites[1].setTexture(std::make_unique<Texture>("bknight.png"));
    sprites[2].setTexture(std::make_unique<Texture>("bbishop.png"));
    sprites[4].setTexture(std::make_unique<Texture>("bqueen.png"));
    sprites[3].setTexture(std::make_unique<Texture>("bking.png"));
    sprites[BOARD_SIZE].setTexture(std::make_unique<Texture>("bpawn.png"));
    sprites[BOARD_SQUARES - 1].setTexture(std::make_unique<Texture>("rook.png"));
    sprites[BOARD_SQUARES - 2].setTexture(std::make_unique<Texture>("knight.png"));
    sprites[BOARD_SQUARES - 3].setTexture(std::make_unique<Texture>("bishop.png"));
    sprites[BOARD_SQUARES - 4].setTexture(std::make_unique<Texture>("queen.png"));
    sprites[BOARD_SQUARES - 5].setTexture(std::make_unique<Texture>("king.png"));
    sprites[BOARD_SQUARES - BOARD_SIZE - 1].setTexture(std::make_unique<Texture>("pawn.png"));
    for (unsigned int i = BOARD_SIZE * 2;; i++) {
        if (board[i] != 0) break;
        sprites[i].textureIndex = 0;
    }
}

inline unsigned char isPeiceBlack(unsigned int index) { return blackPeice[index]; }
inline bool isOnLeftEdge() { return peice % BOARD_SIZE == 0; }
inline bool isOnRightEdge() { return peice % BOARD_SIZE == BOARD_SIZE - 1; }
inline bool isOnTop() { return peice < BOARD_SIZE; }
inline bool isOnBottom() { return peice > (BOARD_SQUARES - BOARD_SIZE) - 1; }
inline bool isTwoFromLeftEdge() { return peice % BOARD_SIZE == 1; }
inline bool isTwoFromRightEdge() { return peice % BOARD_SIZE == BOARD_SIZE - 2; }
inline bool isTwoFromTop() { return peice < BOARD_SIZE * 2; }
inline bool isTwoFromBottom() { return peice > (BOARD_SQUARES - BOARD_SIZE * 2) - 1; }

void pawnMovement() {
    short move = isPeiceBlack(peice) ? BOARD_SIZE : -BOARD_SIZE;

    /* check if pawn can move forward */
    if (board[peice + move] == 0) {
        movements.emplace_back(peice + move);

        if ((move == BOARD_SIZE && peice - BOARD_SIZE < BOARD_SIZE) || /* check if black pawn is on home row */
            (move == -BOARD_SIZE && peice > BOARD_SQUARES - (BOARD_SIZE * 2 + 1))) { /* check if white pawn is on home row */

            if (board[peice + move * 2] == 0) { movements.emplace_back(peice + move * 2); } /* if so, allow moving two spaces */
        }
    }

    /* check if pawn can capture */
    if (board[peice + move + 1] != 0 && !isOnLeftEdge()) { movements.emplace_back(peice + move + 1); }
    if (board[peice + move - 1] != 0 && !isOnRightEdge()) { movements.emplace_back(peice + move - 1); }

    /* check if they are en-passaunt-able */
    unsigned int LastMove = (unsigned int)(lastMove >> 32);
    //std::cout << LastMove << " | " << peice << std::endl;
    if ((LastMove - 1 == peice || LastMove + 1 == peice) && /* the pawn that last moved is next the pawn you are moving */
        board[LastMove] == 1) { /* the peice next to you is a pawn */

        //std::cout << (unsigned int)lastMove << " | " << LastMove + (move * 2) << std::endl;
        if ((unsigned int)lastMove == LastMove + (move * 2)) { /* the pawn moved two spaces the last move */
            if (!(move == (BOARD_SIZE ^ !isPeiceBlack(LastMove)))) { /* both pawns are different colors */
                movements.emplace_back(LastMove + move); /* en-passaunt */
            }
        }
    }
}

void rookMovement() {
    /* up movement*/
    bool stop = false;
    unsigned int pos = 0;
    if (!isOnTop()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE);
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnTop()) stop = true;
        }
    }

    /* down movement*/
    stop = false;
    if (!isOnBottom()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice + i * BOARD_SIZE;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnBottom()) stop = true;
        }
    }

    /* left movement*/
    stop = false;
    if (!isOnLeftEdge()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice - i;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnLeftEdge()) stop = true;
        }
    }

    /* right movement*/
    stop = false;
    if (!isOnRightEdge()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice + i;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnRightEdge()) stop = true;
        }
    }
}

void knightMovement() {
    if (!isOnBottom() && !isTwoFromBottom()) { /* make sure the knight isnt on the bottom of the board */
        if (!isOnLeftEdge()) { /* make sure it's not on the left edge */
            movements.emplace_back((peice + BOARD_SIZE * 2) - 1);
        }
        if (!isOnRightEdge()) { /* make sure it's not on the right edge */
            movements.emplace_back((peice + BOARD_SIZE * 2) + 1);
        }
    }

    if (!isOnTop() && !isTwoFromTop()) { /* make sure the knight isnt on the top of the board */
        if (!isOnLeftEdge()) {
            movements.emplace_back((peice - BOARD_SIZE * 2) - 1);
        }
        if (!isOnRightEdge()) {
            movements.emplace_back((peice - BOARD_SIZE * 2) + 1);
        }
    }

    if (!isOnRightEdge() && !isTwoFromRightEdge()) { /* make sure the knight isnt on the */
        if (!isOnTop()) {
            movements.emplace_back(peice + 2 - BOARD_SIZE);
        }
        if (!isOnBottom()) {
            movements.emplace_back(peice + 2 + BOARD_SIZE);
        }
    }

    if (!isOnLeftEdge() && !isTwoFromLeftEdge()) {
        if (!isOnTop()) {
            movements.emplace_back(peice - 2 - BOARD_SIZE);
        }
        if (!isOnBottom()) {
            movements.emplace_back(peice - 2 + BOARD_SIZE);
        }
    }
}

void bishopMovement() {
    /* up -> movement*/
    bool stop = false;
    unsigned int pos = 0;
    if (!isOnTop() && !isOnRightEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE - i);
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnTop() || isOnRightEdge()) stop = true;
        }
    }

    /* up <- movement*/
    stop = false;
    if (!isOnTop() && !isOnLeftEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE + i);
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnTop() || isOnLeftEdge()) stop = true;
        }
    }

    /* down -> movement*/
    stop = false;
    if (!isOnBottom() && !isOnRightEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice + (i * BOARD_SIZE + i);
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnBottom() || isOnRightEdge()) stop = true;
        }
    }

    /* down <- movement*/
    stop = false;
    if (!isOnBottom() && !isOnLeftEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < 255; i++) {
            if (stop) break;
            pos = peice + (i * BOARD_SIZE - i);
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnBottom() || isOnLeftEdge()) stop = true;
        }
    }
}

void kingMovement() {
    /* down movement */
    if (!isOnBottom()) {
        movements.emplace_back(peice + BOARD_SIZE);
        if (!isOnLeftEdge()) { movements.emplace_back(peice + BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { movements.emplace_back(peice + BOARD_SIZE + 1); }
    }

    /* up movement */
    if (!isOnTop()) {
        movements.emplace_back(peice - BOARD_SIZE);
        if (!isOnLeftEdge()) { movements.emplace_back(peice - BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { movements.emplace_back(peice - BOARD_SIZE + 1); }
    }

    /* side movement */
    if (!isOnLeftEdge()) { movements.emplace_back(peice - 1); }
    if (!isOnRightEdge()) { movements.emplace_back(peice + 1); }

    //board[0] = board[BOARD_SIZE - 1] = board[BOARD_SQUARES - BOARD_SIZE] = board[BOARD_SQUARES - 1] = 2; /* create rooks */

    /* castling */
    if (isPeiceBlack(peice)) {
        if (!kingMoved[1]) { /* king hasn't moved */
            if (board[0] == 2) movements.emplace_back(2);
            if (board[BOARD_SIZE - 1] == 2) movements.emplace_back(BOARD_SIZE - 2);
        }
    }
    else {
        if (!kingMoved[0]) { /* king hasn't moved  */
            if (board[BOARD_SQUARES - 1] == 2) movements.emplace_back(BOARD_SQUARES - 2);
            if (board[BOARD_SQUARES - BOARD_SIZE] == 2) movements.emplace_back(BOARD_SQUARES - BOARD_SIZE + 2);
        }
    }
}

void movePeice() {
    for (unsigned int i = 0; i < movements.size(); i++) {
        if (to == movements[i]) {
            if (board[peice] == 1) { /* check for en-passaunt */
                if (board[to] == 0 && abs(to - peice) % BOARD_SIZE != 0) { /* the pawn moved to an empty square, but also took */
                    long long pos = isPeiceBlack(peice) ? -BOARD_SIZE : BOARD_SIZE;
                    board[to + pos] = 0;
                    sprites[to + pos].textureIndex = 0;
                }
            }

            blackPeice[to] = isPeiceBlack(peice);
            blackPeice[peice] = 0;

            board[to] = board[peice];
            sprites[to].textureIndex = board[to] + (isPeiceBlack(to) ? 6 : 0);
            board[peice] = 0;
            sprites[peice].textureIndex = 0;

            if (board[to] == 1) {
                if (isPeiceBlack(to)) {
                    if (to > BOARD_SQUARES - BOARD_SIZE - 1) {
                        board[to] = 5;
                        sprites[to].textureIndex = 5;
                    }
                }
                else {
                    if (to < BOARD_SIZE) {
                        board[to] = 5;
                        sprites[to].textureIndex = 5;
                    }
                }
            }

            /* check for castling */
            if (board[to] == 6) {
                kingMoved[isPeiceBlack(to)] = 1; /* keep track of king movement */

                if (abs(to - peice) < BOARD_SIZE - 1 && abs(to - peice) > 1) { /* the king castled ( -1 for diagonals)*/
                    if (isPeiceBlack(to)) {
                        if (to - peice > 0) { /* king moved left, move left rook */
                            board[BOARD_SIZE - 1] = 0;
                            sprites[BOARD_SIZE - 1].textureIndex = 0;
                            board[to - 1] = 2;
                            sprites[to - 1].textureIndex = 8;
                        }
                        else {
                            board[0] = 0;
                            sprites[0].textureIndex = 0;
                            board[to + 1] = 2;
                            sprites[to + 1].textureIndex = 8;
                        }
                    }
                    else {
                        if (to - peice > 0) { /* king moved left, move left rook */
                            board[BOARD_SQUARES - 1] = 0;
                            sprites[BOARD_SQUARES - 1].textureIndex = 0;
                            board[to - 1] = 2;
                            sprites[to - 1].textureIndex = 2;
                        }
                        else {
                            board[BOARD_SQUARES - BOARD_SIZE] = 0;
                            sprites[BOARD_SQUARES - BOARD_SIZE].textureIndex = 0;
                            board[to + 1] = 2;
                            sprites[to + 1].textureIndex = 2;
                        }
                    }
                }
            }

            /* check if a king was taken*/
            bool kingsAlive[2] = {0};

            for (unsigned int i = 0; i < BOARD_SQUARES; i++) {
                if (board[i] == 6) { kingsAlive[isPeiceBlack(i)] = 1; }
            }
            if (kingsAlive[0] && !kingsAlive[1]) { std::cout << "CHECKMATE! White wins!"; }
            else if (!kingsAlive[0] && kingsAlive[1]) { std::cout << "CHECKMATE! Black wins!"; }

            return;
        }
    }
}

inline unsigned int squareClicked() { return (unsigned int)(zwindow.getMouseX() * BOARD_SIZE) + (unsigned int)(zwindow.getMouseY() * BOARD_SIZE) * BOARD_SIZE; }

void tick() {
    if (zwindow.LMBHit()) { /* select peice */
        peice = squareClicked();
 
        switch(board[peice]) {
        case 0: { break; }
        case 1: { pawnMovement(); break; }
        case 2: { rookMovement(); break; }
        case 3: { knightMovement(); break; }
        case 4: { bishopMovement(); break; }
        case 5: { /* queen */
            rookMovement();
            bishopMovement();
            break;
        }
        case 6: { kingMovement(); break; }
        }

        posx = sprites[peice].position[0];
        posy = sprites[peice].position[1];
        ZEngineSpriteRemap = true;
    }
    else if (zwindow.LMBPressed()) { /* drag peice */
        sprites[peice].position[0] = -1.f + zwindow.getMouseX() * 2.f;
        sprites[peice].position[1] = -1.f + zwindow.getMouseY() * 2.f;
        ZEngineSpriteRemap = true;
    }
    else if (zwindow.LMBReleased()) { /* place peice */
        sprites[peice].position[0] = posx;
        sprites[peice].position[1] = posy;

        to = squareClicked();
        unsigned long long newMove = to;
        movePeice();
        lastMove = (unsigned int)peice + (newMove << 32);
        movements.clear();
        ZEngineSpriteRemap = true;
    }
}

private:
    ZWindow& zwindow;
};

#endif // ZCHESS_HPP
