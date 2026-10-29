/* 0x5a8540, the setting lookup by the good (31 B): the record of 0x5a84b0, its slot table +0xc, else 0 */

int ls_Read(void *buf, int size, int count, int f); void * mem_Alloc(int size, char *tag);
unsigned vfs_fwrite();

/* The production settings (the "hp_" module with the map window: the literals "hp_AddProdSetting" 0x62c284 and
   "hp_LoadProdSetting" 0x62c298 climb right after the map's "Misc\Landkarte2" 0x62c274): the linked list
   0x6f26c0 of 0x4c-byte records {the good +0, the seed +4, the rotation index byte +8, seven {good, weight, lock}
   slots at +0xc (stride 8), the factor +0x44 (1.0f), next +0x48}; 0x5a83b0 / 0x5a84d0 fill the weights. */

int *ai_Unk5a8540(int param_1)
{
    int *p;

    p = (int *)*(int *)((unsigned)&full_data_6f26c0);
    while (p != 0) {
        if (*p == param_1)
            return p + 3;
        p = (int *)p[0x12];
    }
    return 0;
}
