/* 0x5286a0, tints the banners of the building b: its model's parts named "sp_WIMPEL" (0x438d40, 0x40, the collector 0x528660; the model's +0x1f0 cleared when its flag +0x210 & 1 is not set) are recoloured
   (0x458780 with the colour 0x6f1c28, when set) and updated (0x447190) */

extern int d3_QuadTree, ai_Unk623864, ai_Unk623868, ai_Unk62386c, ai_Unk62387c, ai_Unk623888, ai_Unk62388c, ai_Unk623890, ai_Unk623894, ai_Unk631078;  /* 0x6f1c28, 0x623864, 0x623868, 0x62386c, 0x62387c, 0x623888, 0x62388c, 0x623890, 0x623894, 0x631078 */

int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjFont();                     /* 0x4760b0 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */
int gm_BuildingLevel();                     /* 0x508d70 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);                     /* 0x46a960 */
int unk_Unk503530();                     /* 0x503530 */
unsigned short * gm_Unk50a050(unsigned short *p);                     /* 0x50a050 */
int gm_Unk508cc0();                     /* 0x508cc0 */
int sv_Unk528800(int x, int y, int text);

void sv_Unk5286a0(char *b)
{
    int list[33];
    int i, m, save;

    m = *(int *)(b + 0x61);
    if (m != 0) {
        for (i = 0; i < 0x21; i++)
            list[i] = 0;
        if ((*(unsigned char *)(m + 0x210) & 1) == 0) {
            save = *(int *)(m + 0x1f0);
            *(int *)(m + 0x1f0) = 0;
            d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x40, sv_Unk528660, list);
            *(int *)(*(int *)(b + 0x61) + 0x1f0) = save;
        } else
            d3_ForEachObject(ai_Unk5fb5b0, m, 0x40, sv_Unk528660, list);
        i = 0;
        if (0 < list[0]) {
            do {
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)list[i + 1], (D3QNODE *)d3_QuadTree);
                d3if_Delete((struct D3OBJECT *)list[i + 1]);
                i++;
            } while (i < list[0]);
        }
    }
}
