/* 0x52a680, finds the object under the mouse cursor (0x448630 at the cursor position, kind 0x20 then 0x40) and builds the hint text 0xf8aae0 for it: the object kind +0x4b is 1 (item
   of a building, 0x6f22f0), 2 (a building, 0x6f22f4) or 3 (a character, 0x6f22f8); contact gates ("contact_gate_*") resolve to the contact item; ROUGH */

struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);   /* 0x448630 */

void sv_Unk52a680(void)
{
    char *o, *s, *t;
    unsigned char *b;
    unsigned short *it;
    int i, k, msg, lastarg;
    char buf[0x60];

    SV_I((unsigned)&ai_Unk6f22e8) = 0;
    if (SV_US((unsigned)&d2_PointerLook) != 0 && SV_US((unsigned)&d2_PointerLook) < 9) {
        SV_I((unsigned)&ai_Unk6f22e8) = 0;
        return;
    }
    SV_I((unsigned)&full_data_623e98) = SV_S((unsigned)&dm_L);
    SV_I((unsigned)&full_data_623e9c) = SV_S(((unsigned)&dm_L + 0x2));
    if (SV_I((unsigned)&full_data_623e98) <= SV_I((unsigned)&sv_ViewRectLeft) || SV_I((unsigned)&sv_ViewRectRight) <= SV_I((unsigned)&full_data_623e98) || SV_I((unsigned)&full_data_623e9c) <= SV_I((unsigned)&sv_ViewRectTop)
        || SV_I((unsigned)&sv_ViewRectBottom) <= SV_I((unsigned)&full_data_623e9c) || d2_HitWindow != -1 || SV_I((unsigned)&d2_HitUnderPointer) != -1
        || ((SV_C((unsigned)&ai_Unk6f22e8) = d3if_Pick((float)SV_I((unsigned)&full_data_623e98), (float)SV_I((unsigned)&full_data_623e9c), 0x20, 1)) == 0
            && (SV_C((unsigned)&ai_Unk6f22e8) = d3if_Pick((float)SV_S((unsigned)&dm_L), (float)SV_S(((unsigned)&dm_L + 0x2)), 0x40, 1)) == 0)) {
        if (ai_Unk5fe710 == -1 || (SV_B(SV_I((unsigned)&d2_Objs) + 0x1c0 + ai_Unk5fe710 * 0x2ec) & 0x40) == 0)
            SV_B((unsigned)&ai_Unkf8aae0) = 0;
        if (SV_I((unsigned)&d2_PointerSprite) != -1) {
            d2_FreeAnim(SV_I((unsigned)&d2_PointerSprite));
            SV_I((unsigned)&d2_PointerSprite) = -1;
            SV_I((unsigned)&full_data_6f23dc) = -1;
        }
        SV_I((unsigned)&full_data_6f1c4c) = 0;
        SV_I((unsigned)&full_data_6f22f0) = 0;
        SV_I((unsigned)&full_data_6f22f4) = 0;
        SV_I((unsigned)&ai_Unk6f22f8) = 0;
        SV_US((unsigned)&d2_PointerLook) = 0;
        return;
    }
    if ((SV_B((unsigned)&sv_FrameMask) & 0x10) == 0)
        return;
    if (SV_B((unsigned)&full_data_6ed9c8) != 0)
        return;
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (o == 0)
        return;
    if (d2_HitWindow != -1)
        return;
    if (SV_I((unsigned)&d2_HitUnderPointer) != -1)
        return;
    SV_I((unsigned)&full_data_6f2320) = 0;
    SV_I((unsigned)&full_data_6f2328) = 0;
    SV_I((unsigned)&full_data_6f2324) = 0;
    SV_I((unsigned)&full_data_6f232c) = 0;
    SV_US((unsigned)&d2_PointerLook) = 0;
    SV_I((unsigned)&full_data_6f22f0) = 0;
    SV_I((unsigned)&full_data_6f22f4) = 0;
    SV_I((unsigned)&ai_Unk6f22f8) = 0;
    SV_I((unsigned)&full_data_6f2300) = 0;
    k = o[0x4b];
    if (k == 1) {
        SV_I((unsigned)&full_data_6f22f0) = *(int *)(o + 0x200);
        b = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
        if (b == 0)
            b = 0;
        SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, b, SV_I((unsigned)&full_data_6f22f0), 0);
        if (SV_I((unsigned)&full_data_6f22f0) != 0) {
            i = *(unsigned short *)SV_I((unsigned)&full_data_6f22f0);
            k = ai_Goods[i * 0x41];
            if ((k == 6 || k == 2) && (SV_I((unsigned)&full_data_6f2320) & 0x4000) != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", SV_I((unsigned)&ai_Unk143c860 + i * 8), SV_I((unsigned)&full_data_143a3cc));
                SV_US((unsigned)&d2_PointerLook) = 0xb;
            } else {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s", SV_I((unsigned)&ai_Unk143c860 + i * 8));
                SV_US((unsigned)&d2_PointerLook) = 0x16;
            }
        }
        SV_I((unsigned)&full_data_6f2328) = SV_I((unsigned)&full_data_6f2320);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
    } else if (k == 2) {
        SV_I((unsigned)&full_data_6f22f4) = *(int *)(o + 0x200);
        SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, SV_I((unsigned)&full_data_6f22f4), 0, 0);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
        if (b != 0 && (b[0x5a] & 1) == 0) {
            if (b[5] != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s<", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5);
                SV_I((unsigned)&full_data_6f2324) = SV_I((unsigned)&full_data_6f2320);
                b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
                goto la9a;
            }
            strcpy((char *)(unsigned)&ai_Unkf8aae0, (char *)SV_I((unsigned)&full_data_143b3f8 + *b * 0x38));
        }
        SV_I((unsigned)&full_data_6f2324) = SV_I((unsigned)&full_data_6f2320);
    } else if (k == 3) {
        SV_I((unsigned)&ai_Unk6f22f8) = *(int *)(o + 0x200);
        SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, 0, 0, SV_I((unsigned)&ai_Unk6f22f8));
        if (SV_I((unsigned)&ai_Unk6f22f8) != 0) {
            io_CharacterName(*(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8), 1, 4, (unsigned)&ai_Unkf8aae0);
            if (*(char *)(SV_I((unsigned)&ai_Unk6f22f8) + 0x165) == 0x1c && (*(unsigned char *)(SV_I((unsigned)&ai_Unk6f22f8) + 0x1b4) & 1) != 0 && (SV_I((unsigned)&full_data_6f2320) & 0x800) != 0
                && SV_I((unsigned)&d3_CurrentScene) != 0) {
                txt_Format((char *)(unsigned)&ai_Unkf8aae0, 0x414c, *(unsigned short *)SV_I((unsigned)&ai_Unk6f22f8));
                SV_US((unsigned)&d2_PointerLook) = 0x2e;
            }
        }
        SV_I((unsigned)&full_data_6f232c) = SV_I((unsigned)&full_data_6f2320);
        b = (unsigned char *)SV_I((unsigned)&full_data_6f22f4);
    } else {
        SV_B((unsigned)&ai_Unkf8aae0) = 0;
        b = 0;
    }
