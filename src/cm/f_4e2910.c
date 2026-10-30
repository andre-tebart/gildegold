/* 0x4e2910 */

/* 0x4e2910, leaves the scene of the object o (+0x29 the id, +0x5d its owner / place, +0 the building kind) for the place id: draws the
   frozen screen (0x6ed924 / 0x6ed92c, the window flag +0x26 & 0x20 of 0x6f0d90) and fades to black ("BLACK", 0x5a ticks) when o is
   the current object (0x6f2308 / 0x6f230c) with that id; else selects the scene of the place (0x76faa0[id], or of the building kind
   0x76fa60[kind]) and runs 0x513050; at the end the stop flag of 0x63349c / 0x6f0d98 is cleared (0x43deb0). Not in the replay mode. ROUGH */
#define G27_FRAME(a, b) d2_BlitPart(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, (a), 0, 0, (b))
void ai_Unk4e2910(char *o, int id)
{
    char *bld;
    int k;

    if (ai_Unk6f0d8c != 0)
        return;
    if (((char *)ai_PlayerBuildingT == o && *(unsigned short *)(o + 0x29) == id) || ((char *)ai_FallbackBuildingT == o && *(unsigned short *)(o + 0x29) == id)) {
        if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
            d2_ClearSurface(d2_ScreenSurf);
            G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
            sv_RunFrame(0, 0x86);
            G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
        }
        if (ai_Unk6f0da0 != 0) {
            d2_ReleaseFade(ai_Unk6f0da0);
            ai_Unk6f0da0 = 0;
            d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
        }
        return;
    }
    if (gm_Query(*(int *)(o + 0x5d), 1, 0, id) != 0) {
        if (gm_IsHomeType(o) == 0) {
            if (init_ObjectUniverse[id] != (char)0xfe) {
                if (init_ObjectUniverse[id] == (char)0xff || *(char *)(ai_Unkfd7c50 + 0x325 + init_ObjectUniverse[id] * 0x3e0) != 0)
                    init_LoadObjectScene(id);
                d3if_SelectScene(init_ObjectUniverse[id], 0);
                init_ShowBuildingScene(o, id);
            }
        } else {
            bld = (char *)ai_BTypes + *(unsigned char *)o * 0x24d;
            k = *(unsigned char *)bld;
            if (init_BuildingUniverse[k] != 0) {
                if (init_BuildingUniverse[k] == (char)0xff || *(char *)(ai_Unkfd7c50 + 0x325 + init_BuildingUniverse[k] * 0x3e0) != 0)
                    init_LoadBuildingScene(k);
                k = *(unsigned char *)((char *)ai_BTypes + *(unsigned char *)o * 0x24d);
                d3if_SelectScene(init_BuildingUniverse[k], 0);
                init_ShowBuildingScene(o, 0xfd);
            }
        }
    }
    if (ai_Unk63349c != 0) {
        ai_Unk6f0d98 = ai_Unk63349c;
        d3_InflateScene(ai_Unk63349c & ~2);
        ai_Unk6f0d98 = ai_Unk6f0d98 & ~2;
    }
    if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
        sv_RunFrame(0, 0x86);
        G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
    }
    if (ai_Unk6f0da0 != 0) {
        d2_ReleaseFade(ai_Unk6f0da0);
        ai_Unk6f0da0 = 0;
        d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
    }
}
