/* 0x4e4a50, "cut_CheckMaster": 1 when the owner (m +0xc) is one of the party m (+0x30 count, +0x34 ids); else the first member of the kind 6 / 7 that
   still exists (as long as none was found, 16 slots) becomes the new owner: the log "new master for id %i => %i", a request (0x4c61f0) and the wait for its
   answer (0x4c3ae0 with the frame wait 0x52fe00); 0 then. The log "invalid master for id %i" comes first. ROUGH */

int ai_Unk4e4a50(char *m)
{
    char buf[0x80];
    int flag, i, h;
    char *c;

    if (ai_Unk4e4870(*(int *)(m + 0xc), m) == 0) {
        flag = 1;
        c = (char *)gm_FindChar(*(int *)(m + 0xc));
        sprintf(buf, "cut_CheckMaster(): invalid master for id %i", *(int *)m);
        for (i = 0; i < 0x10 && flag != 0; i++) {
            if (*(int *)(m + 0x34 + i * 4) != -1) {
                c = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
                if (c != 0 && (c[2] == 6 || c[2] == 7))
                    flag = 0;
            }
        }
        if (c != 0 && *(int *)(c + 4) != *(int *)(m + 0xc)) {
            sprintf(buf, "cut_CheckMaster(): new master for id %i => %i", *(int *)m, *(int *)(c + 4));
            h = ((int (*)())ai_Unk4c61f0)(m, c);
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
            return 0;
        }
    }
    return 1;
}
