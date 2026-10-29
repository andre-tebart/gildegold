/* 0x4eb4c0, reads the player and recipe prototypes from "alchemy_data.ini" (next to the exe): [MAIN_PLAYER_SPECS] NUM_OF_PLAYER_PROTOTYPES, then [PLR_%li] ATTR0 .. ATTR3
   (0 .. 252, bytes 0 .. 3), ATTR4 (18 .. 30, byte 4), ATTR5 (an object name, 0x4ebf50, int +8) into the table 0x6f0e2c (12 bytes each); [MAIN_RECIPES_SPECS]
   NUM_OF_RECIPES_PROTOTYPES, then [RCP_%li] ATTR0 / ATTR1 (percent, 0 .. 1 floats +4 / +8) and RCPID (an object name, int +0) into 0x6f0e34; 1 when all was read,
   else the tables are freed and 0. The clamps: ahm_ClampAttr keeps 1 .. 0xfb and caps at 0xfc, ATTR4 is 0x12 .. 0x1e; ahm_ClampShare is min(max(atof / 100, 0), 1)
   in double, re-evaluating atof (0x4eb8d7 .. is four atof calls per check).
   @deep B3 draft: two hunks left, both the cleanup (fail) block's place: ours sits at the end of the function, the original's right after the
   recipe-alloc check / before the recipe loop (0x4eb83b, with its ATTR4 tail at 0x4eb883), so the ATTR4 0x12 tail and the two dup-tails differ
   in position only; the block itself is the same 18 instructions. Leads tried: the ATTR4 clamp as <= 0x12 / >= 0x1e and as the swapped
   if/else (0x4eb75e, gained, kept), the goto fail vs fallthrough shapes (source order puts the label last), the union views (AHMGRID
   plr/rcp/w) that removed the frame temp and made the stores direct. Q1..Q7 pass. */

/* unlinked; the mask zeroes 0 and below, the cap is 0xfc (the original's cmp eax,0xfc / jl) */
static __inline int ahm_ClampAttr(char *s)
{
    int v;

    v = atoi(s);
    v = v & ((v <= 0) - 1);
    if (v >= 0xfc)
        v = 0xfc;
    return v;
}

/* unlinked; the original evaluates atof(s) again at every use (the min/max are macros) and does it in double (the constants are QWORDs,
   only the store to the float table field narrows) */
#define ahm_min(a, b) (((a) < (b)) ? (a) : (b))
#define ahm_max(a, b) (((a) > (b)) ? (a) : (b))
static __inline double ahm_ClampShare(char *s)
{
    return ahm_min(ahm_max(atof(s) * 0.01, 0.0), 1.0);
}

int ahm_LoadIni(void)
{
    char ini[260], buf[256], sec[256], *p;
    int i;

    GetModuleFileNameA((HMODULE)main_Instance, ini, 0x104);
    p = strrchr(ini, '\\');
    p[1] = 0;
    strcat(ini, "alchemy_data.ini");
    GetPrivateProfileStringA("MAIN_PLAYER_SPECS", "NUM_OF_PLAYER_PROTOTYPES", "", buf, 0x100, ini);
    ai_PlayerPrototypeCount = atoi(buf);
    if (ai_PlayerPrototypeCount > 0) {
        if (ai_PlayerPrototypes != 0) {
            mem_Free(ai_PlayerPrototypes);
            ai_PlayerPrototypes = 0;
        }
        ai_PlayerPrototypes = mem_Alloc(ai_PlayerPrototypeCount * 12, "ahm_AllocPlayerPrototypes");
        if (ai_PlayerPrototypes == 0)
            goto fail;
        for (i = 0; i < ai_PlayerPrototypeCount; i++) {
            sprintf(sec, "PLR_%li", i);
            GetPrivateProfileStringA(sec, "ATTR0", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[0] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR1", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[1] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR2", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[2] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR3", "0", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.attr[3] = ahm_ClampAttr(buf);
            GetPrivateProfileStringA(sec, "ATTR4", "20", buf, 0x100, ini);
            p = (char *)(long)atoi(buf);
            if ((int)p > 0x12) {
                if ((int)p >= 0x1e)
                    p = (char *)0x1e;
            } else
                p = (char *)0x12;
            ai_PlayerPrototypes[i].plr.attr[4] = (unsigned char)(int)p;
            GetPrivateProfileStringA(sec, "ATTR5", "ob_NULL", buf, 0x100, ini);
            ai_PlayerPrototypes[i].plr.id = ahm_ObjectId(buf);
        }
        GetPrivateProfileStringA("MAIN_RECIPES_SPECS", "NUM_OF_RECIPES_PROTOTYPES", "", buf, 0x100, ini);
        ai_RecipePrototypeCount = atoi(buf);
        if (ai_RecipePrototypeCount > 0) {
            if (ai_RecipePrototypes != 0) {
                mem_Free(ai_RecipePrototypes);
                ai_RecipePrototypes = 0;
            }
            ai_RecipePrototypes = mem_Alloc(ai_RecipePrototypeCount * 12, "ahm_AllocRecipePrototypes");
            if (ai_RecipePrototypes != 0) {
                for (i = 0; i < ai_RecipePrototypeCount; i++) {
                    sprintf(sec, "RCP_%li", i);
                    GetPrivateProfileStringA(sec, "ATTR0", "25", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.a = ahm_ClampShare(buf);
                    GetPrivateProfileStringA(sec, "ATTR1", "50", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.b = ahm_ClampShare(buf);
                    GetPrivateProfileStringA(sec, "RCPID", "ob_NULL", buf, 0x100, ini);
                    ai_RecipePrototypes[i].rcp.id = ahm_ObjectId(buf);
                }
                return 1;
            }
        }
    }
    if (ai_PlayerPrototypes != 0) {
        mem_Free(ai_PlayerPrototypes);
        ai_PlayerPrototypes = 0;
    }
fail:
    if (ai_RecipePrototypes != 0) {
        mem_Free(ai_RecipePrototypes);
        ai_RecipePrototypes = 0;
    }
    ai_RecipePrototypeCount = 0;
    ai_PlayerPrototypeCount = 0;
    return 0;
}
