/* 0x555b60, the run handler "he_TransportStadtStadt()" of the city-to-city transport (the error text "character-action not attached but target should be reached : %s" is in the 0x627330 literal; ROUGH, outlined from the disassembly): +0xb0 / +0xb4 the buildings (0x501190 (1, 1, id)), +0xc4 the id of the building the cart is at, +0xd0 started, +0xd4 rounds, +0xac the cart's id. Before the start (+0xc4 = the first building's id, +0xd0 = 0): a start position is taken (the building's transporter dummy 0x555210 or its +0x61 object, 0x458a20, the ground 0x45f170), the cart object is looked up (0x4ffe90 (building object, 1, 1, +0xac), else 0x4fe980) and put on the road (0x4c5260, 0x4c5100, the guide 0x4c4920) and +0xd0 = 1; with +0xc4 = the second building's id the cart's arrival (0x453e80 against the target, or more than 6 rounds) ends the trip: the guide is dropped, the cart gets the message 0x13 / 0x40, the arrival news is formatted (0x1c39 .. 0x1c44 by kind) and shown to a player character (0x53dee0, unless the world map shows the building: 0x6f229c / 0x6f2308 / 0x6f2310 with the ranges 0x92 .. 0x97 and 0x2db), the event is posted with the handle 3; the cart not found while it should have arrived gives the debug error 0x4222b0 */

struct D3HMAP * ch_GetHMap();
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);
void ai_Unk4c5100(int a, int b, int c, int d);

void he_Unk555b60(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, cart, o, e;
    float pos[3];
    char text[0x400];
    char err[0x200];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (*(int *)(ev + 0xc4) == *(int *)(b0 + 1) && *(unsigned char *)(ev + 0xd0) == 0) {
        d3if_SelectScene(0, 1);
        o = *(int *)(b1 + 0x61);
        d3_PointToWorld(o, o + 0x4c, pos);
        if (gm_IsType10(b0) != 0)
            cart = gm_Query(*(int *)(a44 + 0x5d), 1, 1, *(int *)(ev + 0xac));
        else
            cart = gm_Query(*(int *)(b0 + 0x5d), 1, 1, *(int *)(ev + 0xac));
        if (cart == 0)
            cart = gm_FindObjectInst(*(int *)(ev + 0xac));
        if (cart != 0) {
            he_Unk5552c0(ev, cart);
            ch_GetHMap(*(int *)(cart + 0x3b));
            d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), pos, 0, 0, 0);
            if (gm_IsType10(b0) == 0) {
                ai_Unk4c5260(*(int *)(cart + 2), 0);
                ((void (*)(int, int))ai_Unk4c5100)(*(int *)(cart + 2), pos);
                if (gm_IsType10(b0) == 0)
                    *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(cart + 2));
            }
            *(unsigned char *)(ev + 0xd0) = 1;
        }
        d3if_SelectScene(sc, 1);
        return;
    }
    if (*(int *)(ev + 0xc4) != *(int *)(b1 + 1))
        return;
    cart = gm_Query(*(int *)(b1 + 0x5d), 1, 1, *(int *)(ev + 0xac));                 /* 0x555daf */
    if (cart == 0)
        cart = gm_Query(cart, 2, 7, 1, *(int *)(ev + 0xac));                        /* 0x555dcb: (0, 2, 7, 1, +0xac) */
    if (cart == 0) {
        __asm int 3                                                                     /* 0x556055: the original traps here, repeats the query and returns */
        gm_Query(0, 2, 7, 1, *(int *)(ev + 0xac));
        return;
    }
    he_Unk5552c0(ev, cart);
    if (*(unsigned char *)(cart + 9) == 0x2d || (d3_VecEqual(*(int *)(*(int *)(cart + 0x3b) + 0x34) + 0x4c, ev + 0xd8, 100.0f) != 0) || *(int *)(ev + 0xd4) > 6) {
        cm_SendSetField(*(int *)(cart + 2), 0x13, 1, 0, 0x40);
        if (gm_IsType10(b1) == 0)
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(b1 + 1), *(int *)(a45 + 1), *(int *)(cart + 2));
        if (gm_IsType10(b1) != 0)
            txt_Format(text, 0x1c43);
        else
            txt_Format(text, 0x1c44, b1);
        e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
        if ((e == 6 || e == 7) && U_I((unsigned)&ai_PlayerBuilding) != b1)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(cart + 2), text, 0, 0x608, *(int *)(b1 + 1), -1, 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
    } else {
        /* 0x556022..0x556054 (ROUGH: the name index is a guess): the target should have been reached */
        sprintf(err, "he_TransportStadtStadt(): character-action not attached but target should be reached : %s",
                *(char **)((unsigned)&full_data_143b3f8 + *(unsigned char *)b1 * 7 * 8));
    }
}
