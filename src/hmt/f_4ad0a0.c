/* 0x4ad0a0, when the current event / person (hmt_Event / hmt_Person) has begun and object 0x26 / 0x27 is free,
   it is created (0x4c4650 with 0x15 / 0x16, a delay of 25..32) */

void hmt_Begin(void)
{
    int year;
    HMTOBJ ev, ps;
    HMTREC *r;
    int kind;

    year = ai_TimeT.year + 0x578;
    if (amt_GetFirstOffice(0x26, &ev) && amt_GetFirstOffice(0x27, &ps)) {
        if (ev.id == -1 && hmt_Event != -1) {
            r = hmt_Find(hmt_Event, &kind);
            if (r != 0 && r->begin <= year)
                cm_SendCreateCharacter(0x15, hmt_Event, -1, util_RandomU(8) + 0x19, -1, 0, 0, 0);
        }
        if (ps.id == -1 && hmt_Person != -1) {
            r = hmt_Find(hmt_Person, &kind);
            if (r != 0 && r->begin <= year)
                cm_SendCreateCharacter(0x16, hmt_Person, -1, util_RandomU(8) + 0x19, -1, 0, 0, 0);
        }
    }
}
