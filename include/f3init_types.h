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
    char pad4a[2];                  /* 0x4a, no user seen */
    int mission;                    /* 0x4c, the chosen mission (ai_ChooseMission's result, main_Unk536980; ai_NewMission's argument) */
    unsigned char talents[5];       /* 0x50, the five talent values (main_ChooseTalents; copied as an int and a byte) */
    char model[0x33];               /* 0x55, the character model name ("patrizier_MANN2", the model chooser main_Unk534b30) */
    int modelId;                    /* 0x88, the model's id: unk_Unk4fb770(model) + 0x639 */
} INIT_PLAYER;                      /* 0x8c bytes (main_Menue clears 0x8c) */

#endif
