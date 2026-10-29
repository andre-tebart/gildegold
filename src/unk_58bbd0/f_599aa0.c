/* 0x599aa0, opens the dark corner of a table (79 B): the record 0x4ffe90 (param_2 + 0x14, 1, 0, 300); when the
   table's +0x27 is the player, the menu 0x5992c0 (param_1, param_2) opens, else the buying screen 0x5996b0
   (param_1, the record); then 0x52d7a0 */

void ai_Unk599aa0(int param_1, int param_2)
{
    int rec;

    rec = gm_Query(*(int *)(param_2 + 0x14), 1, 0, 300);
    if (rec != 0) {
        if (*(unsigned short *)(param_1 + 0x27) == U_W((unsigned)&ai_PlayerId)) {
            /* ROUGH: the third argument is lost in the Ghidra call (the offer source) */
            ai_Unk5992c0(param_1, param_2, param_2);
            sv_CameraFollow();
            return;
        }
        ai_Unk5996b0(param_1, rec);
    }
    sv_CameraFollow();
}
