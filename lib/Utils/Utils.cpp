#include "Utils.hpp"

void time_calculations(TaskTiming* tt)
{
    int timeDiff = tt->timeEnd - tt->timeStart;    
    tt->timeCount += 1;
    tt->timeSum += timeDiff;
    if(timeDiff > tt->timeMax) tt->timeMax = timeDiff;
    if(timeDiff < tt->timeMin) tt->timeMin = timeDiff;
}