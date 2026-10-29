/* 0x4226a0, allocate and zero the max-entry MEMENTRY table, reset the counters and the main group */

void mem_Init(int max)
{
    int size;

    mem_Max = max;
    size = max * sizeof(MEMENTRY);
    mem_Bytes = 0;
    mem_MaxBytes = 0;
    mem_Blocks = 0;
    mem_MaxBlocks = 0;
    mem_Table = malloc(size);
    D3_ZERO_AT(mem_Table, size);
    mem_Main.blocks = 0;
    mem_Main.bytes = 0;
    mem_Main.maxblocks = 0;
    mem_Main.maxbytes = 0;
    mem_Main.calls = 0;
    mem_Main.next = NULL;
    strcpy(mem_Main.name, "_main_:");
}
