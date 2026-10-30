/* The ambient-sound flag snd_AmbientOn's union, for the units that declare it without the rest of f3snd.h:
   types only. f3snd.h includes it. */
#ifndef F3SND_TYPES_H
#define F3SND_TYPES_H

typedef union G16AMBIENTON {            /* 0x6f0b60: the ambient-sound flag; 0x4c23f0 reads the word */
    char on;                            /* 0x00, 1: ambient sounds on */
    unsigned int word;                  /* 0x00, the same bytes read as a word (0x4c23f0) */
} G16AMBIENTON;

#endif
