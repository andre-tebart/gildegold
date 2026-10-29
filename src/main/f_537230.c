/* 0x537230, the player setup / city choice of a new game ("Menu\CHOOSECITY", the start menu page "Startmenu/A_Stadtwahl.esc"; host = 1 for a network host (the list of ".NET" files, else ".CTY"): lists the city files
   "gamedata/cities/*" (0x5d33b0: up to 64 records of 0x210 bytes, the path at +0x109), reads each header (0x51cee0: +5 the name, +9 the object name, +0x34 the player limit; diff[i] = limit - players (0x62489c)),
   creates the name object of the city (0x59a2c0 "stadt_<name>", the text plate 0x52e1c0 from the table 0x143a1c0 by the key "_STADTAUSWAHL_<NAME>_INFO+0" (0x42e770)), puts the city tower "sp_STADTTURM" (0x446e10,
   "sonstiges\wimpel_STADTTURM.baf") over the picked city (the pick object 0x448630 with a name starting with "stadt_" selects: sel; the default is the object of the 5th city) and shows its description
   ("_STADTAUSWAHL_<NAME>_BESCHR+0"); the buttons: ok (or enter / 0x1c) goes on to the difficulty choice 0x536e80 and the history choice 0x536980 (host: ends at once), the second button opens the opponent
   selection 0x537040 (diff of the picked city); returns 1 when a game was set up (ROUGH: from the disassembly; the arrays sit at fixed offsets of the 0x8984 frame, the second parameter is the host flag, the first unused) */

