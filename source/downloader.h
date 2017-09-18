#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#pragma once
#include <mutex>
#include <thread>
#if BUILD_WITH_DOWNLOADER
#include <curl\curl.h>
#endif
#include <iostream>
using namespace std;
namespace downloader
{
void Pauser(bool *STOP, bool *PAUSE, bool *toSkip, mutex *mtx);
#if BUILD_WITH_DOWNLOADER
bool getSudokuList(bool *STOP,bool *PAUSE, mutex *mtx, int indexBeg=0, int indexEnd=40000, char *fName="Sdks-2.dat");
#endif
    
}


#endif
