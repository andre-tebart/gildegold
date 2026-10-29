/* 0x5867c0, the graveyard "Schlag" action (114 B): when the grave object 0x6f22f8 exists with the flag +0x1b4
   bit 0 and U_I (0x146738c) is set: 0x587c40 (the object, 0), 0x4c4b00 (the character + 4, the object's +4, -5,
   0), then 0x52b490 (the object, "locations/friedhof/schlag_FRIEDHOF.esc" 0x62a718) +0xc8c = 1 */

void ai_EndCharExtra(char *ch, int f); void cm_SendRelations(int a, int b, int c, int d); int sv_Unk52b490();

void ai_Unk5867c0(char *unused)
{
    char *p;

    p = *(char **)(unsigned)&ai_Unk6f22f8;
    if (p != 0 && (*(unsigned char *)(p + 0x1b4) & 1) && U_I(((unsigned)&dm_L + 0xc)) != 0) {
        ai_EndCharExtra(p, 0);
        p = *(char **)(unsigned)&ai_Unk6f22f8;
        cm_SendRelations(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(int *)(p + 4), -5, 0);
        p = (char *)sv_Unk52b490((int)*(char **)(unsigned)&ai_Unk6f22f8, (char *)(unsigned)&full_data_62a718);
        if (p != 0)
            *(int *)(p + 0xc8c) = 1;
    }
}
