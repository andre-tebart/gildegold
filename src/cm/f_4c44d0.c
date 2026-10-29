/* 0x4c44d0, message 8: sends the size of the load buffer that the blocks of message 9 follow (the handler
   of type 8 allocates it, "cm_LoadBuf") */

void cm_SendLoadSize(int v)
{
    G17MSG m;

    m.type = 8;
    G17_I(m, 0) = v;
    ai_Unk4c39c0(&m);
}
