/* 0x561190, the run handler of the ransom event (a thief gang, "L\366segeld" 0x627978; ROUGH skeleton, 736 bytes): states -2 / -1 free; the victim figure +0xac (0x505df0) and the building (0x501190, +0x65 the owner link) are looked up, the news 0x19da (0x5fb) / 0x19c2 (0x5f0) tells the victim and the owner (0x53dda0, 0x53eec0 (-2)), the victim walks to the hideout (0x4c52e0 "L\366segeld", 0x55eba0 scene, 0x4c3ba0 / 0x4c3c00 / 0x4c49a0 mark it, the job byte +0x1b1 1 .. 4 selects the text) and the record is freed */

int gm_NearestBuilding();
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);

void he_Unk561190(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b;
    char text[0x200];

    if (st == -2 || st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (p == 0 || b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    txt_Format(text, 0x19da, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fb, 0);
    he_Unk55eba0(*(int *)(p + 0x184));
    ai_Unk4c52e0(*(int *)(p + 4), -1, -1, 0, 0, "L\366segeld");
    cm_BeginFieldBlock(p, *(int *)(p + 4));
    cm_AddFieldDelta(1, 1, 0, text);
    cm_SendFieldDeltas();
    he_Unk53e1f0((int *)ev);
}
