/* 0x552520, the run handler of a counting event: a negative state or a counter +0xac <= 0 frees the record; flag 4 does nothing; else the counter is decremented and the event posted 0x18 later (handle in +0x84) */

void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldDelta(int size, int n, short field, void *v);
void cm_SendFieldDeltas(void);

void he_Unk552520(int ev)
{
    if (*(int *)(ev + 0x70) < 0 || *(int *)(ev + 0xac) <= 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(int *)(ev + 0xac) = *(int *)(ev + 0xac) - 1;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
