/* 0x519d20, the buildings: the count of the used ones (0x100 records of 0xa9 bytes at 0x889f74), then each in pieces (the fields +0 .. +0x5c, then the type specific tail: type 0x1e (the 0x600 bytes of 0x18-byte stock records behind +0x71), the types 0x48 .. 0x4a by 0x4f17f0, 0x4b .. 0x4d by 0x4ecf10, the others 0x30 bytes at +0x65), the 0x10 bytes at +0x99; at the end the table of the bounding boxes 0xf812e4 (count 0xf812e0, 0x60 bytes each: name, min, max) @deep B: 280 code + 114 regs hunks, one systematic register swap through the whole body: ours keeps the building record in edi and the file f in esi, the original esi / edi (sampled asmdiff at 0x519d9a .. 0x519ded: every block is push esi / lea edx,[edi+off] vs push edi / lea edx,[esi+off] with the same offsets and sizes); the count n lives in a stack slot in the original (inc DWORD PTR [esp+0x10]) and the original's frame is 1 slot (push ecx) against our 2 (sub esp,0x8), which shifts everything. The meaning matches (0x100 records of 0xa9 bytes, the field pieces, the type tails). Shape is match-deep's (vargen ren). Q2/Q7: the callees (0x430e30 -> vfs_fwrite is now named) and the ai_Unk globals are still unnamed; Q6 described. */


int unk_Unk519d20(int f)
{
    int n = 0, off = 0, i, j;
    char *p, c;

    p = ai_Blds;
    for (i = 0x100; i != 0; i--) {
        if (*p != 0)
            n++;
        p += 0xa9;
    }
    LS_W(&n, 4)
    do {
        p = ai_Blds + off;
        if (ai_Blds[off] != 0) {
            LS_W(p, 1)
            LS_W(p + 1, 4)
            LS_W(p + 5, 0x20)
            LS_W(p + 0x25, 2)
            LS_W(p + 0x27, 2)
            LS_W(p + 0x29, 2)
            LS_W(p + 0x2b, 4)
            LS_W(p + 0x2f, 1)
            LS_W(p + 0x34, 1)
            LS_W(p + 0x35, 4)
            LS_W(p + 0x39, 4)
            LS_W(p + 0x3d, 4)
            LS_W(p + 0x41, 4)
            LS_W(p + 0x45, 4)
            LS_W(p + 0x49, 4)
            LS_W(p + 0x4d, 4)
            LS_W(p + 0x51, 4)
            LS_W(p + 0x55, 4)
            LS_W(p + 0x5a, 2)
            LS_W(p + 0x5c, 1)
            c = *p;
            if (c == 0x1e) {
                for (j = 0; j < 0x600; j += 0x18) {
                    char *q = *(char **)(p + 0x71);

                    LS_W(q + j, 4)
                    LS_W(q + j + 4, 4)
                    LS_W(q + j + 8, 1)
                    LS_W(q + j + 9, 1)
                    LS_W(q + j + 10, 2)
                    LS_W(q + j + 0x10, 4)
                    LS_W(q + j + 0xc, 1)
                    LS_W(q + j + 0xd, 1)
                }
            } else if (c == 'H' || c == 'I' || c == 'J') {
                if (unk_Unk4f17f0(f, p) == 0)
                    return 0;
            } else if (c == 'K' || c == 'L' || c == 'M') {
                if (ahm_WriteRecord(f, p) == 0)
                    return 0;
            } else {
                LS_W(p + 0x65, 0x30)
            }
            LS_W(p + 0x99, 0x10)
        }
        off += 0xa9;
    } while (off < 0xa900);
    LS_W(&ai_NBauplaetze, 4)
    ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0; i < ai_NBauplaetze; i++) {
        LS_W(ai_Bauplaetze[i].name, 0x40)
        LS_W(&ai_Bauplaetze[i].x0, 0x10)
        LS_W(&ai_Bauplaetze[i].x1, 0x10)
    }
    return 1;
}
