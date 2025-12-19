#include "Utils.hpp"

void tasK_time_calculations(TaskTiming* tt, int timeStart, int timeEnd)
{
    int timeDiff = timeEnd - timeStart;    
    tt->timeCount += 1;
    tt->timeSum += timeDiff;
    if(timeDiff > tt->timeMax) tt->timeMax = timeDiff;
    if(timeDiff < tt->timeMin) tt->timeMin = timeDiff;
}