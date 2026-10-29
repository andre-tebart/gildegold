/* 0x5aa520, the selected character of a Domizil record (449 B): the carrier 0x53e400 (2, 0, param_3, 3, the +1
   field of param_1) with its +0xb0 + param_4 * 4 entry: the text 0x1440a50 and the state 2; else the character
   param_2 + 8 with a byte +8: the text of its record (0x5143d0) and the state 1; else the record is freed
   (0x5aa4b0) and the texts 0x1440ac8 / 0x631090 with the state 0 are written to the controls +0x14 / +0x18 */

void ai_Unk5aa520(int param_1, int param_2, int param_3, int param_4)
{
    char text[256];
    int rec;
    int ch;

    rec = he_FindEvent(2, 0, param_3, 3, *(int *)(param_1 + 1));
    if (rec != 0 && *(int *)(rec + 0xb0 + param_4 * 4) != 0) {
        strcpy(text, (char *)U_I((unsigned)&full_data_1440a50));
        *(unsigned char *)(param_2 + 0x20) = 2;
        d2_SetObjText(*(int *)(param_2 + 0x14), text);
        U_W((unsigned)&d2_Objs + 0x14 + *(int *)(param_2 + 0x14) * 0x2ec) = 0xa0;
        U_W((unsigned)&d2_Objs + 0x16 + *(int *)(param_2 + 0x14) * 0x2ec) = 0x28;
        *(unsigned char *)(param_2 + 0x20) = 2;
        return;
    }
    ch = *(int *)(param_2 + 8);
    if (ch != 0 && *(char *)(ch + 8) != 0) {
        io_CharacterName(U_W(ch), 1, 1, text);
        d2_SetObjText(*(int *)(param_2 + 0x14), text);
        U_I((unsigned)&d2_Objs + 0x5c + *(int *)(param_2 + 0x14) * 0x2ec) = 1;
        U_W((unsigned)&d2_Objs + 0x14 + *(int *)(param_2 + 0x14) * 0x2ec) = 0xa0;
        *(unsigned char *)(param_2 + 0x20) = 1;
        return;
    }
    ai_Unk5aa4b0(param_2);
    strcpy(text, (char *)U_I((unsigned)&ai_Unk1440ac8));
    *(unsigned char *)(param_2 + 0x20) = 0;
    d2_SetObjText(*(int *)(param_2 + 0x14), text);
    U_W((unsigned)&d2_Objs + 0x14 + *(int *)(param_2 + 0x14) * 0x2ec) = 0xa0;
    U_W((unsigned)&d2_Objs + 0x16 + *(int *)(param_2 + 0x14) * 0x2ec) = 0x28;
    d2_SetObjText(*(int *)(param_2 + 0x18), (char *)(unsigned)&ch_EmptyName);
    *(unsigned char *)(param_2 + 0x20) = 0;
    U_I((unsigned)&d2_Objs + 0x2e8 + *(int *)(param_2 + 0xc) * 0x2ec) = 0;
}
