/*
 * taimeris.c
 *
 *  Created on: 25 May 2026
 *      Author: ksvaz
 */

#include "taimeris.h"

char CheckTimer(Timer_t* timer, unsigned long time)
{
  if (!timer) return false;
  if (timer->enabled && (time - timer->lastTime >= timer->period))
  {
    timer->lastTime = time;
    if (timer->oneTime) timer->enabled = false;
    return true;
  }
  return false;
}

void InitTimer(Timer_t* timer, unsigned long period, unsigned long period_offset, char enabled, char oneTime)
{
  timer->lastTime = period_offset;
  timer->period = period;
  timer->period_offset = period_offset;
  timer->enabled = enabled;
  timer->oneTime = oneTime;
}

void EnableTimer(Timer_t* timer, unsigned long time)
{
  timer->lastTime = time - timer->period_offset;
  timer->enabled = true;
}

void DisableTimer(Timer_t* timer, unsigned long time)
{
	timer->lastTime = time - timer->period_offset;
  timer->enabled = false;
}
