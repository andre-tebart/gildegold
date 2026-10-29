/* 0x498470, execute callback: hands the job s1->unk04 over */

int ai_Unk498470(G5CH *p, AIARG *s1)
{
    AIJOB *j;

    if (s1->kind != 4)
        return 0;
    for (j = he_FindEvent(1, 0, 0x1b); j != 0; j = he_NextEvent()) {
        if (((int *)j)[0xac / 4] == s1->unk04) {
            gm_Unk502fd0(s1->unk04, p->netId, ((int *)j)[0xb0 / 4], (AIJOBX *)j, 1);
            return 0x10;
        }
    }
    return 0;
}
