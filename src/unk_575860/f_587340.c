/* 0x587340, the graveyard "GEHILFEN_ERSCHAFFEN" contact (47 B): 0x520fe0 (rec) == 0 -> the contact window
   0x583240 (0, rec, U_I (0x144a684), 0, 0x584b70) and 0x52d7a0 () */

void ai_Unk587340(char *rec)
{
    if (sv_CheckCharacterFlag(rec) == 0)
        ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a684), 0, ai_Unk584b70);
    sv_CameraFollow();
}
