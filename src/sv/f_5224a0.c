/* 0x5224a0, loads the groundplan bitmap of the selected building (0x6f2308 or 0x6f230c) into the view 0x6f22c4: "%sbmp\\groundplans\\riss_%s.bmp" with the building's name, else by its class (0x501d80: 1 craftsmen: tinkturei /
   parfuemer / wirtshaus / handwerks, 3 rathaus / Arbeiter, 5 zunfthaus, others geldleihe / lagerhaus / kirche / stadtwache / handwerks), copies it to the view 0x6f22c8, then walks the companion bitmap (the name + a
   suffix 0x623ee8) for the colours of the table 0x623794 and creates one button (0x46cd80) per marked cell for the matching offered item (0x4ffe90 / 0x4ffcc0 / 0x502570); the buttons go to the table 0x7662e0. ROUGH */

extern char *ai_Goods;              /* 0x91aaf0 */
int d2_BlitPartMode();                     /* 0x479150 */  /* @original: sv.c's call at 1099 passes 1 argument to d2.h's 8-parameter prototype */
extern int ai_Unk6f22dc, sv_ResearchClass, sv_ScreenDirty, ai_Unk6f235c, ai_Unk6f2360, ai_Unk623e8c;  /* 0x6f22dc, 0x6f233c, 0x6f2358, 0x6f235c, 0x6f2360, 0x623e8c */
extern int sv_ResearchClass;  /* 0x6f233c, the class number of the research list the panel shows (gm_BuildingClass) */
extern char ai_Unkf8ef80[];             /* 0xf8ef80, the data directory */
extern char ai_Unk623794[];
extern unsigned char ai_Unk623ee8[8];
extern int d3_CurrentScene;                /* 0x633444 */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */
struct D2SURF * d2_LoadBmpSurface();                     /* 0x478b50 */
void d2_GetPixelColour();                     /* 0x4798f0 */
int d2_PutPixelRGB();                     /* 0x4799e0 */
int d2_CompareColour();                     /* 0x478930 */
int gm_BuildingClass(int kind);                /* 0x501d80, f3gm.c, the class of a building kind */
int unk_Unk502570();                     /* 0x502570 */
int d2_CreatePicObj();                     /* 0x46cd80 */
unsigned short * gm_QueryNext(void);                   /* 0x4ffcc0 */
int gm_ResearchClass(int k, int *out);         /* 0x50b090, f3gm.c, the research class of a building kind */

