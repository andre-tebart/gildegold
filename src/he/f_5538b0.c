/* 0x5538b0, ends every running event record that involves the person `p` (+4 the id; nothing for a free slot): the records of the types 0x41, 0x6f, 0x45 (the two partners +0xac / +0xb0, the other one is looked up (0x505df0) before the record is ended by 0x541200), 0x2c (the partners +0xac / +0xb4 / +0xc0 get the message 0x4c5580 (+0xac, +0xb0, +4, 0)), 0x47 (+0xac or +0xb0), 0x5e and 0x5f (+0xac); only records with a bit of 3 in +0x78 are touched (ROUGH) */

void ai_Unk4c5580(int a, int b, int c, int d);

void he_Unk5538b0(unsigned short *p)
{
    char *e;
    int id, a, b;

    if (p == 0 || *p == 0xffff)
        return;
    id = *(int *)((char *)p + 4);
    for (e = he_FindEvent(1, 0, 0x41); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id)
            gm_FindChar(*(int *)(e + 0xb0));
        else if (*(int *)(e + 0xb0) == id)
            gm_FindChar(a);
        else
            continue;
        he_Unk541200((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x6f); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id)
            b = *(int *)(e + 0xb0);
        else if (*(int *)(e + 0xb0) == id)
            b = a;
        else
            continue;
        gm_FindChar(b);
        he_Unk541200((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x45); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        b = *(int *)(e + 0xb0);
        if (a == id)
            gm_FindChar(b);
        else if (b == id)
            gm_FindChar(a);
        else
            continue;
        he_Unk541200((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x2c); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id || *(int *)(e + 0xb4) == id || *(int *)(e + 0xc0) == id) {
            ai_Unk4c5580(a, *(int *)(e + 0xb0), *(int *)(e + 4), 0);
            he_Unk541200((int)e);
        } else if (*(int *)(e + 0xb0) == id) {
            he_Unk541200((int)e);
        }
    }
    for (e = he_FindEvent(1, 0, 0x47); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        if (*(int *)(e + 0xac) == id || *(int *)(e + 0xb0) == id)
            he_Unk541200((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x5e); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) != 0 && *(int *)(e + 0xac) == id)
            he_Unk541200((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x5f); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) != 0 && *(int *)(e + 0xac) == id)
            he_Unk541200((int)e);
    }
}
