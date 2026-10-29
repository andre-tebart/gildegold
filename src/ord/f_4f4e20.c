/* 0x4f4e20, the order "apply for office" (table 0x611768 entry 0; c the character, m the message: m[0] the
   kind 1 = do / 3 = ask, the text at m + 5): only when c has no partner (partner = -1) or the partner record
   (0x505df0) is gone / not in use (health 0) or of kind 0xf; the ask runs the office application 0x5c6d00
   (its result | 2), the do of a player (kind 6) looks for the player's first office event (0x53e400 (1, 0,
   0x41)) and writes the text 0x1ce2 with the event's target id, else the text 0x1ce1, into m + 5; returns 1
   (not handled) or 2
   @deep meaning settled, only allocation/layout left: the original's m = ebx, c = edi, r = esi (ours m = edi, c = ebx, r = esi), its return-1 block sits at the end of the function (ours inline before the body), and its loop return is mov eax,esi where ours constant-folds mov eax,2. Read vs tools/disasm.py 0x4f4e20. Tried and kept (no WORSE, 240 -> 191 differing bytes): r = 2 + if/else-if with a shared return r (the 0x4f4ff0 idiom), typed ORDCHAR/ORDEVENT views (gv.h). Tried and reverted (build shows the same m = edi allocation): spell.py menu push-pop/dup-tail/reload leads; early-return guard if (!(...)) return 1; explicit else return 1; goto fail: label at the end; int partner = c->partner temp; char kind = c->kind temp; partner + kind temps; declaration-order permutations of o/p/r; unsigned r; unsigned char *o; int *m copy (int *msg = m, copy-propagated back). Needs match-deep's register-allocation toolbox (spell 'push-pop: the block the callee-saved register crosses' / vargen ren). */

unsigned ord_ApplyForOffice(ORDCHAR *c, int *m)
{
    ORDEVENT *ev;
    ORDCHAR *ch;
    unsigned short *p;
    int r;

    if ((m[0] == 3 || m[0] == 1)
        && (c->partner == -1
            || ((ch = (ORDCHAR *)gm_FindChar(c->partner)) != 0 && (ch->health == 0 || ch->kind == 0x0f)))) {
        r = 2;
        if (m[0] == 3) {
            r = ai_OfficeApplication((short *)c, (int)m);
            r |= 2;
        } else if (m[0] == 1 && c->kind == 6) {
            ev = (ORDEVENT *)he_FindEvent(1, 0, 0x41);
            while (ev != 0) {
                if (ev->ownerId == ORD_CHARS[ai_PlayerId].netId) {
                    p = (unsigned short *)gm_FindChar(ev->targetId);
                    txt_Format(m + 5, 0x1ce2, *p);
                    return r;
                }
                ev = (ORDEVENT *)he_NextEvent();
            }
            txt_Format(m + 5, "%s", 0x1ce1);
        }
        return r;
    }
    return 1;
}
