/* 0x53e8b0, adds the gfx info of the event `r` by its type (2 / 0x1f plus hammer, 4 / 0x35 / 0x6b exclamation marks over the parent, 0x1a blows, 0x1b coin, 0x1c / 0x1e hammer gold, 0x40 binoculars) when the event has none (ROUGH) */

void he_Unk53e8b0(unsigned char *r)
{
    int iv, o;
    unsigned short *pl;

    if (*(char *)((unsigned)&ai_GameSwitches + 0x3) == 0)
        return;
    if (*(int *)(r + 0x88) != 0)
        return;
    switch (*r) {
    case 2:
        o = *(int *)(r + 0x10);
        goto L_hammer;
    case 4:
        if (*(unsigned short *)(r + 8) != (unsigned short)U_I((unsigned)&ai_PlayerId))
            return;
        if (*(int *)(r + 200) == 0)
            return;
        iv = gm_FindBuildingById(*(int *)(r + 0xbc));
        if (iv == 0)
            return;
        iv = *(int *)(iv + 0x61);
        goto L_mark;
    case 0x1a:
        iv = gm_FindBuildingById(*(int *)(r + 0xd0));
        if (iv != 0 && *(int *)(iv + 0x61) != 0) {
            he_Unk53e780((int)r, "he_schlaege", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x1b:
        iv = gm_FindBuildingById(*(int *)(r + 0xac));
        if (iv != 0) {
            he_Unk53e780((int)r, "he_muenze", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x1c:
        iv = gm_FindBuildingById(*(int *)(r + 0xac));
        if (iv != 0 && *(int *)(iv + 0x61) != 0) {
            he_Unk53e780((int)r, "he_hammer_gold", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x1e:
        iv = gm_FindBuildingById(*(int *)(r + 0xac));
        if (iv != 0 && *(int *)(iv + 0x61) != 0) {
            he_Unk53e780((int)r, "he_saege_hammer_gold", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x1f:
        o = *(int *)(r + 0xac);
L_hammer:
        iv = gm_FindBuildingById(o);
        if (iv != 0 && *(int *)(iv + 0x61) != 0) {
            he_Unk53e780((int)r, "he_plus_hammer", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x35:
        pl = (unsigned short *)gm_FindChar(*(int *)(r + 0xbc));
        if (pl == 0)
            return;
        if (*pl != (unsigned short)U_I((unsigned)&ai_PlayerId))
            return;
        if (U_I((unsigned)&d3_CurrentScene) != 0)
            return;
        iv = d3_FindObject(0, 0x40, 0, "ob_SCHWARZES_BRETT");
        goto L_mark;
    case 0x40:
        iv = gm_FindBuildingById(*(int *)(r + 0xac));
        if (iv != 0 && *(int *)(iv + 0x61) != 0 && *(unsigned short *)(r + 8) == (unsigned short)U_I((unsigned)&ai_PlayerId)) {
            he_Unk53e780((int)r, "he_fernglas", *(int *)(iv + 0x61));
            return;
        }
        break;
    case 0x6b:
        if (*(int *)(r + 0x70) != 2)
            return;
        if (U_I((unsigned)&d3_CurrentScene) != 0)
            return;
        if ((U_I(((unsigned)&ai_Selected + 0x40) + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c) & 0x20000) != 0)
            return;
        iv = d3_FindObject(0, 0x40, 0, "ob_TRIBUENE");
L_mark:
        if (iv != 0)
            he_Unk53e780((int)r, "he_ausrufezeichen", iv);
    }
}
