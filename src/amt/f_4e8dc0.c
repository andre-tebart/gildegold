/* 0x4e8dc0, fills the office of the kind `kind` (0x1e .. 0x21, the mayors of the four towns) from the characters on the screen objects of the matching kind with
   the ranks of the town: the best by 0x50a640 takes it. When the town's object (0x17 .. 0x1a) does not exist the town is built: a building (0x38 / 0x3b / 0x3e / 0x41) is
   picked from the building records, a free place is looked for ("amt_calc_zuenfte(): could not find a free place...") and the order message (0xd) is sent
   (0x4c4b70). The name is the function's own error string at 0x60f11c.
   @deep B3 far draft, 919/1200 bytes (our function is 32 B longer than the original 1168), 209 differing instructions of 351 (ours 356). This session fixed the frame: the original's AMTREC a (0x1c), pos (0x1c..0x27, overlapping a - a is dead on the return path) and v3 (0x34) give 0x368; our pos[6] and the function-scope pos/v3 gave 0x400 - int pos[3] + pos/v3 declared inside the build branch (+ nm[128] removed, the gb_ name built in buf) now give the same 0x368 frame and the first 14 instructions match. Also int lo/hi (the original stores both as dwords at [esp+0x10]/[esp+0x18]; as unsigned char they were byte stores) - 251 -> 209 differing instructions, but our code is 32 B longer (unitdiff MIXED) because the block-local declarations changed the allocation. Left: allocation only - hi enregistered in ebx vs the original's [esp+0x18], cand[16] at 0x138 vs 0x40, n/edi vs ebx (the whole kind selection and memset block). Q4 blocked header: the 26 raw accesses are the message build (type 0xd) - AIMSG in ai.h has type/to/fromId/time/flags/arg at the same offsets but NOT pos at 0x80 (inside arg[10]) or angles at 0x90 or name at 0x9c, and am.c's ai_Unk4a5320 (amt_ExecGebaeudeBauen) uses the same raw casts, so the view belongs in ai.h + am.c (shared-header decision). Q2/Q7 blocked: d3if_SelectScene = d3if_SelectScene, d3_CurrentScene = d3_CurrentScene, err_Message = err_Message are one-line renames (prototypes in am.h/d3.h), but d3_PointToWorld = d3_PointToWorld needs the canonical (D3OBJECT *, D3VECTOR *, D3VECTOR *) with the call site passing an int object pointer and (char *)j + 0x4c - a type debt recorded in notes/units/amt.md; ai_BuildingName / ai_FindBauplaetze are now real prototypes in amt.h (Q7 for those two gone). Q1/Q5 done this session: amt_CalcZuenfte (the function's own error string at 0x60f11c) and ROUGH removed. */

