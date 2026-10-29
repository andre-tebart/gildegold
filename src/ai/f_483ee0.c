/* 0x483ee0, the worker step of the kind-0x1d master (the graveyard: 0x47e990 dispatches 0x483fc0 for the
   building kind 0x1d), run at 7 / 12 / 18 o'clock and once per hour (flag bit 0x1000, cleared in every other
   hour): of the characters standing in the master's building it takes the first with the worker title 0x1c (the
   two titles 0x1b / 0x1c that he.c's work events 0x8c / 0x8d scale differently and that sv.c 0x52b500 poses as
   "faulenzen"), at work (+0x1b4 bit 0) and with its model in the building, and with a 25% chance ends that
   character's role pose (ai_EndCharExtra(f, 0)), which sv.c then builds again from the current role */

void ai_EndWorkerPose(AICHAR *ch)
{
    AICHAR *f;
    int c;

    f = 0;
    if ((short)ai_Time.hour != 7 && (short)ai_Time.hour != 0xc && (short)ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x1000;
        return;
    }
    if (!(ch->flags1b4 & 0x1000)) {
        ch->flags1b4 |= 0x1000;
        for (c = 0; !f; c++) {
            if (c >= 768) {
                if (!f)
                    return;
                break;
            }
            if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title == 0x1c
                && ai_Chars[c].kind == 0x14 && (ai_Chars[c].flags1b4 & 1) && ai_Chars[c].model
                && ai_Chars[c].model->bldId == ai_Chars[c].bld->id)
                f = &ai_Chars[c];
        }
        if (util_RandomF() > 0.75f)
            ai_EndCharExtra(f, 0);
    }
}
