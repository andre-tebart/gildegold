/* 0x5128d0, switches the seasonal textures of the town scene (d3_Terrain, the names of 0x40 bytes at +0x1a64) to
   the current season (gm_ClockSeason): when no name carries the season's suffix yet, each is cut at its old suffix
   ("_FRUEHLING" / "_HERBST" / "_SNOW") and gets the season's suffix when that texture exists (tex_FindFile), then
   the tile is set again (d3fl_ReplaceTexture). The summer (1) only cuts the suffixes; the four seasons are four
   near copies in the original.
   @deep rewrote the four seasons as the original's four near copies (was one shared mark/fmt loop) and use d3_Terrain at every mention (the original reloads 0x633738 before each call, not a local); tex_FindFile/d3fl_ReplaceTexture are the canonical names of 0x45f9f0 / 0x442210, gm_ClockSeason prototyped: 511 -> 82 differing instructions. Tried for the scan loops: while+break (82), while+return (rotated), for(i<0x200&&!found) (found test at the top), while(i<0x200&&!found) with the increment in the body (82): VC6 rotates our while to a bottom i test, the original keeps cmp esi,0x200/jge at the top with the found test at the bottom and an inline ret (512900..512945). Remaining: that loop shape x4 plus the [eax+esi+0x1a64] vs [esi+eax+0x1a64] operand order in the update loops */


void init_SetSeasonTextures(void)
{
    int season, i, j, found;
    char *nm, *u;
    char a[0x80], b[0x80];

    season = gm_ClockSeason(ai_Time);
    if (d3_Terrain == 0)
        return;
    if (season == 3) {
        found = 0;
        i = 0;
        while (i < 0x200) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_SNOW") != 0)
                found = 1;
            i += 0x40;
            if (found)
                break;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_SNOW", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 2) {
        found = 0;
        i = 0;
        while (i < 0x200) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_HERBST") != 0)
                found = 1;
            i += 0x40;
            if (found)
                break;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_HERBST", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 0) {
        found = 0;
        i = 0;
        while (i < 0x200) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0 && strstr(nm, "_FRUEHLING") != 0)
                found = 1;
            i += 0x40;
            if (found)
                break;
        }
        if (found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                sprintf(b, "%s_FRUEHLING", a);
                if (tex_FindFile(b) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, b, j);
                else
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
        return;
    }
    if (season == 1) {
        found = 0;
        i = 0;
        for (i = 0; i < 0x200 && !found; i += 0x40) {
            if (*((char *)d3_Terrain + 0x1a64 + i) != 0
                && (strstr((char *)d3_Terrain + 0x1a64 + i, "_FRUEHLING") != 0
                    || strstr((char *)d3_Terrain + 0x1a64 + i, "_HERBST") != 0
                    || strstr((char *)d3_Terrain + 0x1a64 + i, "_SNOW") != 0))
                found = 1;
        }
        if (!found)
            return;
        j = 0;
        for (i = 0; i < 0x200; i += 0x40, j++) {
            nm = (char *)d3_Terrain + 0x1a64 + i;
            if (*nm != 0) {
                strcpy(a, nm);
                if ((u = strstr(a, "_FRUEHLING")) != 0 || (u = strstr(a, "_HERBST")) != 0 || (u = strstr(a, "_SNOW")) != 0)
                    *u = 0;
                if (tex_FindFile(a) != 0)
                    d3fl_ReplaceTexture((struct D3FLMAP *)d3_Terrain, a, j);
            }
        }
    }
}
