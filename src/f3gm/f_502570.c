/* 0x502570, the string-table number of the object o (0x60e + n) for the object kinds 2 / 6: a table of (n, object) pairs (0x13 -> 0x54, 2 -> 0x15, 4 -> 0x18 ..) searched for the prototype number of the object; -1 when it is not in it. ROUGH */

int gm_JobBuildingType(int x);
extern int sv_PanelShown;                /* 0x6f2310 */
/* ai_Unk61a688: view of ai_Unk61a686 (tools/data_views.py) */
extern int ai_CharCount, ai_Unk6f1bb8, ai_Unk6f1bbc;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                     /* 0x447300 */
void cm_BeginGroup(const char *s);                     /* 0x4c4540 */
void cm_EndGroup(const char *s);                     /* 0x4c4580 */
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);                     /* 0x4c47e0 */
void cm_SendOwnership(int a, int b, int c);                     /* 0x4c54a0 */
void cm_SendBoss(int a, int b);                     /* 0x4c5540 */
void cm_SendExSetHE(unsigned char k, char *p);                     /* 0x4c4c60 */
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);                     /* 0x53dee0 */
int d3t_SceneIndex();                     /* 0x411be0 */
int ch_Delete();                     /* 0x4028a0 */
int gm_Unk50a0f0(int y);                     /* 0x50a0f0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);                     /* 0x524260 */
int he_Unk5538b0();                     /* 0x5538b0 */
int he_Unk568aa0();                     /* 0x568aa0 */
void d3io_NoopCallback();                     /* 0x44ee40 */
int amt_ClearOffices();                     /* 0x4e6d80 */
void unk_Unk503c60(int c, int mode);                     /* 0x503c60 */
extern char *ai_Unk5fb5b0;  /* 0x5fb5b0 (d3_DefaultScene, now defined in d3_engine.c) */

int unk_Unk502570(unsigned short *o)
{
    int t[0x6d] = { 0x1, 0x54, 0x2, 0x15, 0x4, 0x18, 0xd, 0x14, 0x9, 0x16, 0x18, 0x17, 0xf, 0x37, 0x0, 0x12, 0x19, 0x9, 0x1, 0xcf, 0x20, 0xd6, 0x1b, 0xdd, 0x1f, 0x120, 0xa, 0x12e, 0x1c, 0x10e, 0x1d, 0x110, 0x1, 0x114, 0x1e, 0x139, 0x1e, 0x138, 0x1, 0x137, 0x1d, 0xf2, 0xe, 0xe6, 0x21, 0xe5, 0x21, 0xf0, 0xb, 0xf7, 0x1c, 0xb7, 0x15, 0xba, 0x1a, 0xbb, 0x5, 0x4d, 0x7, 0x5c, 0x14, 0x60, 0x12, 0x65, 0x1a, 0x145, 0x2, 0x146, 0x15, 0x14c, 0x16, 0x74, 0x20, 0x85, 0x20, 0x118, 0x20, 0x9b, 0x20, 0xa5, 0x1c, 0xf3, 0x22, 0x205, 0x23, 0x21b, 0x24, 0x211, 0x25, 0x276, 0x26, 0x28a, 0x27, 0x27f, 0x28, 0x24b, 0x29, 0x256, 0x2a, 0x266, 0x9, 0x2c, 0x20, 0x2d, 0x20, 0x33, 0x20, 0x0, -1 };
    char k;
    int i;
    unsigned v;

    k = ai_Goods[*o * 0x41];
    if (k == 2 || k == 6) {
        i = 0;
        v = 0x13;
        do {
            if (v == *o)
                return t[i * 2] + 0x60e;
            v = t[i * 2 + 1];
            i++;
        } while (v != 0);
    }
    return -1;
}