la9a:
    if (SV_I((unsigned)&ai_Unk75ec60) != 0 && (int)b == SV_I((unsigned)&ai_Unk75ec60) && (SV_I((unsigned)&full_data_6f2324) & 1) != 0 && (b[0x5a] & 1) == 0)
        SV_US((unsigned)&d2_PointerLook) = 0xb;
    b = (unsigned char *)SV_I((unsigned)&ai_FallbackBuilding);
    if ((b != 0 || (b = (unsigned char *)SV_I((unsigned)&ai_PlayerBuilding)) != 0) && SV_C((unsigned)&ai_Unk6f22e8) != 0 && strncmp(SV_C((unsigned)&ai_Unk6f22e8), "contact_gate_", 0xd) == 0) {
        strcpy(buf, SV_C((unsigned)&ai_Unk6f22e8) + 0x10);
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0) {
            msg = *(int *)(b + 0x5d);
            lastarg = 2;
        } else {
            msg = *(int *)(b + 0x5d);
            lastarg = 6;
        }
        it = (unsigned short *)gm_Query(msg, 1, 4, lastarg);
        while (it != 0) {
            if (_strcmpi(buf, ai_Goods + *it * 0x41 + 1) == 0)
                goto lb9e;
            it = (unsigned short *)gm_QueryNext();
        }
        it = (unsigned short *)SV_I((unsigned)&full_data_6f2300);
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0)
            it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
