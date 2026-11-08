/* licensed under GPL v3.0 see https://github.com/ZDev22/ZChess/blob/main/LICENSE for current license

ZChess is a chess engine written by ZDev, you can take your own peices but it ok

#define MIDNIGHT_ZCHESS - queens can move like knights
*/

#ifndef ZCHESS_H
#define ZCHESS_H

#include "../zengine.h"

#define BOARD_SIZE 8 /* MIN BOARD SIZE: 8 | MAX BOARD SIZE: 255  (MAX_SPRITES is capped at 65 in main.cpp)*/
#define BOARD_SQUARES BOARD_SIZE * BOARD_SIZE

unsigned char board[BOARD_SQUARES]      = {0}; /* create board */
_Bool blackPeice[BOARD_SQUARES]         = {0}; /* black peice positions */
unsigned long long lastMove             =  0 ; /* keep track of the last move played [(first 32 bits - where the peice was) | (second 32 bits - where the peice moved)]*/
unsigned int movements[100];                   /* all possible movements for selected peice */
unsigned char movementSize              =  0 ; /* keep track of how many items are in the movements list */
_Bool movingPeice                       =  0 ; /* flag to know if your mvoing a peice */
_Bool kingMoved[2]                      = {0}; /* keep track if kings have moved, 0 = white, 1 = black */
unsigned int promotion        = BOARD_SQUARES; /* the position of the promoted pawn */
Sprite* boardSprites[BOARD_SQUARES]  = {NULL}; /* array of all the sprites for the pieces on the board */
Sprite* boardSprite                    = NULL; /* the actual board itself, pretty boring */

long long peice  = 0  ;
unsigned int to  = 0  ;
int id           = 0  ;
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

float getMouseX(float aspect) {
    i32 x = 0;
    i32 sizex = 0;
    RGFW_window_getMouse(zwindow, &x, NULL);
    RGFW_window_getSize(zwindow, &sizex, NULL);
    return ((((float)x / sizex) * 2.f) - 1.f) * aspect;
}
float getMouseY() {
    i32 y = 0;
    i32 sizey = 0;
    RGFW_window_getMouse(zwindow, NULL, &y);
    RGFW_window_getSize(zwindow, NULL, &sizey);
    return (((float)y / sizey) * 2.f) - 1.f;
}

unsigned char isPeiceBlack(unsigned int index) { return blackPeice[index]; }
_Bool isOnLeftEdge(void) { return peice % BOARD_SIZE == 0; }
_Bool isOnRightEdge(void) { return peice % BOARD_SIZE == BOARD_SIZE - 1; }
_Bool isOnTop(void) { return peice < BOARD_SIZE; }
_Bool isOnBottom(void) { return peice > (BOARD_SQUARES - BOARD_SIZE) - 1; }
_Bool isTwoFromLeftEdge(void) { return peice % BOARD_SIZE == 1; }
_Bool isTwoFromRightEdge(void) { return peice % BOARD_SIZE == BOARD_SIZE - 2; }
_Bool isTwoFromTop(void) { return peice < BOARD_SIZE * 2; }
_Bool isTwoFromBottom(void) { return peice > (BOARD_SQUARES - BOARD_SIZE * 2) - 1; }

