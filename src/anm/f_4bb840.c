/* 0x4bb840, frees the animal list */

void anm_Free(void)
{
    if (anm_Animals != 0) {
        mem_Free(anm_Animals);
        anm_Animals = 0;
    }
}
