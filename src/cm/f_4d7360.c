/* 0x4d7360, the attack of the character c on the character g in the fight record rec: -1 when c cannot attack (no
   ammunition for the ranged kinds, the animation of c is busy, g not on the map); in range the hit is rolled
   (k+4 percent of c's skill against random 255) with the damage (k+5 .. k+6 percent of g's health, 0x5ba6b0 / 0x2b9
   scales it), and the action record (0x2c bytes, id of c, 2, id of g, the hit / damage) goes to message 0x4c5dc0;
   a player's attack also arms the AI of the other side (0x4cffd0). Out of range c walks to g (0x4c5bc0). Rough draft. */

int ai_Unk4d7360(char *rec, char *c, char *g)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int s[11];
    char *k, *x, *other, *r2;
    int i, n, hit, dmg, ix, iz, id;
    unsigned short r;
    float py;

    memset(s, 0, sizeof(s));
    s[0] = *(int *)(c + 4);
    s[1] = 2;
    s[4] = *(int *)(g + 4);
    if (c[0x1c4] == 1 || c[0x1c4] == 2 || c[0x1c4] == 3)
        ((char *)s)[0x1c] = 1;
    k = ai_Unk4cda50(c);
    if (*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) {
        x = ai_Unk4cdac0(c);
        if (x != 0 && *(int *)(x + 0x20) <= 0)
            return -1;
    }
    if (*(int *)k == 0x154 || *(int *)k == 0x156 || *(int *)k == 0x158 || *(int *)k == 0x16e || *(int *)k == 0x172 ||
        *(int *)k == 0 || *(int *)k == 0x176) {
        if (ai_Unk4ce1b0(c, *(char **)(*(char **)(g + 0x184) + 0x34)) > *(float *)(k + 0xc))
            goto walk;
    }
    if (*(int *)(*(char **)(c + 0x184) + 0x128) != 0)
        return -1;
    r = util_Random(0xff);
    x = *(char **)(c + 0x16c);
    if (r > 0xff - (int)((unsigned char)k[4] * 0.01 * (unsigned char)c[0x83])) {
        n = (unsigned char)k[6] - (unsigned char)k[5];
        hit = 1;
        dmg = (int)((double)(unsigned int)(ai_Unk4e52f0(n) + (unsigned char)k[5]) * *(float *)(g + 0x1c) * 0.01);
    } else {
        hit = 0;
        dmg = 0;
    }
    if (x != 0 && ai_InventoryHas(x, 0x2b9) != 0)
        dmg = (int)(dmg * 1.5f);
    ((char *)s)[0x1d] = 1;
    s[8] = hit;
    s[9] = dmg;
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && *(int *)(k + 0x58) == 0) {
        other = 0;
        n = (unsigned char)ai_Unk6f0d04[0x30];
        for (i = 0; i < n; i++) {
            if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1 && rec != ai_Unk92cae0[i]) {
                other = ai_Unk92cae0[i];
                break;
            }
        }
        r2 = (char *)ai_Unk4cdf00(other, g);
        if (*(int *)(r2 + 8) == -1 || cm_GetSlotState(*(int *)(r2 + 8)) == 0) {
            id = *(int *)(r2 + 4);
            *(int *)(r2 + 8) = -1;
            if (id == 0) {
                x = ai_Unk4cdac0(g);
                if (x != 0 && *(int *)(x + 0x20) > 0)
                    *(int *)(r2 + 8) = ai_Unk4cffd0(other, g, c, 0, 0);
            }
        }
    }
    cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
    ai_Unk4c5dc0(c, 2, s);
    return 0;
walk:
    if (*(int *)(*(char **)(c + 0x184) + 0x128) != 0 || ((char *)s)[0x1c] != 0)
        return -1;
    if (!d3_GetHeightAt(h, *(char **)(g + 0x34) + 0x4c, &ix, &iz, &py))
        return -1;
    if (ai_Unk4ce040(ix, iz, &ix, &iz) == 0)
        return -1;
    ai_Unk4c5bc0(*(char **)(*(char **)(c + 0x184) + 0x34), ix, iz, 0);
    return 0;
}
