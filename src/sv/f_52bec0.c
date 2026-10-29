/* 0x52bec0, the mouse release of the selection (called every frame with 0x6f2334 counting the selected characters): with the selection mode 0x1467394 (button released 0x14673a4): the click without a pick sends
   the selected characters of the player (flag +0x188 / kind from 0x503530 & 0x800) to the clicked map cell (0x45f2b0 (map +0xb0 of the scene, x, y, &cx, &cy), the cell types 0 / 0xd are skipped over the nine
   neighbour offsets off[9], 0x4c5100 (id, cx, cy, 0x633444)); with a picked group 0x6f22fc the group members (0x7603a0, 0x10 slots) are sent to it (0x4c5230, 0x4c5460 (id, target +1, pick +1)); 0x52bbc0 moves
   the selection into the picked object when 0x760458 is set; then the selected flags are mirrored (0x75e920) and the focus / hilight of each (0x4134a0 (+0x34, flag), 0x476b90) updated (ROUGH: the offset
   table off[] is the neighbour spiral, the __ftol operands (off * 50.0 / cell size + c) are decoded from the disassembly) */

void sv_Unk52bec0(void)
{
    int off[9][2] = { {0, 0}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
    char *pk; int map, v, cx, cy, ix, iy, size, i, cell, t, tgt;
    int *p;
    char *pc;
    char name[0x80];
    unsigned short *ch;
    unsigned *pp;

    cx = -1;
    cy = -1;
    map = *(int *)(SV_I((unsigned)&ai_Unk5fb5b0) + 0xb0);
    v = SV_I((unsigned)&ai_PlayerBuilding);
    if (v == 0)
        v = SV_I((unsigned)&ai_FallbackBuilding);
    if (SV_I(((unsigned)&dm_L + 0x14)) == 0) {
        if (SV_I((unsigned)&ai_Unk760440) != 0) {
            snd_PlaySelectedClick();
            sv_Unk52eed0();
            if (SV_I(((unsigned)&dm_L + 0x14)) != 0)
                goto lrel;
        }
        goto lend;
    }
lrel:
    if (SV_I(((unsigned)&dm_L + 0x24)) == 0) {
        if (SV_I((unsigned)&ai_Unk760440) == 0 && SV_I((unsigned)&di_Absolute) != 0 && SV_I((unsigned)&d2_HitWindow) == -1 && v != 0 && *(short *)(v + 0x27) == (short)SV_I((unsigned)&ai_PlayerId))
            sv_Unk52ee50();
        goto lend;
    }
    if (SV_I((unsigned)&ai_Unk6f2334) == 0)
        goto lend;
    if (SV_I((unsigned)&full_data_6f22fc) == 0) {
        if (SV_I((unsigned)&ai_ClickedObject) != 0 || SV_I((unsigned)&full_data_76045c) != 0)
            goto lafter;
        if (SV_I((unsigned)&full_data_760458) == 0) {
            if (map == 0 || v == 0 || SV_I(((unsigned)&dm_L + 0xc)) == 0 || SV_I((unsigned)&d2_HitWindow) != -1)
                goto lend;
            i = 0;
            ch = (unsigned short *)(unsigned)&ai_Chars;
            do {
                cell = 0;
                if (*(char *)(ch + 0xc4) != 0 && *(char *)(ch + 4) != 0 && (unk_Unk503530(SV_I((unsigned)&ai_PlayerId), 0, 0, ch) & 0x800) != 0 && *(int *)(ch + 0xc2) != 0) {
                    if (d3sm_Pick(map, (int)*(short *)(unsigned)&dm_L, (int)*(short *)((unsigned)&dm_L + 0x2), &cx, &cy) != 0) {
                        size = *(int *)(map + 0x20);
                        do {
                            ix = (int)((float)off[i][0] * (50.0 / *(float *)(map + 0x10)) + cx);
                            iy = (int)((float)off[i][1] * (50.0 / *(float *)(map + 0x18)) + cy);
                            if (ix < size && iy < size)
                                cell = *(int *)(*(int *)(map + 0x24) + (size * iy + ix) * 0x1c);
                            i++;
                            t = cx;
                            tgt = cy;
                        } while (i < 8 && (ix < 0 || size <= ix || iy < 0 || size <= iy || cell == 0 || (t = ix, tgt = iy, cell == 0xd)));
                        if (*(int *)(ch + 0xb6) != 0)
                            sv_MoveToGroup(*(int *)(ch + 0xb6), 0, *ch, 0);
                        if (*(int *)(*(int *)(ch + 0xc2) + 0x28) != -1 && (pk = (int)evt_FindById(*(int *)(*(int *)(ch + 0xc2) + 0x28))) != 0)
                            evt_Stop((EVTSCRIPT *)pk);
                        ai_Unk4c5100(*(int *)(ch + 2), t, tgt, SV_I((unsigned)&d3_CurrentScene));
                    }
                }
                ch += 0x10e;
            } while ((int)ch < (unsigned)((unsigned)ai_Chars + 0x65400));
            if (SV_I((unsigned)&full_data_6f22fc) != 0)
                goto lgroup;
            goto lafter;
        }
    } else {
lgroup:
        if (map != 0 && SV_I(((unsigned)&dm_L + 0xc)) != 0 && SV_I((unsigned)&d2_HitWindow) == -1) {
            t = -1;
            sprintf(name, "dummy_%s", (char *)SV_I((unsigned)&ai_ClickedObject) + 8);
            if (d3_FindObject(SV_I((unsigned)&ai_ClickedObject), 0x100, 0, name) == 0)
                strcpy(name, (char *)SV_I((unsigned)&ai_ClickedObject));
            pp = (unsigned *)(unsigned)&ai_Unk7603a0;
            do {
                ch = (unsigned short *)*pp;
                v = 0;
                if (ch != 0 && *(char *)(ch + 0xc4) != 0) {
                    ai_Unk4c5230(*(int *)(ch + 2));
                    if (*(int *)(ch + 0xb6) != 0)
                        sv_MoveToGroup(*(int *)(ch + 0xb6), 0, *ch, 0);
                    if (**(short **)(unsigned)&full_data_6f22fc == 0xfd) {
                        if (SV_I((unsigned)&ai_FallbackBuilding) != 0) {
                            t = init_BuildingSceneUniverse(SV_I((unsigned)&ai_FallbackBuilding));
                            v = SV_I((unsigned)&ai_FallbackBuilding);
                            if (v == 0)
                                v = SV_I((unsigned)&ai_PlayerBuilding);
                        } else if (SV_I((unsigned)&ai_PlayerBuilding) != 0) {
                            t = 0;
                            v = SV_I((unsigned)&ai_PlayerBuilding);
                        }
                    } else {
                        t = init_ObjectSceneUniverse(SV_I((unsigned)&full_data_6f22fc));
                        v = SV_I((unsigned)&ai_FallbackBuilding);
                        if (v == 0)
                            v = SV_I((unsigned)&ai_PlayerBuilding);
                    }
                    if (-1 < t)
                        ai_Unk4c5460(*(int *)(ch + 2), *(int *)(v + 1), *(int *)(SV_I((unsigned)&full_data_6f22fc) + 1));
                }
                pp++;
            } while ((int)pp < (unsigned)((unsigned)ai_Unk7603a0 + 0x80));
            sv_Unk52ae40();
            SV_I((unsigned)&full_data_6f22fc) = 0;
        }
lafter:
        if (SV_I((unsigned)&full_data_760458) == 0)
            goto lend;
    }
    sv_Unk52bbc0();
lend:
    SV_I((unsigned)&ai_Unk6f2334) = 0;
    d2_ClearPointerItems();
    i = 0;
    p = (int *)((unsigned)&ai_Unk770cec + 0x18);
    do {
        cell = *p;
        if (cell != 0) {
            pc = (char *)p;
            if ((char)p[1] != *(char *)((unsigned)&sv_SelectionFlags + i) && (*(unsigned char *)(cell + 0x8c) & 0x40) == 0)
                d3t_SetGrey(*(int *)(cell + 0x34), (char)p[1] != 0);
            if ((char)p[1] != 0) {
                if (SV_I((unsigned)&ai_Unk6f2334) == 0)
                    SV_I((unsigned)&ai_Unk7662c0) = (int)(p - 0x61);
                SV_I((unsigned)&ai_Unk6f2334) = SV_I((unsigned)&ai_Unk6f2334) + 1;
                if (p[-6] != 0 && *(int *)(p[-6] + 1) == *(int *)(*p + 0x2c) && (SV_I((unsigned)&ai_FallbackBuilding) != 0 || (SV_I((unsigned)&sv_PanelShown) != 0 && **(short **)(unsigned)&sv_PanelShown != 0x120)))
                    d2_AddPointerItem(p[2], 1);
            }
        }
        t = p[1];
        p += 0x87;
        *(char *)((unsigned)&sv_SelectionFlags + i) = (char)t;
        i++;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182));
    if (SV_I(((unsigned)&dm_L + 0x20)) != 0 && SV_I((unsigned)&ai_Unk6f2334) != 0)
        sv_Unk52ae40();
}
