/* 0x558f10, the run handler of the sabotage event "Sabotage" (3600 bytes, 11 states by the jump table 0x559ce8; ROUGH skeleton, the details of every state are not drafted): the saboteur +0xb0 (the handle +0xb4 / +0xb8 of its job 0x4c3ae0 / 0x4c3b10, 0x4c52e0 "Sabotage" sends it), the building +0x10 and the target person +0xb4 are followed through the states: walk to the building, take the tool ("spezial/einhaendig_vom_boden_nehmen" 0x4c5410), the 0x553ae0 detection check and the guards (0x53e400 over the type 0x1d / 0x2c records), the success roll (0x5ba6b0 skill, 0x50cc50, a random float util_RandomF / gm_Mix), the damage (0x4c4b00 relation, 0x4c4ed0 / 0x4c4830 payment, 0x4f2e30 the relation check) and the news to the owner / the saboteur ("_NACHRICHTEN_HS_17" / "_18", 0x53dee0 / 0x53dda0); the end releases the group (0x524260) and frees the record */

int unk_Unk4f2e30();
void cm_SendProofExecuted(int a, int b);
void cm_BeginGroup(const char *s);
void cm_EndGroup(const char *s);
int ord_DispatchOrder(int id, char *c, int *m);
int ai_InventoryHas(void *ch, int id);
void gm_SendMoneyDelta(int a, int b);

void he_Unk558f10(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b, h, o = 0;
    char text[0x200];

    if (st < -2 || st > 8) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        cm_SendMoveGoodsUnchecked(*(int *)(p + 4), 0, 0, 0);
        ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Sabotage");
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    default:
        h = *(int *)(ev + 0xc4);
        if (h != -1 && cm_GetSlotState(h) == 0)
            return;
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb4));
        txt_Format(text, 0x1bba, b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_18", 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        return;
    }
}
