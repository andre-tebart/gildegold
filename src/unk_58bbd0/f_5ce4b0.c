/* 0x5ce4b0, the spy payment handler (345 B, ROUGH SKELETON: no Ghidra export; from the disassembly):
   param_1 the message record (the AIMSG: +2 the type 6 / 7, +4 the id, +0x166 the kind), param_2 the
   request (+4 the handle). The kind 0x4e6f10 / 0x4e6fc0 (the kind, 6, the ids) list; its entries: the
   message's own id pays 0x4c4b00 (the request handle, the id, 0x64, 1) and, for the types 6 / 7, the
   text 0x5176d0 (0x1e0a, the id) and the message 0x53dda0 (the handle, -1, 0, the text, 0x5fb, the key
   0x62d694); other ids pay (the id, -0x14, 1) after the target 0x505df0 and send the same text to the
   target. ROUGH SKELETON: the second text block and the return values are not written. */

int ai_Unk5ce4b0(int param_1, int param_2)
{
    int ids[0x100];
    char text[0x100];
    int kindrec;
    int count;
    int id;
    int target;
    int i;

    kindrec = amt_KindColumn(*(unsigned char *)(param_1 + 0x166));
    count = amt_GetColumnOffices(kindrec, 6, ids);
    if (count <= 0)
        return 0x60;   /* ROUGH: the tail at 0x5ce5e6 */
    for (i = 0; i < count; i++) {
        id = ids[i];
        if (id == -1)
            break;
        if (*(int *)(param_1 + 4) == id) {
            cm_SendRelations(*(int *)(param_2 + 4), id, 0x64, 1);
            if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
                txt_Format(text, (char *)0x1e0a, *(unsigned short *)param_2);
                he_ShowNews(*(int *)(param_1 + 4), 0xffffffff, 0, text, 0x5fb,
                             (char *)(unsigned)&full_data_62d694);
            }
        } else {
            target = gm_FindChar(id);
            if (target == 0)
                continue;
            cm_SendRelations(*(int *)(param_2 + 4), id, 0xffffffec, 1);
            if (*(char *)(target + 2) == 6 || *(char *)(target + 2) == 7) {
                txt_Format(text, (char *)0x1e0a, *(unsigned short *)param_2);
                he_ShowNews(*(int *)(target + 4), 0xffffffff, 0, text, 0x5fb,
                             (char *)(unsigned)&full_data_62d694);
            }
        }
    }
    return 0x10;   /* ROUGH */
}
