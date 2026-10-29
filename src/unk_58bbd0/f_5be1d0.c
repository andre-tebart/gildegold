/* 0x5be1d0, the status of a special-amount record (100 B): the record id +4, the kind +0xc, the +0x1c8
   flags; field +0x1c = 0x6ca - (the byte +0xc != 0), +0x20 the 0x6cc carrier match (0x53e400 (1, 0, 0x6f)
   over the +0xac owner and the +0xb0 id), +0x24 the 0x6cd / 0x6ce contact, +0x28 the 0x6cb .. 0x6d2
   price class from 0x4fc820; returns 1 */

int unk_Unk4f4130(); float gm_CharacterHealth(); int amt_HasColumn7Holder(); int d2_WinCreateSlider();
void d2_SetObjWidth(int obj, short w); int ai_Unk4e70f0(); int amt_GetColumnOffices(int col, int max, struct AMTREC *out);

int ai_AmountRowStatus(int *param_1)
{
    int chr;
    int car;
    float f;

    chr = *param_1;
    if (chr == 0)
        return 0;
    param_1[8] = 0;
    param_1[7] = 0x6ca - (*(char *)(chr + 0xc) != 0);
    if (*(char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) != *(char *)(chr + 9) &&
        (U_I(chr + 0x1c8) & ((unsigned)&ai_Relations + 0x1aca0)) != 0) {
        car = he_FindEvent(1, 0, 0x6f);
        while (car != 0) {
            if (U_I(car + 0xac) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c) &&
                U_I(chr + 4) == U_I(car + 0xb0)) {
                param_1[8] = 0x6cc;
                break;
            }
            car = he_NextEvent();
        }
    }
    param_1[9] = 0;
    if (unk_Unk4f4130(U_I(chr + 4), U_W((unsigned)&ai_PlayerId)) < 1) {
        car = he_FindEvent(1, 0, 0x18);
        while (car != 0) {
            if (U_W(car + 8) == U_W((unsigned)&ai_PlayerId) && U_I(chr + 4) == U_I(car + 0xc4)) {
                param_1[9] = 0x6ce;
                break;
            }
            car = he_NextEvent();
        }
    } else {
        param_1[9] = 0x6cd;
    }
    param_1[10] = 0;
    car = he_FindEvent(1, 0, 0x45);
    while (car != 0) {
        if (U_I(car + 0xac) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c) &&
            U_I(chr + 4) == U_I(car + 0xb0)) {
            param_1[10] = 0x6cb;
            break;
        }
        car = he_NextEvent();
    }
    if (param_1[10] == 0) {
        f = gm_CharacterHealth(chr);
        if (f < 100.0f)
            param_1[10] = 0x6d2;
        else if (f < 350.0f)
            param_1[10] = 0x6d1;
        else if (f < 650.0f)
            param_1[10] = 0x6d0;
        else
            param_1[10] = 0x6cf;
    }
    return 1;
}
