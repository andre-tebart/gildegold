/* 0x52c5c0, picks the speech bubble for the character under the cursor (0x6f22f8) or the object (0x6f22e8) and shows it through 0x52c4e0: a text id (0x11e9 + n * 0xf, or by the role +0x180 of the
   character: 0x1250 ..) or a text built into a buffer; ROUGH (the switch cases are read from the Ghidra output, the 5 kinds of the final switch are mostly lost) */

char d3if_ScreenBox(D3OBJECT *obj, D3RECT *r);                     /* 0x4486e0 */
/* 0x42ec00 */
struct CHARACTER * ch_FindByObject();                     /* 0x402ac0 */
int gm_Unk504c00();                     /* 0x504c00 */
int gm_TitleGroup();                     /* 0x504ad0 */
char * gm_FindBuildingById(int id);                     /* 0x502020 */
float unk_Unk4f9e00(int *o);                     /* 0x4f9e00 */
float ord_BuildingCapacityRatio();                     /* 0x4f97c0 */

void sv_Unk52c5c0(void)
{
    char *cls, *g, *o, *b, *r;
    int v, id, k, bv;
    unsigned kind;
    unsigned short *q;
    char text[0x200];
    char tmp[0x28];

    g = SV_C((unsigned)&ai_Unk6f22f8);
    o = SV_C((unsigned)&ai_Unk6f22e8);
    if (g == 0) {
        if (o == 0)
            return;
        if (*(int *)(o + 0x220) != 2)
            return;
        if (SV_I((unsigned)&full_data_6f22f0) != 0)
            return;
    }
    if (SV_I(((unsigned)&dm_L + 0xc)) == 0)
        return;
    cls = 0;
    b = SV_C((unsigned)&ai_PlayerBuilding);
    if (b == 0)
        b = SV_C((unsigned)&ai_FallbackBuilding);
    if (b != 0)
        cls = ai_BTypes + *(unsigned char *)b * 0x24d;
    if (g == 0 || (unk_Unk503530(ai_PlayerId, 0, 0, g), (g = SV_C((unsigned)&ai_Unk6f22f8)) == 0)) {
        if (o != 0) {
            v = ch_FindByObject(o);
            goto l67e;
        }
    } else {
        v = *(int *)(g + 0x184);
l67e:
        if (v != 0 && (*(unsigned char *)(v + 4) & 1) == 0)
            return;
    }
    if (b != 0 && ((*cls != 0x16 || *(unsigned short *)SV_I((unsigned)&sv_PanelShown) != 0x120) && *cls != 0x0f))
        return;
    util_RandomU(4);
    text[0] = ch_EmptyName[0];
    memset(text + 1, 0, 0x1ff);
    id = -1;
    if (g == 0) {
        kind = (unsigned short)util_RandomU(4) + 1;
lcc95:
        if (kind < 5)
            goto lcc9e;
    } else {
        int plain = 1;

        if (*(char *)(g + 0x164) == 0) {
            if (*(char *)(g + 0x165) != 0) {
                v = gm_Unk504c00(*(char *)(g + 0x165));
                goto l712;
            }
l77c:
            kind = (unsigned short)util_RandomU(4) + 1;
        } else {
            v = gm_TitleGroup(*(char *)(g + 0x164));
l712:
            if (v != 0xb && v != 0xc)
                goto l77c;
            plain = 0;
            kind = (unsigned short)util_RandomU(4) + 1;
            if (kind == 3)
                kind = 4;
        }
        v = *(int *)(g + 0x17c);
        if (v == 0)
            goto lcc95;
        kind = 0xffffffff;
        switch (*(unsigned char *)(g + 0x180)) {
        case 0x14:
            if (*(char **)(g + 0x16c) != 0) {
                if (ai_BTypes[**(unsigned char **)(g + 0x16c) * 0x24d] == ' ')
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1286);
                else
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1256);
            }
            break;
        default:
            kind = (unsigned short)util_RandomU(4) + 1;
            if (plain || kind != 3)
                goto lcc95;
            kind = 4;
            goto lcc9e;
        case 0x18:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xc4));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1262);
            break;
        case 0x19:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x125c);
            break;
        case 0x1a:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xb4));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1277);
            break;
        case 0x28:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x124d);
            break;
        case 0x3c:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xac));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x1253);
            break;
        case 0x3f:
        case 0x75:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1250);
            break;
        case 0x40:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0 && *(short *)(r + 0x27) != -1)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(short *)(r + 0x27), r) + 0x1265);
            break;
        case 0x43:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1268);
            break;
        case 0x44:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1259);
            break;
        case 0x48:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x126e);
            break;
        case 0x49:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xac));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x124a);
            break;
        case 0x61:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x126b);
            break;
        case 0x62:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x125f);
            break;
        case 100:
            r = (char *)gm_FindBuildingById(*(int *)(v + 0xb0));
            if (r != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *, int))util_Random)(3, *(unsigned short *)(r + 0x27), r) + 0x1271);
            break;
        case 0x65:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1274);
            break;
        case 0x89:
            q = (unsigned short *)gm_FindChar(*(int *)(v + 0xb0));
            if (q != 0)
                txt_Format(text, (unsigned short)((int (*)(int, void *))util_Random)(3, *q) + 0x127a);
            break;
        case 0x8c:
            if (*(char **)(g + 0x16c) != 0) {
                k = ai_BTypes[**(unsigned char **)(g + 0x16c) * 0x24d];
                if (k == 0x1d)
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1283);
                else if (k == 0x1f)
                    txt_Format(text, (unsigned short)util_RandomU(3) + 0x1289);
            }
            break;
        case 0x8e:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x127d);
            break;
        case 0xa1:
            txt_Format(text, (unsigned short)util_RandomU(3) + 0x1280);
        }
    }
    goto lcca;
lcc9e:
    id = kind * 0xf + 0x11e9;
lcca:
    if ((short)util_RandomU(2) == 0 && he_FindEvent(1, 0, 0x52) != 0)
        kind = 5;
    unk_Unk4f9e00(tmp);
    switch (kind) {
    case 2:
        ord_BuildingCapacityRatio();
        goto lftol;
    case 1:
    case 3:
    case 4:
lftol:
        id = (int)(0.0) + (unsigned short)util_RandomU(3);       /* the float operand of the first term is lost in the pseudocode */
        break;
    case 5:
        if (SV_I((unsigned)&hst_KeyNext) != -1) {
            id = 1;
            for (q = (unsigned short *)(((unsigned)&txt_GroupLevel + 0x47f0) + SV_I((unsigned)&hst_KeyNext) * 4); id < 5; id++, q = (unsigned short *)((char *)q + 4)) {
                if (**(char **)q == 0)
                    break;
            }
            id = ((unsigned short)util_RandomU(id - 1) - 5) + SV_I((unsigned)&hst_KeyNext);
        }
    default:
        break;
    }
    if (g == 0) {
        if (o != 0)
            sv_Unk52c4e0(ch_FindByObject(o, id, 0), 0, 0);
    } else {
        if (id != -1) {
            sv_Unk52c4e0(*(int *)(g + 0x184), id, 0);
            return;
        }
        if (text[0] != 0) {
            sv_Unk52c4e0(*(int *)(g + 0x184), 0, text);
            return;
        }
    }
}
