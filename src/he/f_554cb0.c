/* 0x554cb0, the allocation handler of the two-building event (a Ghidra miss; ROUGH, the tail is only sketched): the clocks +0x52 / +0x60 are the start clock, +0xe8 = -1, the buildings +0xb0 / +0xb4 (0x501190) and the person +0xac (0x4fea50) are looked up (any missing: free the record); when the second building is of the kind 0xa and the person has flag 1 (+0x20) the nearest building of the kind 5 / 0xa list (0x501190 (1, 5, 0xa), 0x458a20 positions, start range 1e8) replaces it in all records of type +0xf4 (+0xb8 .. / +0xb0) and +0xb4; the two buildings' 0x502520 flags choose the mode +0xc8 (1 / 2 / 3, 4 when both: equal buildings = error text 0x62728c and free), +0xb8 = the sum of both +0x30, +0xc4 = the first building's id, the event is posted 1 later and the person's data (0x502020, 0x5092e0, the +0x1a0 factor from the person's byte +0x12 with the type 0x135 / 0x136 correction) is set up; flag 2 of +0x78 sends the person the message 0x13 / 0x40, the person gets +0x24 = ev and +0x10 = its id; with flag 1 the 0x50aa00 / 0x627258 text decides a count +0xcc (<= the person's limit) and its tag +0x28 (0x50a970), the handle +0x84 / +0xec = -1 */

int gm_IsHomeType();
char *gm_FindBuildingById(int id);
int gm_BuildingSkill();
int gm_StallGoodsList(char *c, unsigned *out);
void gm_Unk50a970(int a, int kind);

void he_Unk554cb0(int ev)
{
    int b1, b2, o = 0, best, e, f1 = 0, f2 = 0, mode;
    float mind, dist;
    float p1[3], p2[3];
    char tmp[0x100];

    *(int *)(ev + 0xe8) = -1;
    *(int *)(ev + 0xd4) = 0;
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(unsigned char *)(ev + 0xd0) = 0;
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    if (b1 == 0)
        goto fail;
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0)
        goto fail;
    b2 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (b2 == 0)
        goto fail;
    if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b2 * 0x24d) == 0xa && (*(unsigned char *)(o + 0x20) & 1) != 0) {
        best = b2;
        mind = 1e8f;
        d3_PointToWorld(*(int *)(b1 + 0x61), *(int *)(b1 + 0x61) + 0x4c, p1);
        for (e = gm_BuildQuery(1, 5, 0xa); e != 0; e = gm_BuildNext()) {
            float dx, dy, dz;

            d3_PointToWorld(*(int *)(e + 0x61), *(int *)(e + 0x61) + 0x4c, p2);
            dx = p2[0] - p1[0];
            dy = p2[1] - p1[1];
            dz = p2[2] - p1[2];
            dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (!(dist > mind)) {
                mind = dist;
                best = e;
            }
        }
        if (best != b2) {
            if (*(int *)(ev + 0xf4) != 0 && *(int *)(ev + 0xf4) != -1) {
                char *r = he_FindEvent(1, 1, *(int *)(ev + 0xf4));
                int k;

                if (r != 0) {
                    for (k = 0; k < 3; k++) {
                        if (*(int *)(r + 0xc4 + k * 4) == *(int *)(b2 + 1))
                            *(int *)(r + 0xc4 + k * 4) = *(int *)(best + 1);
                    }
                    if (*(int *)(r + 0xb0) == *(int *)(b2 + 1))
                        *(int *)(r + 0xb0) = *(int *)(best + 1);
                }
            }
            *(int *)(ev + 0xb4) = *(int *)(best + 1);
            b2 = best;
        }
    }
    if (gm_IsHomeType(b1) != 0)
        f1 = 1;
    if (gm_IsHomeType(b2) != 0)
        f2 = 1;
    mode = f1 ? (f2 ? 4 : 2) : (f2 ? 1 : 3);
    *(int *)(ev + 0xc8) = mode;
    if (mode == 4 && b1 == b2) {
        err_Message((char *)((unsigned)&ai_Unk625820 + 0x1a6c));
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(int *)(ev + 0xb8) = *(int *)(b2 + 0x30) + *(int *)(b1 + 0x30);
    *(int *)(ev + 0xc4) = *(int *)(b1 + 1);
    *(unsigned char *)(ev + 0xd0) = 0;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    gm_FindBuildingById(*(int *)(o + 0x1c));
    *(unsigned char *)(o + 0x13) |= 0x40;
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0)
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0x40, 0);
    *(int *)(o + 0x24) = ev;
    *(int *)(ev + 0x10) = *(int *)(o + 2);
    if ((*(unsigned char *)(o + 0x20) & 1) != 0) {
        gm_StallGoodsList(o, tmp);
        sprintf(tmp, "%s", "");
        *(unsigned char *)(o + 0x28) = *(unsigned char *)(ev + 0xcc);
        gm_Unk50a970K(*(int *)(ev + 0xcc), *(char *)(o + 0x28));
    }
    *(int *)(ev + 0x84) = -1;
    *(int *)(ev + 0xec) = -1;
    return;
fail:
    he_Unk53e1f0((int *)ev);
}
