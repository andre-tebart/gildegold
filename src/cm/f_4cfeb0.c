/* 0x4cfeb0, the click on an own object of the pick ai_Unk6f22e8 (type 2) while characters are selected: the first selected
   character that stands still gets the comment "_ARBEITER_KOMMENTARE_KLICK", the object is marked (+0x188). Rough draft. */

void ai_Unk4cfeb0()
{
    char *t = ai_Unk6f0d74;
    char *c, *sel, *o, **pp;

    ai_ClickedObject = 0;
    for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        c = *pp;
        if (c != 0 && c[0x188] != 0 && (ai_Unk4ce170(*(unsigned short *)t, c) & 0x800) && *(int *)(c + 0x184) != 0 &&
            c[8] != 0)
            break;
    }
    if (dm_L.lup != 0 && ai_Unk5fe710 == -1) {
        sel = ai_Unk6f22e8;
        if (sel != 0 && *(int *)(sel + 0x220) == 2) {
            o = *(char **)(sel + 0x200);
            if ((ai_Unk4ce170(*(unsigned short *)t, o) & 0x800) && kb_Down[0x36] == 0 &&
                (c == 0 || ai_Unk4ce270(c, o) == 0)) {
                ai_Unk4d08c0();
                o[0x188] = 1;
                if (*(int *)(o + 0x16c) == ai_Unk6f0cec)
                    snd_QueueVoice(*(unsigned short *)o, ai_Unk6f0d24, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                else
                    snd_QueueVoice(*(unsigned short *)o, ai_Unk6f0d28, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                sel = ai_Unk6f22e8;
            }
        }
        ai_ClickedObject = (int)sel;
    }
}
