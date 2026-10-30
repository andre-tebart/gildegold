/* 0x483d30, starts a work on a building (the event 0x8d) for the kind-0x1d master at 7 / 12 / 18 o'clock
   when none runs yet: picks the work entry below a random threshold (unk_Unk4f22a0) and a free worker
   of the building, then sends message 0x8d with the entry's value, its list index and the worker's netId
   @deep behaviour, Q-only handover. Q2: AIBLD.unk65 only - read as the work record pointer (a char * load at building + 0x65 in gv.c / f3gm.c, the work event 0x571640 in he.c) and as a number in ai_CalcMeisterBank (compared to AIPOS.unk18 and written as field delta 0x65), so no single role name exists in the sources. Q4: the one raw access (a char cast written into msg.arg[2]) needs an AIMSG view; the typed member tried this session (an anonymous struct with a char entryIndex at 0x60 in the arg union) was built and regressed ai_Unk483a60 from byte to 5 differing bytes (the [esp+0x10]/[esp+0x18] slot swap) - reverted; a shared-header decision for match-deep. Readability done here: Q1 ai_StartBuildingWork (role: posts the work event 0x8d for the kind-0x1d master), Q6 description, ai_Unk4ef6b0 -> gv_EntryIndex (with its gv.c definition); the 0x4f22a0 rename to gv.c's unk_name was tried and reverted - the AI caller needs an int * result while the gv.c draft returns int, so the two headers disagree (lint). the remaining 13 differing bytes are order hunks (behaviour). */

void ai_StartBuildingWork(AICHAR *ch)
{
    AIMSG msg;
    AICHAR *f;
    int *p;
    int c;

    if ((short)ai_TimeT.hour != 7 && (short)ai_TimeT.hour != 0xc && (short)ai_TimeT.hour != 0x12) {
        ch->flags1b4 &= ~0x800;
        return;
    }
    if (!(ch->flags1b4 & 0x800)) {
        ch->flags1b4 |= 0x800;
        if (!he_FindEvent(2, 0, 0x8d, 3, ch->bld->id)) {
            p = unk_Unk4f22a0(ch->bld, (char)util_Random(0x40) - 0x80);
            if (p) {
                f = 0;
                for (c = 0; !f; c++) {
                    if (c >= 768) {
                        if (!f)
                            return;
                        break;
                    }
                    if (ai_CharsT[c].id != 0xffff && ai_CharsT[c].bld == ch->bld && ai_CharsT[c].title
                        && (!ai_CharsT[c].job || ai_CharsT[c].job->kind == 4 || ai_CharsT[c].job->kind == (char)0x8c)
                        && ai_CharsT[c].model && ai_CharsT[c].model->bldId == ai_CharsT[c].bld->id)
                        f = &ai_CharsT[c];
                }
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x8d;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.time = ai_TimeT;
                msg.flags = 1;
                msg.arg[1] = *p;
                *(char *)&msg.arg[2] = gv_EntryIndex(ch->bld->unk65, p);
                msg.ids[0] = f->netId;
                cm_SendLongMessage(&msg);
            }
        }
    }
}
