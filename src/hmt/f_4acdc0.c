/* 0x4acdc0, back to the first person / event */

void hmt_Reset(void)
{
    hmt_Person = hmt_File.p->idx;
    hmt_Event = hmt_File.k->idx;
}