void initZChess(void) {
    memset(&board[BOARD_SIZE], 1, BOARD_SIZE); /* create black pawns */
    memset(&board[BOARD_SQUARES - BOARD_SIZE * 2], 1, BOARD_SIZE); /* create white pawns*/
    board[0] = board[BOARD_SIZE - 1] = board[BOARD_SQUARES - BOARD_SIZE] = board[BOARD_SQUARES - 1] = 2; /* create rooks */
    board[1] = board[BOARD_SIZE - 2] = board[BOARD_SQUARES - BOARD_SIZE + 1] = board[BOARD_SQUARES - 2] = 3; /* create knights */
    board[2] = board[BOARD_SIZE - 3] = board[BOARD_SQUARES - BOARD_SIZE + 2] = board[BOARD_SQUARES - 3] = 4; /* create bishops */
    board[3] = board[BOARD_SQUARES - 5] = 5; /* create queens */
    board[4] = board[BOARD_SQUARES - 4] = 6; /* create kings*/
    memset(blackPeice, 1, BOARD_SQUARES / 2 - 1); /* set black peices */

    /* load sprites */
    boardSprite = createSprite(-.13f, 0.f, 2.01f, 2.f, 0.f, 0);

    RGFW_window_getSize(zwindow, &id, NULL);
    float x = -1.f;
    float y = -.88f;
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (board[i * BOARD_SIZE + j] == 0) { boardSprites[(i * BOARD_SIZE) + j] = createSprite(x, y, .22f, .22f, 0.f, 1); }
            else { boardSprites[(i * BOARD_SIZE) + j] = createSprite(x, y, .22f, .22f, 0.f, board[i * BOARD_SIZE + j] + (isPeiceBlack(i * BOARD_SIZE + j) ? 7 : 1)); }
            x += ((float)(id / BOARD_SIZE) / id) * 2.f;
        }
        x = -1.f;
        y += ((float)(id / BOARD_SIZE) / id) * 2.02f;
    }

    createTexture("assets/img/chessboard.png", 1.f, 0);
    createTexture("assets/img/pawn.png", 1.f, 2);
    createTexture("assets/img/rook.png", 1.f, 3);
    createTexture("assets/img/knight.png", 1.f, 4);
    createTexture("assets/img/bishop.png", 1.f, 5);
    createTexture("assets/img/queen.png", 1.f, 6);
    createTexture("assets/img/king.png", 1.f, 7);
    createTexture("assets/img/bpawn.png", 1.f, 8);
    createTexture("assets/img/brook.png", 1.f, 9);
    createTexture("assets/img/bknight.png", 1.f, 10);
    createTexture("assets/img/bbishop.png", 1.f, 11);
    createTexture("assets/img/bqueen.png", 1.f, 12);
    createTexture("assets/img/bking.png", 1.f, 13);
}

void addMove(unsigned int move) {
    movements[movementSize] = move;
    movementSize++;
}

void pawnMovement() {
    short move = isPeiceBlack(peice) ? BOARD_SIZE : -BOARD_SIZE;

    /* check if pawn can move forward */
    if (board[peice + move] == 0) {
        addMove(peice + move);

        if ((move == BOARD_SIZE && peice - BOARD_SIZE < BOARD_SIZE) || /* check if black pawn is on home row */
            (move == -BOARD_SIZE && peice > BOARD_SQUARES - (BOARD_SIZE * 2 + 1))) { /* check if white pawn is on home row */

            if (board[peice + move * 2] == 0) { addMove(peice + move * 2); } /* if so, allow moving two spaces */
        }
    }

    /* check if pawn can capture */
    if (board[peice + move + 1] != 0 && !isOnRightEdge()) { addMove(peice + move + 1); }
    if (board[peice + move - 1] != 0 && !isOnLeftEdge()) { addMove(peice + move - 1); }

    /* check if they are en-passaunt-able */
    unsigned int LastMove = (unsigned int)(lastMove >> 32);
    if ((LastMove - 1 == peice || LastMove + 1 == peice) && /* the pawn that last moved is next the pawn you are moving */
        board[LastMove] == 1) { /* the peice next to you is a pawn */

        if ((unsigned int)lastMove == LastMove + (move * 2)) { /* the pawn moved two spaces the last move */
            if (!(move == (BOARD_SIZE ^ !isPeiceBlack(LastMove)))) { /* both pawns are different colors */
                addMove(LastMove + move); /* en-passaunt */
            }
        }
    }
}

