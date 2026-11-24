/*
 * taimeris.h
 *
 *  Created on: 25 May 2026
 *      Author: ksvaz
 */

#ifndef INC_TAIMERIS_H_
#define INC_TAIMERIS_H_

#define true (0==0)
#define false (0!=0)

typedef struct
{
  unsigned long lastTime;
  unsigned long period;
  unsigned long period_offset;
  char enabled;
  char oneTime;
}Timer_t;

char CheckTimer(Timer_t* timer, unsigned long time);

void InitTimer(Timer_t* timer, unsigned long period, unsigned long period_offset, char enabled, char oneTime);

void EnableTimer(Timer_t* timer, unsigned long time);

void DisableTimer(Timer_t* timer, unsigned long time);


#endif /* INC_TAIMERIS_H_ */
