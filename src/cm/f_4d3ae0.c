/* 0x4d3ae0, the target puts its carried object down: the object (+0x1ac of the attacker) belongs to the defender's side now */

void ai_Unk4d3ae0(char *ev)
{
    char *att = *(char **)(ev + 0x17c);
    int o = *(int *)(ev + 0x180);
    char *p;

    p = ai_Unk4ceb70((char *)o);
    if (p != 0)
        *(int *)(p + 0xa) = *(unsigned short *)ai_Unk6f0cec;
    *(int *)(att + 0x1ac) = o;
    d3if_ShowTree(o, 0);
}
