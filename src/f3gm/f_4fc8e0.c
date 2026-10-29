/* 0x4fc8e0, health now / health of the character c without the extra (0 for a free slot or an inactive one) */

float unk_Unk4fc8e0(short *c)
{
    if (*c == -1)
        return 0.0f;
    if (((char *)c)[8] == 0)
        return 0.0f;
    return (float)(gm_CharacterHealth(c) / unk_Unk4fc890(c));
}
