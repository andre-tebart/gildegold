/* 0x56b780, shows the construction state of the building scene object "he_%i" (+4 the id) for the building event ev: with +0xc8 = -1 every child (listed by 0x438d40 with the callback 0x56b750) becomes the type 5 and
   is switched off (0x447300), below the count +0xc4 each child is switched on / off by the numbers in its name ("..._n1_n2": one number = the stage from which it shows, two = the range), above it the
   building (+0xb8, its object +0x61) is shown and the scene object is deleted (0x447190); the scene is restored at the end (ROUGH) */

char d3if_ShowTree(struct D3OBJECT *obj, char show);

void he_ShowConstruction(int ev)
{
    char name[0x100];
    char nm[0x100];
    char dig[4];
    int list[0x42];
    int scene, obj, w, i, k, n, o;
    int nums[2];
    char *p;

    scene = U_I((unsigned)&d3_CurrentScene);
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", *(int *)(ev + 4));
    obj = d3_FindObject(0, 0x1c0, 0, name);
    w = *(int *)(ev + 0xc8);
    if (w == -1 && obj != 0) {
        list[0x41] = 0;
        list[0] = obj;
        d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), obj, 0x2c0, (unsigned)&he_Unk56b750, list);
        for (i = 0; i < list[0x41]; i++) {
            o = list[i + 1];
            if (*(int *)(o + 0x220) != 5 || (*(unsigned char *)(o + 0x213) & 4)) {
                *(int *)(o + 0x220) = 5;
                o = list[i + 1];
                if ((*(unsigned char *)(o + 0x213) & 4) && U_I((unsigned)&d3_QuadTree) != 0)
                    d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
                *(unsigned char *)(list[i + 1] + 0x213) &= ~4;
            }
        }
        d3if_ShowTree(obj, 0);
    } else if (w < *(int *)(ev + 0xc4)) {
        if (obj != 0) {
            list[0x41] = 0;
            list[0] = obj;
            d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), obj, 0x2c0, (unsigned)&he_Unk56b750, list);
            for (i = 0; i < list[0x41]; i++) {
                o = list[i + 1];
                if (*(int *)(o + 0x220) != 5 || (*(unsigned char *)(o + 0x213) & 4)) {
                    if ((*(unsigned char *)(o + 0x213) & 4) && U_I((unsigned)&d3_QuadTree) != 0)
                        d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
                    *(int *)(list[i + 1] + 0x220) = 5;
                    *(unsigned char *)(list[i + 1] + 0x213) &= ~4;
                }
                *(int *)(list[i + 1] + 0x224) = 0;
                strcpy(nm, (char *)list[i + 1]);
                n = 0;
                for (p = strchr(nm, '_'); p != 0 && n < 2; p = strchr(nm, '_')) {
                    strncpy(dig, p + 1, 2);
                    dig[2] = 0;
                    if (isdigit(dig[0])) {
                        nums[n] = atoi(dig);
                        n++;
                    }
                    *p = 0;
                }
                if (n == 1)
                    d3if_ShowTree(list[i + 1], w >= nums[0] ? 1 : 0);
                else if (n == 2) {
                    if (w <= nums[0] && w >= nums[1])
                        d3if_ShowTree(list[i + 1], 1);
                    else
                        d3if_ShowTree(list[i + 1], 0);
                }
            }
        }
    } else if (w > *(int *)(ev + 0xc4)) {
        o = gm_FindBuildingById(*(int *)(ev + 0xb8));
        if (o != 0 && *(int *)(o + 0x61) != 0)
            d3if_ShowTree(*(int *)(o + 0x61), 1);
        if (obj != 0) {
            if (U_I((unsigned)&d3_QuadTree) != 0)
                d3q_RemoveObject((D3OBJECT *)obj, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
            d3if_Delete((struct D3OBJECT *)obj);
        }
    }
    d3if_SelectScene(scene, 1);
}
