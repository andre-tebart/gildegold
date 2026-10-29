/* 0x4c58a0, message 0x44: sends the office application (the int id, the two office bytes and the kind of p, G17B7) and returns the
   send slot of 0x4c39c0 */

int cm_SendApplication(char *p)
{
    G17MSG m;

    m.type = 0x44;
    *(G17B7 *)m.data = *(G17B7 *)p;
    return ai_Unk4c39c0(&m);
}
