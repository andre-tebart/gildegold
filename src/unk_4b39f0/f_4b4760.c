/* 0x4b4760, mission condition: c has three buildings of the building type of its job (gm_BuildingTypeOfJob
   of AICHAR.jobId +0x164) that stock every needed good of their type's list (AIBTYPE.slot +0x23 of the
   0x24d-byte records at ai_BTypes, bit 15 = flag)
   @deep behaviour: one code hunk left, 3 instructions (44 -> 5 differing asm lines this session; the loop is matched): our lea edi,[eax+edx*4+0x23] folds the AIBTYPE.slot member offset into the SIB displacement, the original materializes the record address (lea eax,[eax+edx*4]) and adds the member offset in a second lea (lea edi,[eax+0x23]); the fill 2 hunk is the same 2-byte size difference. The real difference was the loop condition: the original source is 'for (b = gm_BuildQuery(...); n < 3 && b != 0; b = gm_BuildNext())' with 'n += ok' at the end of the body (other spellings tried and reverted: increment in the body + in-body 'if (n >= 3) return 1;', 'b != 0 && n < 3', continue + inverted tail, while - each 33-62 differing instructions). Tried for the slot address, one build each, all fold to one lea: ai_BTypes[b->type].slot (kept, typed), &ai_BTypes[b->type].slot[0], (unsigned short *)((char *)&ai_BTypes[b->type] + 0x23), char* record local + 0x23, AIBTYPE *bt = ai_BTypes + b->type then (char *)bt + 0x23, two-step w assignment. spell.py: the code hunk has no menu rule; meaning checked (same address, typed AIBTYPE.slot). Readability done (Q1/Q2/Q4 cleared, Q6 description, Q7 clean). */

int ai_HasThreeStockedBuildings(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;
    unsigned short *w;
    int k, n, j, ok;

    k = gm_BuildingTypeOfJob(c->jobId);
    if (k == 0)
        return 0;
    n = 0;
    for (b = gm_BuildQuery(2, 4, c->id, 5, k); n < 3 && b != 0; b = gm_BuildNext()) {
        ok = 1;
        w = ai_BTypes[b->type].slot;
        for (j = 0; j < 0x40 && *w != 0; j++, w++) {
            if (!gm_Query(b->objList, 2, 6, 0, *w & 0x7fff)) {
                ok = 0;
                break;
            }
        }
        n += ok;
    }
    if (n >= 3)
        return 1;
    return 0;
}
