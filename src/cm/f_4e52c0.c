/* 0x4e52c0, the seed (logged "cut_getrandseed: %i") */

unsigned int ai_Unk4e52c0(void)
{
    char buf[0x40];

    sprintf(buf, "cut_getrandseed: %i", ai_Unk92c4e0);
    return ai_Unk92c4e0;
}
