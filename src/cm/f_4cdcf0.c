/* 0x4cdcf0, a factor by the record kind of the character (0x154 .. 0x176) */

float ai_Unk4cdcf0(char *c)
{
    float f = 0.5f;
    float g;
    int *p;

    g = gm_GetSkill(c, 3);
    p = (int *)ai_Unk4cda50(c);
    if (p != 0) {
        switch (*p) {
        case 0x154:
            return unk_Unk4fc8e0(c) * g * 0.5f;
        case 0x156:
        case 0x16e:
        case 0x172:
            return unk_Unk4fc8e0(c) * g * 0.8f;
        case 0x158:
            return unk_Unk4fc8e0(c) * g * 1.0f;
        case 0x15e:
        case 0x160:
            return unk_Unk4fc8e0(c) * g * 0.7f;
        case 0x174:
        case 0x176:
            return unk_Unk4fc8e0(c) * g * 0.9f;
        default:
            f = 0.3f;
        }
    }
    return unk_Unk4fc8e0(c) * g * f;
}
