/* 0x4104e0, fill kb_Map[256] with the VkKeyScan / MapVirtualKey code of every character, 0xffff when no key; always returns 1 */

int kb_BuildMap(void)
{
    int i;
    unsigned int v;
    SHORT s;

    for (i = 0; i < 256; i++) {
        s = VkKeyScan((CHAR)i);
        v = s;
        if (v != 0xffffffff) {
            if ((v & 0xff00) == 0x600)
                v = 0x400 | (v & 0xff);
            kb_Map[i] = ((MapVirtualKey(v & 0xff, 0) ^ v) & 0xff) ^ v;
        } else
            kb_Map[i] = 0xffff;
    }
    return 1;
}
