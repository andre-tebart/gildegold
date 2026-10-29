/* 0x4d0a80, a callback of the object walk: the dummies "dummy_sp_ESCAPE" get a flag object "ub_FLUCHTFLAGGE" (named
   "sp_ESCAPE", the animation "anim_FLUCHTFLAGGE.baf"), "dummy_CONQUER" one "ub_CONQUER" (named "sp_CONQUER_<n>"), both
   kept in ai_Unk935980[ai_Unk6f0d58++]; returns 1. Rough draft. */

char ai_Unk4d0a80(char *o)
{
    char *r;
    float pos[4];
    float rot[3];

    if (strstr(o, "dummy_sp_ESCAPE") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(o, o + 0x4c, pos);
        r = (char *)d3_CreateObject(0, pos, rot, "ub_FLUCHTFLAGGE");
        ai_Unk935980[ai_Unk6f0d58] = r;
        strcpy(r, "sp_ESCAPE");
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x220) = 4;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x224) = 1;
        ai_Unk935980[ai_Unk6f0d58][0x212] = (ai_Unk935980[ai_Unk6f0d58][0x212] & 0xf7) | 4;
        ai_Unk935980[ai_Unk6f0d58][0x212] |= 0x40;
        d3t_PlayAnim(ai_Unk935980[ai_Unk6f0d58], "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
        ai_Unk6f0d58++;
        return 1;
    }
    if (strstr(o, "dummy_CONQUER") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(o, o + 0x4c, pos);
        r = (char *)d3_CreateObject(0, pos, rot, "ub_CONQUER");
        ai_Unk935980[ai_Unk6f0d58] = r;
        d3if_SelectTextureSet(r, *(int *)(r + 0x1cc), 1, 1);
        sprintf(ai_Unk935980[ai_Unk6f0d58], "sp_CONQUER_%i", ai_Unk6f0d58);
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x200) = 0;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x220) = 4;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x224) = 1;
        ai_Unk935980[ai_Unk6f0d58][0x212] = (ai_Unk935980[ai_Unk6f0d58][0x212] & 0xf7) | 4;
        ai_Unk6f0d58++;
    }
    return 1;
}
