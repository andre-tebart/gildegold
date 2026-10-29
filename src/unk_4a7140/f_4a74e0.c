/* 0x4a74e0, one step of the game: returns the dialog to show (0x10 / 0x21 / 9 / 0x29 / 0x1c) or 0 */

int ai_Unk4a74e0(void)
{
    switch (ai_Unkf83654->phase) {
    case 0:
        ai_Unkf83654->phase = 1;
        return ai_Unkf83654->unkb0 != 0 ? 0x10 : 0x21;
    case 1:
        ai_Unk4a8650();
        ai_Unkf83654->phase++;
        ai_Unk4a8520();
        ai_Unk4a7ac0();
        ai_Unk4a86f0();
        return 0;
    case 2:
        ai_Unkf83654->phase = 3;
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 3:
        ai_Unk4a7ac0();
        ai_Unk4a7c20();
        ai_Unkf83654->phase++;
        ai_Unk4a8590();
        ai_Unk4a7a20();
        return 0;
    case 4:
        ai_Unkf83654->phase = 5;
        ai_Unk4a7ac0();
        ai_Unk4a86f0();
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 5:
        ai_Unk4a7c20();
        ai_Unk4a7a20();
        ai_Unkf83654->phase++;
        return 0x1c;
    case 6:
        ai_Unk4a77e0();
        ai_Unkf83654->phase++;
        return 0x10;
    case 7:
        ai_Unk4a85e0();
        ai_Unkf83654->phase++;
        break;
    case 8:
        ai_Unk4a7ac0();
        ai_Unkf83654->phase++;
        return ai_Unkf83654->unkb0 != 0 ? 9 : 0x29;
    case 9:
        ai_Unk4a7c20();
        ai_Unk4a7a20();
        ai_Unkf83654->phase++;
        return 0;
    case 10:
        ai_Unkf83654->phase = 11;
        ai_Unk4a8690();
        return 0x10;
    }
    return 0;
}
