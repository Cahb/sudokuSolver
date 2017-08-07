#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#pragma once
#include <mutex>
#include <thread>
#include <curl\curl.h>
#include <conio.h>
#include <iostream>
using namespace std;
namespace downloader
{
void Pauser(bool *STOP, bool *PAUSE, bool *toSkip, mutex *mtx);
bool getSudokuList(bool *STOP,bool *PAUSE, mutex *mtx, int indexBeg=0, int indexEnd=40000, char *fName="Sdks-2.dat");
}


#endif
