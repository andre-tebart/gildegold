/* 0x524160, removes the character c (index) from the member slots (+0x8c) of the group record g @deep @deep: behaviour draft, readable done. 11 code hunks left, two causes, both entry scheduling / dead values: (1) the original materialises the character base (lea ecx,[eax*4+0x770b80]) and then never loads through it - it loads the id with the SIB form mov edx,[eax*4+0x770b84]; ours emits only the load. Tried: 'ch = ai_Chars[c]; id = *(int *)(ch + 4)' (CSE, no lea), the typed ((SVCHARREC *)ai_Chars[c])->netId (no lea), the char[0x21c] index form - all 11; (2) the original computes p = g+0x8c after 'i = 0' (xor eax,eax; lea ecx,[esi+0x8c]), ours hoists the lea above the id computation - the statement order is already id / i = 0 / p = ..., a scheduler choice; spelling p as grp->member + i changes the lea to a SIB form (worse). Kept: id via SVCHARREC.netId, the scan through SVGROUP.member (the pointer scan + indexed store, the original's add ecx,4 loop), which made it readable (Q4 clear). Nothing else in the body differs: the do-while, the -1 store and the 8 bound are the original's. */

void sv_RemoveGroupMember(int c, char *g)
{
    int i, id;
    int *p;
    SVGROUP *grp;

    id = ((SVCHARREC *)ai_Chars[c])->netId;
    i = 0;
    grp = (SVGROUP *)g;
    p = grp->member;
    do {
        if (*p == id) {
            grp->member[i] = -1;
            return;
        }
        i++;
        p++;
    } while (i < 8);
}
