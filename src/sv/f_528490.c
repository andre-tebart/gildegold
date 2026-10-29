/* 0x528490, the chimney smoke of the building o (not kind 7): when one of its characters (type 4 at +0x180, home +0x16c = o) is at work (no group or a group without +0xc8) and the time of day is inside the window of
   0x617830 / 0x617840, an effect "effekte\\Schornstein_dunkel_esc" (0x4287b0) is started at the "dummy_RAUCH_0" dummy of the model (0x429ab0) and its handle stored at +0x95; otherwise a running effect is stopped
   (0x429e90) and the handle cleared. ROUGH */

void sv_Unk528490(unsigned char *o)
{
    int k, i, scene, d, e;
    char *c;
    float pos[3];

    k = gm_ClockSeason(ai_Time);
    scene = d3_CurrentScene;
    if (ai_BTypes[*o * 0x24d] != 7 && *(int *)(o + 0x61) != 0) {
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (c[8] != 0 && *(short *)c != -1 && c[2] != 10 && *(unsigned char **)(c + 0x16c) == o && c[0x180] == 4) {
                if (*(int *)(c + 0x17c) == 0 || *(int *)(*(int *)(c + 0x17c) + 200) == 0) {
                    if (*(int *)(o + 0x95) != -1)
                        return;
                    if ((float)(*(unsigned short *)(ai_Time + 4)) < ai_Unk617830[k])
                        return;
                    if (ai_Unk617840[k] <= (float)(*(unsigned short *)(ai_Time + 4)))
                        return;
                    d3if_SelectScene(0, 1);
                    d = d3_FindObject(*(int *)(o + 0x61), 0x100, 0, "dummy_RAUCH_0");
                    if (d != 0 && (e = (int)evt_Load("effekte\\Schornstein_dunkel_esc")) != 0) {
                        d3_PointToWorld(d, d + 0x4c, pos);
                        if (evt_RunScriptArgs((EVTSCRIPT *)e, 3, (int)pos[0], (int)pos[1], (int)pos[2]) != 0)
                            *(int *)(o + 0x95) = *(int *)(e + 0x100);
                        *(int *)(e + 0x104) = 0;
                    }
                    d3if_SelectScene(scene, 1);
                    return;
                }
                break;
            }
        }
        if (*(int *)(o + 0x95) != -1) {
            e = (int)evt_FindById(*(int *)(o + 0x95));
            if (e != 0)
                evt_Stop((EVTSCRIPT *)e);
            o[0x95] = 0xff;
            o[0x96] = 0xff;
            o[0x97] = 0xff;
            o[0x98] = 0xff;
        }
    }
}
