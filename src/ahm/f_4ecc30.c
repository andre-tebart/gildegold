/* 0x4ecc30, the message 'cRSa' (0x4c61b0) of the character c for the place id with the value v clamped to 0 .. 1; -1 when c has no object or no such place (0x4eabe0) @deep B3 draft, close (89/89 instructions, 12 differing): the same clamp block order as ahm_SendSetShare - the original lays the m.value.f = 0.0f block out first (0x4ecccb), ours the 1.0 block, and the block's lea register differs (edx vs eax); the 56 leading instructions and both branch tests match. Read: disasm 0x4ecc30..0x4eccf8, the sibling ahm_SendSetShare. Leads tried (same source family as the sibling, one build each): the else-form, the inverted form, if (!(v > 0)), the no-else form (1.0 inside the outer if, 0.0 after - the current source, 12), the two-guard early-return form (worse, 30 on the sibling). spell.py: no menu rule matches the 4ecccb hunk. Q1..Q7 pass. */

int ahm_SendSetPlaceValue(char *c, int id, float v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    if (ahm_FindSlot(c, id) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_SET_PLACE;
    m.id = id;
    if (v > 0.0f) {
        if (v < 1.0f) {
            m.value.f = v;
            return ((int (*)(void *))cm_SendCommand)(&m);
        }
        m.value.f = 1.0f;
        return ((int (*)(void *))cm_SendCommand)(&m);
    }
    m.value.f = 0.0f;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
