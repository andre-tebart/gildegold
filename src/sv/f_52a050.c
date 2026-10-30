/* 0x52a050, the per-frame refresh of the info panel: the right mouse button / the 0x2000 key flag act on the object under the cursor (selection, jump to a contact), then the panel
   is rebuilt when the shown object / group / item changed (0x6f23bc .. 0x6f23d8 hold the last shown state); ROUGH */

void d2_SetObjEnabled();                     /* 0x475e40 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
int ai_AmountMenu();                     /* 0x5c1130 */
int gm_BuildingClass(int kind);                /* 0x501d80, f3gm.c, the class of a building kind */
unsigned unk_Unk503530(unsigned p1, unsigned char *b, unsigned short *o, int c);                     /* 0x503530 */
int ai_TransportWindow();                     /* 0x5af1d0 */
int ai_Unk5b8fc0();                     /* 0x5b8fc0 */
int ai_Unk5b84f0();                     /* 0x5b84f0 */
void ai_TechTreeWindow(int param_1, int param_2);                     /* 0x591bf0 */
void d2_FreeAnim(int n);                     /* 0x47ce30 */
int d3t_PlaceAtObject();                     /* 0x4118a0 */
void snd_PlayWorkerClick(struct G16CHARCLICK *c);                     /* 0x4c2c70 */

void sv_UpdatePanel(void)
{
    unsigned char *o, *p;
    unsigned fl;
    int c, k, f, v;
    int st[0x14];
    int *q;

    if (SV_I((unsigned)&sv_PlayerBarForm) != -1)
        return;
    if (SV_B((unsigned)&ai_GameState) == 0)
        return;
    o = (unsigned char *)SV_I((unsigned)&ai_Unk75ec60);
    if (o == 0 || gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a) != 0 || gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x116) != 0)
        f = 1;
    else
        f = 0;
    d2_SetObjEnabled(SV_I((unsigned)&ai_Unk62388c), f);
    p = o;
    fl = SV_U((unsigned)&sv_FrameMask) & 0x2000;
    if (fl == 0) {
l260:
        if (d2_ClickedId != -1) {
            if (SV_I((unsigned)&ai_Unk62388c) == -1 || ai_Unk5fe710 != SV_I((unsigned)&ai_Unk62388c) || fl == 0) {
                if (SV_I((unsigned)&ai_Unk623888) != -1 && ai_Unk5fe710 == SV_I((unsigned)&ai_Unk623888) && fl != 0 && ai_Unk5763c0(0x80) == 1) {
                    q = st;
                    for (c = 0x14; c != 0; c--)
                        *q++ = 0;
                    st[1] = 0;
                    st[5] = 4;
                    ai_Unk576040(0xf, 0, 0, 0);
                    ai_AmountMenu(st, ch_EmptyName, ch_EmptyName, sv_ReturnZero);
                    ai_Unk576040(0xf, 0, 1, 0);
                }
            } else if (SV_I((unsigned)&ai_Unk75ec60) != 0 && *(unsigned char *)SV_I((unsigned)&ai_Unk75ec60) != 0x47) {
                c = gm_BuildingClass(*(unsigned char *)SV_I((unsigned)&ai_Unk75ec60));
                if ((c == 1 || c == 2 || c == 4 || c == 7 || c == 8 || (k = ai_BTypes[*p * 0x24d]) == 0x10 || k == 0x1e) && ai_Unk5763c0(0x400) == 1) {
                    SV_I((unsigned)&ai_BuildingPerms) = unk_Unk503530(ai_PlayerId, p, 0, 0);
                    ai_Unk576040(0x12, p, 0, 0);
                    ai_TransportWindow(p, 1);
                    ai_Unk576040(0x12, p, 1, 0);
                    SV_I((unsigned)&ai_Unk75ec60) = (int)p;
                    sv_ResetScreenClip();
                }
            }
        }
    } else if (d2_ClickedId != -1) {
        if (SV_I((unsigned)&ai_Unk623894) == -1 || ai_Unk5fe710 != SV_I((unsigned)&ai_Unk623894))
            goto l260;
        p = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding);
        if (p == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
                p = (unsigned char *)SV_I((unsigned)&ai_Unk75ec60);
                if (p == 0)
                    goto l3b8;
                goto l188;
            }
            p = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
            if (SV_I((unsigned)&sv_PanelShown) == 0)
                goto l188;
            v = gm_Query(*(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d), 1, 0, *(unsigned short *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x29));
            SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, 0, v, 0);
        } else {
l188:
            SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, p, 0, 0);
        }
        if (SV_I((unsigned)&ai_PlayerBuilding) == 0 || (SV_I((unsigned)&full_data_6f2320) & 0x10) == 0) {
            if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
l1f2:
                if (SV_I((unsigned)&ai_Unk75ec60) != 0) {
                    c = gm_BuildingClass(*(unsigned char *)SV_I((unsigned)&ai_Unk75ec60));
                    SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, SV_I((unsigned)&ai_Unk75ec60), 0, 0);
                    if ((c == 3 || c == 5) && (SV_I((unsigned)&full_data_6f2320) & 0x10) != 0)
                        ai_Unk5b8fc0(SV_I((unsigned)&ai_Unk75ec60));
                    else
                        ai_Unk5b84f0(SV_I((unsigned)&ai_Unk75ec60));
                }
            } else if (SV_I((unsigned)&sv_PanelShown) == 0) {
                ai_Unk5b84f0(SV_I((unsigned)&ai_FallbackBuilding));
            } else {
                if ((SV_I((unsigned)&full_data_6f2320) & 0x10) == 0)
                    goto l1f2;
                ai_TechTreeWindow(SV_I((unsigned)&ai_FallbackBuilding), *(unsigned short *)SV_I((unsigned)&sv_PanelShown));
            }
        } else {
            ai_TechTreeWindow(SV_I((unsigned)&ai_PlayerBuilding), *(unsigned short *)(SV_I((unsigned)&ai_PlayerBuilding) + 0x29));
        }
    }
