/* 0x502b60, hands the building b over to the owner (character slot) own, at the place a: the old owner (a kind 6 character) loses it (0x5a86c0), the new one (kind 6) gets it (0x5a8560); the person living in it is cleaned up, the objects of kind 0x1d (graveyard) in its list get the owner, and the scene objects are refreshed (0x5286a0 / 0x5282f0). ROUGH */

/* ---- the character and building life cycle 0x502b60 .. 0x504510 (ROUGH drafts, written out of address order at the end of the file) ---- */
/* ai_Unk91f716: view of ai_Unk91f700 (tools/data_views.py) */
extern int ai_Unk91b6e0[];              /* 0x91b6e0 .. 0x91f6e0: pairs of ints */
extern int ai_Unk770ae0_day;            /* the clock's day (0x770ae0) */
#ifndef TIME_VIEW_H
extern unsigned short ai_TimeHour;     /* 0x770ae4, the clock's hour (unaligned view) */
#endif
extern char ai_Unk770ce4[];             /* 0x770ce4: the field +0x164 of the character records (stride 0x21c) */
int ai_Unk5a86c0();                     /* 0x5a86c0 */
int ai_Unk5a8560();                     /* 0x5a8560 */
int sv_Unk5286a0();                     /* 0x5286a0 */
int sv_Unk5282f0();                     /* 0x5282f0 */
void ch_Unk402540();                     /* 0x402540 */
int ch_Delete();                     /* 0x4028a0 */
int ai_Unk5a9430();                     /* 0x5a9430 */
int gm_FreeList();                     /* 0x500140 */
int gm_TitlesLeft();                     /* 0x504ce0 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
int gm_CompareTime();                     /* 0x4fe210 */
int gm_MinutesBetween();                     /* 0x4fe280 */
int d3t_SceneIndex();                     /* 0x411be0 */
double ai_Unk5de73a();                     /* 0x5de73a: a CRT call */
extern int ai_CharCount;  /* 0x6f1bc0 */
extern int f3gm_ObjectCount;  /* 0x6f1bdc */

void gm_AssignBuilding(unsigned char *b, unsigned short place, unsigned own)
{
    char *p;
    unsigned short o;
    char *bl;
    int per, k;
    unsigned short *w;
    int cnt, scn, ob, owid;
    char t;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    *(unsigned short *)(b + 0x25) = place;
    if (own != 0xffff) {
        o = *(unsigned short *)(b + 0x27);
        if (o != own) {
            if (o != 0xffff && ai_Chars[o][2] == 6)
                ai_Unk5a86c0(*(int *)(b + 1));
            if (ai_Chars[own][2] == 6)
                ai_Unk5a8560(b);
        }
        o = *(unsigned short *)(b + 0x27);
        if (o != 0xffff && *(unsigned char **)(ai_Chars[o] + 0x170) == b)
            *(int *)(ai_Chars[o] + 0x170) = 0;
        *(unsigned short *)(b + 0x27) = own;
        if (*p == 2) {
            gm_ShowBuildingObjects(b);
            if (*p == 2) {
                bl = *(char **)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x170);
                if (bl == 0 || (unsigned char)*bl <= *b)
                    *(unsigned char **)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x170) = b;
            }
        }
        t = ai_Chars[own][2];
        if (t == 6 || t == 7) {
            per = gm_FindCharacterByMaster(b);
            if (per != 0) {
                *(char *)(per + 0x165) = 0;
                *(unsigned *)(per + 0x1b4) = *(unsigned *)(per + 0x1b4) & 0xfffffffe;
                if (*p == 5 || *p == 9)
                    *(int *)(per + 0x16c) = 0;
            }
        }
        cnt = 0x5008;
        w = (unsigned short *)ai_Objects;
        do {
            if (*w != 0 && ai_Goods[*w * 0x41] == 0x1d && *(int *)(w + 0xe) == *(int *)(b + 1)) {
                *(int *)(w + 5) = *(int *)(ai_Chars[own] + 4);
                if (*(int *)(w + 10) != 0)
                    *(int *)(*(int *)(w + 10) + 10) = *(int *)(ai_Chars[own] + 4);
                scn = d3_CurrentScene;
                ob = *(int *)((char *)w + 0x3b);
                if (*(int *)(ai_Chars[own] + 0x54) == 0 || ((t = ai_Chars[own][2]) != 5 && t != 6 && t != 7))
                    owid = 0;
                else
                    owid = *(int *)(ai_Chars[own] + 0x54) - 0x599;
                k = d3t_SceneIndex(*(int *)(ob + 0x88), 1);
                d3if_SelectScene(k);
                if (*(int *)(*(int *)(ob + 0x34) + 0x1cc) == 0)
                    ch_Unk402540(ob);
                d3if_SelectTextureSet(**(int **)(ob + 0x124), *(int *)(**(int **)(ob + 0x124) + 0x1cc), owid, 1);
                d3if_SelectScene(scn, 1);
            }
            scn = d3_CurrentScene;
            w = (unsigned short *)((char *)w + 0x43);
            cnt--;
        } while (cnt != 0);
        if (ai_Chars[own] != ai_PlayerChar && *(int *)(b + 0x61) != 0 && ((t = ai_Chars[own][2]) == 6 || t == 7 || t == 5)) {
            d3if_SelectScene(0, 1);
            sv_Unk5286a0(b);
            sv_Unk5282f0(b);
            d3if_SelectScene(scn, 1);
            return;
        }
        d3if_SelectScene(0, 1);
        sv_Unk5286a0(b);
        d3if_SelectScene(scn, 1);
    }
}
