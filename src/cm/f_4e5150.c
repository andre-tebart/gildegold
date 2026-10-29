/* 0x4e5150, stores a copy of the party src (0x114 bytes) in a free record, with the next id 0x61a658; the byte +0x31 defaults to 8; the record */

int ai_Unk4e5150(int *src)
{
    G28REC *r;

    r = (G28REC *)ai_Unk4e5120();
    if (r != 0) {
        memcpy(r, src, 0x114);
        r->id = ai_Unk61a658;
        if (((unsigned char *)r)[0x31] == 0)
            ((unsigned char *)r)[0x31] = 8;
    }
    return (int)r;
}
