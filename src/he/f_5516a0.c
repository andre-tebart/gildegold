/* 0x5516a0, queues a random animation on a person (+4 the figure; random 0x42ec00(4) & 0xffff, jump table 0x551764): 0 = 0x626fd0, 1 = 0x626fbc then 0x626fd0, 2 = 0x626a4c then 0x626fd0, 3 = 0x6268c8, 0x626fbc, 0x626fd0 (all with 0x4c5410 (id, name, 1)) */

void ai_Unk4c5410(int a, const char *s, unsigned char c);

void he_Unk5516a0(int p)
{
    switch ((unsigned short)util_Random(4)) {
    case 0:
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x17b0), 1);
        break;
    case 1:
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x179c), 1);
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x17b0), 1);
        break;
    case 2:
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x122c), 1);
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x17b0), 1);
        break;
    case 3:
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x10a8), 1);
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x179c), 1);
        ai_Unk4c5410(*(int *)(p + 4), (char *)((unsigned)&ai_Unk625820 + 0x17b0), 1);
        break;
    }
}
