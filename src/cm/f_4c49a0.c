/* 0x4c49a0, message 0x16: the field block (deltas) */

void cm_SendFieldDeltas(void)
{
    G17MSG m;

    m.type = 0x16;
    memcpy(m.data, &ai_Unka68c20, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