int main_Unk537230(int unused, int host)
{
    int ret, tower, last, cap, n, i, k, f, sel, f1, f2, run, h, ok, opp, out[2];
    int cityobj[64], diff[64];
    char recs[64][0x210], hdr[0xfc], key[0x100], name[0x40], buf4[0x100];
    float pos[4], rot[3];
    char *s, *d;
    char c;

    ret = 0;
    tower = 0;
    last = 0;
    cap = -1;
    sv_ClearNamePlates();
    sv_ReleaseNamePlates();
    d3if_SelectScene(0, 0);
    d3_SetObjectPos(U_I((unsigned)&ai_Unkfc2dec), U_I((unsigned)&ai_Unkfc2dec) + 0x5c);
    d3_SetObjectRot(U_I((unsigned)&ai_Unkfc2dec), U_I((unsigned)&ai_Unkfc2dec) + 0x90);
    d3_SetLightSet(0, 0, 0x3f800000, 0);
    n = ai_ListFiles("gamedata/cities", host == 0 ? ".CTY" : ".NET", (char *)recs);
    for (i = 0; i < n; i++) {
        f = vfs_Tfopen(recs[i] + 0x109, (char *)(unsigned)"rb", U_I((unsigned)&d3_Unk6333fc));
        if (f == 0)
            return 0;
        if (ls_ReadSaveHeader(f, hdr) == 0) {
            vfs_fclose(f);
            return 0;
        }
        vfs_fclose(f);
        diff[i] = (signed char)hdr[0x34] - (U_I((unsigned)&ai_NumPlayers) & 0xff);
        h = ai_Unk59a2c0(hdr + 9);
        cityobj[i] = h;
        if (h != 0) {
            sprintf(key, "_STADTAUSWAHL_%s_INFO+0", hdr + 5);
            _strupr(key);
            k = txt_FindKey(key);
            if (k == -1) {
                k = 0;
                do {
                    c = hdr[5 + k];
                    name[k] = c;
                    k++;
                } while (c != 0);
            } else {
                s = (char *)U_I((unsigned)&d2_Strings + k * 4);
                d = name;
                do {
                    c = *s++;
                    *d++ = c;
                } while (c != 0);
            }
            sprintf(buf4, "stadt_%s", hdr + 5);
            s = buf4;
            d = (char *)h;
            do {
                c = *s++;
                *d++ = c;
            } while (c != 0);
            sv_RegisterNamePlate(buf4, name, 0);
        }
    }
    sel = cityobj[4];
    if (sel != 0) {
        pos[0] = *(float *)(sel + 0x4c);
        pos[1] = *(float *)(sel + 0x50);
        pos[2] = *(float *)(sel + 0x54);
        pos[3] = 0;
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        tower = d3_CreateObject(0, pos, rot, "sp_STADTTURM");
        *(unsigned char *)(tower + 0x211) |= 4;
        *(unsigned char *)(tower + 0x212) = (*(unsigned char *)(tower + 0x212) & 0xf7) | 4;
        d3_RelightObject(tower);
        d3t_PlayAnim(tower, "sonstiges\\wimpel_STADTTURM.baf", 1);
    }
    d3if_Show(sel, 0);
    U_I((unsigned)&sv_FrameMask) = 0x12d6;
    U_I((unsigned)&ai_Unk60d360) = 0x12d6;
    ai_Unk4e2c70(ai_Unk4e2bf0("Startmenu/A_Stadtwahl.esc"));
    f1 = d2_OpenForm(0, 0, "Menu\\CHOOSECITY_HEADER");
    d2_PlaceMainWindow(f1, 3);
    d2_PlaceViewWindows(f1, 1);
    d2_SetForm(f1, 0);
    io_PrintText(0x1abb);
    d3_SetViewport((float)(int)*(short *)(unsigned)&d2_ScreenW, (float)(int)*(short *)(unsigned)&d2_ScreenH, 0.0f, 0.0f, *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH);
    f2 = d2_OpenForm(0, 0, "Menu\\CHOOSECITY");
    d2_PlaceMainWindow(f2, 3);
    d2_PlaceViewWindows(f2, 1);
    d2_SetForm(f2, 2);
    sv_ButtonRow(d2_LastWindow, 0, out, 2, U_I((unsigned)&full_data_144acf0), U_I((unsigned)&full_data_144acf4));
    ok = out[0];
    opp = out[1];
    run = sv_RunFrame((int)main_Unk537230, 0x10d6);
    while (run != 0) {
        U_I((unsigned)&ai_ClickedObject) = 0;
        h = d3if_Pick((float)(int)*(short *)(unsigned)&dm_L, (float)(int)*(short *)((unsigned)&dm_L + 0x2), 0x60, 1);
        U_I((unsigned)&ai_Unk6f22e8) = h;
        if (U_I(((unsigned)&dm_L + 0xc)) != 0) {
            U_I((unsigned)&ai_ClickedObject) = h;
        } else if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0 && strncmp((char *)U_I((unsigned)&ai_ClickedObject), "stadt_", 6) == 0)
            sel = U_I((unsigned)&ai_ClickedObject);
        if (sel != last) {
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0;
            pos[3] = 0;
            rot[0] = 0;
            rot[1] = 0;
            rot[2] = 0;
            d2_SetForm(f2, 1);
            io_PrintText((unsigned)"$C");
            pos[0] = pos[0] + *(float *)(sel + 0x4c);
            pos[1] = pos[1] + *(float *)(sel + 0x50);
            pos[2] = pos[2] + *(float *)(sel + 0x54);
            d3t_PlaceObjectV(tower, pos, rot, 0x28, 0x20);
            for (i = 0; i < n; i++) {
                if (sel == cityobj[i]) {
                    if (cityobj[i] != 0) {
                        s = (char *)sel + 6;
                        d = (char *)((unsigned)&ai_DataPath + 0x220);
                        do {
                            c = *s++;
                            *d++ = c;
                        } while (c != 0);
                        sprintf(key, "_STADTAUSWAHL_%s_BESCHR+0", (char *)((unsigned)&ai_DataPath + 0x220));
                        _strupr(key);
                        k = txt_FindKey(key);
                        if (k != -1) {
                            d2_SetForm(f2, 1);
                            io_PrintText(k);
                            io_PrintText("$A");
                            io_PrintText(k + 1);
                        }
                        d3if_Show(sel, 0);
                        cap = diff[i];
                    }
                } else if (cityobj[i] != 0) {
                    d3if_Show(cityobj[i], 1);
                }
            }
            last = sel;
        }
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            if (ai_Unk5fe710 == opp) {
                d2_SetViewEnabled(f2, 0);
                d2_SetViewEnabled(f1, 0);
                main_Unk537040(cap);
                d2_SetViewEnabled(f2, 1);
                d2_SetViewEnabled(f1, 1);
                if (U_I((unsigned)&d2_ClickedId) != 0)
                    goto lkey;
            }
            if (ai_Unk5fe710 != ok)
                goto lkey;
            goto lgo;
        } else {
lkey:
            if (*(char *)(unsigned)&kb_Last == 0x1c) {
lgo:
                d2_SetViewEnabled(f2, 0);
                d2_SetViewEnabled(f1, 0);
                if (host != 0) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 1;
                } else {
                    if (main_Unk536e80() != 0 && main_Unk536980() != 0) {
                        U_I((unsigned)&sv_PopRequest) = 1;
                        ret = 1;
                        d2_SetViewEnabled(f2, 1);
                    }
                    d2_SetViewEnabled(f2, 1);
                    d2_SetViewEnabled(f1, 1);
                }
            }
        }
        run = sv_RunFrame((int)main_Unk537230, 0x10d6);
    }
    if (f2 != -1)
        d2_CloseView(f2);
    if (f1 != -1)
        d2_CloseView(f1);
    if (tower != 0) {
        d3t_ReleaseObjectTexture(tower, "wimpel_STADTTURM");
        d3if_Delete((struct D3OBJECT *)tower);
    }
    for (i = 0; i < n; i++) {
        if (cityobj[i] != 0)
            d3if_Delete((struct D3OBJECT *)cityobj[i]);
    }
    return ret;
}
