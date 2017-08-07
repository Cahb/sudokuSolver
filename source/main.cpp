#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <mutex>
#include <thread>
#include "solver.h"
#include <exception>
#include "downloader.h"
#include <time.h>
#include "parser.h"
#include <vector>
// http://sudoku.org.ua/t/map.php?id=
using namespace std;

solver::grdPtr* Solve(solver::grdPtr *grid, bool save,string filePath,char ID[6], bool *toSkip,vector <solver::projection> projections)
{
    clock_t clock1;
    clock_t timediff;
    float timediff_sec=0;
    if(filePath.empty())
    {
        filePath = string("tempSdk.txt");
    }
    int currIndex[2]={0,0};
    printf("SOLVING:%s\n", ID);
    printf("PROJECTIONS_SIZE:%d\n",projections.size());
    clock1 = clock();
    grid = solver::solve(grid, currIndex, toSkip, &projections, ID);
    if(grid && save && !(*toSkip))
    {
        FILE *f1 = fopen(filePath.c_str(), "a");
        fprintf(f1, "%s\t", ID);
        for(int i = 0; i < 9;i++)
        {
            for(int j =0; j< 9;j++)
            {
                fprintf(f1, "%c", (*grid)[i][j]);
            }
        }
        clock_t clock2 = clock();
        timediff = clock2 - clock1;
        timediff_sec = ((float)timediff) / CLOCKS_PER_SEC;
        fprintf(f1, "\t%.4f\n", timediff_sec);
        fclose(f1);
    }
    return grid;
}

string getSudokuFromFile(char *fName="tempSdk2.txt", char *ID="-1", long long int *filePointer=0)
{
    ifstream in(fName, ios_base::in);
    string buf;
    if(filePointer && *filePointer > 0)
    {
        in.seekg(*filePointer);
    }
    if(in.is_open())
    {
        do
        {
            buf.clear();
            getline(in, buf, '\n');
            *filePointer = in.tellg();
        }while(!in.eof() && (strcmp(ID, (parser::extractKey(buf, "mapId")).c_str())!= 0) );
        if(strcmp(ID, (parser::extractKey(buf, "mapId").c_str())) == 0)
        {
            in.close();
            return buf;
        }
    }
    in.close();
    return string("EMPTY");
}

int autoProjectionsFillGrid(solver::grdPtr *gridPtr, bool byAllValues=1, int exactValue=0,vector <solver::projection> *projections=0)
{
    int filledElements=0;
    int num=0;
    int asquare=0;
    int bsquare=0;
    int currBox[2]={0};
    int currIndex[2] = {0};
    char tempValue=0;
    int indeX=0,indeY=0,cellValue=0;
    for(int i = 0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            asquare = i/3;
            bsquare = j/3;
            if( ( (*gridPtr)[i][j] == '0'|| (*gridPtr)[i][j] == '-' )  && i /3 == currBox[0] && j/3 == currBox[1])
            {
                for(int g = (byAllValues&&exactValue==0?1:exactValue ) ; g < (byAllValues&&exactValue==0?10:exactValue);g++)
                {
                    for ( register int i1 = asquare * 3; i1 < (asquare * 3 + 3); i1++)
                    {
                        for ( register int j1 = bsquare * 3; j1 < (bsquare * 3 + 3); j1++)
                        {
                            if((*gridPtr)[i1][j1] == '0' || (*gridPtr)[i][j] == '-')
                            {
                                currIndex[0] = i1;
                                currIndex[1] = j1;
                                tempValue = (*gridPtr)[i1][j1];
                                (*gridPtr)[i1][j1] = g+48;
                                if(!solver::checkPosition(gridPtr, currIndex, g+48,0,projections))
                                {
                                    (*gridPtr)[i1][j1] = '0';
                                    continue;
                                }
                                if(solver::checkPosition(gridPtr, currIndex, g+48,0,projections))
                                {
                                    indeX = i1;
                                    indeY = j1;
                                    cellValue = g+48;
                                    (*gridPtr)[i1][j1] = '0';
                                    num++;    
                                }
                            }
                        }
                    }
                    if(num == 1)
                    {
                        (*gridPtr)[indeX][indeY] = cellValue;
                        filledElements++;
                    }
                    num = 0;
                }
                if(currBox[1] < 2)
                {
                    currBox[1]++;
                }
                else
                {
                    currBox[0]++;
                    currBox[1] = 0;
                }
            }
        }                  
    }
    return filledElements;
}

