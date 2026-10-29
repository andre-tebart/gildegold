/* 0x55fa90, the run handler "he_RunMarktSupervisor" of the market (3472 bytes, ROUGH skeleton; the debug text "he_RunMarktSupervisor(): Could not find markt-lager:%s"): the market object (gm_FindMarketObject) and the stores (gm_Query, 0x50aaf0 / 0x4ffa10) of the town are looked at; from random rolls (util_Random / util_RandomF) the supervisor buys and sells goods (0x4c4880 payments, 0x5de518 _ftol amounts), a figure goes shopping (0x4c5710) and the clock event is posted again; a missing store prints the debug text and frees the record */

void he_Unk55fa90(int ev)
{
    int st = *(int *)(ev + 0x70);
    char text[0x100];

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_Unk509ee0(*(char **)(unsigned)&ai_PlayerChar, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0) {
        sprintf(text, "he_RunMarktSupervisor(): Could not find markt-lager:%s", "");
        he_Unk53e1f0((int *)ev);
        return;
    }
    ((void (*)(int))cm_SendMoveGoods)(ev);
    cm_SendExSellObject(*(int *)(ev + 0xac), -1, 0, 0, 0, 0);
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
