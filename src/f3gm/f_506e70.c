/* 0x506e70, the inheritance of the dead character c (the heirs are the characters of the group (1, 3, id) i.e. buildings it owns: those of the kinds in the mask 0xe07d43b0 / the kind 0x20 go to the best claimant by 0x509b10, the others of kind 2 to everyone): messages 0x4c54a0 (hand over), the message 0x1b to the new owner and 0x31 to the tenants, the money (0x509eb0 of the dead goes to the player 0x6f1bf8), the objects of its list (+0x178) by 0x4c5010; when flag is set the character is reset to a new person (the fields +8 .. +0x1b0 are sent through 0x4c3c00 / 0x4c3d90: age 0x10, 0x100 = 100 .., health floats), and the text of the announcement is built into out from the formats 0x1c4f (name, money), "%1G %A" per heir and 0x1c50 / 0xb6 (the rest). ROUGH */

void gm_Unk506e70(unsigned short *c, char *out, int flag)
{
    unsigned char *own;
    unsigned char *best, *second;
    unsigned char *heirs[0x80];
    int n, bestv, v, i, k;
    unsigned char *b;
    unsigned mask, mask2;
    unsigned char kind;
    int *q;
    int money;
    char buf[2048];
    int msg[0x3e];
    int tmp[0x10];

    own = *(unsigned char **)(c + 0xb8);
    n = 0;
    best = 0;
    bestv = 0;
    b = (unsigned char *)gm_BuildQuery(1, 3, *c);
    while (b != 0) {
        mask = 0;
        mask2 = 0;
        kind = ai_BTypes[*b * 0x24d];
        if (kind < 0x20)
            mask = 1 << (kind & 0x1f);
        else
            mask2 = 1 << ((kind - 0x20) & 0x1f);
        if ((mask & 0xe07d43b0) == 0 && (mask2 & 1) == 0) {
            if (b != own && mask == 4) {
            take:
                heirs[n] = b;
                n++;
                second = b;
                b = best;
                goto next;
            }
        } else {
            v = gm_EquipmentValue(b, (int)(char)b[0x5c]);
            if (v <= bestv)
                goto take;
            second = 0;
            bestv = v;
            if (best != 0) {
                heirs[n] = best;
                n++;
                second = best;
            }
        next:
            best = b;
            if (second != 0) {
                cm_SendOwnership(*(int *)(second + 1), *(int *)(ai_PlayerCharC + 4), *(int *)(ai_PlayerCharC + 4));
                memset(msg, 0, sizeof(msg));
                ((char *)msg)[4] = 0x1b;
                msg[2] = *(int *)(ai_PlayerCharC + 4);
                msg[3] = -1;
                ((char *)msg)[0x36] = 2;
                msg[0x58 / 4] = *(int *)(second + 1);
                msg[0x5c / 4] = gm_EquipmentValue(second, (int)(char)second[0x5c]);
                cm_SendLongMessage(msg);
                q = (int *)(ai_Chars[0] + 4);
                do {
                    if (q[0x5a] == (int)second && *q != *(int *)(c + 2)) {
                        memset(msg, 0, sizeof(msg));
                        ((char *)msg)[4] = 0x31;
                        msg[2] = *(int *)(ai_PlayerCharC + 4);
                        msg[3] = *(int *)(second + 1);
                        msg[0x28 / 4] = *(int *)ai_Time;
                        msg[0x30 / 4] = *(int *)(ai_Time + 8);
                        msg[0x2c / 4] = *(int *)(ai_Time + 4);
                        ((char *)msg)[0x36] = 1;
                        *(short *)((char *)msg + 0x34) = *(short *)(ai_Time + 0xc);
                        msg[0x58 / 4] = *q;
                        cm_SendLongMessage(msg);
                    }
                    q += 0x87;
                } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x2));
            }
        }
        b = gm_BuildNext();
    }
    money = gm_Unk509eb0(c);
    cm_SendMoveGoods(*(int *)(ai_PlayerCharC + 4), -1, ai_OwnPlayer, money);
    k = (int)gm_Query(*(int *)(c + 0xbc), 1, 5);
    while (k != 0) {
        ai_Unk4c5010(*(int *)(c + 2), -1, *(int *)(k + 2));
        k = (int)gm_QueryNext();
    }
    cm_SendMoveGoods(*(int *)(c + 2), -1, ai_OwnPlayer, gm_PlayerFactor(0x5dc, ai_OwnPlayer));
    if (flag != 0) {
        char z2, z3;
        int age, st;

        z2 = 100;
        age = 0x10;
        st = 3;
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldDelta(1, 1, (int)c + (8 - cm_FieldObject), &z2);
        cm_AddFieldDelta(2, 1, (int)c + (10 - cm_FieldObject), &age);
        cm_AddFieldDelta(4, 1, (int)c + (0x194 - cm_FieldObject), &st);
        z3 = 0;
        cm_AddFieldDelta(1, 1, (int)c + (0x1b0 - cm_FieldObject), &z3);
        cm_AddFieldDelta(1, 1, (int)c + (0x1b1 - cm_FieldObject), &z3);
        cm_SendSetField(*(int *)(c + 2), 0x1c8, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
        cm_SendFieldDeltas();
        ai_Unk4c4ac0(*(int *)(c + 2), 0x7c, -*(float *)(c + 0x3e));
        ai_Unk4c4ac0(*(int *)(c + 2), 0x14, (float)(util_RandomSq() * 7.0 + 30.0 - *(float *)(c + 10)));
        ai_Unk4c4ac0(*(int *)(c + 2), 0x1c, 1000.0f - *(float *)(c + 0xe));
        ai_Unk4c4ac0(*(int *)(c + 2), 0x18, 1000.0f - *(float *)(c + 0xc));
        ai_Unk4c4ac0(*(int *)(c + 2), 0x20, 12.0f - *(float *)(c + 0x10));
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        for (i = 0; i < 5; i++) {
            z3 = 0;
            cm_AddFieldValue(1, 1, (i - cm_FieldObject) + 0x80 + (int)c, &z3);
        }
        cm_SendFieldValues();
    }
    txt_Format(out, 0x1c4f, *c, gm_PlayerFactor(0x5dc, ai_OwnPlayer));
    if (best != 0) {
        txt_Format(buf, "%1G$A", best);   /* 0x61ac98: the pooled format literal */
        strcat(out, buf);
    }
    if (own != 0) {
        txt_Format(buf, "%1G$A", own);
        strcat(out, buf);
    }
    if (n != 0) {
        txt_Format(buf, 0x1c50);
        strcat(out, buf);
        i = 0;
        if (0 < n) {
            while (i < 6) {
                txt_Format(buf, "%1G$A", heirs[i]);
                strcat(out, buf);
                i++;
                if (n <= i)
                    return;
            }
            if (i < n) {
                txt_Format(buf, 0xb6, n - i);
                strcat(out, buf);
            }
        }
    }
}
