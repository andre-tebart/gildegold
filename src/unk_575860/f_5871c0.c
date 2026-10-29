/* 0x5871c0, the graveyard "GRAB_MIETEN" contact (30 B): the contact window 0x583240 (0, rec,
   U_I (0x144a5d0), 0, 0), then 0x52d7a0 () */

void ai_Unk5871c0(char *rec)
{
    ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a5d0), 0, 0);
    sv_CameraFollow();
}