l3b8:
    sv_Unk529380();
    if (SV_I((unsigned)&ai_Unk75ec60) == SV_I((unsigned)&full_data_6f23c4) && SV_I((unsigned)&ai_FallbackBuilding) == SV_I((unsigned)&full_data_6f23c0) && SV_I((unsigned)&ai_PlayerBuilding) == SV_I((unsigned)&full_data_6f23bc)
        && SV_I((unsigned)&sv_PanelShown) == SV_I((unsigned)&full_data_6f23c8) && SV_I((unsigned)&ai_Unk75ec5c) == SV_I((unsigned)&full_data_6f23cc) && SV_I((unsigned)&ai_Unk7662c0) == SV_I((unsigned)&full_data_6f23d0)
        && SV_B((unsigned)&full_data_6f2338) == SV_I((unsigned)&full_data_6f23d4) && SV_I((unsigned)&ai_Unk6f2334) == SV_I((unsigned)&full_data_6f23d8) && SV_I((unsigned)&ai_Unk6237cc) == 0)
        return;
    SV_I((unsigned)&ai_Unk6237cc) = 0;
    sv_Unk529fb0();
    if (SV_I((unsigned)&ai_Unk75ec5c) == 0 || ai_Goods[*(unsigned short *)SV_I((unsigned)&ai_Unk75ec5c) * 0x41] != 0x1d || SV_I((unsigned)&ai_Unk6f2334) != 0 || SV_I((unsigned)&ai_Unk7662c0) != 0) {
        if (SV_I((unsigned)&ai_Unk6f2304) == 0 && SV_B((unsigned)&full_data_6f2338) == 0) {
            if (SV_I((unsigned)&ai_Unk6f2334) == 0) {
                if (SV_I((unsigned)&sv_PanelShown) == 0 && SV_I((unsigned)&ai_Unk7662c0) == 0)
                    sv_Unk528ee0();
                goto l510;
            }
        } else if (SV_I((unsigned)&ai_Unk6f2334) == 0)
            goto l510;
        if (SV_I((unsigned)&ai_Unk7662c0) != SV_I((unsigned)&full_data_6f23d0) || SV_I((unsigned)&full_data_6f23d8) != SV_I((unsigned)&ai_Unk6f2334)) {
            if (SV_I((unsigned)&ai_Unk6f2334) < 2) {
                sv_Unk5290f0(SV_I((unsigned)&ai_Unk7662c0));
                SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&ai_Unk7662c0);
                sv_Unk529380();
            } else
                sv_Unk5290f0(0);
        }
    } else
        sv_Unk528d90(SV_I((unsigned)&ai_Unk75ec5c));
