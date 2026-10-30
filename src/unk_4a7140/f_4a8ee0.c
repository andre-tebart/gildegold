/* 0x4a8ee0, eval callback: two in three if p's jobId is of kind 7, else one in four */

int ai_Unk4a8ee0(int a, AIPER *p)
{
    if (gm_TitleGroup(p->jobId) == 7 && util_RandomU(3) != 0)
        return 1;
    return util_RandomU(4) == 0;
}