void amt_CalcZuenfte(unsigned int kind)
{
    AMTREC a;
    char *cand[16];
    char *it, *cur, *pick, *cp, *q;
    int n, total, i, j, best, s, bld, cnt, scene, k1, k2;
    unsigned char k;
    int lo, hi;
    unsigned short id;
    char buf[128], name[128];
    int list[64];
    char msg[0xf8];

    memset(&a, 0, sizeof(a));
    a.owner = -1;
    if (kind == 0x1e) {
        k1 = 0xe; k2 = 0x17; lo = 0x22; hi = 0x27; bld = 0x38;
    } else if (kind == 0x1f) {
        k1 = 0x12; k2 = 0x18; lo = 0x2e; hi = 0x33; bld = 0x3b;
    } else if (kind == 0x20) {
        k1 = 0x14; k2 = 0x19; lo = 0x3a; hi = 0x3f; bld = 0x3e;
    } else {
        if (kind != 0x21)
            return;
        k1 = 0x15; k2 = 0x1a; lo = 0x40; hi = 0x45; bld = 0x41;
    }
    memset(cand, 0, sizeof(cand));
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, k1);
    if (it != 0) {
        do {
            if (n > 15)
                break;
            id = *(unsigned short *)(it + 0x27);
            if (id != 0xffff && (it[0x5a] & 1) == 0) {
                total++;
                k = ai_Chars[id][0x164];
                if (lo <= k && k <= hi) {
                    for (i = 0; i < 16; i++) {
                        if (cand[i] == ai_Chars[id])
                            break;
                    }
                    if (i == 16) {
                        cand[n] = ai_Chars[id];
                        n++;
                    }
                }
            }
            it = gm_BuildNext();
        } while (it != 0);
        if (total > 2 && n != 0) {
            it = gm_BuildQuery(1, 5, k2);
            scene = d3_CurrentScene;
            if (it == 0) {
                int pos[3], v3[3];

                d3if_SelectScene(0, 1);
                q = ai_BTypes + bld * 0x24d;
                ai_BuildingName(bld, buf);
                cnt = ai_FindBauplaetze(buf, list);
                j = list[util_RandomU(cnt) & 0xffff];
                i = 0;
                while (j == 0) {
                    if (cnt <= i) {
                        err_Message("amt_calc_zuenfte(): could not find a free place...");
                        return;
                    }
                    if (list[i] != 0)
                        j = list[i];
                    i++;
                }
                d3_PointToWorld(j, j + 0x4c, pos);
                ai_ObjectAngle(v3, j);
                d3if_SelectScene(scene, 1);
                strcpy(buf, "gb_");
                strcat(buf, q + 1);
                _strupr(buf + 2);
                sprintf(name, "%sgebaeude/*%s.ogr", ai_DataPath, buf);
                memset(msg, 0, sizeof(msg));
                msg[4] = 0xd;
                *(int *)(msg + 0x28) = *(int *)ai_Time;
                *(int *)(msg + 0x8) = *(int *)(*(char **)&ai_PlayerChar + 4);
                *(int *)(msg + 0xc) = -1;
                *(int *)(msg + 0x2c) = *(int *)&ai_TimeHour;
                *(int *)(msg + 0x30) = *(int *)(ai_Time + 8);
                *(short *)(msg + 0x34) = *(short *)(ai_Time + 12);
                msg[0x36] = 2;
                *(int *)(msg + 0x80) = pos[0];
                *(int *)(msg + 0x58) = *(int *)(*(char **)&ai_PlayerChar + 4);
                *(int *)(msg + 0x88) = pos[2];
                *(int *)(msg + 0x84) = pos[1];
                *(int *)(msg + 0x90) = v3[0];
                *(int *)(msg + 0x5c) = bld;
                *(int *)(msg + 0x60) = 0;
                *(int *)(msg + 0x94) = v3[1];
                *(int *)(msg + 0x98) = v3[2];
                strcpy(msg + 0x9c, name);
                cm_SendLongMessage(msg);
                return;
            }
            if ((it[0x5a] & 1) == 0) {
                i = 36;
                j = 1;
                do {
                    if (j == 0)
                        break;
                    if (amt_Table[i].kind == kind) {
                        j--;
                        a = amt_Table[i];
                    }
                    i--;
                } while (i >= 0);
                best = 0;
                cur = 0;
                if (a.owner != -1 && (cur = gm_FindChar(a.owner)) != 0)
                    best = gm_GetWorth(*(unsigned short *)cur);
                pick = 0;
                if (n > 0) {
                    for (j = 0; j < n; j++) {
                        cp = cand[j];
                        s = gm_GetWorth(*(unsigned short *)cp);
                        if (best < s) {
                            best = s;
                            pick = cp;
                        }
                    }
                    if (pick != 0 && pick != cur) {
                        if (cur != 0) {
                            amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                            amt_SendOfficeLost((unsigned short *)cur, kind);
                            amt_SendOfficeNews((unsigned short *)pick, kind);
                            return;
                        }
                        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                        amt_SendOfficeNews((unsigned short *)pick, kind);
                    }
                }
            }
        }
    }
}
