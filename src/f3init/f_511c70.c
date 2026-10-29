/* 0x511c70, gives the buildings of the classes 1 / 4 / 2 and of the types 0x13 / 4 / 0x10 / 5 / 9 (not 0xb) to the first character of the kind 6: 64000 in the building's money word (+0x39), the ownership (0x4c54a0), the staff (0x4c55d0) and the object 0x135 */


void unk_Unk511c70(void)
{
    char *c;
    int i;
    unsigned char *b;
    char k;
    int v;

    c = ai_Chars[0];
    i = 0;
    do {
        b = (unsigned char *)ai_Blds;
        if (c[2] == 6)
            break;
        c += 0x21c;
        i++;
    } while (i < 0x300);
    while (1) {
        if (*b != 0 &&
            ((gm_BuildingClass(*b) == 1 || gm_BuildingClass(*b) == 4 || gm_BuildingClass(*b) == 2 ||
              ((k = ai_BTypes[*b * 0x24d]) == 0x13 || k == 4)) || (k == 0x10 || (k == 5 || k == 9))) &&
            ai_BTypes[*b * 0x24d] != 0xb) {
            v = 64000;
            cm_BeginFieldBlock(b, *(int *)(b + 1));
            cm_AddFieldDelta(4, 1, (int)b + (0x39 - cm_FieldObject), &v);
            cm_SendFieldDeltas();
            cm_SendOwnership(*(int *)(b + 1), *(int *)(c + 4), *(int *)(c + 4));
            cm_SendMessage3d(b, 1, 0, (unsigned char)ai_BTypes[*b * 0x24d + 0x223]);
            cm_SendExSellObject(*(int *)(b + 1), -1, 0x135, 1, ai_OwnPlayer, 0);
        }
        b += 0xa9;
    }
}
