/* 0x4ace40, once a year is reached: the end of the current event (object 0x26) and the current person (object
   0x27) are announced to the human players (kind 6 / 7) */

void hmt_Update(void)
{
    int year;
    HMTOBJ ev, ps;
    AICHAR *ce, *cp;
    HMTREC *r;
    int kind;
    char text[256];
    int i;

    year = ai_Time.year + 0x578;
    if (amt_GetFirstOffice(0x26, &ev) && amt_GetFirstOffice(0x27, &ps)) {
        ce = gm_FindChar(ev.id);
        cp = gm_FindChar(ps.id);
        if (ev.id != -1 && hmt_CurrentEvent != -1) {
            r = hmt_Find(hmt_CurrentEvent, &kind);
            if (r != 0 && r->end <= year) {
                amt_SendOfficeMessage(ev.obj, 0, 0, 1, 0xff);
                cm_SendKillCharacter(ev.id, 1);
                if (ce != 0) {
                    txt_Format(text, 0x42b2, ce->id);
                    for (i = 0; i < 768; i++) {
                        if (ai_Chars[i].kind == 6 || ai_Chars[i].kind == 7)
                            he_ShowNews(ai_Chars[i].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_00");
                    }
                }
            }
        }
        if (ps.id != -1 && hmt_CurrentPerson != -1) {
            r = hmt_Find(hmt_CurrentPerson, &kind);
            if (r != 0 && r->end <= year) {
                amt_SendOfficeMessage(ps.obj, 0, 0, 1, 0xff);
                cm_SendKillCharacter(ps.id, 1);
                if (cp != 0) {
                    txt_Format(text, 0x42b4, cp->id);
                    for (i = 0; i < 768; i++) {
                        if (ai_Chars[i].kind == 6 || ai_Chars[i].kind == 7)
                            he_ShowNews(ai_Chars[i].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_02");
                    }
                }
            }
        }
    }
}