void sv_Unk5224a0(void)
{
    unsigned char *b;
    char *rec;
    int k, x, y, w, h, c, n, i, cnt;
    char name[0x100];
    unsigned char col[3], px[4];
    char *dir;
    short *it;
    unsigned short *lst;
    char *ext;

    col[0] = 0xff;
    col[1] = 0xff;
    col[2] = 0xff;
    b = (unsigned char *)ai_PlayerBuilding;
    if (b == 0)
        b = (unsigned char *)ai_FallbackBuilding;
    if (ai_Unk6f22c4 != 0) {
        d2_FreeSurface(ai_Unk6f22c4);
        ai_Unk6f22c4 = 0;
    }
    rec = ai_BTypes + *b * 0x24d;
    sprintf(name, "%sbmp\\groundplans\\riss_%s.bmp", ai_Unkf8ef80, rec + 1);
    ai_Unk6f22c4 = d2_LoadBmpSurface(name);
    if (ai_Unk6f22c4 == 0) {
        k = gm_BuildingClass(*b);
        if (k == 1) {
            if (rec[0] == 0x16)
                dir = "%sbmp\\groundplans\\riss_wirtshaus.bmp";
            else if (rec[0] == 0x0e)
                dir = "%sbmp\\groundplans\\riss_parfuemer.bmp";
            else if (rec[0] == 8)
                dir = "%sbmp\\groundplans\\riss_tinkturei.bmp";
            else {
                sprintf(name, "%sbmp\\groundplans\\riss_%s.bmp", ai_Unkf8ef80, rec + 1);
                ai_Unk6f22c4 = d2_LoadBmpSurface(name);
                if (ai_Unk6f22c4 != 0)
                    goto have;
                dir = "%sbmp\\groundplans\\riss_handwerks.bmp";
            }
        } else if (k == 3) {
            if (rec[0] == 0x0f)
                dir = "%sbmp\\groundplans\\riss_rathaus_b.bmp";
            else if (rec[0] == 1)
                dir = "%sbmp\\groundplans\\Riss_Arbeiterunterkunft.bmp";
            else
                dir = 0;
        } else {
            if (k == 5)
                dir = "%sbmp\\groundplans\\riss_zunfthaus.bmp";
            else if (rec[0] == 5)
                dir = "%sbmp\\groundplans\\riss_geldleihe.bmp";
            else if (rec[0] == 9)
                dir = "%sbmp\\groundplans\\riss_lagerhaus.bmp";
            else if (rec[0] == 7)
                dir = "%sbmp\\groundplans\\riss_kirche_bm.bmp";
            else if (rec[0] == 0x13)
                dir = "%sbmp\\groundplans\\riss_stadtwache.bmp";
            else {
                sprintf(name, "%sbmp\\groundplans\\riss_%s.bmp", ai_Unkf8ef80, rec + 1);
                ai_Unk6f22c4 = d2_LoadBmpSurface(name);
                if (ai_Unk6f22c4 != 0)
                    goto have;
                dir = "%sbmp\\groundplans\\riss_handwerks.bmp";
            }
        }
        if (dir != 0)
            sprintf(name, dir, ai_Unkf8ef80);
        if (ai_Unk6f22c4 == 0) {
            ai_Unk6f22c4 = d2_LoadBmpSurface(name);
            if (ai_Unk6f22c4 == 0)
                return;
        }
    }
have:
    d2_LockSurface(ai_Unk6f22c4);
    d2_LockSurface(ai_Unk6f22c8);
    for (y = 0; y < *(int *)(ai_Unk6f22c4 + 8); y++)
        for (x = 0; x < *(int *)(ai_Unk6f22c4 + 4); x++) {
            d2_GetPixelColour(x, y, ai_Unk6f22c4, px);
            d2_PutPixelRGB(x, y, *(int *)px, *(int *)px, *(int *)px, ai_Unk6f22c8);
        }
    d2_UnlockSurface(ai_Unk6f22c8);
    if (ai_Unk6f22c4 != 0) {
        d2_FreeSurface(ai_Unk6f22c4);
        ai_Unk6f22c4 = 0;
    }
    ext = strrchr(name, '.');
    *ext = 0;
    memcpy(name + strlen(name), ai_Unk623ee8, 6);
    ai_Unk6f22c4 = d2_LoadBmpSurface(name);
    n = 0;
    d2_LockSurface(ai_Unk6f22c4);
    if (ai_Unk6f22c4 != 0) {
        for (y = 0; y < *(int *)(ai_Unk6f22c4 + 8); y++)
            for (x = 0; x < *(int *)(ai_Unk6f22c4 + 4); x++) {
                d2_GetPixelColour(x, y, ai_Unk6f22c4, px);
                if (d2_CompareColour(px, col) != 0)
                    continue;
                d2_GetPixelColour(x + 1, y, ai_Unk6f22c4, px);
                for (i = 0; i < 15; i++)
                    if (d2_CompareColour(px, ai_Unk623794 + i * 3) == 0)
                        break;
                if (i >= 15)
                    continue;
                it = 0;
                cnt = 0;
                if (rec[0] != 2) {
                    lst = (unsigned short *)(rec + 0x23);
                    for (c = 0; c < 0x40; c++, lst++) {
                        unsigned id = *lst & 0x7fff;
                        char cls;
                        if (*lst == 0xffff)
                            continue;
                        cls = ai_Goods[id * 0x41];
                        if ((cls != 2 && cls != 6) || id == 0xfd)
                            continue;
                        if (cnt != i) {
                            cnt++;
                            continue;
                        }
                        it = (short *)gm_Query(*(int *)(b + 0x5d), 1, 0, id);
                        break;
                    }
                } else {
                    it = (short *)gm_Query(*(int *)(b + 0x5d), 1, 4, 2);
                    while (it != 0) {
                        if (*it != 0xfd) {
                            if (cnt == i)
                                break;
                            cnt++;
                        }
                        it = (short *)gm_QueryNext();
                    }
                }
                if (it != 0 && (k = unk_Unk502570(it)) != -1) {
                    ai_Unk7662e0[n] = d2_CreatePicObj(d2_ScreenW + x - 0x7e, y + 0x4c, k);
                    n++;
                }
            }
    }
    if (ai_Unk6f22c8 != 0)
        d2_UnlockSurface(ai_Unk6f22c8);
    if (ai_Unk6f22c4 != 0)
        d2_UnlockSurface(ai_Unk6f22c4);
}
