/* 0x4fddb0, creates a character: flags bit 0 = 0x4fd5c0 (a pair born from the group), else 0x507690 (p2, -1, -1, p3, 0, p4, p5, 2); bit 1: stores the job bytes p4 / p5 (+0x164 / +0x165); bit 2: puts it on the stage of the building p6 (0x4fbd10). Returns the character or 0 */

short *unk_Unk4fddb0(unsigned char flags, int p2, int p3, unsigned char p4, unsigned char p5, char *p6)
{
    unsigned id;
    char *o;

    if ((flags & 1) == 0) {
        id = gm_Unk507690(p2, -1, -1, p3, 0, p4, p5, 2);
        id &= 0xffff;
    } else {
        id = unk_Unk4fd5c0(p3);
        id &= 0xffff;
        if (id == 0xffff)
            return 0;
        if (flags & 2) {
            ai_Chars[id][0x165] = p5;
            ai_Chars[id][0x164] = p4;
        }
    }
    if (id == 0xffff)
        return 0;
    if ((flags & 4) != 0 && p6 != 0 && (o = unk_Unk4fdc10((unsigned char *)p6)) != 0)
        gm_PlaceOnStage(*(int *)(ai_Chars[id] + 4), *(int *)(p6 + 1), *(int *)(o + 2), 0, 0);
    return (short *)ai_Chars[id];
}
