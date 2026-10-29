/* 0x4d1a10, ends the fight scene (the counterpart of 0x4d0fa0): releases the sound sets, the flag objects
   ai_Unk935980[], the characters' objects (the strength effect of non-kind-0xb characters 0x4fca40) and resets the scene */

void ai_Unk4d1a10(char *rec)
{
    char *r, *k;
    int i;

    ai_Unk4e3060();
    if (ai_Unk6f0cf0 != 0) {
        d3if_Delete((struct D3OBJECT *)ai_Unk6f0cf0);
        ai_Unk6f0cf0 = 0;
    }
    sb_UnloadSampleBank(ai_Unk6f0d0c);
    sb_UnloadSampleBank(ai_Unk6f0d10);
    sb_UnloadSampleBank(ai_Unk6f0d14);
    sb_UnloadSampleBank(ai_Unk6f0d18);
    sb_UnloadSampleBank(ai_Unk6f0d1c);
    sb_UnloadSampleBank(ai_Unk6f0d20);
    sb_UnloadSampleBank(ai_Unk6f0d24);
    sb_UnloadSampleBank(ai_Unk6f0d2c);
    sb_UnloadSampleBank(ai_Unk6f0d34);
    sb_UnloadSampleBank(ai_Unk6f0d3c);
    sb_UnloadSampleBank(ai_Unk6f0d44);
    if (ai_Unk6f0d50 != ai_Unk6f0d54) {
        sb_UnloadSampleBank(ai_Unk6f0d28);
        sb_UnloadSampleBank(ai_Unk6f0d30);
        sb_UnloadSampleBank(ai_Unk6f0d38);
        sb_UnloadSampleBank(ai_Unk6f0d40);
        sb_UnloadSampleBank(ai_Unk6f0d48);
    }
    for (i = 0; i < ai_Unk6f0d58; i++)
        d3if_Delete((struct D3OBJECT *)ai_Unk935980[i]);
    if (ai_Unk6f0d5c != 0) {
        d3io_ReleaseObject(ai_Unk6f0d5c);
        ai_Unk6f0d5c = 0;
    }
    for (r = ai_Unk930600; (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (*(unsigned short *)r != 0xffff && *(int *)(r + 0x184) != 0) {
            if (*(int *)(rec + 0xc) == ai_CharInts[ai_PlayerId][0] || ai_Unk1460ae0 == -1) {
                k = gm_FindChar(*(int *)(r + 4));
                if (k[2] != 0xb)
                    gm_AddHealth(*(unsigned short *)k, *(int *)(r + 0x24) - *(int *)(k + 0x24));
            }
            ch_Delete(*(int *)(r + 0x184));
            *(int *)(r + 0x184) = 0;
        }
    }
    ai_Unk623818 = 1.0f;
    if (ai_Unk633738 != 0) {
        ai_Unk633dd4 = 0;
        *(int *)(ai_Unkfd7c50 + d3_CurrentScene * 0x3e0 + 8) = 0;
    }
    ai_Unk4e30c0();
    snd_EndSpecialTrack();
    ai_Unk62381c = 1;
    d2_CopyCaption((char *)&ch_EmptyName);
}