lb9e:
        SV_I((unsigned)&full_data_6f2300) = (int)it;
        if (SV_I((unsigned)&full_data_6f2300) != 0) {
            SV_I((unsigned)&full_data_6f2320) = unk_Unk503530(ai_PlayerId, b, SV_I((unsigned)&full_data_6f2300), 0);
            if ((SV_I((unsigned)&full_data_6f2320) & 1) == 0) {
                SV_I((unsigned)&full_data_6f2300) = 0;
            } else {
                SV_US((unsigned)&d2_PointerLook) = 0xb;
                if (*(unsigned short *)SV_I((unsigned)&full_data_6f2300) == 0xfd) {
                    t = (char *)SV_I((unsigned)&full_data_143a3d0);
                    if (SV_I((unsigned)&sv_PanelShown) == 0)
                        msg = SV_I((unsigned)&full_data_143b3f8 + *b * 0x38);
                    else
                        msg = SV_I((unsigned)&ai_Unk143c860 + *(unsigned short *)SV_I((unsigned)&sv_PanelShown) * 8);
                } else {
                    msg = SV_I((unsigned)&ai_Unk143c860 + *(unsigned short *)SV_I((unsigned)&full_data_6f2300) * 8);
                    t = (char *)SV_I((unsigned)&full_data_143a3cc);
                }
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", msg, t);
            }
        }
    }
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (memcmp(o, "tp_TUER", 8) == 0 && (b = *(unsigned char **)(o + 0x200)) != 0) {
        if ((SV_I((unsigned)&full_data_6f2320) & 1) == 0) {
            if (b[5] == 0) {
                strcpy((char *)(unsigned)&ai_Unkf8aae0, (char *)SV_I((unsigned)&full_data_143b3f8 + *b * 0x38));
                goto lad73;
            }
            sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s<", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5);
        } else {
            SV_US((unsigned)&d2_PointerLook) = 0xb;
            if (b[5] != 0) {
                sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s >%s< %s", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), b + 5, SV_I((unsigned)&full_data_143a3cc));
                o = SV_C((unsigned)&ai_Unk6f22e8);
                goto lad73;
            }
            sprintf((char *)(unsigned)&ai_Unkf8aae0, "%s %s", SV_I((unsigned)&full_data_143b3f8 + *b * 0x38), SV_I((unsigned)&full_data_143a3cc));
        }
        o = SV_C((unsigned)&ai_Unk6f22e8);
    } else {
        s = (char *)((unsigned)&sv_NamePlates + 0x48);
        for (i = 0x20; i != 0; i--) {
            if (*(char **)(s - 0x48) == o) {
                strcpy((char *)(unsigned)&ai_Unkf8aae0, s);
                SV_US((unsigned)&d2_PointerLook) = *(unsigned short *)(s - 4);
            }
            s += 200;
        }
    }
lad73:
    if (SV_I((unsigned)&d2_PointerSprite) != -1) {
        d2_FreeAnim(SV_I((unsigned)&d2_PointerSprite));
        SV_I((unsigned)&d2_PointerSprite) = -1;
        SV_I((unsigned)&full_data_6f23dc) = 0;
        o = SV_C((unsigned)&ai_Unk6f22e8);
    }
    SV_C((unsigned)&full_data_6f1c4c) = o;
}
