/* 0x4b46f0, mission condition: c has a building (kind >= 5, owner c) older than t->unk18 minutes */

int gm_BuildingTypeOfJob(int k);                                                         /* 0x503fa0, defined elsewhere */

int ai_Unk4b46f0(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;

    for (b = gm_BuildQuery(2, 4, c->id, 5, 2); b != 0; b = gm_BuildNext()) {
        if (G13_B(b, 0) >= 5 && *(int *)((char *)b + 0x65) == c->netId
            && gm_MinutesBetween((AITIME *)((char *)b + 0x69), &ai_TimeT) > t->unk18)
            return 1;
    }
    return 0;
}
