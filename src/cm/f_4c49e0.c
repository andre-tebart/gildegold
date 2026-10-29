/* 0x4c49e0, message 0x17: the field block (values) */

void cm_SendFieldValues(void)
{
    G17MSG m;

    m.type = 0x17;
    memcpy(m.data, &ai_Unka68c20, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
