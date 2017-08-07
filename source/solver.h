#ifndef SOLVER_H
#define SOLVER_H
#pragma once
#include <iostream>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

namespace solver{
    using grdPtr = char[9][10];
    typedef char byte;
    typedef struct projection{
        byte x=0,y=0,value=0;
        char projectionLine='0';
    }projection;
    typedef struct possibleCellInputs{
        char *possibleValues=0;
        byte numberOfValues=0;
        byte posX=0,posY=0;
        byte currentPosition=0;
    }possibleCellInputs;
    byte fillSingleCells(grdPtr *grid, vector <projection> *projections=0);
    void skippedSudoku(char *fName="skippedSdk.sdk", bool toWrite=1, char *reason="UNKN",char *ID="NULL");
    byte howManyElementsFitCell(grdPtr *grid, byte indX, byte indY, vector <projection> *projections=0);
    inline bool isVerticallyOk(grdPtr *grid, int index[2], char value, bool isNullsTaken=1, vector <projection> *projections=0);
    inline bool isHorizontallyOk(grdPtr *grid, int index[2], char value, bool isNullsTaken=1, vector <projection> *projections=0);
    inline bool checkBox(grdPtr *grid,int index[2], char number, bool isNullsTaken=1);
    inline bool checkPosition(grdPtr *grid,int index[2], char number, bool isNullsTaken=1, vector <projection> *projections=0);
    inline bool isSolved(grdPtr *grid, vector <solver::projection> *projections=0);
    void printGrid(grdPtr *grid);
    inline void stepForwardIndexes(int *indexes);
    inline void stepBackwardIndexes(int *indexes);
    grdPtr* solve(grdPtr *grid, int currIndex[2], bool *toSkip,vector <solver::projection> *projections=0, char *ID="NULL");
    void stringToArray(grdPtr *toArr, string &fromArr, int x=9, int y=9, bool byRows=0);
}
#endif
