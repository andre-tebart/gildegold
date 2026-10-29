/* 0x4c4a20, message 0x18: the second field block */

void ai_Unk4c4a20(void)
{
    G17MSG m;

    m.type = 0x18;
    memcpy(m.data, &ai_Unka68ca0, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