vector<solver::projection> projectionsFinder(solver::grdPtr *gridPtr,vector <solver::projection> *readyProjections){
    solver::byte gValue=0, indeX=0,indeY=0,cellValue=0,numberOfFittingCells=0;
    solver::byte asquare=0,bsquare=0,num=0,filledElements=0, emptyCells=0;
    int currBox[2]={0};
    int currIndex[2] = {0};
    char tempValue=0;
    vector <solver::projection> projections;
    for(int i = 0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            asquare = i/3;
            bsquare = j/3;
            if( ( (*gridPtr)[i][j] == '0'|| (*gridPtr)[i][j] == '-' )  && i /3 == currBox[0] && j/3 == currBox[1])
            {
                for(int g = 1; g<= 9;g++)
                {
                    for ( register int i1 = asquare * 3; i1 < (asquare * 3 + 3); i1++)
                    {
                        for ( register int j1 = bsquare * 3; j1 < (bsquare * 3 + 3); j1++)
                        {
                            if( ((*gridPtr)[i1][j1] == '0' || (*gridPtr)[i][j] == '-' ) && asquare == currBox[0] && bsquare == currBox[1])
                            {
                                currIndex[0] = i1;
                                currIndex[1] = j1;
                                (*gridPtr)[i1][j1] = g+48;
                                if(!solver::checkPosition(gridPtr, currIndex, g+48,0,readyProjections!=0?readyProjections:&projections))
                                {
                                    (*gridPtr)[i1][j1] = '0';
                                }
                                else if(solver::checkPosition(gridPtr, currIndex, g+48,0,readyProjections!=0?readyProjections:&projections))
                                {
                                    emptyCells++;
                                    gValue = g+48;
                                    indeX = i1;
                                    indeY = j1;
                                    cellValue = g+48;
                                    (*gridPtr)[i1][j1] = '0';
                                    num++;
                                }
                            }
                        }
                    }
                    if(num >= 1 && num <= 3)
                    { 
                        for ( register int i1 = asquare * 3; i1 < (asquare * 3 + 3); i1++)
                    {
                        for ( register int j1 = bsquare * 3; j1 < (bsquare * 3 + 3); j1++)
                        {
                            if((*gridPtr)[i1][j1] == '0' || (*gridPtr)[i][j] == '-')
                            {
                            currIndex[0] = i1;
                            currIndex[1] = j1;
                            (*gridPtr)[i1][j1] = g+48;
                            if(solver::checkPosition(gridPtr, currIndex, g+48,0, readyProjections!=0?readyProjections:&projections))
                            {
                                solver::projection tempProjection;
                                tempProjection.x = i1;
                                tempProjection.y = j1;
                                tempProjection.value = g+48;
                                tempProjection.projectionLine = '0';
                                projections.push_back(tempProjection);
                                (*gridPtr)[i1][j1] = '0';
                            }
                            (*gridPtr)[i1][j1] = '0';
                            }
                        }
                    }
                    if(num == 1)
                    {
                        (*gridPtr)[projections[projections.size()-1].x][projections[projections.size()-1].y] = projections[projections.size()-1].value;
                        projections.erase(projections.begin() + projections.size()-1);
                        
                    }
                    else if(num == 2)
                    {
                        if((projections[projections.size() -1].y - projections[projections.size() -2].y )== 0 && (projections[projections.size() -1 ].x - projections[projections.size() -2].x )== 1  )
                        {
                            projections[projections.size() -2 ].projectionLine = projections[projections.size() -1].projectionLine = int('|');
                        }
                        else if((projections[projections.size() -1 ].x - projections[projections.size() -2].x )== 0 && (projections[projections.size() -1 ].y - projections[projections.size() -2].y )== 1 )
                        {
                            projections[projections.size() -2 ].projectionLine = projections[projections.size() -1].projectionLine = int('-');
                        }
                        else if (emptyCells==2)
                        {
                            if((projections[projections.size() -1].y - projections[projections.size() -2].y )== 0 && (projections[projections.size() -1 ].x - projections[projections.size() -2].x )== 2  )
                            {
                                projections[projections.size() -2 ].projectionLine = projections[projections.size() -1].projectionLine = int('|');
                            }
                            else if((projections[projections.size() -1 ].x - projections[projections.size() -2].x )== 0 && (projections[projections.size() -1 ].y - projections[projections.size() -2].y )== 2 )
                            {
                                projections[projections.size() -2 ].projectionLine = projections[projections.size() -1].projectionLine = int('-');
                            }
                            else
                            {
                                projections.erase(projections.begin()+projections.size()-num,projections.end());
                            }
                        }
                        else
                        {
                            projections.erase(projections.begin()+projections.size()-num,projections.end());
                        }
                        
                    }
                    else if(num == 3)
                    {
                        if( ( (projections[projections.size() -1].y - projections[projections.size() -2 ].y) - (projections[projections.size() -2].y - projections[projections.size() -3].y))== 0
                               && projections[projections.size() -1].x == projections[projections.size() -2].x == projections[projections.size() -3].x )
                        {
                            projections[projections.size() -3].projectionLine = projections[projections.size() -2].projectionLine = projections[projections.size() -1].projectionLine = ('-'); 
                        }
                        else if (  ( (projections[projections.size() - 1].x - projections[projections.size() -2 ].x) - (projections[projections.size() -2].x - projections[projections.size() -3].x))== 0
                               && projections[projections.size() -1].y == projections[projections.size() -2].y == projections[projections.size() -3].y )
                        {
                            projections[projections.size() -1].projectionLine = projections[projections.size() -2 ].projectionLine = projections[projections.size() -3].projectionLine = ('|');
                            
                        }
                        else
                        {
                            projections.erase(projections.begin()+projections.size()-num,projections.end());
                        }
                    }
                    }
                    num = 0;
                    emptyCells = 0;
                }
                if(currBox[1] < 2)
                {
                    currBox[1]++;
                }
                else
                {
                    currBox[0]++;
                    currBox[1] = 0;
                }
            }
        }                  
    }
    return projections;
}



