/* 0x50d550, the scene object callback (a function start Ghidra missed) that allocates the building for a "gb_NAME" scene object o (0x50d100 resolved the name to the building type, bits 0x2000000 in +0x48): a new instance of the type without an owner (0x501580), for the KONTOR (type 0x47) the office of the player slot whose name is the part after the last '_' of the object name (the slot number goes to +0x65, its name to +5); the object and the building are linked (+0x61 / +0x200) and "f3_init:Gebaeude des Typs %s allokiert" is the (unused) log text. An object without a state of its own takes its parent's record. @deep draft, 90 code hunks / 114 vs 106 instructions, every one read against the original 0x50d550..0x50d6a0: no missing / extra statement, constant or callee. What is left is allocation / scheduling: the original hoists the id (and eax,0xffffff before the tests + movsx of the 0x4b byte, ours compares the byte directly), swaps the loop induction registers (original i=esi / slot pointer=edi, ours the reverse), spills id at [esp+0xc] vs [esp+0x14] and places the inlined strcpy byte loops differently. spell.py: 50d5d3 reload - tried using the masked id expression directly at the sprintf use (90 code hunks unchanged, reverted); 50d6a8 dup-tail - tried the parent-record tail as if (par == 0) return (unchanged, reverted); 50d57d / 50d65c have no menu rule and the meaning is equal. Readability: Q1 init_AllocSceneBuilding, Q2 cleared (gm_CreateBuilding / ai_PlayerSlots / ai_BuildingNames named), Q3 cleared (INIT_PROTO_MASK / INIT_PLAYER_SLOTS_END), Q6 done. Q4 (11 raw accesses) is the same shared-header blocker as init_HookObject and ls.c 0x51d4f0: o's D3OBJECT has no field at 0x200 (d3.h char pad200[0x8]) and the building instance b is an AIBLD from ai.h, which this unit cannot include (gv.h declares ai_Blds / ai_Chars with other types, 30 compile errors). */


void init_AllocSceneBuilding(int o)
{
    char buf[0x100];
    char tail[0x80];
    unsigned id;
    unsigned char *b;
    int i;
    char *u;
    int par;

    if (init_SceneNameToPrototype((char *)o) != 0) {
        if (*(int *)(o + 0x48) != -1 && *(char *)(o + 0x4b) == 2 && *(int *)(o + 0x218) != 0) {
            id = *(unsigned *)(o + 0x48) & INIT_PROTO_MASK;
            if (id == 0x47) {
                b = (unsigned char *)gm_CreateBuilding(0x47, 0xffff);
                u = strrchr((char *)o, '_') + 1;
                strcpy(tail, u);
                for (i = 1; (int)ai_PlayerSlotsV[i] < INIT_PLAYER_SLOTS_END; i++) {
                    if (ai_PlayerSlotsV[i][0] == 0)
                        break;
                    if (_stricmp(tail, ai_PlayerSlotsV[i]) == 0) {
                        *(int *)(b + 0x65) = i;
                        strcpy((char *)b + 5, ai_PlayerSlotsV[i]);
                        break;
                    }
                }
            } else
                b = (unsigned char *)gm_CreateBuilding(id, 0xffff);
            if (b != 0) {
                sprintf(buf, "f3_init:Geb\xe4ude des Typs %s allokiert", (char *)ai_BuildingNames[id * 0xe - 2]);
                *(int *)(b + 0x61) = o;
                *(int *)(o + 0x200) = (int)b;
            }
        }
        if (*(int *)(o + 0x218) == 0) {
            par = *(int *)(o + 0x1f8);
            if (par != 0)
                *(int *)(o + 0x200) = *(int *)(par + 0x200);
        }
    }
}
