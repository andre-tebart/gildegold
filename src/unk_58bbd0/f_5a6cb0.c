/* 0x5a6cb0, the town map's per-frame rebuild (3049 B, ROUGH: the frame / slots are approximate; the object
   fields +0x34 kind, +0x61 the carrier, +0x65 the city, +0x27 the character, the entry array at +0xdc is 0x100 x
   6 dwords {id, object, x, y, map x, map y}, the screen record 0xf8f120 + screen * 0x3c4, the control records
   0xf8f080 + id * 0x2ec): the entries of the player's city objects (0x501190 (2, 5, 9, 4, the player 0x6f25b8);
   0x501030 next) are collected by the kind letter (D / E a building of the carrier, the byte +0x5a & 1, G only
   with the hall 0x501190 (1, 6)), sorted by their y and drawn: the buildings of the city at the kind direction
   (0, -1) .. (-1, -1) scaled by the scene cell 0xfd7c50 + 0x20 * 5.33f and the globals 0x5eed90 (G) / 0x5eed8c,
   the rest projected by 0x458a20 / 0x45f170 and 0x5a6b70; the picture 0x4731d0 is the kind letter + 0x47f, the
   name the city table 0x7e3f40 (+0x65 * 0x2f4) or 0x5a6b50 ("%1G"), the character marker (the kind 5 .. 7 at
   0x770b82, the id +0x54) at x - 8 / y - 8; the visible flag 0x4c of the carried objects is cleared by 0x5c39a0
   (the player record 0x770b80 + the player * 0x21c, the record + 0xc, the object); with the flag 4 the eight
   "sp_AUFLAUERLEGEN_*" spots 0x625720 (0x449690 (0, 0x100, 0, name), picture 0x550, the ids +0x26dc), with the
   flag 8 the effect objects 0x6f26b0 (0x400 bytes, picture 0x54f, the ids +0x24dc); then the view is centred on
   the object 0x6f2308 / 0x6f230c (+0x61) or 0xfc2dec (0x5a6b70, the screen half 0xf8f128 / 0xf8f12a, clamped to
   the map 0x6f4ee8 - 0x200 / 0x6f4ee4 - 0x168) and 0x46f840 scrolls */

char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py); void d2_RaiseObject(); int ai_Unk5c39a0(); void d2_ScrollWindow();