void rookMovement() {
    /* up movement*/
    _Bool stop = 0;
    unsigned int pos = 0;
    if (!isOnTop()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE);
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnTop()) stop = 1;
        }
    }

    /* down movement*/
    stop = 0;
    if (!isOnBottom()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice + i * BOARD_SIZE;
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnBottom()) stop = 1;
        }
    }

    /* left movement*/
    stop = 0;
    if (!isOnLeftEdge()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice - i;
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnLeftEdge()) stop = 1;
        }
    }

    /* right movement*/
    stop = 0;
    if (!isOnRightEdge()) { /* make sure the rook isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SIZE; i++) {
            if (stop) break;
            pos = peice + i;
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnRightEdge()) stop = 1;
        }
    }
}

void knightMovement() {
    if (!isOnBottom() && !isTwoFromBottom()) {
        if (!isOnLeftEdge()) addMove((peice + BOARD_SIZE * 2) - 1);
        if (!isOnRightEdge()) addMove((peice + BOARD_SIZE * 2) + 1);
    }

    if (!isOnTop() && !isTwoFromTop()) {
        if (!isOnLeftEdge()) addMove((peice - BOARD_SIZE * 2) - 1);
        if (!isOnRightEdge()) addMove((peice - BOARD_SIZE * 2) + 1);
    }

    if (!isOnRightEdge() && !isTwoFromRightEdge()) {
        if (!isOnTop()) addMove(peice + 2 - BOARD_SIZE);
        if (!isOnBottom()) addMove(peice + 2 + BOARD_SIZE);
    }

    if (!isOnLeftEdge() && !isTwoFromLeftEdge()) {
        if (!isOnTop()) addMove(peice - 2 - BOARD_SIZE);
        if (!isOnBottom()) addMove(peice - 2 + BOARD_SIZE);
    }
}

void bishopMovement() {
    /* up -> movement*/
    _Bool stop = 0;
    unsigned int pos = 0;
    if (!isOnTop() && !isOnRightEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE - i);
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnTop() || isOnRightEdge()) stop = 1;
        }
    }

    /* up <- movement*/
    stop = 0;
    if (!isOnTop() && !isOnLeftEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice - (i * BOARD_SIZE + i);
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnTop() || isOnLeftEdge()) stop = 1;
        }
    }

    /* down -> movement*/
    stop = 0;
    if (!isOnBottom() && !isOnRightEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice + (i * BOARD_SIZE + i);
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnBottom() || isOnRightEdge()) stop = 1;
        }
    }

    /* down <- movement*/
    stop = 0;
    if (!isOnBottom() && !isOnLeftEdge()) { /* make sure the bishop isnt on the edge of the board */
        for (unsigned int i = 1; i < BOARD_SQUARES; i++) {
            if (stop) break;
            pos = peice + (i * BOARD_SIZE - i);
            if (pos >= BOARD_SQUARES) break;
            addMove(pos);
            if (board[pos] != 0) stop = 1;
            if (isOnBottom() || isOnLeftEdge()) stop = 1;
        }
    }
}

