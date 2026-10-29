/* 0x4aa430, check callback of the visit actions (0x4aa120): as 0x4a9fa0; prints the action name into a debug text
   that nobody reads */

int ai_Unk4aa430(AIPER *p, unsigned char *in, unsigned char *out)
{
    char name[64];
    int ctx[0x88];
    char text[256];
    char *s;
    AIJOB *j;

    if (in[0] == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && *(int *)(in + 8) == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->unkac == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->unkb0);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = *(int *)(in + 8);
        switch (*(int *)(in + 4)) {
        case 0:
            s = "prva_WERBUNG";
            break;
        default:
            return 0;
        case 3:
            s = "prva_ANKLAGE_ERHEBEN";
            break;
        case 4:
            s = "prva_ERPRESSUNG";
            break;
        case 7:
            s = "prva_FURCHT_EINFLOESSEN";
            break;
        case 8:
            s = "prva_BELEIDIGUNG";
            break;
        }
        sprintf(name, s);
        ctx[0x85] = *(int *)(out + 4);
        sprintf(text, "NPC %s <%i> w\xe4hlt %s", (char *)p + 0x30, p->id, name);
        return (ord_DispatchOrder(*(int *)(in + 4), p, ctx) & 0x60) == 0;
    }
    return 0;
}
