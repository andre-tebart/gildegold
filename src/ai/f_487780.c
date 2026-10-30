/* 0x487780, adds the equipment the building's characters need (their object list at 0x178, queried for the
   prototype kind 0xc, matched against ai_WeaponList / ai_WeaponList2) to the slot table with the mark
   `make` and the flag 2, then reduces the marks until make * price fits the building's limit (0x39)
   @deep behaviour draft, far: 168 code / 52 order / 18 regs / 5 fill hunks (143 differing instructions of 311, 700/912 bytes). Read whole against tools/disasm.py 0x487780 0x390, the Ghidra export and the VC6 listing. Meaning-class leads done this session, one edit one build each: (1) the production-budget float compare was the ai.md lesson form - ours test ah,0x41 / jnp, the original and eax,0x4100 / jne = !(x > limit); rewritten to 'if (!((float)make * price + (float)sum > (float)limit)) break;' in both weapon loops - FIXED, 147 -> 143 differing instructions, code 176 -> 168; (2) the two stores of the original (make = u at 0x487949 then make = u & mask at 0x487951) - the read-back spelling 'ai_Slots[i].make = ai_Slots[i].make & (((int)ai_Slots[i].make < 0) - 1)' after 'ai_Slots[i].make = u' byte-identical (VC6 folds the first store, keeps one), and its add-then-sub association (add ecx,ebp / sub eax,ecx) stays sub, sub in ours; (3) 'int u' instead of 'unsigned int u', byte-identical; (4) the char-loop base: the original strength-reduces to &ai_Chars[i].bld and keeps that pointer in the FRAME ([esp+0x10], reloaded at each use, ours keeps it in esi and spills once) and re-tests found (test edx,edx) before each slot search - pointer form / allocation, not a missing statement (every statement, constant, callee and field offset read). Readability done: Q4 raw access *(int *)((char *)&ai_Chars[i] + 0x178) replaced by the typed AICHAR.inventory (renamed project-wide from unk178: the character's goods inventory list, next at +0x3f; am.h AIPX mirror, am.c 8 refs, unk_4b6480.c 1), Q2/Q6 clean. Left: register allocation of the char pointer, the store scheduling (order 52) and the slot choice = match-deep vargen / slotmap */

void ai_AddEquipmentNeeds(AICHAR *ch, AIOBJ *obj, int limit)
{
    unsigned short *it;
    int *p;
    int i, j, k, n, sum, id, found;
    unsigned int u;

    n = ai_NSlots;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == ch->bld && ai_CharsT[i].title != 0) {
            it = (unsigned short *)gm_Query(ai_CharsT[i].inventory, 1, 4, 0xc);
            n = ai_NSlots;
            if (it == 0) {
                for (j = 0; j < n; j++) {
                    if (ai_Slots[j].item == ai_WeaponList[0]) {
                        ai_Slots[j].make++;
                        ai_Slots[j].flags |= 2;
                        break;
                    }
                }
            } else {
                found = 0;
                p = &ai_WeaponList[1];
                do {
                    if (p[-1] == 0 || (id = *p) == 0) {
                        if (!found) {
                            p = &ai_WeaponList2[1];
                            do {
                                if (p[-1] == 0 || (id = *p) == 0)
                                    break;
                                u = *it;
                                p[-1] = u;
                                if (u != 0 && !found) {
                                    j = 0;
                                    do {
                                        if (n <= j)
                                            break;
                                        if (ai_Slots[j].item == id) {
                                            ai_Slots[j].make++;
                                            ai_Slots[j].flags |= 2;
                                            found = 1;
                                        }
                                        j++;
                                    } while (!found);
                                }
                                p++;
                            } while (!found);
                        }
                        break;
                    }
                    u = *it;
                    p[-1] = u;
                    if (u != 0 && !found) {
                        j = 0;
                        do {
                            if (n <= j)
                                break;
                            if (ai_Slots[j].item == id) {
                                ai_Slots[j].make++;
                                ai_Slots[j].flags |= 2;
                                found = 1;
                            }
                            j++;
                        } while (!found);
                    }
                    p++;
                } while (!found);
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (ai_Slots[i].make != 0) {
            found = 0;
            p = ai_WeaponList;
            do {
                if (*p == 0) {
                    if (!found) {
                        p = ai_WeaponList2;
                        do {
                            if (*p == 0)
                                break;
                            if (ai_Slots[i].item == *p) {
                                u = ai_Slots[i].make - (ai_Slots[i].pending + ai_Slots[i].amount);
                                ai_Slots[i].make = u;
                                ai_Slots[i].make = u & (((int)u < 0) - 1);
                                found = 1;
                            }
                            p++;
                        } while (!found);
                    }
                    break;
                }
                if (ai_Slots[i].item == *p) {
                    u = ai_Slots[i].make - (ai_Slots[i].pending + ai_Slots[i].amount);
                    ai_Slots[i].make = u;
                    ai_Slots[i].make = u & (((int)u < 0) - 1);
                    found = 1;
                }
                p++;
            } while (!found);
            if (ai_Slots[i].make != 0) {
                gm_CollectOffers(ai_Slots[i].item);
                for (k = 0; k < gm_NOffers; k++) {
                    if (gm_Offers[k].goodsObj != 0 && gm_Offers[k].count > 0)
                        ai_Slots[i].bld = gm_Offers[k].bld;
                }
                n = ai_NSlots;
            }
            if (ai_Slots[i].bld == 0)
                ai_Slots[i].make = 0;
        }
    }
    sum = 0;
    for (p = ai_WeaponList; *p != 0; p++) {
        id = *p;
        for (j = 0; j < ai_NSlots; j++) {
            if (id == ai_Slots[j].item && ai_Slots[j].make != 0) {
                if (ai_Slots[j].make > 0) {
                    do {
                        if (!((float)ai_Slots[j].make * ai_Slots[j].price + (float)sum > (float)limit))
                            break;
                    } while (--ai_Slots[j].make > 0);
                }
                sum = (int)(ai_Slots[j].make * ai_Slots[j].price + sum);
            }
        }
    }
    for (p = ai_WeaponList2; *p != 0; p++) {
        id = *p;
        for (j = 0; j < ai_NSlots; j++) {
            if (id == ai_Slots[j].item && ai_Slots[j].make != 0) {
                if (ai_Slots[j].make > 0) {
                    do {
                        if (!((float)ai_Slots[j].make * ai_Slots[j].price + (float)sum > (float)limit))
                            break;
                    } while (--ai_Slots[j].make > 0);
                }
                sum = (int)(ai_Slots[j].make * ai_Slots[j].price + sum);
            }
        }
    }
}
