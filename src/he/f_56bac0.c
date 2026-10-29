/* 0x56bac0, the highest number in the "_nn" suffixes of the children of the building scene object "he_%i" (id +4), i.e. the construction stage reached: the children are listed by 0x438d40 with the callback 0x56b750, their names parsed from the right ("..._n1_n2" = the stage range) */

int he_Unk56bac0(int ev)
{
    char name[0x100];
    char nm[0x100];
    char dig[4];
    int list[0x42];
    int scene, obj, idx, cnt, o, best;
    int nums[2];
    char *p;

    scene = U_I((unsigned)&d3_CurrentScene);
    best = 0;
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", *(int *)(ev + 4));
    obj = d3_FindObject(0, 0x1c0, 0, name);
    list[0] = obj;
    list[0x41] = 0;
    d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), obj, 0x2c0, (unsigned)&he_Unk56b750, list);
    if (list[0x41] > 1) {
        for (idx = 1; idx < list[0x41]; idx++) {
            o = list[idx + 1];
            *(int *)(o + 0x224) = 0;
            strcpy(nm, (char *)o);
            cnt = 0;
            for (p = strrchr(nm, '_'); p != 0 && cnt < 2; p = strrchr(nm, '_')) {
                strncpy(dig, p + 1, 2);
                dig[2] = 0;
                if (isdigit(dig[0])) {
                    nums[cnt] = atoi(dig);
                    cnt++;
                }
                *p = 0;
            }
            if (cnt != 0 && best < nums[0])
                best = nums[0];
        }
    }
    d3if_SelectScene(scene, 1);
    return best;
}
