/* 0x587300, the graveyard "GRAEBER_PFLEGEN" contact (50 B): 0x520fe0 (rec) == 0 -> the contact window 0x583240
   (0, rec, U_I (0x144a638), 1, 0x584900) and 0x52d7a0 () */

void ai_Unk587300(char *rec)
{
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a638), 1, ai_Unk584900);
    sv_CameraFollow();
}
