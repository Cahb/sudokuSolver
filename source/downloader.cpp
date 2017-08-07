#include "downloader.h"
void downloader::Pauser(bool *STOP, bool *PAUSE, bool *TOSKIP, mutex *mtx)
{
    while(1)
    {
        char c= getch();
        mtx->lock();
        if(c == 'P' || c == 'p')
        {
            *PAUSE = true;
        }
        else if(c == 'S' || c == 's')
        {
            *STOP = true;
            mtx->unlock();
            return;
        }
        else if(c == 'X' || c == 'x')
        {
            *TOSKIP = true;
        }
        mtx->unlock();
    }
}


bool downloader::getSudokuList(bool *STOP,bool *PAUSE, mutex *mtx, int indexBeg, int indexEnd,char *fName)
{
    CURL *crl =  curl_easy_init();
    FILE *f1= fopen(fName, "a");
    if(!f1)
    {
        std::cout << "Can't open file\n'";
        getch();
    }
    curl_easy_setopt(crl, CURLOPT_WRITEDATA, f1);
    curl_easy_setopt(crl, CURLOPT_HEADER, 0L);
    const string s = "sudoku.org.ua/t/map.php?id=";
    char tmp[10];
    for(int i = indexBeg; i < indexEnd;i++)
    {
        snprintf(tmp, sizeof(tmp), "%d", i);
        curl_easy_setopt(crl, CURLOPT_URL, string(s + tmp).c_str());
        curl_easy_perform(crl);
        if(i % 10 == 0)
        {
            system("cls");
            printf("INDX[%d] (/%d)\n",i,indexEnd);
            printf("Press P for PAUSE, S for STOP");
        }
        mtx->lock();
        
        if(*PAUSE)
        {
            system("cls");
            fclose(f1);
            f1 = fopen(fName,"a");
            if(!f1)
            {
                printf("CANNOT REOPEN FILE...NOW CLOSING APP\n");
                curl_easy_cleanup(crl);
                getch();
                *STOP=1;
                return 0;
            }
            printf("\nPROGRAM HAS BEEN PAUSE...PRESS ANY KEY TO CONTINUE\n");
            getch();
            
            //*STOP = false;
            *PAUSE = false;
        }
        if(*STOP)
        {
            system("cls");
            printf("PROGRAM HAS BEEN STOPPED (press any key...)\n");
            getch();
            curl_easy_cleanup(crl);
            fclose(f1);
            mtx->unlock();
            return 0;
            break;
        }
        mtx->unlock();
        fprintf(f1, "\n");
    }
    curl_easy_cleanup(crl);
    fclose(f1);
    *STOP;
    return 1;
}


