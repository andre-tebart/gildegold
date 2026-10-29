
MEMGROUP mem_Main;  /* 0x1461400, "_main_:", head of the group list */
int mem_Max = 0;  /* 0x632384, entries in mem_Table */
int mem_Bytes = 0;  /* 0x632388 */
int mem_Blocks = 0;  /* 0x63238c */
int mem_MaxBytes = 0;  /* 0x632390 */
int mem_MaxBlocks = 0;  /* 0x632394 */
int mem_Count = 0;  /* 0x632398, allocations minus frees */
int mem_Verbose = 0;  /* 0x63239c, dump the groups at exit */
MEMENTRY * mem_Table = 0;  /* 0x6323a0 */
