/* 0x4cf670, the mouse pick: the object under the cursor (ai_Unk6f22e8, from the scene, else the ground point) and the
   cursor look d2_PointerLook by the selected characters' kind (0x160 -> 0x18 .. 0x176 -> 0x25; more than one selected: 0x23;
   "sp_ESCAPE" 0x1c, "sp_CONQUER" 0x1b, "WARE" 0xe). Rough draft. */

void ai_Unk4cf670()
{
    int n = 0, cur = 0x23;
    char *sc, *c, *r, **pp;
    int px, py;
    float v[3];
    float h;

    sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    ai_Unk6f22e8 = 0;
    d2_PointerLook = 0;
    if (dm_L.x > 0 && dm_L.x < d2_ScreenW && dm_L.y > 0 && dm_L.y < d2_ScreenH &&
        ai_Unk5fe710 == -1) {
        ai_Unk6f22e8 = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x40, 1);
        if (ai_Unk6f22e8 == 0 && ai_Unk6f0d08 != 0) {
            if ((d3sm_Pick(sc, dm_L.x + 0x1a, dm_L.y + 0x1a, &py, &px) & 0xff) != -1) {
                d3sm_GetVertexI(sc, py, px, v);
                h = v[1] + ai_Unk5ebb68;
                d2_PointerLook = 0;
            }
        } else if (ai_Unk6f0cf0 != 0) {
            d3if_Show(ai_Unk6f0cf0, 0);
        }
    }
    if (ai_Unk6f22e8 == 0) {
        ai_Unk6f0ce0 = 0;
        ai_Unk6f22f8 = 0;
        return;
    }
    for (pp = ai_Unk7603a0; (int)pp < ((unsigned)&ai_Unk7603a0 + 0x7c); pp++) {
        c = *pp;
        if (c != 0 && c[0x188] != 0) {
            if (ai_Unk4ce170(*(unsigned short *)ai_Unk6f0d74, c) & 0x800) {
                if (kb_Down[0x36] == 0) {
                    r = ai_Unk4cda50(c);
                    if (r != 0) {
                        switch (*(int *)r) {
                        case 0x160: cur = 0x18; break;
                        case 0x174: cur = 0x19; break;
                        case 0x16e: cur = 0x1e; break;
                        case 0x154: cur = 0x1a; break;
                        case 0x158: cur = 0x1f; break;
                        case 0x15e: cur = 0x21; break;
                        case 0x156: cur = 0x23; break;
                        case 0x176: cur = 0x25; break;
                        }
                    }
                    n++;
                }
            }
        }
    }
    if (n > 1)
        cur = 0x23;
    if (ai_Unk6f22e8 != 0 && ai_Unk5fe710 == -1) {
        if (strcmp(ai_Unk6f22e8, "sp_ESCAPE") == 0 && n != 0) {
            d2_PointerLook = 0x1c;
            ai_Unk6f0ce0 = ai_Unk6f22e8;
            return;
        }
        if (strncmp(ai_Unk6f22e8, "sp_CONQUER", 10) == 0 && n != 0) {
            d2_PointerLook = 0x1b;
            ai_Unk6f0ce0 = ai_Unk6f22e8;
            return;
        }
        if (strncmp(ai_Unk6f22e8, "WARE", 4) == 0 && n != 0) {
            d2_PointerLook = 0xe;
            ai_Unk6f0ce0 = ai_Unk6f22e8;
            return;
        }
        if (*(int *)(ai_Unk6f22e8 + 0x220) == 2 && n != 0) {
            if (!(ai_Unk4ce170(*(unsigned short *)ai_Unk6f0d74, *(int *)(ai_Unk6f22e8 + 0x200)) & 0x800) ||
                kb_Down[0x36] != 0) {
                ai_Unk6f0ce0 = ai_Unk6f22e8;
                d2_PointerLook = cur;
                return;
            }
        } else
            d2_PointerLook = 0;
        ai_Unk6f0ce0 = ai_Unk6f22e8;
        return;
    }
    d2_PointerLook = 0;
}
