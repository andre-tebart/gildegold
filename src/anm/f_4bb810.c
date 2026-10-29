/* 0x4bb810, allocates the animal list ("anm:animals") and resets the animal count and the first record */
void anm_Alloc(void)
{
    anm_Animals = mem_Alloc(0x2480, "anm:animals");
    anm_NAnimals = 0;
    anm_Animals[0].obj = 0;
}
