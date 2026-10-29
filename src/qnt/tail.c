
unsigned short qnt_G[256] = {0};  /* 0x631228 */
unsigned char qnt_PalR[256] = {0};  /* 0x631428 */
unsigned char qnt_PalG[256] = {0};  /* 0x631528 */
unsigned char qnt_PalB[256] = {0};  /* 0x631628 */
int qnt_Colors = 0;  /* 0x631728 */
int qnt_Sq[256] = {0};  /* 0x631b34, i * i for i = -255..255 (indexed around 0) */
unsigned short qnt_B[256] = {0};  /* 0x631f38 */
int * qnt_SqPtr = 0;  /* 0x632138 */
unsigned short qnt_R[256] = {0};  /* 0x632140, r bits spread to octree index positions */
QNODE * qnt_Level[6] = {0};  /* 0x632340 */
unsigned int qnt_HeapCount = 0;  /* 0x632358 */
int qnt_Inited = 0;  /* 0x63235c */
QHEAP * qnt_Heap = 0;  /* 0x632360, 1-based min-heap on total */
void * qnt_Unk632364 = 0;  /* 0x632364 */
