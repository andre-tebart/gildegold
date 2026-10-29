/* 0x5a5910, the kidnapping attempt (694 B): the player's character 0x770b80 + slot * 0x21c, the owner's
   0x770b80 + U_W(param_1 + 0x27) * 0x21c, 0x476f80; the text 0x3fea (the crew byte 0x1b1 * 2) through 0x521150
   (0x11) and the gate 0x520f90 (the character, the byte * 2); the chance (0x504ec0 (4) + 0x504ec0 (3) +
   0x504ec0 (2)) * 0.2f + 0.15f, 0x4c5f60 (the character, -the byte * 2); a roll 0x42ec20 below it: the
   capture 0x4c3ba0 / 0x4c3c00 (4, 1, param_1 - 0xa68d2c + 0x65, -1) / 0x4c49a0 / 0x4c5fa0 (the character,
   -the byte, 0), the 0x55eba0 (the character, &a, &b, 0) record sent by 0x4c52e0, the type 0x3d record with the
   owner +0xb0 and the character +0xac released 0x4c4c60 (-1, the record) and the texts 0x3feb / 0x3fed through
   0x53dda0; else the texts 0x3fec / 0x3fee; the exit 0x52d7a0 */

float gm_GetSkill(char *c, int skill); int ai_Unk4c52e0(); 

void ai_Unk5a5910(int param_1)
{
    float s;
    int ok;
    int a;
    int b;
    int rec;
    char *ch;
    char *own;
    char buf1[0x400];
    char buf2[0x400];

    ch = (char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    own = (char *)((unsigned)&ai_Chars + U_W(param_1 + 0x27) * 0x21c);
    d2_SetPointerSprite(0, 0);
    txt_Format(buf1, 0x3fea, (unsigned char)*(ch + 0x1b1) * 2);
    if (sv_MessageBox(buf1, 0x11) != 0 &&
        sv_CheckMoney(ch, (unsigned char)*(ch + 0x1b1) * 2) != 0) {
        s = gm_GetSkill(ch, 4);
        s = gm_GetSkill(ch, 3) + s;
        s = (gm_GetSkill(ch, 2) + s) * 0.2f + 0.15f;
        cm_SendMoneyDelta(U_I(ch + 4), -((unsigned char)*(ch + 0x1b1)) * 2);
        if (util_RandomF() < s) {
            ok = -1;
            cm_BeginFieldBlock(param_1, U_I(param_1 + 1));
            cm_AddFieldDelta(4, 1, (param_1 - U_I((unsigned)&cm_FieldObject)) + 0x65, &ok);
            cm_SendFieldDeltas();
            ai_Unk4c5fa0(U_I(ch + 4), -(unsigned char)*(ch + 0x1b1), 0);
            if (U_I(((unsigned)&ai_Unk770cec + 0x18) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 &&
                he_Unk55eba0(ch, &a, &b, 0) != 0)
                ai_Unk4c52e0(U_I(ch + 4), a, b, 0, 0, (char *)U_I((unsigned)"L\366segeld"));
            rec = he_FindEvent(1, 0, 0x3d);
            while (rec != 0) {
                if (U_I(rec + 0xb0) == U_I(param_1 + 1) && U_I(ch + 4) == U_I(rec + 0xac)) {
                    U_I(rec + 0x84) = cm_SendExSetHE(-1, rec);
                    break;
                }
                rec = he_NextEvent();
            }
            txt_Format(buf2, 0x3feb);
            he_ShowNews(U_I(ch + 4), -1, buf2, 0, 0x5f3, 0);
            txt_Format(buf2, 0x3fed, U_W(ch));
            he_ShowNews(U_I(own + 4), -1, buf2, 0, 0x5f3, 0);
        } else {
            txt_Format(buf1, 0x3fec);
            he_ShowNews(U_I(ch + 4), -1, buf1, 0, 0x5f3, 0);
            txt_Format(buf1, 0x3fee, U_W(ch));
            he_ShowNews(U_I(own + 4), -1, buf1, 0, 0x5f3, 0);
        }
    }
    sv_CameraFollow();
}
