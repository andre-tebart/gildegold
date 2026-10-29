/* 0x54b170, a Ghidra miss: the init of the protection money event: +0xb8 = -1, and (bit 4 of +0x78 clear) due now, state 1, posted 2 units ahead */

int gm_EquipmentValue(unsigned char *b, int unused);
int gm_BuildingLevel();

void he_Unk54b170(int ev)
{
    *(int *)(ev + 0x84) = -1;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(int *)(ev + 0xb8) = -1;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x70) = 1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
    }
}
