/* 0x4c6780, check of message 0x44 */

int ai_Unk4c6780(G17MSG *m)
{
    return amt_CheckApplication(m->data) == 0;
}
