/* 0x5a9b50, a Domizil message (108 B): only for the kinds 6 / 7 of the record param_1 (+2): the text 0x4074 with
   the two +0x30 / +0x40 fields of param_2 and param_4 + 0x4172, then 0x53dee0 with the +4 id of param_1, the
   +1 field of the record at +0x16c of param_2, the +2 field of param_3 and the key "_NACHRICHTEN_DOMIZIL_03"
   0x62c3b8 */

int d2_WinOpenPic(short x, short y, int pic, int win); int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out); 
/* the ai.h view of 0x50c520 (the opinion of two characters, word arguments as the original pushes them) */
int gm_Opinion(int a, int b);

void ai_Unk5a9b50(int param_1, int param_2, int param_3, int param_4)
{
    if (*(unsigned char *)(param_1 + 2) != 6 && *(unsigned char *)(param_1 + 2) != 7)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4074, param_2 + 0x30, param_2 + 0x40, param_4 + 0x4172);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ec,
                 *(int *)(*(int *)(param_2 + 0x16c) + 1), *(int *)(param_3 + 2), 0, (char *)(unsigned)&full_data_62c3b8);
}
