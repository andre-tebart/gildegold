/* 0x4c4b70, message 0x1c: msg (0xf8 bytes, AIMSG) as a long message; zeros at 0x38..0x57 become -1 */

int cm_SendLongMessage(char *msg)
{
    G17MSG m;
    int i;

    m.type = 0x1c;
    for (i = 0; i < 8; i++)
        if (((int *)(msg + 0x38))[i] == 0)
            ((int *)(msg + 0x38))[i] = -1;
    ai_Unk4c4480(0xf8, msg);
    ai_Unk4c39c0(&m);
}
