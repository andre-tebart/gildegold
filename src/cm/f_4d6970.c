/* 0x4d6970, one frame of the fight UI (r: the fight record): the view 0x60cb64 sized to the fight area, the redraws
   0x525ff0 .. 0x4ce830, and the click on the button 0x60cb68 (while 0x6f0d88 / 0x6f0cf4 are clear) sends message 0x50 with
   the fight record of the player's character (0x4cdf00) marked 5, else the dialog is closed (0x6f22a0) */

void ai_Unk4d6970(char *r)
{
    int sl[11];
    int *q;

    d2_SetObjValue(ai_Unk60cb64, 0, ai_Unk60d364, ai_Unk6f0d84 - ai_Unk6f0d80 + 1);
    sv_Unk525ff0();
    ai_Unk4cf9c0();
    ai_Unk4cf670();
    ai_Unk4cfeb0();
    ai_Unk4d0740(r);
    ai_Unk4cf000();
    ai_Unk4ced80(r);
    ai_Unk4ce300();
    ai_Unk4ce830();
    if (ai_Unk6f0d88 == 0 && ai_Unk6f0cf4 == 0) {
        if (d2_ClickedId != -1 && ai_Unk5fe710 == ai_Unk60cb68) {
            q = ai_Unk4cdf00(r, ai_Unk6f0d74);
            memcpy(sl, q, 0x2c);
            sl[0] = *(int *)(ai_Unk6f0d74 + 4);
            sl[1] = 5;
            ai_Unk4c5c40(*(int *)(r + 0x10), sl);
        }
    } else
        sv_PopRequest = 1;
}
