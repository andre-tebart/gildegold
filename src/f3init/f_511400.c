/* 0x511400, the setup of a new game for the player slot n: seeds the random generator once (timeGetTime; the
   only place that seeds it, so every new game starts the same), init_StartNewGame (0x50e0d0), with the cheat
   flag 0x6f2430 the start house of the first kind 6 character (0x534040), hands out the opponent slots for the
   players the city supports beyond the first (init_CreateOpponents with a table of eight job kinds
   0x18 0x27 0x33 0x3f 0x45 0x4b 0x12 0x06, without the kinds the persons already have; with the negative
   ai_GameFlags only the kind 0x12), init_OpenTown (0x50f910), and gives the first building of the class 1 the
   480000 start money. Returns 1
   @deep behaviour: 153 code hunks, meaning equal - the whole 0x511400..0x5115b0 was read and the diff is 97 +/- lines, every one a register or stack slot. The structural difference is the frame pointer: the original is 'push ebp / mov ebp,esp / sub esp,0x24' with ebp-relative locals (the tab array at ebp-0x24, the parameter at ebp+8), ours is esp-relative with no ebp at all, so every slot reference and the epilogue differ; no statement of the original accounts for the frame pointer (no alloca, no setjmp, the frame is 0x24), a frame job. Readability done this session: Q2 ai_Unk1460ae0 -> net_Conn.s (net.h's NETCONN socket, -1 = no network game; hst.c's view), ai_Unk7e3f88 -> ai_PlayerSlots[n][0x48] (the city's MaxPlayer, 0x513a20 writes it), srand -> srand (the seed store 0x5de62a -> 0x62e108 the LCG rand reads), ai_Unk50f910 -> init_OpenTown, gm_FirstBuilding/gm_FirstBuilding -> gm_FirstBuilding (project-wide, 28 refs), main_StartHouseCheat -> main_StartHouseCheat (its only reader in the exe is this gate, no writer), Q3 0x7d5f82 -> INIT_CHARACTERS_END + 2, Q4 through INIT_BLD.objList (0x5d) / INIT_OBJ.objId (0x02), Q7 cm_SendMoveGoodsUnchecked / gm_FirstBuilding / init_OpenTown prototypes, Q6 rewritten. Remaining Q2: main_Unk534040 (0x534040, main.c's undrafted range: it makes the "Home-Sweet" house and hands it to a character) - the name main_GiveStartHouse is evidenced but the link cannot place a renamed function that has no definition ('1 functions not placed: unresolved _main_GiveStartHouse'), so the rename is blocked until 0x534040 is drafted in main.c. */


int init_SetupNewGame(int n)
{
    unsigned long t;
    char *p;
    int i, d;
    unsigned tab[8];
    unsigned *q;
    INIT_OBJ *o;
    INIT_BLD *b;
    int *pi;

    if (net_Conn.s == -1) {
        t = timeGetTime();
        srand(t);
    }
    init_StartNewGame();
    if (main_StartHouseCheat != 0) {
        i = 0;
        p = ai_Chars[0] + 2;
        do {
            if (*p == 6) {
                main_Unk534040(i);
                break;
            }
            p += 0x21c;
            i++;
        } while ((int)p < INIT_CHARACTERS_END + 2);
    }
    if (0 < (int)ai_PlayerSlotsV[n][0x48] - (unsigned)ai_NumPlayers) {
        tab[0] = 0x18;
        tab[1] = 0x27;
        tab[2] = 0x33;
        tab[3] = 0x3f;
        tab[4] = 0x45;
        tab[5] = 0x4b;
        tab[6] = 0x12;
        tab[7] = 6;
        if ((char)ai_GameFlags < 0) {
            for (i = 0; i < 8; i++)
                tab[i] = 0;
            tab[0] = 0x12;
        } else {
            p = ai_Chars[0] + 2;
            do {
                if (*p == 6 || *p == 7) {
                    i = 0;
                    do {
                        if ((unsigned char)p[0x162] == tab[i]) {
                            tab[i] = 0;
                            break;
                        }
                        i++;
                    } while (i < 8);
                }
                p += 0x21c;
            } while ((int)p < INIT_CHARACTERS_END + 2);
        }
        init_CreateOpponents((int)ai_PlayerSlotsV[n][0x48] - ai_NumPlayers, 8, tab);
    }
    init_OpenTown();
    b = (INIT_BLD *)gm_FirstBuilding(1);
    if (b != 0) {
        o = (INIT_OBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
        if (o != 0)
            cm_SendMoveGoodsUnchecked(o->objId, -1, ai_OwnPlayer, 480000);
    }
    return 1;
}
