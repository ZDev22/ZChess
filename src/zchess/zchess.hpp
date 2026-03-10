/* licensed under GPL v3.0 see https://github.com/ZDev22/ZChess/blob/main/LICENSE for current license

ZChess is a chess engine written by ZDev, you can take your own peices but it's ok

#define MIDNIGHT_ZCHESS - queens can move like knights
*/

#ifndef ZCHESS_HPP
#define ZCHESS_HPP

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

inline unsigned char isPeiceBlack(unsigned int index) { return blackPeice[index]; }
inline bool isOnLeftEdge() { return peice % BOARD_SIZE == 0; }
inline bool isOnRightEdge() { return peice % BOARD_SIZE == BOARD_SIZE - 1; }
inline bool isOnTop() { return peice < BOARD_SIZE; }
inline bool isOnBottom() { return peice > (BOARD_SQUARES - BOARD_SIZE) - 1; }
inline bool isTwoFromLeftEdge() { return peice % BOARD_SIZE == 1; }
inline bool isTwoFromRightEdge() { return peice % BOARD_SIZE == BOARD_SIZE - 2; }
inline bool isTwoFromTop() { return peice < BOARD_SIZE * 2; }
inline bool isTwoFromBottom() { return peice > (BOARD_SQUARES - BOARD_SIZE * 2) - 1; }
unsigned char emptyTileTexture(unsigned int index) {
    if (index == 0) return 0;
    return (index + index / BOARD_SIZE) % 2;
}

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
    id = zwindow.getSizeX();
    float x = -.95f - (float)(id / BOARD_SIZE) / id;
    float y = -.88f;
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            x += ((float)(id / BOARD_SIZE) / id) * 1.3f;
            if (board[i * BOARD_SIZE + j] == 0) createSprite(squareModel, emptyTileTexture(i * BOARD_SIZE + j), x, y, .16f, .25f, 0.f);
            else createSprite(squareModel, board[i * BOARD_SIZE + j] + (isPeiceBlack(i * BOARD_SIZE + j) ? 7 : 1), x, y, .16f, .25f, 0.f);
            sprites[spritesSize - 1].setRotationMatrix();
        }
        x = -.95f - (float)(id / BOARD_SIZE) / id;
        y += ((float)(id / BOARD_SIZE) / id) * 2.f;
    }

    sprites[0].setTexture(std::make_unique<Texture>("brook.png"));
    sprites[1].setTexture(std::make_unique<Texture>("bknight.png"));
    sprites[2].setTexture(std::make_unique<Texture>("bbishop.png"));
    sprites[3].setTexture(std::make_unique<Texture>("bqueen.png"));
    sprites[4].setTexture(std::make_unique<Texture>("bking.png"));
    sprites[BOARD_SIZE].setTexture(std::make_unique<Texture>("bpawn.png"));
    sprites[BOARD_SQUARES - 1].setTexture(std::make_unique<Texture>("rook.png"));
    sprites[BOARD_SQUARES - 2].setTexture(std::make_unique<Texture>("knight.png"));
    sprites[BOARD_SQUARES - 3].setTexture(std::make_unique<Texture>("bishop.png"));
    sprites[BOARD_SQUARES - 5].setTexture(std::make_unique<Texture>("queen.png"));
    sprites[BOARD_SQUARES - 4].setTexture(std::make_unique<Texture>("king.png"));
    sprites[BOARD_SQUARES - BOARD_SIZE - 1].setTexture(std::make_unique<Texture>("pawn.png"));
    sprites[BOARD_SIZE * 2].setTexture(std::make_unique<Texture>("empty2.png"));
    zwindow.setName("ZChess");
}

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
    if (board[peice + move + 1] != 0 && !isOnRightEdge()) { movements.emplace_back(peice + move + 1); }
    if (board[peice + move - 1] != 0 && !isOnLeftEdge()) { movements.emplace_back(peice + move - 1); }

    /* check if they are en-passaunt-able */
    unsigned int LastMove = (unsigned int)(lastMove >> 32);
    if ((LastMove - 1 == peice || LastMove + 1 == peice) && /* the pawn that last moved is next the pawn you are moving */
        board[LastMove] == 1) { /* the peice next to you is a pawn */

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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnRightEdge()) stop = true;
        }
    }
}

void knightMovement() {
    if (!isOnBottom() && !isTwoFromBottom()) {
        if (!isOnLeftEdge()) movements.emplace_back((peice + BOARD_SIZE * 2) - 1);
        if (!isOnRightEdge()) movements.emplace_back((peice + BOARD_SIZE * 2) + 1);
    }

    if (!isOnTop() && !isTwoFromTop()) {
        if (!isOnLeftEdge()) movements.emplace_back((peice - BOARD_SIZE * 2) - 1);
        if (!isOnRightEdge()) movements.emplace_back((peice - BOARD_SIZE * 2) + 1);
    }

    if (!isOnRightEdge() && !isTwoFromRightEdge()) {
        if (!isOnTop()) movements.emplace_back(peice + 2 - BOARD_SIZE);
        if (!isOnBottom()) movements.emplace_back(peice + 2 + BOARD_SIZE);
    }

    if (!isOnLeftEdge() && !isTwoFromLeftEdge()) {
        if (!isOnTop()) movements.emplace_back(peice - 2 - BOARD_SIZE);
        if (!isOnBottom()) movements.emplace_back(peice - 2 + BOARD_SIZE);
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
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
            if (pos < 0 || pos >= BOARD_SQUARES) break;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnBottom() || isOnRightEdge()) stop = true;
        }
    }

    /* down <- movement*/
    stop = false;
    if (!isOnBottom() && !isOnLeftEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice + (i * BOARD_SIZE - i);
            if (pos < 0 || pos >= BOARD_SQUARES) break;
            movements.emplace_back(pos);
            if (board[pos] != 0) stop = true;
            if (isOnBottom() || isOnLeftEdge()) stop = true;
        }
    }
}

