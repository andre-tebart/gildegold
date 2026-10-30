/* ai_TimeHour: the clock's hour (0x770ae4) as a view of ai_Time (0x770ae0 + 4), not an object of its own.
   Include this after the unit's headers and the placeholder declarations (a unit keeps those declarations for
   its code shape); every use of ai_TimeHour below the include is the view. */
#ifndef TIME_VIEW_H
#define TIME_VIEW_H
#define ai_TimeHour (*(unsigned short *)((char *)&ai_Time + 0x4))  /* 0x770ae4 */
#endif
