#ifndef UTILS_H
#define UTILS_H

#include <base/base.h>
#include <os/os.h>
typedef struct U_Clock U_Clock;
struct U_Clock
{
    r32 elapsed;
    r32 now;
    r32 start_time;
    
};



torn_function void
U_ClockStart(U_Clock* clock, OS_App* app);

torn_function r32
U_ClockGetElapsedTime(U_Clock* clock, OS_App* app);


#endif

