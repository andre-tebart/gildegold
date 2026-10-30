/* 0x532da0 */

/* 0x532da0, the network game list ("Menu\SEARCH_NETWORK"; host: 0 = saved games (code 0x7f records), 1 = new games (0x3f)): asks the LAN (0x401ea0: port 12345, 3 s, up to 8 records of 0x80 bytes, the code at +0x7c), lists
   the usable ones as rows (0x4731d0 / 0x472aa0 / 0x515960 0x1cbc), refreshes after 1.5 s or on the second button, the first button (join) writes the host address to game.INI ([Network] Host = 0x75e1c0) and
   copies the record to 0x936460; a saved game (0x7f) is only usable when its file "Gamedata\network\<name>.SAV" exists and its player ids match; the third button asks for an address (0x532c30); returns 1 when a game
   was chosen (ROUGH: from the disassembly, Ghidra lost all call arguments) */
void sv_ButtonRow(int f, int y, int *out, int n, ...);
int d2_RemoveWindowObjects();
void d2_RadioAdd(int g, int obj);
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_SetObjFont();
int d2_FindPic();
int udp_FindServers(u_short port, DWORD timeout, struct UDPSERVER *list, int max);
int sv_OpenMessageBox();
int ls_ReadSaveHeader();

int main_Unk532da0(int host)
{
    int ret, redraw, ok, f, btn[3], sel, h, n, i, k, cnt, run, last, list, ent[8][3], ent_i;
    unsigned ts;
    char recs[8][0x80], names[8][0x80], path[0x100], key[0x100], hdr[0xfc];
    char *r, *s;
    char c;
    int *d;

    ret = 0;
    redraw = 1;
    ts = U_I((unsigned)&ai_Unk6333cc);
    last = -1;
    f = d2_OpenForm(0, 0, "Menu\\SEARCH_NETWORK");
    d2_PlaceViewWindows(f, 1);
    d2_PlaceMainWindow(f, 2);
    d2_SetForm(f, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(f, 1);
    io_PrintText(0x1ca5);
    d2_SetForm(f, 3);
    sv_ButtonRow(d2_LastWindow, 0, btn, 3, U_I((unsigned)&full_data_14414b4), U_I((unsigned)&full_data_14414b8), U_I((unsigned)&full_data_14414bc));
    d2_SetObjEnabled(btn[0], 0);
    run = sv_RunFrame((int)main_Unk532da0, 0x24087);
    if (run != 0) {
        do {
            if (ts + 0x5dc < U_I((unsigned)&ai_Unk6333cc) || redraw) {
                ok = 1;
                U_I((unsigned)&d2_CutsceneActive) = 1;
                h = sv_OpenMessageBox(U_I(((unsigned)&ai_DifficultyTexts + 0x94)), 0);
                n = udp_FindServers(0x3039, 0xbb8, recs, 8);
                d2_CloseView(h);
                U_I((unsigned)&d2_CutsceneActive) = 0;
                d2_SetForm(f, 2);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                if (last != -1)
                    d2_RadioClear(last);
                list = d2_CreateRadioGroup(0);
                for (i = 0; i < n; i++) {
                    r = recs[i];
                    ent[i][0] = -1;
                    if ((*(int *)(r + 0x7c) == 0x3f && host != 0) || (*(int *)(r + 0x7c) == 0x7f && host == 0))
                        continue;
                    if (*(int *)(r + 0x7c) == 0x3f) {
                        k = 0;
                        do {
                            c = r[0x16 + k];
                            names[i][k] = c;
                            k++;
                        } while (c != 0);
                    } else {
                        sprintf(path, "Gamedata\\network\\%s.SAV", r + 0x3a);
                        h = vfs_Tfopen(path, (char *)(unsigned)"rb", U_I((unsigned)&d3_Unk6333fc));
                        if (h == 0) {
                            ok = 0;
                        } else {
                            ok = 1;
                            ls_ReadSaveHeader(h, hdr);
                            cnt = (unsigned char)hdr[0x31];
                            for (k = 0; k < cnt; k++) {
                                if (*(int *)(r + 0x5a + k * 4) != *(int *)(hdr + 0x78 + k * 4)) {
                                    ok = 0;
                                    break;
                                }
                            }
                            vfs_fclose(h);
                        }
                        k = 0;
                        do {
                            c = r[0x16 + k];
                            names[i][k] = c;
                            k++;
                        } while (c != 0);
                    }
                    if (ok == 0) {
                        ent[i][0] = -1;
                    } else {
                        _strupr(names[i]);
                        sprintf(key, "_STADTWAPPEN_%s", names[i]);
                        h = d2_FindPic(key);
                        if (h == -1)
                            h = d2_FindPic("_STADTWAPPEN_BERLIN");
                        d2_SetForm(f, 2);
                        ent_i = d2_WinOpenPic(8, i << 7, h, d2_LastWindow);
                        ent[i][0] = ent_i;
                        *(int *)(U_I((unsigned)&d2_Objs) + ent_i * 0x2ec + 0x4c) = 1;
                        if (*(int *)(r + 0x7c) == 0x3f && (unsigned char)r[0x37] >= (unsigned char)r[0x36])
                            d2_SetObjEnabled(ent_i, 0);
                        *(int *)(U_I((unsigned)&d2_Objs) + ent_i * 0x2ec + 0x1c0) = 3;
                        d2_RadioAdd(list, ent_i);
                        ent[i][1] = d2_CreateChildWindow(0x88, (i << 7) + 0xc, 0x190, 0x78, 0x10, d2_LastWindow);
                        d2_SetObjFont(U_I(((unsigned)&ai_Unkf8f13c + 0x250) + d2_LastWindow * 0x3c4), 0x53);
                        io_PrintText(0x1cbc, r, r + 0x16, (unsigned char)r[0x36]);
                    }
                }
                redraw = 0;
                ts = U_I((unsigned)&ai_Unk6333cc);
                last = list;
            }
            if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(char *)(unsigned)&kb_Last == 1)
                U_I((unsigned)&sv_PopRequest) = 1;
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (ai_Unk5fe710 == btn[0]) {
                    sel = U_I((unsigned)&d2_RadioSel + list * 0x8c);
                    if (sel != -1) {
                        r = recs[sel];
                        k = 0;
                        do {
                            c = r[k];
                            *(char *)((unsigned)&main_NetHost + k) = c;
                            k++;
                        } while (c != 0);
                        WritePrivateProfileStringA("Network", "Host", (char *)(unsigned)&main_NetHost, (char *)(unsigned)&ai_OptionsFile);
                        U_I((unsigned)&sv_PopRequest) = 1;
                        ret = 1;
                        d = (int *)(unsigned)&full_data_936460;
                        for (k = 0; k < 0x20; k++)
                            d[k] = ((int *)r)[k];
                        if (host != 0 && *(int *)(r + 0x7c) == 0x7f) {
                            k = 0;
                            do {
                                c = r[0x16 + k];
                                *(char *)(((unsigned)&ai_DataPath + 0x220) + k) = c;
                                k++;
                            } while (c != 0);
                            s = r + 0x3a;
                            k = 0;
                            do {
                                c = s[k];
                                *(char *)((unsigned)&main_NetSaveName + k) = c;
                                k++;
                            } while (c != 0);
                            k = 0;
                            do {
                                c = s[k];
                                *(char *)(((unsigned)&ls_SavePath + 0xca) + k) = c;
                                k++;
                            } while (c != 0);
                            U_I((unsigned)&ai_GameFlags) |= 0x40;
                            sprintf(path, "Gamedata\\network\\%s.SAV", (char *)(unsigned)&main_NetSaveName);
                            memset(hdr, 0, 0xfc);
                            h = vfs_Tfopen(path, (char *)(unsigned)"rb", U_I((unsigned)&d3_Unk6333fc));
                            if (h != 0) {
                                ls_ReadSaveHeader(h, hdr);
                                vfs_fclose(h);
                            }
                            d = (int *)((unsigned)&ai_Unk9364e0 + 0x2c0);
                            for (k = 0; k < 0xc; k++)
                                d[k] = 0;
                            U_I(((unsigned)&ai_Unk9364e0 + 0x2e0)) = *((unsigned *)hdr + 0x60 / 4) & 0xff;
                            U_I(((unsigned)&ai_Unk9364e0 + 0x2e4)) = *((unsigned *)hdr + 0x6c / 4) & 0xff;
                            k = 0;
                            do {
                                c = hdr[0x40 + k];
                                *(char *)(((unsigned)&ai_Unk9364e0 + 0x2c0) + k) = c;
                                k++;
                            } while (c != 0);
                            if (0x10053 < *(int *)hdr) {
                                k = 0;
                                do {
                                    c = hdr[0x50 + k];
                                    *(char *)(((unsigned)&ai_Unk9364e0 + 0x2d0) + k) = c;
                                    k++;
                                } while (c != 0);
                                U_I(((unsigned)&ai_Unk9364e0 + 0x2e8)) = *((unsigned *)hdr + 0x60 / 4) >> 0x18;
                                U_I(((unsigned)&ai_Unk9364e0 + 0x2ec)) = *((int *)hdr + 0x64 / 4);
                            }
                        }
                    }
                } else if (ai_Unk5fe710 == btn[1]) {
                    redraw = 1;
                } else if (ai_Unk5fe710 == btn[2]) {
                    d2_SetViewEnabled(f, 0);
                    if (main_Unk532c30() != 0) {
                        U_I((unsigned)&sv_PopRequest) = 1;
                        ret = 1;
                    }
                    d2_SetViewEnabled(f, 1);
                }
            }
            if (U_I((unsigned)&d2_RadioSel + list * 0x8c) != -1)
                d2_SetObjEnabled(btn[0], 1);
            else
                d2_SetObjEnabled(btn[0], 0);
            run = sv_RunFrame((int)main_Unk532da0, 0x24087);
        } while (run != 0);
        if (list != -1)
            d2_RadioClear(list);
    }
    d2_CloseView(f);
    return ret;
}
