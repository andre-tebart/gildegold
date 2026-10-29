/* 0x50af00, collects what the person `id` carries into the record out (0x218 bytes: the first word counts the 0xe-byte units, +0x31 the free capacity, +0x41 .. the entries of 0x1e bytes built from the goods list 0x74e1f4 (0x29 words each, owner at -2 words) that belong to the person, the entry count +0x219 and the sum +0x215). The person's own character record is searched with 0x501190 / 0x501030 (0x502eb0 gives its fields; one with the id equal adds 0xe to the first word and to +0x31). Returns 0. ROUGH */

/* ---- layout 2026-10-23: the standing / family helpers 0x50b010 .. 0x50d0b0 (ROUGH drafts) ---- */
char ai_Unk76fde0[3328];  /* 0x76fde0, the family records ( */
int ai_Unk770af0;  /* 0x770af0, the window of the family tree */
int ai_Unk76fdc8;  /* 0x76fdc8 */
/* ai_Unkfb1030: view of d2_Views (tools/data_views.py) */
/* ai_Unkf8f128: view of d2_Wins (tools/data_views.py) */
void d2_PlotLine();                     /* 0x4764f0 */
void ai_Unk4c5510(int a);                     /* 0x4c5510 */
float gm_Liking();
int gm_Unk50b3a0();
int gm_Unk50b9f0();

int gm_Unk50af00(int id, int *out)
{
    int *p, *e;
    int rec;
    char buf[148];
    int c;

    memset(out, 0, 0x21c);
    rec = (int)gm_BuildQuery(1, 4, id);
    for (;;) {
        if (rec == 0)
            break;
        gm_Unk502eb0(rec, buf);
        c = *(int *)(buf + 0x94);
        if (c != 0xffff && c == id) {
            *out += 0xe;
            *(int *)((char *)out + 0x31) += 0xe;
            break;
        }
        rec = (int)gm_BuildNext();
    }
    p = (int *)((unsigned)&he_QuickList + 0x6b4);
    e = (int *)((char *)out + 0x41);
    do {
        if (p[-5] != 0 && p[-2] == id) {
            e[-3] = p[-5];
            if (p[-3] != 0)
                *e = p[-1];
            e[1] = *p;
            e[2] = p[1];
            e[3] = p[2];
            *(short *)(e + 4) = *(short *)(p + 3);
            e[-1] = (int)p + 0xe;
            if (p[-3] != 0)
                *(int *)((char *)out + 0x215) -= p[-1];
            e[-2] = (int)(p - 5);
            e = (int *)((char *)e + 0x1e);
            *(int *)((char *)out + 0x219) += 1;
        }
        p += 0x29;
    } while ((int)p < ((unsigned)&he_QuickList + 0xaab4));
    return 0;
}
