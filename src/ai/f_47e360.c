/* 0x47e360, a master of this machine looks for a free workshop of its own that needs hiring, then lets its
   partner logic run
   @deep byte; readability left: Q1 no evidenced name (the role is a local master's per-frame scan of its own buildings that announces a master hire - its sprintf 'stellt Meister fuer %s ein' - then runs the partner logic; no string or text id names it), Q2 AICHAR.health (32 uses, truthy / > 0 tests only, no reader names it) and AICHAR.unk1b1 (2 uses, 0x47e360 skips its post-partner action refresh and 0x4bb6c0's sender skips the character), Q3 0x7d5f82 = ai_Chars + 768 * 0x21c + 2 loop bound, still a literal (an extern would need an evidenced name for the one-past-the-end kind field). Named on the way: ai_FindPartner, ai_ChooseAction, ai_ExecuteAction, ai_FillNearRing, cm_SendMessage3d, ai_BuildingTypeMasks, ai_Partner, ai_ActionFlags, gm_FindChar, cm_AddFieldDelta, AICHAR.act / mask, AITIME.hour. gm_BuildingClass (0x501d80) is in no source and has no Ghidra export: read by disassembly, it maps ai_BTypes[type].kind 1..0x20 to the category 1..9 (jump table 0x501e14, default 0) - a role name (gm_BuildingClass) needs the defining unit, which is undrafted. */

void ai_MasterUpdate(AICHAR *c)
{
    AIBLD *b;
    AICHAR *p;
    AIBTYPE *bt;
    char buf[1024];
    unsigned int lo, hi;
    int n, found, v;
    unsigned int sel;

    if (ai_IsLocalMaster(c) && (c->flags1cc & 0x3000) == 0) {
        for (n = 0x100, b = ai_BldsT; n; n--, b++) {
            if (b->type) {
                lo = 0;
                hi = 0;
                bt = &ai_BTypesT[b->type];
                if ((unsigned char)bt->kind > 0x1f)
                    hi = 1 << ((unsigned char)bt->kind - 0x20);
                else
                    lo = 1 << (unsigned char)bt->kind;
                if (b->owner == c->id && !(b->flags5a & 1) && ((lo & ai_BuildingTypeMasks[2]) || (hi & ai_BuildingTypeMasks[3]))) {
                    found = 0;
                    p = ai_CharsT;
                    while (1) {
                        if ((int)&p->kind >= (unsigned)&gm_PlayerRecordBytes)
                            break;
                        if (p->id != 0xffff && p->bld == b && (p->kind == 1 || p->kind == 2))
                            found = 1;
                        p++;
                        if (found)
                            break;
                    }
                    if (gm_BuildingClass(b->type) != 6 && !found) {
                        v = b->type;
                        cm_SendMessage3d(b, 1, ai_BTypesT[v].unk230, ai_BTypesT[v].unk223);
                        sprintf(buf, "%li:%02li - %s stellt Meister f\xfc" "r '%s' ein", ai_TimeT.hour,
                                ai_TimeT.min, c->name, b->name);
                    }
                }
            }
        }
        if (c->health > 0) {
            ai_Partner = ai_FindPartner(c);
            ai_FillNearRing(c);
            sel = c->act;
            if (!c->unk1b1) {
                if (ai_ChooseAction(c->id) && (c->act != sel || (ai_ActionFlags[c->act][0] & 0x200)))
                    ai_ExecuteAction(c->id);
            }
            ai_Partner = 0;
        }
    }
}
