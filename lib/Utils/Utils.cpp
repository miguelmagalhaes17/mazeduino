#include "Utils.hpp"

void time_calculations(TaskTiming* tt)
{
    int timeDiff = tt->timeEnd - tt->timeStart;    
    tt->timeCount += 1;
    tt->timeSum += timeDiff;
    if(timeDiff > tt->timeMax) tt->timeMax = timeDiff;
    if(timeDiff < tt->timeMin) tt->timeMin = timeDiff;
}

void print_time_calculations(String taskName, TaskTiming* tt) // I do not care
{
    Serial.print("-------------------------------------------------------------------\n");
    Serial.print("------------------------- TASK TIME INFO --------------------------\n");
    Serial.printf("Task: %s task\n", taskName);
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", tt->timeMin, round(tt->timeSum/tt->timeCount), tt->timeMax);
}