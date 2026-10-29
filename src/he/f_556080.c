/* 0x556080, the fourth leg of the transport event (mode 4 of +0xc8, called by the run handler 0x556600; ROUGH, a skeleton from the call sequence): like 0x555350 / 0x555690 / 0x555b60 the buildings +0xb0 / +0xb4 and the cart +0xac are looked up (0x501190 x4, 0x4fe980); the first round sends the cart (0x4c5230, "Trans" 0x4c52e0, the guide 0x4c4920), later rounds wait for the arrival (0x524810) and send it on (0x4c51b0, scene select 0x511e60), put it at the entry dummy "dummy_TRANSPORTER_ENTRY" (0x555210 random dummy, ground 0x45f170, 0x4c5100, message 0x13 / 0x40) and tell the player (news 0x1c39 .. by 0x1c3b / 0x1c3d, "_NACHRICHTEN_HS_59" / "_NACHRICHTEN_HS_60", 0x53dee0); the event is posted with the handle 3 and the scene reselected (0x4478e0) */

int init_BuildingSceneUniverse();
void ai_Unk4c5a70(int a);
void cm_BeginFieldBlock(char *o, int id);

void he_Unk556080(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, cart, o, e;
    float pos[3];
    char text[0x400];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    cart = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a45;
    if (cart == 0) {
        d3if_SelectScene(sc, 1);
        return;
    }
    he_Unk5552c0(ev, cart);
    if (*(unsigned char *)(ev + 0xd0) == 0) {
        ai_Unk4c5230(*(int *)(cart + 2));
        ai_Unk4c52e0(*(int *)(cart + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
        *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(cart + 2));
        *(unsigned char *)(ev + 0xd0) = 1;
        d3if_SelectScene(sc, 1);
        return;
    }
    if (sv_Unk524810(cart, b0) != 0) {
        *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(cart + 2));
        cm_RequestChrMoveToUniverse(*(int *)(cart + 2), *(int *)(b1 + 1), -1, (char *)0);
        init_BuildingSceneUniverse(b1);
        d3if_SelectScene(0, 1);
        o = he_Unk555210((char *)b1, 0, 0, 0);
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, pos);
            d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), pos, 0, 0, 0);
            ((void (*)(int, int))ai_Unk4c5100)(*(int *)(cart + 2), pos);
        }
        cm_SendSetField(*(int *)(cart + 2), 0x13, 1, 0, 0x40);
        e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
        if (e == 6 || e == 7) {
            txt_Format(text, 0x1c3b, 1, 0x1c39);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(cart + 2), text, 0, 0x609, 0, 0, (char *)"_NACHRICHTEN_HS_59", 0);
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        ai_Unk4c52e0(*(int *)(cart + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
    }
    d3if_SelectScene(sc, 1);
}
