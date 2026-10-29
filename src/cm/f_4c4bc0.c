/* 0x4c4bc0, message 0x1c with len extra bytes after msg (0xf8 bytes) */

int ai_Unk4c4bc0(char *msg, unsigned int len, void *extra)
{
    G17MSG m;
    char *buf;

    m.type = 0x1c;
    if (gm_FindChar(*(int *)(msg + 8)) == 0)
        return -1;
    buf = mem_Alloc(len + 0xf8, "tmp");
    memcpy(buf, msg, 0xf8);
    memcpy(buf + 0xf8, extra, len);
    ai_Unk4c4480(len + 0xf8, buf);
    mem_Free(buf);
    return ai_Unk4c39c0(&m);
}