int main(void)
{   
    char decision=0;
    bool stop=0,pause=0,toSkip=0;
    int toIterateFromIndex=0, toIterateToIndex=0;
    mutex mtx;
    printf("Do you want to (S)olve or (D)ownload sudoku-list?\n");
    do
    {
        scanf("%c", &decision);
    }while(decision != 'S' && decision != 's' && decision !='D' && decision !='d');
    if(decision == 'S' || decision == 's')
    {
        vector <solver::projection> vectorPointer,tempVectorPointer;
        string sudoku, id;
        char tmp[6],ID[7];
        register char grid[9][10] = {0};
        clock_t time1, time2, timediff;
        float timediff_sec=0;
        solver::grdPtr *gridPointer = &grid;
        bool toAppend=false;
        long long int filePointer=0;
        int index[2]={0};
        solver::byte prevSize=0;
        printf("To begin, please enter START index\n");
        scanf("%d", &toIterateFromIndex);
        printf("To begin, please enter END index\n");
        scanf("%d", &toIterateToIndex);
        system("cls");
        thread stopThread(downloader::Pauser, &stop, &pause, &toSkip,&mtx);
        stopThread.detach();
        for(int i = toIterateFromIndex; i < toIterateToIndex;i++) // 423 3827 3835 4127 4174
        {
            sprintf(tmp, "%d", i);
            sudoku = getSudokuFromFile("SDKS.dat", tmp, &filePointer);
            //getch();
            if(sudoku.size() < 4 || sudoku == "{}")
            {
                continue;
            }
            id = parser::extractKey(sudoku, "mapId");
            sudoku = parser::extractKey(sudoku, "map");
            if(id == "EMPTY" || sudoku == "EMPTY")
            {
                continue;
            }
            solver::stringToArray(gridPointer, sudoku,9,9,0);
            solver::printGrid(gridPointer);
            strcpy(ID, id.c_str());
            for(;;)
            {
                autoProjectionsFillGrid(gridPointer,1,0,&vectorPointer);
                vectorPointer = projectionsFinder(gridPointer,0);
                tempVectorPointer = projectionsFinder(gridPointer,&vectorPointer);
                for(int i = 0; i < tempVectorPointer.size();i++)
                {
                    for(int j = 0; j < vectorPointer.size();j++)
                    {
                        if(vectorPointer[j].x != tempVectorPointer[i].x && vectorPointer[j].y != tempVectorPointer[i].y && vectorPointer[j].value != tempVectorPointer[i].value )
                        {
                            toAppend = true;   
                        }
                    }
                    if(toAppend)
                    {
                        vectorPointer.push_back(tempVectorPointer[i]);
                        toAppend = false;
                    }
                }
                if(prevSize == vectorPointer.size())
                {
                    break;
                }
                prevSize = vectorPointer.size();
            }
            //getch();
            solver::printGrid(gridPointer);
            time1 = clock();
            printf("BEFORE_ERASE:%d\n", vectorPointer.size());
            for(int j = 0; j < vectorPointer.size();j++)
            {
                index[0] = vectorPointer[j].x;
                index[1] = vectorPointer[j].y;
                //printf("id-%d, x-%d, y-%d, value-%c, line-%c, grd:%c\n",j, vectorPointer[j].x,vectorPointer[j].y,vectorPointer[j].value,vectorPointer[j].projectionLine,grid[index[0]][index[1]]);
                if( (grid[index[0]][index[1]] != '0' && grid[index[0]][index[1]] != '-')
                || !solver::checkPosition(gridPointer,index, vectorPointer[j].value,0,0) )
                {
                    vectorPointer.erase(vectorPointer.begin()+j);
                    //printf("Erased\n");
                }
            }
            printf("AFTER_ERASE:%d\n", vectorPointer.size());
            //getch();
            gridPointer = Solve(gridPointer, 1, "tempSdk2.txt", ID, &toSkip,vectorPointer);
            system("cls");
            time2 = clock();
            timediff = time2 - time1;
            timediff_sec = ((float)timediff) / CLOCKS_PER_SEC;
            printf("\nIt took: %f seconds!\n",timediff_sec);
            if(!toSkip)
            {
                printf(" /////SOLVED//// \n");
                solver::printGrid(gridPointer);
                printf("SUDOKU ID:%s\n", id.c_str());
                printf("\nOVERALL PROGRESS: %d of %d (%f)\n", atoi(tmp), 40000, (float)(atoi(tmp)) / (float)40000);             
            }
            if(stop)
            {
                break;
            }
            if(pause)
            {
                mtx.lock();
                printf("\nPROGRAM IS NOW IDLE...PRESS ANY KEY TO CONTINUE\n");
                getch();
                printf("NOW CONTINUING...\n");
                //std::this_thread::sleep_for(chrono::seconds(1));           
                pause = false;
                mtx.unlock();
            }
            if(toSkip)
            {
                mtx.lock();
                printf("\nSDK %s's been skipped!\n'", id.c_str());
                printf("NOW CONTINUING...\n");
                std::this_thread::sleep_for(chrono::seconds(2)); 
                toSkip = false;
                mtx.unlock();
            }
            vectorPointer.erase(vectorPointer.begin(), vectorPointer.end());
            tempVectorPointer.erase(vectorPointer.begin(), vectorPointer.end());
            }
        }
    else
    {
        char fName[20];
        printf("To begin enter NAME of the file you want to download skd-list to:");
        scanf("%s", fName);
        printf("To begin, please enter START index\n");
        scanf("%d", &toIterateFromIndex);
        printf("To begin, please enter END index\n");
        scanf("%d", &toIterateToIndex);
        system("cls");    
        thread stopThread(downloader::Pauser, &stop, &pause, &toSkip,&mtx);
        stopThread.detach();
        return downloader::getSudokuList(&stop, &pause, &mtx, toIterateFromIndex, toIterateToIndex,fName);
               
    }
        printf("PROGRAM HAS BEEN STOPPED!\n");
        getch();
        return 0;  
}

