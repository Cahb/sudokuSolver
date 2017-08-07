#include "solver.h"
#include <thread>
#include <chrono>

    inline bool solver::isVerticallyOk(grdPtr *grid, int index[2], char value, bool isNullsTaken, vector <projection> *projections)
{
    if(projections == 0 || (projections != 0 && projections->size() == 0))
    {
        for (register int i = 0; i < 9;i++)
        {
            if(isNullsTaken)
            {
                if(i != index[0] && (*grid)[i][index[1]] == value )
                {
                    return false;
                }
            }
            else
            {
                if(i != index[0] && (*grid)[i][index[1]] == value && value != '-' && value != '0' )
                {
                        return false;
                }
            }
        }
    }
    else
    {
       for (register int i = 0; i < 9;i++)
        {
            for(register int j= 0; j < projections->size();j++)
            {
                if(isNullsTaken)
                {
                    if( ( i != index[0] && ((*grid)[i][index[1]] == value ) || ( i / 9 != index[0] / 9 && (*projections)[j].x == index[0]&& ((*projections)[j].projectionLine) == '|' && ((*projections)[j].value) == value )))
                    {
                        return false;
                    }
                }
                else
                {
                    if( ( (i/ 3 != index[0] / 3 &&   (*projections)[j].y == index[1] && (*projections)[j].projectionLine == '|' && (*projections)[j].value == value ) || (i != index[0] &&  (*grid)[i][index[1]] == value && i / 3 != index[0] / 3 )  ) && value != '-' && value != '0' ) 
                    {
                            return false;
                    }
                }
        }
        } 
    }
    return true;
}

