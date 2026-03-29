#ifndef _00COMPUTATION00_
#define _00COMPUTATION00_

#include <time.h>

void computation()
{
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            for (int k = 0; k < 1000; k++)
                ;
}

void heavy_computation()
{
    for (int i = 0; i < 5; i++)
        computation();
}

#endif
