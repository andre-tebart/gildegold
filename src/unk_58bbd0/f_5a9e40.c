/* 0x5a9e40, a Domizil message without the kind test (76 B): the format text 0x4070 with param_2 and its +0x4071
   field (a name), then 0x53dee0 with the +1 field of param_2 and the key "_NACHRICHTEN_DOMIZIL_12" 0x62c400 */

void ai_Unk5a9e40(int param_1, int param_2)
{
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4070, param_2, param_2 + 0x4071);
    he_MessageBoxQuickjump(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, *(int *)(param_2 + 1), -1, 0,
                 (char *)(unsigned)&full_data_62c400);
}
