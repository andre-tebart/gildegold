
int txt_GroupCount = 0;  /* 0x6333bc, text groups parsed by the last txt_OpenText, never read */
char * txt_Buffer = 0;  /* 0x6333c0, mem_Alloc'd text pool */
int txt_Size = 0;  /* 0x6333c4, bytes used in txt_Buffer */
char txt_BinPath[256] = {0};  /* 0xfe7c40 */
char txt_Dir[256] = {0};  /* 0xfe7d40 */
char txt_Keys[0x4800][0x50] = {0};  /* 0xfe7e40 */
char txt_GroupIds[0x4800][0x50] = {0};  /* 0x1161e80, the "_..." identifier of each group */
char txt_HeaderName[0x120] = {0};  /* 0x12c9e80, "#headerfile" */
char txt_Names[0x4800][0x50] = {0};  /* 0x12c9fa0, indexed by group - 1 */
TXTFILE txt_Files[0x80] = {0};  /* 0x1431fc0 */
unsigned char txt_GroupLevel[0x4800] = {0};  /* 0x14359c0, label column of each group */
int txt_GroupFirst[0x4800] = {0};  /* 0x144c1e0, first text of each group */