void kingMovement() {
    if (!isOnBottom()) { /* bottom movement */
        addMove(peice + BOARD_SIZE);
        if (!isOnLeftEdge()) { addMove(peice + BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { addMove(peice + BOARD_SIZE + 1); }
    }

    if (!isOnTop()) { /* top movement */
        addMove(peice - BOARD_SIZE);
        if (!isOnLeftEdge()) { addMove(peice - BOARD_SIZE - 1); }
        if (!isOnRightEdge()) { addMove(peice - BOARD_SIZE + 1); }
    }

    /* side movement */
    if (!isOnLeftEdge()) { addMove(peice - 1); }
    if (!isOnRightEdge()) { addMove(peice + 1); }

    /* castling */
    if (isPeiceBlack(peice)) {
        if (!kingMoved[1]) { /* king hasn't moved */
            if (board[0] == 2) addMove(2);
            if (board[BOARD_SIZE - 1] == 2) addMove(BOARD_SIZE - 2);
        }
    }
    else {
        if (!kingMoved[0]) { /* king hasn't moved  */
            if (board[BOARD_SQUARES - 1] == 2) addMove(BOARD_SQUARES - 2);
            if (board[BOARD_SQUARES - BOARD_SIZE] == 2) addMove(BOARD_SQUARES - BOARD_SIZE + 2);
        }
    }
}

void checkPeice(unsigned int index) {
    movementSize = 0;

    switch(board[index]) {
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
}

void movePeice() {
    for (unsigned int i = 0; i < movementSize; i++) {
        if (to == movements[i]) {
            if (board[peice] == 1) { /* check for en-passaunt */
                if (board[to] == 0 && abs(to - peice) % BOARD_SIZE != 0) { /* the pawn moved to an empty square, but also took */
                    long long pos = isPeiceBlack(peice) ? -BOARD_SIZE : BOARD_SIZE;
                    board[to + pos] = 0;
                    boardSprites[to + pos]->textureIndex = 1;
                }
            }

            /* update board data */
            blackPeice[to] = isPeiceBlack(peice);
            blackPeice[peice] = 0;

            board[to] = board[peice];
            boardSprites[to]->textureIndex = board[to] + (isPeiceBlack(to) ? 7 : 1);
            board[peice] = 0;
            boardSprites[peice]->textureIndex = 1;

            /* check for promotion */
            promotion = BOARD_SQUARES;
            if (board[to] == 1) {
                if (isPeiceBlack(to)) {
                    if (to > BOARD_SQUARES - BOARD_SIZE - 1) {
                        board[to] = 5;
                        boardSprites[to]->textureIndex = 12;
                        promotion = to;
                        RGFW_window_setName(zwindow, "PROMOTION! 0 = pawn, 1 = rook, 2 = bishop, 3 = knight, 4 = queen, 5 = king");
                    }
                }
                else {
                    if (to < BOARD_SIZE) {
                        board[to] = 5;
                        boardSprites[to]->textureIndex = 6;
                        promotion = to;
                        RGFW_window_setName(zwindow, "PROMOTION! 0 = pawn, 1 = rook, 2 = bishop, 3 = knight, 4 = queen, 5 = king");
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
                            boardSprites[BOARD_SIZE - 1]->textureIndex = 1;
                            board[to - 1] = 2;
                            boardSprites[to - 1]->textureIndex = 9;
                            blackPeice[to - 1] = 1;
                            blackPeice[BOARD_SIZE - 1] = 0;
                        }
                        else { /* king moved left, move right rook */
                            board[0] = 0;
                            boardSprites[0]->textureIndex = 1;
                            board[to + 1] = 2;
                            boardSprites[to + 1]->textureIndex = 9;
                            blackPeice[to + 1] = 1;
                            blackPeice[0] = 0;
                        }
                    }
                    else {
                        if (to - peice > 0) { /* king moved left, move left rook */
                            board[BOARD_SQUARES - 1] = (BOARD_SQUARES - 1) % 2;
                            boardSprites[BOARD_SQUARES - 1]->textureIndex = 1;
                            board[to - 1] = 2;
                            boardSprites[to - 1]->textureIndex = 3;
                        }
                        else { /* king moved left, move right rook */
                            board[BOARD_SQUARES - BOARD_SIZE] = (BOARD_SQUARES - BOARD_SIZE) % 2;
                            boardSprites[BOARD_SQUARES - BOARD_SIZE]->textureIndex = 1;
                            board[to + 1] = 2;
                            boardSprites[to + 1]->textureIndex = 3;
                        }
                    }
                }
            }

            /* check for checkmate */
            _Bool kingsAlive[2] = {0};
            unsigned int kingPos[2] = {65}; /* keep track of king position for checks */

            for (unsigned int i = 0; i < BOARD_SQUARES; i++) {
                if (board[i] == 6) {
                    kingsAlive[isPeiceBlack(i)] = 1;
                    kingPos[isPeiceBlack(i)] = i;
                }
            }
            if (kingsAlive[0] && !kingsAlive[1]) { RGFW_window_setName(zwindow, "CHECKMATE! White wins!"); }
            else if (!kingsAlive[0] && kingsAlive[1]) { RGFW_window_setName(zwindow, "CHECKMATE! Black wins!"); }

            return;
        }
    }
}

unsigned int squareClicked() {
    if (getMouseX(camera.aspect) < -1.125f || getMouseX(camera.aspect) > .88f) return BOARD_SQUARES;
    return (unsigned int)((((getMouseX(camera.aspect) + 1.125f) * 1.13f) * BOARD_SIZE) / 2.25f) + (unsigned int)((getMouseY() + 1.f) * BOARD_SIZE / 2) * BOARD_SIZE;
}

void tickZChess() {
    if (RGFW_isMousePressed(RGFW_mouseLeft)) { /* select peice */
        peice = squareClicked();
        checkPeice(peice);

        posx = boardSprites[peice]->position[0];
        posy = boardSprites[peice]->position[1];
    }
    else if (RGFW_isMouseDown(RGFW_mouseLeft)) { /* drag peice */
        boardSprites[peice]->position[0] = getMouseX(camera.aspect);
        boardSprites[peice]->position[1] = getMouseY();
    }
    else if (RGFW_isMouseReleased(RGFW_mouseLeft)) { /* place peice */
        boardSprites[peice]->position[0] = posx;
        boardSprites[peice]->position[1] = posy;

        to = squareClicked();
        movePeice();
        lastMove = (unsigned int)peice + ((unsigned long long)to << 32);
    }
    else if (promotion != BOARD_SQUARES) {
        if (RGFW_isKeyPressed(RGFW_key0)) {
            board[promotion] = 1;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 8 : 2;
        }
        else if (RGFW_isKeyPressed(RGFW_key1)) {
            board[promotion] = 2;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 9 : 3;
        }
        else if (RGFW_isKeyPressed(RGFW_key2)) {
            board[promotion] = 3;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 10 : 4;
        }
        else if (RGFW_isKeyPressed(RGFW_key3)) {
            board[promotion] = 4;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 11 : 5;
        }
        else if (RGFW_isKeyPressed(RGFW_key4)) {
            board[promotion] = 5;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 12 : 6;
        }
        else if (RGFW_isKeyPressed(RGFW_key5)) {
            board[promotion] = 6;
            boardSprites[promotion]->textureIndex = isPeiceBlack(promotion) ? 13 : 7;
        }
    }
}

#endif // ZCHESS_H