inline bool solver::isHorizontallyOk(grdPtr *grid, int index[2], char value, bool isNullsTaken, vector <projection> *projections)
{
    if(projections == 0 || (projections != 0 && projections->size() == 0))
    {
       for (register int i = 0; i < 9;i++)
    {
            if(isNullsTaken)
            {
                if(i != index[1] && (*grid)[index[0]][i] == value )
                {
                    return false;
                }
            }
            else
            {   
                if(i != index[1] && (*grid)[index[0]][i] == value && value != '-' && value != '0' )
                {
                    return false;
                }
           }
    } 
    }
    else
    {
        for (register int i = 0; i < 9;i++)
        {
            for(register int j = 0; j < projections->size();j++)
            {
                if(isNullsTaken)
                {                                                          
                    if( (i != index[1] && ((*grid)[i][index[1]] == value ) ) || ((*projections)[j].y == index[1]&& (*projections)[j].projectionLine == '-' && (*projections)[j].value == value )) 
                    {
                        return false;
                    }
                }
                else
                {   
                    if( ( ( i / 3 != index[1] / 3 && (*projections)[j].x == index[0]&& (*projections)[j].projectionLine == '-' && (*projections)[j].value == value) || (i != index[1] && (*grid)[index[0]][i] == value && i / 3 != index[1] / 3  )   ) && value != '-' && value != '0' ) /* && value != '-' && value != '0' )*/
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

inline bool solver::checkBox(grdPtr *grid,int index[2], char number, bool isNullsTaken)
{
    int asquare = index[0] / 3;
    int bsquare = index[1] / 3;
    for ( register int i = asquare * 3; i < (asquare * 3 + 3); i++)
    {
        for ( register int j = bsquare * 3; j < (bsquare * 3 + 3); j++)
        {
            if(isNullsTaken)
            {
                if( (i != index[0] || j != index[1] ) && (*grid)[i][j] == number)
                {
                    return false;
                }
            }
            else
            {
                if( (i != index[0] || j != index[1] ) && (*grid)[i][j] == number && number != '-' && number !='0')
                {
                    return false;
                }
                
            }
        }
    }
    return true;
}

inline bool solver::checkPosition(grdPtr *grid,int index[2], char number, bool isNulltsTaken, vector <projection> *projections)
{
    return (checkBox(grid, index, (*grid)[index[0]][index[1]],isNulltsTaken)==1 && isVerticallyOk(grid, index, (*grid)[index[0]][index[1]],isNulltsTaken,projections)==1 && isHorizontallyOk(grid, index, (*grid)[index[0]][index[1]],isNulltsTaken,projections)==1);
}

inline bool solver::isSolved(grdPtr *grid, vector <solver::projection> *projections)
{
    int index[2]={0,0};
    for(register int i = 0; i < 9;i++)
    {
        for(register int j = 0; j < 9;j++)
        {
            index[0] = i;
            index[1] = j;
            if(!checkPosition(grid, index, (*grid)[i][j],1, projections) || (*grid)[i][j] == '-' || (*grid)[i][j] == '0')
            {
                return false;
            }
        }
    }
    return true;
}

void solver::printGrid(grdPtr *grid)
{
    for(register int i = 0; i < 9;i++)
    {
        for(register int j = 0; j < 9;j++)
        {
            cout << (*grid)[i][j];
            if( (j+1) % 3 == 0)
            {
                cout << " ";
            }
        }
        cout << endl;
        if( (i+1) %3==0 )
        {
            cout << endl;
        }
    }
    cout << endl;
}

void solver::stepForwardIndexes(int *indexes)
{
    if(indexes[1] <  8)
    {
        (indexes[1])++;
    }
    else
    {
        indexes[0]++;
        indexes[1]=0;
    }
    return;
}

void solver::stepBackwardIndexes(int *indexes)
{
    if(indexes[1] > 0)
    {
        indexes[1]--;
    }
    else
    {
        indexes[0]--;
        indexes[1]=8;
    }
    return;
}

solver::byte solver::howManyElementsFitCell(solver::grdPtr *grid, solver::byte indX, solver::byte indY, vector <solver::projection> *projections)
{
    int currIndex[2]={indX,indY};
    char prevValue=(*grid)[indX][indY];
    solver::byte howManyElements=0;
    for(int g = 1; g <= 9;g++)
    {
        (*grid)[indX][indY] = g+48;
        if(solver::checkPosition(grid, currIndex, prevValue, 0,0))
         {
            howManyElements++;
        }
    }
    (*grid)[indX][indY] = prevValue;
    return howManyElements;
}
solver::possibleCellInputs *elementOfPossibleInputs (solver::byte x, solver::byte y, solver::possibleCellInputs *arr, solver::byte length)
{
    for(int i = 0; i <  length;i++)
    {
        if(arr[i].posX == x && arr[i].posY == y)
        {
            return &arr[i];
        }
    }
    return NULL;
}

solver::byte solver::fillSingleCells(solver::grdPtr *grid, vector <solver::projection> *projections)
{
    solver::byte filledCells=0, possibleFills=0;
    char lastValue = 0;
    int currIndex[2] = {0};
    for(int i = 0; i < 9;i++)
    {
        for(int j = 0; j < 9; j++)
        {
            currIndex[0] = i;
            currIndex[1] = j;
            if( ((*grid)[i][j] == '-' || (*grid)[i][j] == '0' )&& solver::howManyElementsFitCell(grid, i, j,0) == 1)
            {
                for(int g = 1; g <= 9;g++)
                {
                    (*grid)[i][j] = g+48;
                    if(solver::checkPosition(grid, currIndex, g+48, 0,0))
                    {
                        break;
                    }
                }
            }
        }
     } 
     return filledCells;
}
void solver::skippedSudoku(char *fName, bool toWrite, char *reason, char *ID)
{
    if(toWrite)
    {
        char buf[1024]={0};
        strcpy(buf, "SKIPPED ID:");
        strcat(buf, ID);
        strcat(buf, " --REASON:");
        strcat(buf, reason);
        FILE *f1 = fopen(fName, "a");
        if(f1)
        {
            fprintf(f1, "%s\n", buf);
        }
        fclose(f1);
    }
}

solver::grdPtr* solver::solve(grdPtr *grid, int currIndex[2], bool *toSkip,vector <solver::projection> *projections,char *ID)
{
    int firstIndexes[2] = {0};
    register char gridConst[9][10];
    possibleCellInputs* inputs;
    solver::byte staticElements =0;
    solver::byte autoFilled=0;
    solver::byte length=0;
    
    do
    {
        autoFilled = fillSingleCells(grid,0);
    }while(autoFilled > 0);
    for(int i = 0; i < projections->size();i++)
    {
        if((*grid)[(*projections)[i].x][(*projections)[i].y] != '0' && (*grid)[(*projections)[i].x][(*projections)[i].y] != '-' )
        {
            projections->erase(projections->begin() + i);
        }
    }
    
    autoFilled = 0;
    // /*
    
    for(int i = 0; i < 9;i++)
    {
        for(int j = 0; j < 9;j++)
        {
            if ( (*grid)[i][j] != '-' && (*grid)[i][j] != '0')
            {
                staticElements++;
            }

        }
    }
    length = 81 - staticElements;
    inputs = new possibleCellInputs[length];
    solver::byte arrIndex=0,lastSubArrIndex=0;
    for(int i = 0; i < 9;i++)
    {
        for(int j = 0 ; j < 9;j++)
        {
            currIndex[0] = i;
            currIndex[1] = j;
            if((*grid)[i][j] == '0' || (*grid)[i][j] == '-')
            {
                inputs[arrIndex].posX = currIndex[0];
                inputs[arrIndex].posY = currIndex[1];
                inputs[arrIndex].numberOfValues = howManyElementsFitCell(grid, i,j,0); // projections
                (*grid)[i][j] = '0';
                inputs[arrIndex].possibleValues = new char[inputs[arrIndex].numberOfValues+1];
                inputs[arrIndex].possibleValues[inputs[arrIndex].numberOfValues] = '\0';
                for(int g = 1; g <= 9;g++)
                {
                    (*grid)[i][j] = g+48;
                    if(checkPosition(grid, currIndex,g+48,0,0))
                    {
                        inputs[arrIndex].possibleValues[lastSubArrIndex] = g+48;
                        lastSubArrIndex++;
                    }
                }
                lastSubArrIndex=0;
                (*grid)[i][j] = '0';
                arrIndex++;
            }
        }
    }
    currIndex[0] = firstIndexes[0];
    currIndex[1] = firstIndexes[1];
    for(int i = 0; i < length;i++)
    {
        if(inputs[i].numberOfValues == 1)
        {
            (*grid)[inputs[i].posX][inputs[i].posY] = gridConst[inputs[i].posX][inputs[i].posY] = inputs[i].possibleValues[0];
            autoFilled++;
        }
    }
    
    for(register int i =0; i< 9;i++)
    {
        for(register int j = 0; j<9;j++)
        {
            gridConst[i][j] = (*grid)[i][j];
        }
    }
    currIndex[0] = firstIndexes[0];
    currIndex[1] = firstIndexes[1];
    grdPtr *constGrdPtr  = &gridConst;
    possibleCellInputs *tempPointer=0;
    for(int i = 0; i < 9;i++)
    {
        for(int j = 0; j < 9;j++)
        {
            if((*grid)[i][j] == '0' || (*grid)[i][j] =='-')
            {
                
               firstIndexes[0] = i;
               firstIndexes[1] = j;
               i=j=10; 
            }
        }
    }
    for(register int i = 0; i < 9;i++)
    {
        for(register int j = 0; j<9;j++)
        {
            currIndex[0] = i;
            currIndex[1] = j;
            if(!checkPosition(grid, currIndex, (*grid)[i][j],0))
            {
                *toSkip = true;
                i=j=10;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                printf("CAN'T SOLVE 1\n");
                skippedSudoku("skippedSdk.sdk", 1, "1 (BAD GRID??)",ID);
                break;
            }
            stepForwardIndexes(currIndex);
        }
    }
    currIndex[0] = firstIndexes[0];
    currIndex[1] = firstIndexes[1];
    while(!isSolved(grid) && !(*toSkip))
    {
        if(gridConst[currIndex[0]][currIndex[1]] == '-' || gridConst[currIndex[0]][currIndex[1]] == '0')
        {
            if((*grid)[currIndex[0]][currIndex[1]] == '-' || (*grid)[currIndex[0]][currIndex[1]] == '0')
            {
                tempPointer = elementOfPossibleInputs(currIndex[0],currIndex[1], inputs, length);
                (*grid)[currIndex[0]][currIndex[1]] = tempPointer->possibleValues[tempPointer->currentPosition];
                if(!checkPosition(grid, currIndex, (*grid)[currIndex[0]][currIndex[1]],1,0) && tempPointer->currentPosition +1 < tempPointer->numberOfValues)
                {
                    do
                    {
                        (tempPointer->currentPosition)++;
                        (*grid)[currIndex[0]][currIndex[1]] = tempPointer->possibleValues[tempPointer->currentPosition];
                    }while(tempPointer->currentPosition+1< tempPointer->numberOfValues && !checkPosition(grid, currIndex, (*grid)[currIndex[0]][currIndex[1]],1));
                    if(tempPointer->currentPosition >= tempPointer->numberOfValues || !checkPosition(grid, currIndex, (*grid)[currIndex[0]][currIndex[1]],1))
                    {
                        (*grid)[currIndex[0]][currIndex[1]] = '-';
                        tempPointer->currentPosition = 0;
                        stepBackwardIndexes(currIndex);
                        tempPointer = elementOfPossibleInputs(currIndex[0],currIndex[1], inputs, length);
                        if(currIndex[0] == firstIndexes[0] && currIndex[1] == firstIndexes[1] && gridConst[currIndex[0]][currIndex[1]]!='-' && gridConst[currIndex[0]][currIndex[1]]!='0' )
                        {
                            *toSkip = true;
                            std::this_thread::sleep_for(chrono::seconds(1));
                            
                            skippedSudoku("skippedSdk.sdk", 1, "2",ID);
                            continue;
                        }
                        while(1)
                        {
                            
                            tempPointer = elementOfPossibleInputs(currIndex[0],currIndex[1], inputs, length); 
                            if(currIndex[0] < 0 || currIndex[1] < 0)
                            {
                                *toSkip = true;
                                skippedSudoku("skippedSdk.sdk", 1, "grid_index_is < 0",ID);
                                break;
                            }
                            if(gridConst[currIndex[0]][currIndex[1]] != '-' && gridConst[currIndex[0]][currIndex[1]] != '0')
                            {
                                if(currIndex[0] == 0 && currIndex[1] == 0)
                                {
                                    //printf("SOL#1\n");
                                    *toSkip = true;
                                    //cout << "SKIPPING - can't solve solution\n";
                                    //getch();
                                    std::this_thread::sleep_for(chrono::seconds(1));
                                    skippedSudoku("skippedSdk.sdk", 1, "3",ID);
                                    //printf("CAN'T SOLVE 3\n");
                                    //getch();
                                    break;
                                }
                                stepBackwardIndexes(currIndex);
                                continue;
                            }
                            else if( tempPointer != NULL &&/*(*grid)[currIndex[0]][currIndex[1]] < '9' */ tempPointer->currentPosition + 1 < tempPointer->numberOfValues)
                            {
                                tempPointer->currentPosition++;
                                (*grid)[currIndex[0]][currIndex[1]] = tempPointer->possibleValues[tempPointer->currentPosition];
                                if(checkPosition(grid, currIndex, (*grid)[currIndex[0]][currIndex[1]],1,0))
                                {
                                    //printf("BREAK\n");
                                    //getch();
                                    break;
                                }
                            }
                            else
                            {
                                (*grid)[currIndex[0]][currIndex[1]] = '-';
                                tempPointer->currentPosition=0;
                                stepBackwardIndexes(currIndex);
                            }
                        }
                    }
                    else
                    {
                        stepForwardIndexes(currIndex);
                    }
                }
            }
            else
            {
                tempPointer = elementOfPossibleInputs(currIndex[0],currIndex[1], inputs, length);
                if(checkPosition(grid, currIndex, (*grid)[currIndex[0]][currIndex[1]],1))
                {
                    stepForwardIndexes(currIndex);
                }
                else
                {
                
                    if(tempPointer != NULL && /*(*grid)[currIndex[0]][currIndex[1]] < '9' */ tempPointer->currentPosition +1 < tempPointer->numberOfValues)
                    {
                        //(*grid)[currIndex[0]][currIndex[1]]++;
                        tempPointer->currentPosition++;
                        (*grid)[currIndex[0]][currIndex[1]] = tempPointer->possibleValues[tempPointer->currentPosition];
                    }
                    else
                    {
                        (*grid)[currIndex[0]][currIndex[1]] = '-';
                        tempPointer->currentPosition=0;
                        stepBackwardIndexes(currIndex);
                        tempPointer = elementOfPossibleInputs(currIndex[0],currIndex[1], inputs, length); 
                        if(/*(*grid)[currIndex[0]][currIndex[1]] < '9'*/ tempPointer->currentPosition +1 < tempPointer->numberOfValues)
                        {
                            (*grid)[currIndex[0]][currIndex[1]] = tempPointer->possibleValues[tempPointer->currentPosition];
                            //(*grid)[currIndex[0]][currIndex[1]]++;
                        }
                        else
                        {
                            (*grid)[currIndex[0]][currIndex[1]] = '-';
                            tempPointer->currentPosition=0;
                            stepBackwardIndexes(currIndex);
                        }
                    }
                }
            }
        }
        else
        {
            stepForwardIndexes(currIndex);
        }
    }
    printf("PRINT:\n");
    //getch();
    for(int g = 0; g < length;g++)
    {
        delete[] inputs[g].possibleValues;
    }
    delete[] inputs;

    return grid;
}

void solver::stringToArray(grdPtr *toArr, string &fromArr, int x, int y, bool byRows)
{
    if(!byRows)
    {
        for(register int i = 0; i < x;i++)
        {
            for(register int j = 0; j < y;j++)
            {
                (*toArr)[j][i] = fromArr[(9*i+j)];
            }
        }
        //getch();
        return;
    }
    else
    {
        for(register int i = 0; i < x;i++)
        {
            for(register int j = 0; j < y;j++)
            {
                (*toArr)[i][j] = fromArr[(9*i+j)];
            }
        }
        return;
    }
}
