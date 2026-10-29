/* 0x4c67a0, check of message 0x45 */

int ai_Unk4c67a0(G17MSG *m)
{
    return amt_CheckOfficeMessage(m->data) == 0;
}
