/* 0x405fb0, action callback: plays a->u.name and drops the object at frame args[0] @deep behaviour: the only meaning-class hunk left is 406050 (7 instr): the original reloads a->ch ([esi+0x14]) into ecx for the ch_SetHandObject call inside the frame check, ours forwards the entry local ch (edi); the other two ch_SetHandObject sites reload a->ch in both sides. Same statements and call arguments (read against 405fb0..4060d7). Tried this session: source already passes a->ch (spell reload prescription 'drop the local, re-read the source expression'; the local is required for ch->next/ch->slot/ch_AnimDone), an explicit ch = a->ch; before the call (optimised away, byte-identical 85/272), and the spell push-pop/dup-tail hunks at 406083/4060c3 are regs/order consequences of the same load. Previous attempts (shared unk080 re-test, count re-test, raw offsets, inline helper, first flag, slot local, no ch local) are in notes/units/ch.md. Needs vargen/alias forms. Readability advanced: CHARACTER.unk064 named rightHand (0x64, ch_SetHandObject hand 2, ch_SetVisible), CH_SLOT/CH_UNK080 replaced by ch->slot/ch->next, Q4 clean. Q2 left (evidence gap, deep): CH_ACTOBJ.unk174 is a per-action int payload - gate actions store a room id / ch_InsertAction arg, take/drop store the D3ANIMDATA * of the started animation; no single evidenced name, and CHACTOBJ.unk170 has the same conflict (room id vs animation) so the three ints need a unit-wide decision. */

void ch_ActDroppingObject(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    char buf[0x100];

    if (!a->count) {
        if (!ch->rightHand) {
            ch_RemoveAction(a);
            return;
        }
        if (a->u.name[0])
            CH_ACTOBJ(a)->unk174 = (int)ch_SetNextAnim(ch, a->u.name, 1);
        if (ch->next)
            return;
        ch_SetHandObject(a->ch, a->args[2], 0);
        ch_RemoveAction(a);
        return;
    }
    if (ch->next)
        return;
    if (ch->slot && ch->slot->frame >= a->args[0]) {
        ch_SetHandObject(a->ch, a->args[2], 0);
        a->args[0] = 99999999;
    }
    if (ch->next)
        return;
    if (!ch_AnimDone(ch))
        return;
    if (a->args[0] != 99999999) {
        ch_SetHandObject(a->ch, a->args[2], 0);
        sprintf(buf, "ch_DroppingObject(): object not dropped yet (should be in frame:%i)!", a->args[0]);
    }
    ch_CheckAniMorph(ch);
    ch_RemoveAction(a);
}
