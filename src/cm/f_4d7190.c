/* 0x4d7190, the flag of the escape dummy: the object o at the place of the fighter slot f (+0x10) gets the character of
   the slot as owner (+0x200): the old flag (+0x204) is deleted and a new "ub_FLUCHTFLAGGE" object with the animation
   (and the texture set of the character's side) is put at the dummy; the character's flag bit 4 and text 0x11b8 for the
   slot's character. Rough draft. */

void ai_Unk4d7190(char *f)
{
    char *c = 0;
    char *o, *r, *side;
    char buf[0x100];
    float pos[4];
    float rot[3];
    char k;

    if (*(int *)(f + 4) != 0) {
        o = (char *)d3_FindObject(0, 0x40, 0, f + 0x10);
        if (*(int *)f != -1) {
            c = ai_Unk4ce130(*(int *)f);
            (*(char **)(c + 0x184))[0x8d] |= 4;
            txt_Format(buf, 0x11b8, G20_US(c));
            sv_SetStatusText(buf);
        }
        if (*(char **)(o + 0x200) != c) {
            if (*(int *)(o + 0x204) != 0) {
                d3if_Delete((struct D3OBJECT *)*(int *)(o + 0x204));
                *(int *)(o + 0x204) = 0;
            }
            if (c != 0) {
                rot[0] = 0;
                rot[1] = 0;
                rot[2] = 0;
                d3_PointToWorld(o, o + 0x4c, pos);
                r = (char *)d3_CreateObject(0, pos, rot, "ub_FLUCHTFLAGGE");
                ai_Unk935980[ai_Unk6f0d58] = (char *)d3_CreateObject(0, pos, rot, &ch_EmptyName);
                d3t_PlayAnim(r, "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
                side = *(char **)(c + 0x16c);
                if (side != 0 && *(int *)(side + 0x54) != 0 && (side[2] == 5 || side[2] == 6 || side[2] == 7)) {
                    k = side[0x54] + 0x67;
                    if (k != 0)
                        d3if_SelectTextureSet(r, *(int *)(r + 0x1cc), k, 1);
                }
                sprintf(r, "CONQUER_FLAG");
                *(int *)(r + 0x200) = 0;
                r[0x212] = (r[0x212] & 0xf7) | 0x44;
                *(int *)(r + 0x220) = 4;
                *(int *)(r + 0x224) = 1;
                *(char **)(o + 0x204) = r;
            } else
                *(int *)(o + 0x204) = 0;
        }
        *(char **)(o + 0x200) = c;
    }
}
