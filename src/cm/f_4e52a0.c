/* 0x4e52a0, sets the seed (logged "cut_randseed: %i") */

void ai_Unk4e52a0(unsigned int seed)
{
    char buf[0x40];

    ai_Unk92c4e0 = seed;
    sprintf(buf, "cut_randseed: %i", seed);
}
