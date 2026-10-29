/* 0x4e5210, removes the character who from the party id (his id becomes -1) and, when it is the running party 0x6f0d90, from its member slots (0x92caf0); 0
   when there is no such party */

int ai_Unk4e5210(int id, int who)
{
    G28REC *r;
    int i, n;

    r = (G28REC *)cm_PartyById(id);
    if (r == 0)
        return 0;
    n = r->used;
    for (i = 0; i < n; i++) {
        if (*(int *)((char *)r + 0x34 + i * 4) == who) {
            *(int *)((char *)r + 0x34 + i * 4) = -1;
            break;
        }
    }
    if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == id) {
        for (i = 0; i < 16; i++) {
            if (*(int *)(ai_Unk92cae0[i] + 0x10) == who) {
                *(int *)(ai_Unk92cae0[i] + 0x10) = -1;
                break;
            }
        }
    }
    return 1;
}
