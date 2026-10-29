/* 0x4ba630, message: one of c's buildings of kind 1 / 2 gets item 0x134 / 0x135 (0x4c4880); the text names one of
   up to 16 characters (not kind 6 / 7, below 10) of one half of ai_Chars, else c (the reverse direction finds
   nobody); sent with 0x53dee0
   @deep behaviour: 112 code hunks + 4 regs, 188/192 instructions, far. Read against the original: the statements match (the building loop, the 0x180/0x300 half pick with d = -1, the kind/id filter, the 16-entry cap, p = c fallback, cm_SendExSellObject, the text and he_MessageBoxQuickjump's 8 arguments). The root difference is allocation, not meaning: the original keeps k in ebp (xor ebp,ebp at the entry, inc ebp / cmp ebp,0x10 in the loop) and uses the zeroed ebp for its NULL compares (cmp eax,ebp), ours keeps k in [esp+0xc] and reloads it; that also drags p (original spills it to [esp+0x10], ours keeps it in edi) and the entry block (the original's zero store to c's slot before the call). Leads tried: spell.py has 'no menu rule; meaning check first' on the two entry hunks and reload leads on 4ba74f / 4ba7d4 / 4ba84f (the original re-reads, ours keeps the value - i.e. its spills, not a dropped local in our source; b->id is already re-read at both uses); adding p = 0 (the sibling idiom) compiled identically and was reverted. Q1: no name evidence - the text of message 0x31 (0x1318) is not in the exe and the good / object ids 0x134 / 0x135 it sells are unnamed, so the event's wording cannot be stated. Q4 is fixed (b->type, b->id) and Q2 by the he_MessageBoxQuickjump rename. */

int ai_Unk4ba630(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, lo, hi, d;
    void *a[16];
    char buf[0x100];

    k = 0;
    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(b->type);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_Random(n)];
    d = 1;
    if (util_Random(2)) {
        lo = 0;
        hi = 0x180;
    } else {
        lo = 0x180;
        hi = 0x300;
    }
    if (util_Random(2)) {
        hi = 0;
        lo = 0x180;
        d = -1;
    }
    for (i = lo; i < hi; i += d) {
        if (ai_Chars[i].kind != 6 && ai_Chars[i].kind != 7 && ai_Chars[i].kind < 10 && ai_Chars[i].id != 0xffff) {
            a[k++] = &ai_Chars[i];
            if (k == 16)
                break;
        }
    }
    if (k != 0)
        p = a[util_Random(k)];
    else
        p = c;
    if (b == 0 || p == 0)
        return 0x400;
    cm_SendExSellObject(b->id, -1, util_Random(2) + 0x134, 1, ai_OwnPlayer, 0);
    txt_Format(buf, m->type + 0x12e7, p->id);
    he_MessageBoxQuickjump(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, b->id, -1, 0, 0);
    return 0;
}
