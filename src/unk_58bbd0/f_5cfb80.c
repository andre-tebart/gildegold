/* 0x5cfb80, a message callback (128 B): param_1 the record (+2 the type, +4 the handle, +0x1c a
   float), param_2 the request (+0xc the mode, +0x10 a flag). Mode 1: 0x4c4ac0 (the handle, 0x1c, the
   float +0x1c * 0x5eb91c) - the third argument is the raw 4-byte float (the cm.h int view of 0x4c4ac0
   is wrong for this call site, so the call goes through the cast). Mode 0 with the type 6: the request
   +0x10 = 1 and the text 0x5176d0 (0xff0) through the panel 0x5cf820. Returns 1. */

int ai_Unk5cfb80(int param_1, int param_2)
{
    char text[0x800];

    if (*(int *)(param_2 + 0xc) == 1) {
        ((void (*)(int, int, float))(unsigned)&ai_Unk4c4ac0)(*(int *)(param_1 + 4), 0x1c,
                                             *(float *)(param_1 + 0x1c) * (0.100000001f));
    } else if (*(int *)(param_2 + 0xc) == 0 && *(char *)(param_1 + 2) == 6) {
        *(int *)(param_2 + 0x10) = 1;
        txt_Format(text, (char *)0xff0);
        ai_Unk5cf820((int)text);
    }
    return 1;
}
