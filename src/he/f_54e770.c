/* 0x54e770, a Ghidra miss (disassembled): the allocation of the "Geliebte" (lover) event: the aggressor +0xac (ebx), the victim +0xb0 (edi): the clock now, posted at once; a gone victim or one with the flag 0x800000 (+0x1c8) ends it, else the flag is set (0x4c4a60), the relation of the aggressor falls by 0x32 (0x4c4b00) and the event goes to the state 2 (+0xb4 = 0, +0xb8 = 2, +0xbc = -1) (ROUGH) */

int ai_InventoryHas(void *ch, int id);
int gm_Unk505230();
float gm_CharacterHealth();

void he_Unk54e770(int ev)
{
    int edi = gm_FindChar(*(int *)(ev + 0xb0));
    int ebx = gm_FindChar(*(int *)(ev + 0xac));

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (edi == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if ((*(unsigned *)(edi + 0x1c8) & ((unsigned)&ai_Relations + 0x1aca0)) != 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    cm_BeginGroup("Alloc_Geliebte");
    cm_SendSetField(*(int *)(edi + 4), 0x1c8, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
    cm_SendRelations(*(int *)(ebx + 4), *(int *)(edi + 4), 0x32, 1);
    cm_EndGroup("Alloc_Geliebte AI");
    *(int *)(ev + 0xb4) = 0;
    *(int *)(ev + 0xb8) = 2;
    *(int *)(ev + 0xbc) = -1;
    *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
}
