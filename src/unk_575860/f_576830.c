/* 0x576830, the kinds 0x17 / 0x18 set the pending state 0xa / 0xb (1), else 3. Ends at 0x57689f; 0x5768a0 is a
   new start without nop padding */

int ai_Unk576830(int *p)
{
    if (p[0] == 0x17) {
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    if (p[0] == 0x18) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
