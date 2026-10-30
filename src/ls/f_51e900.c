/* 0x51e900, reads the scene objects and the character scene records of a save game: the two lists of object records (0x51e750; first the ones that point to characters (+0x12c via 0x505df0), then those that point to buildings (0x4ffe90)), then for each scene object (the list 0x1468ae0, 0x300): its universe (0x4478e0), its name "%s_%s" / "holzfaeller_%s" ("hozfaeller" is a misspelling in the data), the model "*%s.bgf" (found with 0x42fca0, else the name of the object's character 0x4fb390 and the name is cut at the last underscore), the placement (0x403070, the owner ids, +0x1a8 / +0x1e8 texts), the object's owner: a character record (0x50d380; "sp_%i"; the karren objects "ob_KARREN_klein / mittel / GROSS" for the kinds 0x134 / 0x135 / 0x136 are added with their colour) or a building (0x43b860 and "sp_%i", the building's owner link +0x1d0 / +0x180); finally the table 0x631050 (the records of 0x194 bytes, the count 0x300 below 0x10044 else 2000 or 0x300: the fields written by 0x51aff0) and its link fixing (invalid links are reported "ls_LoadCharacter(): invalid next or prev-action pointer for action-type:%i" / "invalid stockptr for action-type:%i (character per...)" and the record is cleared). Returns 1. ROUGH @deep every hunk read against the original 0x51e900..0x51f07f; meaning verified equal: the LS_R field order/offsets (flag byte +9, 0xc, 0x14, 0x30, 0x50, 0xf0, 0x190, 0x24, 0x28; 0x194 memset before them), the version bounds (0x10044 -> 0x300, else >= 0x10058 -> 2000), the object walk 0x1468ae0..0x14696e0, the owner split (< 0x770b80 or >= 0x7d5f80 = character, else building), the two messages (dummy = invalid stockptr, file = invalid next/prev-action), the -1 link checks; one type fix made (col is a stack char in the original: byte store + dword reload, so int col -> char col, 650 -> 647 hunks). Left = shape only: frame 0x2a0 vs 0x2a4 (the original keeps col in the frame+0xc byte and pushes part/bld/buf 4 bytes up; VC6 keeps ours in al + movsx), the outer object loop (ours tests o < 0x14696e0 in a preheader, the original is a do-while entered at the stored constant), the -1 check duplicated in our else arm (the original shares one check at 0x51efe3), the queue pointer hoisted out of the read loop (the original reloads ls_ActionQueue per iteration) and the register allocation. Readability blocked beyond that: Q3 image literals (0x1468ae0 / 0x14696e0 / 0x1469700 / 0x770b80 / 0x7d5f7f), Q4 on the AICHAR/AIBLD records (ai.h still cannot be included, gv.h's char* views), Q2 on ~15 unk callees/globals (0x501190, 0x4ffe90, 0x511dc0/0x511e60, 0x4478e0, 0x42fca0, 0x4fb390, 0x403070, 0x406c60, 0x40b530, 0x43b860, 0x4cde20, 0x50d380, 0x446e10, 0x446f30, 0x4034e0, 0x509ff0, 0x402480, 0x404810, 0x505df0, 0x633444, 0x6333fc). */


