/* 0x5d0f20, an action callback of the record table 0x62c420 (record 17 at 0x62c5fc, the text id 0x16a,
   992 B): param_1 the message record (+2 the type), param_2 a state record (+0x10 set to 1, +4 the
   character handle). Type 6: the caption 0x5176d0 and the menu 0x5c1130 (the config 0x400 / 6 / the two
   ids 0x5eed68 / 0x5eed6c) give the character; other types take it from 0x505df0 (param_2 + 4). Then
   0x5ba6b0 (param_1, 0x2c5) picks a value used as 1 / 2, and two random picks 0x42ec00 (0x300) walk the
   0x300 character records 0x770b80 + i * 0x21c, skipping the ids of param_1, param_2 (and of the first
   pick), the free records (word +0 == -1) and the kinds +2 >= 0xa; the byte 0x770ce6 must be nonzero.
   The tail (0x4c4b00, 0x5176d0, 0x53dda0, 0x5ba660 and the panel 0x5cf820) is comments.
   ROUGH (layout 2026-10-24, tools/disasm.py): the pick loops are drafted; the text / message tail is not. */

int ai_Unk5d0f20(int param_1, int param_2)
{
    char text[0x800];
    int cfg[8];
    int actor;
    int first;
    int second;
    int seed;
    int tries;

    *(int *)(param_2 + 0x10) = 1;
    if (*(char *)(param_1 + 2) == 6) {
        /* ROUGH: the caption id of 0x5176d0 (text) and the config block fields are not read yet; the
           disassembly stores the ids U_I(0x5eed68) / U_I(0x5eed6c), 0x400 and 6 into the block at
           esp+0x40 and calls 0x5c1130 (the block, the text, 0, 0). */
        cfg[0] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
        cfg[1] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
        cfg[2] = 0x400;
        cfg[3] = 6;
        txt_Format(text, (char *)0);
        actor = ai_AmountMenu(cfg, text, 0, 0);
    } else {
        actor = gm_FindChar(*(int *)(param_2 + 4));
    }
    if (actor == 0)
        return 0;
    first = 0;
    second = 0;
    seed = util_Random(0x300) & 0xffff;
    for (tries = 0x300; tries != 0; tries--) {
        if (seed != *(unsigned short *)actor && seed != *(unsigned short *)param_2 &&
            *(short *)((unsigned)&ai_Chars + seed * 0x21c) != -1 &&
            *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + seed * 0x21c) < 0xa &&
            *(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + seed * 0x21c) != 0) {
            first = (unsigned)&ai_Chars + seed * 0x21c;
            break;
        }
        seed = (seed + 1) % 0x300;
    }
    seed = util_Random(0x300) & 0xffff;
    for (tries = 0x300; tries != 0; tries--) {
        if (seed != *(unsigned short *)actor && seed != *(unsigned short *)param_2 &&
            (first == 0 || seed != *(unsigned short *)first) &&
            *(short *)((unsigned)&ai_Chars + seed * 0x21c) != -1 &&
            *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + seed * 0x21c) < 0xa &&
            *(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + seed * 0x21c) != 0) {
            second = (unsigned)&ai_Chars + seed * 0x21c;
            break;
        }
        seed = (seed + 1) % 0x300;
    }
    /* ROUGH: the three 0x4c4b00 calls, the texts 0x5176d0, the messages 0x53dda0, the 0x5ba660
       lookup and the panel 0x5cf820 follow. */
    return 0;
}