void kingMovement() {
    if (!isOnBottom()) { /* bottom movement */
        movements.emplace_back(peice + BOARD_SIZE);
        if (!isOnLeftEdge()) { movements.emplace_back(peice + BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { movements.emplace_back(peice + BOARD_SIZE + 1); }
    }

    if (!isOnTop()) { /* top movement */
        movements.emplace_back(peice - BOARD_SIZE);
        if (!isOnLeftEdge()) { movements.emplace_back(peice - BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { movements.emplace_back(peice - BOARD_SIZE + 1); }
    }

    /* side movement */
    if (!isOnLeftEdge()) { movements.emplace_back(peice - 1); }
    if (!isOnRightEdge()) { movements.emplace_back(peice + 1); }

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
                    sprites[to + pos].textureIndex = emptyTileTexture(to + pos);
                }
            }

            /* update board data */
            blackPeice[to] = isPeiceBlack(peice);
            blackPeice[peice] = 0;

            board[to] = board[peice];
            sprites[to].textureIndex = board[to] + (isPeiceBlack(to) ? 7 : 1);
            board[peice] = 0;
            sprites[peice].textureIndex = emptyTileTexture(peice);

            /* check for promotion */
            if (board[to] == 1) {
                if (isPeiceBlack(to)) {
                    if (to > BOARD_SQUARES - BOARD_SIZE - 1) {
                        board[to] = 5;
                        sprites[to].textureIndex = 12;
                    }
                }
                else {
                    if (to < BOARD_SIZE) {
                        board[to] = 5;
                        sprites[to].textureIndex = 6;
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
                            sprites[BOARD_SIZE - 1].textureIndex = emptyTileTexture(BOARD_SIZE - 1);
                            board[to - 1] = 2;
                            sprites[to - 1].textureIndex = 9;
                            blackPeice[to - 1] = 1;
                            blackPeice[BOARD_SIZE - 1] = 0;
                        }
                        else { /* king moved left, move right rook */
                            board[0] = 0;
                            sprites[0].textureIndex = emptyTileTexture(0);
                            board[to + 1] = 2;
                            sprites[to + 1].textureIndex = 9;
                            blackPeice[to + 1] = 1;
                            blackPeice[0] = 0;
                        }
                    }
                    else {
                        if (to - peice > 0) { /* king moved left, move left rook */
                            board[BOARD_SQUARES - 1] = (BOARD_SQUARES - 1) % 2;
                            sprites[BOARD_SQUARES - 1].textureIndex = emptyTileTexture(0);
                            board[to - 1] = 2;
                            sprites[to - 1].textureIndex = 3;
                        }
                        else { /* king moved left, move right rook */
                            board[BOARD_SQUARES - BOARD_SIZE] = (BOARD_SQUARES - BOARD_SIZE) % 2;
                            sprites[BOARD_SQUARES - BOARD_SIZE].textureIndex = emptyTileTexture(0);
                            board[to + 1] = 2;
                            sprites[to + 1].textureIndex = 3;
                        }
                    }
                }
            }

            /* check for checkmate */
            bool kingsAlive[2] = {0};

            for (unsigned int i = 0; i < BOARD_SQUARES; i++) {
                if (board[i] == 6) { kingsAlive[isPeiceBlack(i)] = 1; }
            }
            if (kingsAlive[0] && !kingsAlive[1]) { zwindow.setName("CHECKMATE! White wins!"); }
            else if (!kingsAlive[0] && kingsAlive[1]) { zwindow.setName("CHECKMATE! Black wins!"); }

            return;
        }
    }
}

unsigned int squareClicked() {
    if (zwindow.getMouseX() > .65f) return BOARD_SQUARES;
    return (unsigned int)((zwindow.getMouseX() * 1.5f) * BOARD_SIZE) + (unsigned int)(zwindow.getMouseY() * BOARD_SIZE) * BOARD_SIZE;
}

void tick() {
    if (zwindow.LMBHit()) { /* select peice */
        movements.clear();
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
#ifdef MIDNIGHT_ZCHESS
            knightMovement();
#endif
            break;
        }
        case 6: { kingMovement(); break; }
        }

        posx = sprites[peice].position[0];
        posy = sprites[peice].position[1];
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
        movePeice();
        lastMove = (unsigned int)peice + ((unsigned long long)to << 32);
        ZEngineSpriteRemap = true;
    }
}

private:
    ZWindow& zwindow;
};

#endif // ZCHESS_HPP
