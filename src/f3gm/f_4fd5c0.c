/* 0x4fd5c0, picks two characters (kinds 0 .. 5, active, old enough (age +10 not below the float +0x20), with a chance by the number of the relatives at +0x68 (table 0x60 0x4b 0x28 0x17 9 0 of 256) against random 1 .. 256; 16 tries each, the range shrinking by 3/4 on a free slot) and starts the event 0x507690 (0, id1, id2, param, 0, 0, 0, 2). Returns its result. ROUGH */

char ai_Unk6f0454[1] = {0};  /* 0x6f0454 .. */
char ai_Unkf8ef80[256] = {0}; /* 0xf8ef80, the game root path */
/* ai_Unk143ac5c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143af70: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b178: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_Unk6f1bb4 = 0;  /* 0x6f1bb4 */
char gm_CharacterStatistics[128];  /* 0x91b500, the character statistics ( */
char * ai_Unk60c1ec = (void *)((unsigned)&he_Events + 0xc234);  /* 0x60c1ec, the memory pool tag */
unsigned short gm_Unk507690(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8);                     /* 0x507690 */
int gm_Unk503f30();                     /* 0x503f30 */
int gm_Unk503f50();                     /* 0x503f50 */
int gm_Opinion(int a, int b);                     /* 0x50c520 */
float gm_CharAge();                   /* 0x505e30 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
void mem_Free(void *p);                    /* 0x422620 */

unsigned unk_Unk4fd5c0(int param)
{
    int tab[6];
    int tries, cnt, k, range;
    unsigned short a, b;
    unsigned r;
    int *q;
    char *c;

    tab[0] = 0x60;
    tab[1] = 0x4b;
    tab[2] = 0x28;
    tab[3] = 0x17;
    tab[4] = 9;
    tab[5] = 0;
    range = 0x300;
    for (tries = 0; tries < 0x10; tries++) {
        a = util_Random(range);
        c = ai_Chars[a];
        r = a * 0xf;
        if (*(short *)c == -1) {
            range = (range * 3) >> 2;
            r = range;
        } else if ((unsigned char)c[2] > 5 || (float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20) || c[8] == 0) {
            a = 0xffff;
            r = 0;
        } else {
            q = (int *)(c + 0x68);
            cnt = 0;
            for (k = 5; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            r = (util_Random(0x100) & 0xffff) + 1;
            if ((int)r < tab[cnt])
                break;
        }
    }
    if (a == 0xffff)
        return r & 0xffff0000;
    for (tries = 0; tries < 0x10; tries++) {
        b = util_Random(range);
        c = ai_Chars[b];
        if (*(short *)c == -1)
            range = (range * 3) >> 2;
        else if ((unsigned char)c[2] > 5 || (float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20) || c[8] == 0)
            b = 0xffff;
        else {
            q = (int *)(c + 0x68);
            cnt = 0;
            for (k = 5; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            r = (util_Random(0x100) & 0xffff) + 1;
            if ((int)r < tab[cnt])
                break;
        }
        r = tries + 1;
    }
    if (b != 0xffff)
        return gm_Unk507690(0, *(int *)(ai_Chars[a] + 4), *(int *)(ai_Chars[b] + 4), param, 0, 0, 0, 2);
    return r & 0xffff0000;
}
