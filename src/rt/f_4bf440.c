/* 0x4bf440, on map window m: the building buttons (0x18 bytes each from 0xdc, count at 0x27a4) of buildings other
   than rt_PickedBuilding off; then those of route p's points on
   @deep behaviour: the if/else was checked against the raw disassembly (equal arm: +0x1c0 &= ~1, +0x2c = 1; not-equal: |= 1, = 0 - matches the C) and the rest is scheduling (order 8, regs 8: mov edi,0x1 placement, the mark loop's ebp/eax reload at the end). Q4 (12 raw accesses through the char* d2_Objs view and the map window m+0xdc/+0x27a4) needs typed views - rt.h type debt */

void rt_HighlightRoute(RTPOINT *p, char *m)
{
    int i;
    int *e;
    int found[8];
    int *f;

    for (i = 0, e = (int *)(m + 0xdc); i < *(int *)(m + 0x27a4); i++, e += 6) {
        if (e[1] != 0) {
            if (e[1] == rt_PickedBuilding) {
                *(unsigned int *)(d2_Objs + e[0] * 0x2ec + 0x1c0) &= ~1;
                *(int *)(d2_Objs + e[0] * 0x2ec + 0x2c) = 1;
            } else {
                *(unsigned int *)(d2_Objs + e[0] * 0x2ec + 0x1c0) |= 1;
                *(int *)(d2_Objs + e[0] * 0x2ec + 0x2c) = 0;
            }
        }
    }
    if (p != 0) {
        f = found;
        do {
            for (i = 0, e = (int *)(m + 0xe0); i < *(int *)(m + 0x27a4); i++, e += 6) {
                if (*e != 0 && *(int *)(*e + 1) == p->buildingId) {
                    *(int *)(d2_Objs + *(int *)(m + 0xdc + i * 0x18) * 0x2ec + 0x2c) = 1;
                    *(unsigned int *)(d2_Objs + *(int *)(m + i * 0x18 + 0xdc) * 0x2ec + 0x1c0) &= ~1;
                    *f++ = *(int *)(m + i * 0x18 + 0xe0);
                    break;
                }
            }
            p = p->next;
        } while (p != 0);
    }
}
