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



torn_function i32
U_GetTextLength(u8* text)
{
    i32 length = 0;
    while (text[length++] != '\0');
    return length;
}