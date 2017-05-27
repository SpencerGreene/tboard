/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TetrisBoard_h
#define TetrisBoard_h

#define DIMX 20
#define DIMY 40

// library interface description
class TetrisBoard
{
  // user-accessible "public" interface
  public:
    TetrisBoard();
    void placeTet(int, int, int, int);
    void rotTet();
    void dump();
    bool moveTet(int, int);
    void freezeTet();

  // library-accessible "private" interface
  private:
    int board[DIMX][DIMY];
    int tetMinX();
    int tetMaxX();
    int tetMinY();
    int _tetX, _tetY, _tetType, _tetRot, _tetColor;
};

#endif

