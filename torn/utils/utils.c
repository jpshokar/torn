#include "utils.h"




torn_function r32
U_ClockGetElapsedTime(U_Clock* clock, OS_App* app)
{
    return (OS_GetTime(app) - clock->start_time);
}
torn_function void
U_ClockRestart(U_Clock* clock, OS_App* app)
{
    
    clock->elapsed = OS_GetTime(app) - clock->start_time;
    clock->start_time = OS_GetTime(app);
}
