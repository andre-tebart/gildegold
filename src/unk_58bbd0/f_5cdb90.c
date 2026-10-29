/* 0x5cdb90, the spy list window "privillegien\spionage2" 0x62cd7c (760 B, ROUGH SKELETON: no Ghidra
   export; from the disassembly): the frame 0x878; param_1 the message record (+0 the id), param_2 the
   request (+4 a handle, the word +0 0xffff closes), param_3 / param_4 the two ids. The list 0x4f3fe0
   (the request handle, the ids, the id) returns the count (< 1 -> 0x60); each id picks the record
   0x91f700 + the id * 0x2d (the flag when the first dword is 1); both ids equal clears the flag; the
   sort 0x4f4390 and the window 0x4741f0 / 0x475600 with the list; the rest is a comment. ROUGH
   SKELETON. */

int ai_Unk5cdb90(int param_1, int *param_2, int param_3, int param_4)
{
    int ids[0x100];
    char recs[0x400 * 4];
    int out[8];
    int count;
    int flag;
    int i;

    if (*(unsigned short *)param_2 == 0xffff)
        return 0;
    count = unk_Unk4f3fe0(*(int *)((char *)param_2 + 4), ids, *(unsigned short *)param_1);
    if (count < 1)
        return 0x60;
    flag = 0;
    for (i = 0; i < count; i++) {
        memcpy(recs + i * 0x2d, (char *)((unsigned)&ai_Unk91f700 + ids[i] * 0x2d), 0x2d);
        if (*(int *)(recs + i * 0x2d) == 1)
            flag = 1;
    }
    if (param_3 == param_4)
        flag = 0;
    unk_Unk4f4390(count, out);
    /* ROUGH SKELETON: the window 0x62cd7c, the list and the pump 0x5309a0 are not written yet */
    return flag;
}