void ai_Unk5a6cb0(int *m_)
{
    char *p = (char *)m_;
    char *scene;
    char *obj;
    char *owner;
    char *name;
    char *rec;
    char *src;
    char *dst;
    char *ch;
    char c;
    int *e;
    int *a;
    int *b;
    int dirs[8][2];
    int item[6];
    int pos[4];
    float py;
    int screen;
    int ctrl;
    int hall;
    int cityx;
    int cityy;
    int city;
    int saved;
    int i;
    int j;
    int k;
    int x;
    int y;
    int n;

    scene = (char *)(*(int *)((unsigned)&ai_Unkfd7c50));
    city = *(unsigned char *)((unsigned)&ai_OwnPlayer);
    cityx = *(int *)((unsigned)&ai_Unk7e422c + city * 0x2f4);
    cityy = *(int *)(((unsigned)&ai_Unk7e422c + 0x4) + city * 0x2f4);

    dirs[0][0] = 0; dirs[0][1] = -1;
    dirs[1][0] = 1; dirs[1][1] = 0;
    dirs[2][0] = 1; dirs[2][1] = 0;
    dirs[3][0] = 1; dirs[3][1] = 1;
    dirs[4][0] = 0; dirs[4][1] = 1;
    dirs[5][0] = -1; dirs[5][1] = 1;
    dirs[6][0] = -1; dirs[6][1] = 0;
    dirs[7][0] = -1; dirs[7][1] = -1;

    for (i = 0, e = (int *)(p + 0xdc); i < 0x100; i++, e += 6) {
        e[0] = -1;
        e[1] = 0;
    }
    for (i = 0; i < 0x80; i++)
        *(int *)(p + 0x24dc + i * 4) = -1;
    for (i = 0; i < 0x300; i++)
        *(int *)(p + 0x18dc + i * 4) = -1;
    *(int *)(p + 0x27a4) = 0;

    hall = gm_BuildQuery(2, 5, 9, 4, *(unsigned short *)((unsigned)&ai_PlayerId));
    obj = (char *)gm_BuildQuery(1, 6);
    i = 0;
    if (obj != 0) {
        e = (int *)(p + 0xe0);
        do {
            if (i >= 0x100)
                break;
            if (obj[0] != 'D' && obj[0] != 'E' && (obj[0x5a] & 1) == 0 &&
                (obj[0] != 'G' || hall != 0)) {
                e[0] = (int)obj;
                i++;
                e += 6;
            }
            obj = (char *)gm_BuildNext();
            *(int *)(p + 0x27a4) = i;
        } while (obj != 0);
    }

    screen = *(int *)(p + 8);
    *(int *)((unsigned)&d2_LastWindow) = screen;
    n = *(int *)(p + 0x27a4);
    if (n > 0) {
        for (i = 0, e = (int *)(p + 0xdc); i < n; i++, e += 6) {
            obj = (char *)e[1];
            if (*(int *)(obj + 0x61) == 0) {
                screen = *(int *)(p + 8);
                *(int *)((unsigned)&d2_LastWindow) = screen;
                rec = (char *)((unsigned)&d2_Wins + screen * 0x3c4);
                *(int *)((unsigned)&d2_LastWindowPtr) = (int)rec;
                if (obj[0] == 'G') {
                    x = (int)((float)*(int *)(scene + 0x20) * 5.33f * 0.5f +
                              (float)dirs[*(unsigned char *)(obj + 0x34)][0] *
                              (float)*(int *)(scene + 0x20) * 5.33f * *(float *)((unsigned)&full_rdata_5eed90));
                    y = (int)((float)*(int *)(scene + 0x20) * 5.33f * 0.5f +
                              (float)dirs[*(unsigned char *)(obj + 0x34)][1] *
                              (float)*(int *)(scene + 0x20) * 5.33f * *(float *)((unsigned)&full_rdata_5eed90));
                } else {
                    x = (int)((float)*(int *)(scene + 0x20) * 5.33f * 0.5f +
                              (float)dirs[*(unsigned char *)(obj + 0x34)][0] *
                              (float)*(int *)(scene + 0x20) * 5.33f * *(float *)((unsigned)&full_rdata_5eed8c));
                    y = (int)((float)*(int *)(scene + 0x20) * 5.33f * 0.5f +
                              (float)dirs[*(unsigned char *)(obj + 0x34)][1] *
                              (float)*(int *)(scene + 0x20) * 5.33f * *(float *)((unsigned)&full_rdata_5eed8c));
                }
                ctrl = d2_WinOpenPic(x, y, *(unsigned char *)obj + 0x47f, screen);
                e[0] = ctrl;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 0x43;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1bc) = 0x56;
                x -= (int)*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x14) / 2;
                y -= (int)*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x16) / 2;
                d2_MoveObj((short)(x + *(short *)(rec + 4)), (short)(y + *(short *)(rec + 6)), ctrl);
                if (obj[0] == 'G') {
                    src = (char *)((unsigned)&ai_PlayerSlots + *(int *)(obj + 0x65) * 0x2f4);
                    dst = (char *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0xbc);
                    do {
                        c = *src++;
                        *dst++ = c;
                    } while (c != 0);
                } else {
                    ai_FormatCityName((int)obj, (char *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0xbc));
                }
                if (*(unsigned short *)(obj + 0x27) != 0xffff) {
                    ch = (char *)((unsigned)&ai_Chars + *(unsigned short *)(obj + 0x27) * 0x21c);
                    if (ch[2] == 5 || ch[2] == 6 || ch[2] == 7)
                        d2_WinOpenPic(x - 8, y - 8, *(int *)(ch + 0x54), screen);
                }
            } else {
                owner = (char *)*(int *)(obj + 0x61);
                d3_PointToWorld((int)owner, (int)(owner + 0x4c), pos);
                d3_GetHeightAt((int)scene, pos, &x, &y, &py);
                e[2] = x;
                e[3] = y;
                ai_Unk5a6b70((int)e, cityx, cityy);
            }
        }
    }

    n = *(int *)(p + 0x27a4);
    if (n > 1) {
        for (i = 1; i < n; i++) {
            for (j = i; j < n; j++) {
                if (*(int *)(p + 0xdc + j * 0x18 + 0xc) <
                    *(int *)(p + 0xdc + (i - 1) * 0x18 + 0xc)) {
                    a = (int *)(p + 0xdc + (i - 1) * 0x18);
                    b = (int *)(p + 0xdc + j * 0x18);
                    for (k = 0; k < 6; k++)
                        item[k] = a[k];
                    for (k = 0; k < 6; k++)
                        a[k] = b[k];
                    for (k = 0; k < 6; k++)
                        b[k] = item[k];
                }
            }
        }
    }

    screen = *(int *)(p + 8);
    *(int *)((unsigned)&d2_LastWindow) = screen;
    rec = (char *)((unsigned)&d2_Wins + screen * 0x3c4);
    *(int *)((unsigned)&d2_LastWindowPtr) = (int)rec;
    n = *(int *)(p + 0x27a4);
    if (n > 0) {
        for (i = 0, e = (int *)(p + 0xdc); i < n; i++, e += 6) {
            x = (int)*(float *)((char *)e + 0x10);
            y = (int)*(float *)((char *)e + 0x14);
            if (e[0] == -1) {
                obj = (char *)e[1];
                ctrl = d2_WinOpenPic(x, y, *(unsigned char *)obj + 0x47f, screen);
                e[0] = ctrl;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1cc) = -1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1d0) = -1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 0x43;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1bc) = 0x56;
                if (obj[0] == 'G') {
                    src = (char *)((unsigned)&ai_PlayerSlots + *(int *)(obj + 0x65) * 0x2f4);
                    dst = (char *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0xbc);
                    do {
                        c = *src++;
                        *dst++ = c;
                    } while (c != 0);
                } else {
                    ai_FormatCityName((int)obj, (char *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0xbc));
                }
                x -= (int)*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x14) / 2;
                y -= (int)*(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x16) / 2;
                e[2] = x;
                e[3] = y;
                d2_MoveObj((short)(x + *(short *)(rec + 4)), (short)(y + *(short *)(rec + 6)), ctrl);
                if (ai_Unk5c39a0((int)((unsigned)&ai_Chars + *(unsigned short *)((unsigned)&ai_PlayerId) * 0x21c),
                                 (int)(p + 0xc), (int)obj) == 0) {
                    *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 0;
                    *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 0;
                }
            }
        }
    }

    screen = *(int *)((unsigned)&d2_LastWindow);
    n = *(int *)(p + 0x27a4);
    for (i = 0, e = (int *)(p + 0xdc); i < n; i++, e += 6) {
        if (e[0] != -1) {
            obj = (char *)e[1];
            if (*(unsigned short *)(obj + 0x27) != 0xffff) {
                ch = (char *)((unsigned)&ai_Chars + *(unsigned short *)(obj + 0x27) * 0x21c);
                if (ch[2] == 5 || ch[2] == 6 || ch[2] == 7) {
                    d2_WinOpenPic(e[2] - 8, e[3] - 8, *(int *)(ch + 0x54), screen);
                    screen = *(int *)((unsigned)&d2_LastWindow);
                }
            }
        }
    }

    if ((*(unsigned char *)(p + 4) & 4) != 0) {
        saved = *(int *)((unsigned)&d3_CurrentScene);
        d3if_SelectScene(0, 1);
        screen = *(int *)(p + 8);
        *(int *)((unsigned)&d2_LastWindow) = screen;
        *(int *)((unsigned)&d2_LastWindowPtr) = (unsigned)&d2_Wins + screen * 0x3c4;
        for (i = 0; i < 0x10; i++)
            *(int *)(p + 0x26dc + i * 4) = -1;
        i = 0;
        for (name = (char *)(unsigned)&ai_AmbushSpots; name < (char *)(unsigned)((unsigned)ai_AmbushSpots + 0x100); name += 0x20) {
            obj = (char *)d3_FindObject(0, 0x100, 0, name);
            if (obj != 0) {
                d3_PointToWorld((int)obj, (int)(obj + 0x4c), pos);
                d3_GetHeightAt((int)scene, pos, &x, &y, &py);
                item[2] = x;
                item[3] = y;
                ai_Unk5a6b70((int)item, cityx, cityy);
                x = (int)*(float *)(item + 4);
                y = (int)*(float *)(item + 5);
                ctrl = d2_WinOpenPic(x, y, 0x550, screen);
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 0x43;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1bc) = 0x56;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x2e8) = (int)obj;
                *(int *)(p + 0x26dc + i * 4) = ctrl;
            }
            i++;
        }
        d3if_SelectScene(saved, 1);
    }

    if ((*(unsigned char *)(p + 4) & 8) != 0) {
        screen = *(int *)(p + 8);
        *(int *)((unsigned)&d2_LastWindow) = screen;
        for (i = 0; i < 0x400; i += 4) {
            obj = (char *)*(int *)(*(int *)((unsigned)&full_data_6f26b0) + i);
            if (obj != 0) {
                d3_PointToWorld((int)obj, (int)(obj + 0x4c), pos);
                d3_GetHeightAt((int)scene, pos, &x, &y, &py);
                item[2] = x;
                item[3] = y;
                ai_Unk5a6b70((int)item, cityx, cityy);
                x = (int)*(float *)(item + 4);
                y = (int)*(float *)(item + 5);
                ctrl = d2_WinOpenPic(x, y, 0x54f, screen);
                *(int *)(p + 0x24dc + i) = ctrl;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x4c) = 1;
                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1c0) = 0x43;
            }
        }
    }

    obj = (char *)*(int *)((unsigned)&ai_PlayerBuilding);
    if (obj != 0) {
        obj = (char *)*(int *)(obj + 0x61);
    } else {
        obj = (char *)*(int *)((unsigned)&ai_FallbackBuilding);
        if (obj != 0)
            obj = (char *)*(int *)(obj + 0x61);
    }
    if (obj == 0)
        obj = (char *)*(int *)((unsigned)&ai_Unkfc2dec);
    if (obj == 0)
        return;
    d3_PointToWorld((int)obj, (int)(obj + 0x4c), pos);
    d3_GetHeightAt((int)scene, pos, &x, &y, &py);
    item[2] = x;
    item[3] = y;
    ai_Unk5a6b70((int)item, cityx, cityy);
    screen = *(int *)(p + 8);
    *(int *)((unsigned)&d2_LastWindow) = screen;
    rec = (char *)((unsigned)&d2_Wins + screen * 0x3c4);
    x = (int)*(float *)(item + 4) - (int)*(short *)(rec + 8) / 2;
    y = (int)*(float *)(item + 5) - (int)*(short *)(rec + 0xa) / 2;
    if (x < 0)
        x = 0;
    if (x > *(int *)((unsigned)&ai_MapSize) - 0x200)
        x = *(int *)((unsigned)&ai_MapSize) - 0x200;
    if (y < 0)
        y = 0;
    if (y > *(int *)((unsigned)&full_data_6f4ee4) - 0x168)
        y = *(int *)((unsigned)&full_data_6f4ee4) - 0x168;
    *(int *)(rec + 0x258) = x;
    *(int *)(rec + 0x248) = y;
    d2_ScrollWindow(*(int *)rec);
}
