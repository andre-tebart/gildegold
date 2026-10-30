/* The player profile record of f3init's init_PlayerProfile (0x75e5a0, the "Network" profile keys), for the
   units that declare it: types only (f3init has no header yet). f3init's head includes it. */
#ifndef F3INIT_TYPES_H
#define F3INIT_TYPES_H

typedef struct INIT_PLAYER {
    unsigned char tool1;            /* 0x00, the first start tool (main_ChooseTalents) */
    unsigned char tool2;            /* 0x01, the second start tool */
    unsigned char status;           /* 0x02, 0x10 in the default profile, 0x12 when the creation is done */
    unsigned char job;              /* 0x03, the job / title group (model chooser main_Unk534b30's p, gm_TitleGroup's argument) */
    int coat;                       /* 0x04, the coat of arms as an object id (Wappen: 0x59a + the chosen index) */
    unsigned char sex;              /* 0x08, 0 male / 1 female (Geschlecht) */
    unsigned char faith;            /* 0x09, 0 / 1 (Glauben) */
    char firstName[0x20];           /* 0x0a, the first name (Name) */
    char familyName[0x20];          /* 0x2a, the family name (Familienname) */
} INIT_PLAYER;

#endif
