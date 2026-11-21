/* f3snd.c's music track handles for the units that read them without the rest of f3snd.h (he.c): types only.
   f3snd.h includes it. */
#ifndef F3SND_TYPES_H
#define F3SND_TYPES_H
struct MSXTRACK;

typedef struct G16TRACKHANDLES {        /* 0x8 bytes, snd_TrackHandles: the music track and a copy of it,
                                           always written together (0x4c1bd0, 0x4c18a0) */
    struct MSXTRACK *current;           /* 0x00, the music track */
    struct MSXTRACK *copy;              /* 0x04, a copy of current, never read */
} G16TRACKHANDLES;
#endif