int unk_Unk51e900(int f)
{
    int n, i, k, saved;
    int *o;
    char *rec, *name, *obj;
    char buf[128], file[260], dummy[260];
    char flag;
    int *t;
    char *own;

    n = 0;
    saved = d3_CurrentScene;
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        rec = (char *)ls_ReadCharacter(f);
        *(int *)(rec + 300) = (int)gm_FindChar(*(int *)(rec + 300));
    }
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        rec = (char *)ls_ReadCharacter(f);
        *(int *)(rec + 300) = gm_Query(0, 2, 7, 1, *(int *)(rec + 300));
    }
    for (o = ai_Unk1468ae0V; (int)o < (unsigned)((unsigned)ai_Unk1468ae0V + 0xc00); o++) {
        obj = (char *)*o;
        if (obj != 0) {
            char *bld = 0, *part = 0;
            int uni;

            bld = (char *)gm_BuildQuery(1, 1, *(int *)(obj + 0x2c));
            if (bld == 0) {
                uni = 0;
            } else {
                part = (char *)gm_Query(*(int *)(bld + 0x5d), 2, 6, 1, *(int *)(obj + 0x30));
                if (part == 0 || *(short *)part == 0xfd)
                    uni = init_BuildingSceneUniverse((unsigned char *)bld);
                else
                    uni = init_ObjectSceneUniverse((unsigned short *)part);
            }
            d3if_SelectScene(uni, 1);
            name = obj + 0x130;
            sprintf(buf, "%s_%s", obj + 0x170, name);
            if (strstr(buf, "hozfaeller") != 0)
                sprintf(buf, "holzfaeller_%s", name);
            sprintf(file, "*%s.bgf", buf);
            _strupr(file);
            if (vfs_GetPath(file, vfs_Root, vfs_Root, dummy) == 0) {
                char *c = *(char **)(obj + 300);
                char *s, *u;

                c[0x1f4] = 0;
                s = (char *)unk_Unk4fb390(*(unsigned short *)c) + 4;
                strcpy(obj + 0x170, s);
                u = strrchr(obj + 0x170, 0x5f);
                if (u != 0) {
                    strcpy(name, u + 1);
                    *u = 0;
                }
                sprintf(buf, "%s_%s", obj + 0x170, name);
            }
            ch_CreateMesh(obj, buf, obj + 0x38);
            if (bld != 0)
                *(int *)(obj + 0x2c) = *(int *)(bld + 1);
            if (part != 0)
                *(int *)(obj + 0x30) = *(int *)(part + 2);
            if (*(int *)(obj + 0x34) != 0) {
                if (obj[0x1a8] != 0)
                    ch_SetHandObject(obj, 2, obj + 0x1a8);
                if (obj[0x1e8] != 0) {
                    char *u = strrchr(obj + 0x1e8, 0x5f);

                    if (u != 0)
                        *u = 0;
                    ch_SetSitAnim(obj, obj + 0x1e8);
                }
                own = *(char **)(obj + 300);
                if (own != 0) {
                    if ((unsigned)own < (unsigned)&ai_Chars || (unsigned)own > ((unsigned)&ai_Unk770d8c + 0x651f3)) {
                        init_HookObject(*(int *)(obj + 0x34), own, 1);
                        sprintf(*(char **)(obj + 0x34), "sp_%i", *(int *)(own + 2));
                        *(char **)(own + 0x3b) = obj;
                        {
                            unsigned short kind = *(unsigned short *)own;
                            char *model = 0;
                            char *o2;

                            if (kind == 0x134)
                                model = "ob_KARREN_klein";
                            else if (kind == 0x135)
                                model = "ob_KARREN_mittel";
                            else if (kind == 0x136)
                                model = "ob_KARREN_GROSS";
                            if (model != 0) {
                                o2 = (char *)d3_CreateObject(0, obj + 0x38, obj + 0x48, model);
                                if (o2 != 0) {
                                    char *c2 = (char *)gm_Unk509ff0(own);
                                    char col;

                                    if (c2 != 0 && *(int *)(c2 + 0x54) != 0 && (c2[2] == 5 || c2[2] == 6 || c2[2] == 7))
                                        col = c2[0x54] + 'g';
                                    else
                                        col = 0;
                                    d3if_SelectTextureSet(o2, *(int *)(o2 + 0x1cc), col, 1);
                                    ch_AttachTransport(obj, o2);
                                    *(int *)(o2 + 0x224) = 1;
                                    init_HookObject(o2, own, 1);
                                }
                            }
                        }
                    } else {
                        d3_SetObjectRot(*(int *)(obj + 0x34), obj + 0x48);
                        sprintf(*(char **)(obj + 0x34), "sp_%i", *(int *)(own + 4));
                        *(char **)(*(int *)(obj + 0x34) + 0x200) = own;
                        *(unsigned *)(*(int *)(obj + 0x34) + 0x48) = *(unsigned short *)own | 0x3000000;
                        *(char **)(own + 0x184) = obj;
                        if (*(char **)(own + 0x17c) != 0)
                            own[0x180] = **(char **)(own + 0x17c);
                        ai_Unk4cde20(own);
                    }
                }
                if ((obj[0x8c] & 0x20) != 0)
                    ch_SetVisible(obj, 0);
            }
        }
    }
    d3if_SelectScene(saved, 1);
    i = 0;
    k = 0;
    *(int *)ls_ActionQueue = 0;
    if (ls_Version < 0x10044)
        n = 0x300;
    else
        n = ((ls_Version > 0x10057) - 1 & 0xfffffd30) + 2000;
    do {
        t = (int *)(ls_ActionQueue + k);
        memset(t, 0, 0x194);
        LS_R(&flag, 1)
        *((char *)t + 9) = flag;
        LS_R(t + 3, 4)
        LS_R(t + 5, 4)
        LS_R(t + 0xc, 0x20)
        LS_R(t + 0x14, 0x20)
        LS_R(t + 0x3c, 0xa0)
        LS_R(t + 100, 1)
        LS_R(t + 9, 4)
        LS_R(t + 10, 4)
        i++;
        k += 0x194;
    } while (i < n);
    for (k = 0; k < 0xc5440; k += 0x194) {
        t = (int *)(ls_ActionQueue + k);
        if (*((unsigned char *)t + 9) != 0) {
            int obj2 = ai_Unk1468ae0V[t[5]];

            t[5] = obj2;
            if (obj2 != 0) {
                t[9] = ls_ActionAt(t[9]);
                t[10] = ls_ActionAt(t[10]);
                t[0] = ai_Unk1469700[*((unsigned char *)t + 9) * 0x13];
                t[3] = 0;
                if (t[9] == 0) {
                    int ob = t[5];

                    if (ob == 0) {
                        sprintf(dummy, "ls_LoadCharacter(): invalid stockptr for action-type:%i (character per...)", *((unsigned char *)t + 9));
                        *((char *)t + 9) = 0;
                    } else {
                        if (*(int *)(ob + 0x128) != 0)
                            ch_ClearActions(ob);
                        *(int **)(t[5] + 0x128) = t;
                    }
                }
                if (*((char *)t + 9) == '3') {
                    int b2 = gm_BuildQuery(1, 1, t[0xd]);

                    if (b2 == 0) {
                        t[0xc] = 0;
                        t[0x5c] = 0;
                    } else {
                        short *pr = (short *)gm_Query(*(int *)(b2 + 0x5d), 2, 6, 1, t[0xe]);

                        if (pr == 0 || *pr == 0xfd)
                            t[0xc] = t[0x5c] = init_BuildingSceneUniverse((unsigned char *)b2);
                        else
                            t[0xc] = t[0x5c] = init_ObjectSceneUniverse((unsigned short *)pr);
                    }
                } else if (*((char *)t + 9) == '-') {
                    t[0x55] = -1;
                    t[0x3d] = 0;
                }
                if (t[9] == -1 || t[10] == -1)
                    goto bad;
                continue;
            }
        } else {
            if (t[9] == -1 || t[10] == -1)
                goto bad;
            continue;
        }
        memset(t, 0, 0x194);
        continue;
bad:
        sprintf(file, "ls_LoadCharacter(): invalid next or prev-action pointer for action-type:%i", *((unsigned char *)t + 9));
        t[9] = 0;
        t[10] = 0;
        for (i = 0; i < 0xc5440; i += 0x194) {
            if (*(int **)(i + 0x28 + ls_ActionQueue) == t)
                *(int *)(i + 0x28 + ls_ActionQueue) = 0;
            if (*(int **)(i + 0x24 + ls_ActionQueue) == t)
                *(int *)(i + 0x24 + ls_ActionQueue) = 0;
        }
        memset(t, 0, 0x194);
    }
    return 1;
}
