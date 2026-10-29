/* 0x4ec940, the message 'SsCa' (cm_SendCommand) of the character c with the id and the value v clamped to 0 .. 1; -1 when c has no object (0x4eaad0) @deep B3 draft, close (69/69 instructions, 12 differing): the two out-of-line return blocks are in the other order - the original has m.value.f = 0.0f first (0x4ec9c1), ours the 1.0 block - plus the block's lea register (ecx vs eax); the 42 leading instructions and both branch tests match (fcomp 0x5eb788 0.0 / 0x5eb780 1.0, and 0x4100 / test ah,5 jp = the !(v > 0) / !(v < 1) NaN forms). Read: disasm 0x4ec940..0x4ec9f4, cm_SendCommand, the sibling ahm_SendSetPlaceValue. Leads tried, one build each, no gain: the else-form, the inverted form, if (!(v > 0)), the 1.0 block inside the outer if with the 0.0 block after it (no else keyword - 12 as before), and the two-guard early-return form (worse: 30). spell.py: no menu rule matches the 4ec9c1 hunk, 'no hunk matched the menu; escalate'. Q1..Q7 pass. */

int ahm_SendSetShare(char *c, int id, float v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_SET_SHARE;
    m.id = id;
    if (v > 0.0f) {
        if (v < 1.0f) {
            m.value.f = v;
            return ((int (*)(void *))cm_SendCommand)(&m);
        }
    } else {
        m.value.f = 0.0f;
        return ((int (*)(void *))cm_SendCommand)(&m);
    }
    m.value.f = 1.0f;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