l510:
    SV_I((unsigned)&full_data_6f23d4) = SV_B((unsigned)&full_data_6f2338);
    SV_I((unsigned)&full_data_6f23c0) = SV_I((unsigned)&ai_FallbackBuilding);
    SV_I((unsigned)&full_data_6f23bc) = SV_I((unsigned)&ai_PlayerBuilding);
    SV_I((unsigned)&full_data_6f23d8) = SV_I((unsigned)&ai_Unk6f2334);
    if (SV_I((unsigned)&ai_Unk6f2304) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0 && SV_I((unsigned)&ai_PlayerBuilding) == 0) {
        SV_I((unsigned)&full_data_6f23c0) = 0;
        SV_I((unsigned)&full_data_6f23bc) = 0;
        SV_I((unsigned)&full_data_6f2294) = 0;
    } else {
        SV_I((unsigned)&full_data_6f23c8) = SV_I((unsigned)&sv_PanelShown);
        SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&ai_Unk7662c0);
        SV_I((unsigned)&full_data_6f23cc) = SV_I((unsigned)&ai_Unk75ec5c);
        SV_I((unsigned)&full_data_6f23c4) = SV_I((unsigned)&ai_Unk75ec60);
        if ((SV_I((unsigned)&ai_Unk75ec5c) == 0 && SV_I((unsigned)&sv_PanelShown) == 0) || SV_I((unsigned)&sv_PanelForm) != -1) {
            if (SV_I((unsigned)&ai_Unk75ec60) == 0 && SV_I((unsigned)&ai_FallbackBuilding) == 0) {
                SV_I((unsigned)&full_data_6f2294) = 0;
                return;
            }
            SV_I((unsigned)&full_data_6f23c0) = SV_I((unsigned)&ai_FallbackBuilding);
            SV_I((unsigned)&full_data_6f23bc) = SV_I((unsigned)&ai_PlayerBuilding);
            SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&ai_Unk6f2304);
            p = (unsigned char *)SV_I((unsigned)&ai_Unk75ec60);
            if (p == 0 && (p = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding)) == 0)
                p = 0;
            sv_Unk528860(p);
            sv_Unk529380();
            return;
        }
        SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&ai_Unk6f2304);
        if (SV_I((unsigned)&ai_Unk75ec5c) != 0) {
            SV_I((unsigned)&full_data_6f2294) = SV_I((unsigned)&ai_Unk6f2304);
            sv_Unk528b80(SV_I((unsigned)&ai_Unk75ec5c));
            sv_Unk529380();
            return;
        }
        if (SV_I((unsigned)&sv_PanelShown) != 0) {
            sv_Unk528b80(SV_I((unsigned)&sv_PanelShown));
            sv_Unk529380();
            return;
        }
    }
    SV_I((unsigned)&full_data_6f23c4) = SV_I((unsigned)&ai_Unk75ec60);
    SV_I((unsigned)&full_data_6f23c8) = SV_I((unsigned)&sv_PanelShown);
    SV_I((unsigned)&full_data_6f23cc) = SV_I((unsigned)&ai_Unk75ec5c);
    SV_I((unsigned)&full_data_6f23d0) = SV_I((unsigned)&ai_Unk7662c0);
    SV_I((unsigned)&full_data_6f23d8) = SV_I((unsigned)&ai_Unk6f2334);
}
