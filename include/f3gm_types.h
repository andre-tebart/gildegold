/* The job record of f3gm.c's table 0x61a688 (gm_GetJobTalents returns it by value), for the units that use it
   without the rest of f3gm.h: types only. f3gm.h includes it. */
#ifndef F3GM_TYPES_H
#define F3GM_TYPES_H

typedef struct JOBTALENTS {             /* 6 bytes, one per job at 0x61a688 */
    unsigned char skills[5];            /* 0x0, the five skill bytes a character of the job starts with (0x507690 ages them into a new character's +0x80..+0x84, the player profile copies them into its talents) */
    unsigned char rank;                 /* 0x5, 1..4 in the table: a guess from the values and the rank byte +0xd */
} JOBTALENTS;

#endif
