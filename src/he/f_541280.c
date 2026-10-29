/* 0x541280, a figure's behaviour at an event: for every one of the 8 figures (+0x8c) without a pending job in 40 percent of the cases walks the way points of the event (0x4bc390 / 0x45f170 / 0x40b2f0, job 0x2d, "he_CharacterBehavior"), else plays one of three gestures ("gestik/..") twice (ROUGH) */

float unk_Unk4fc890();
int rd_PointsNear();
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);
struct CHACTION * ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);
int ch_QueueAnim();
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);
void cm_SendSetField(int a, int b, int c, int d, int e);
int ai_Unk4c52e0();

void he_Unk541280(int ev, int p2)
{
    int *pi;
    int k, n, o, e8, f0, r, ec;
    unsigned short us;
    unsigned u;
    char *pu;
    char e4[4];
    char gest[3][0x20];
    char buf[0x80];

    if (U_I((unsigned)&d3_CurrentScene) == 0) {
        pi = (int *)(ev + 0x8c);
        for (k = 8; k != 0; k--, pi++) {
            if (*pi != -1 && (o = gm_FindChar(*pi)) != 0 && *(int *)(*(int *)(o + 0x184) + 0x128) == 0) {
                us = util_Random(100);
                if (us < 0x29) {
                    ec = *(int *)(U_I((unsigned)&ai_Unk5fb5b0) + 0xb0);
                    u = util_Random(3);
                    n = rd_PointsNear(p2, (u & 0xffff) + 1, buf);
                    pu = buf;
                    for (; n > 0; n--) {
                        if ((char)d3_GetHeightAt(ec, pu, &e8, &f0, e4) != 0) {
                            r = ch_AddAction(*(int *)(o + 0x184), 0x2d, e8, f0, 0);
                            strcpy((char *)(r + 0x70), "he_CharacterBehavior");
                        }
                        pu += 0x10 * 4;
                    }
                } else {
                    memset(gest, 0, sizeof(gest));
                    strcpy(gest[0], "gestik/durch_haar_fahr");
                    strcpy(gest[1], "gestik/kopf_kratzen");
                    strcpy(gest[2], "gestik/mit_faust_droh");
                    u = util_Random(3);
                    ch_QueueAnim(*(int *)(o + 0x184), gest[u & 0xffff]);
                    u = util_Random(3);
                    ch_QueueAnim(*(int *)(o + 0x184), gest[u & 0xffff]);
                }
            }
        }
    }
}
