/* 0x504100, the two small numbers a / b of a job number (the job tables of the characters): 2 -> 1, 0; 4 -> 1, 2; 5 -> 1, 0; 6 -> 0, 1; 7 -> 4, 1; 8 -> 0, 3; 9 -> 0, 4; 10 -> 3, 2; 11 -> 2, 3; 12 -> 3, 2; 13 -> 2, 0; others 1, 0. Returns 1 */


unsigned short * ai_Unk6f1bf4 = 0;  /* 0x6f1bf4, the current record */
int ai_Unk6f2318 = 0;  /* 0x6f2318 */
unsigned char * gm_FirstBuilding(int n);                   /* 0x50b010 */
short * gm_PlayerRecord(char *c);                     /* 0x506430 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */


int gm_JobSlots(int job, int *a, int *b)
{
    switch (job) {
    default:
        *a = 1;
        *b = 0;
        return 1;
    case 2:
        *a = 1;
        *b = 0;
        return 1;
    case 4:
        *a = 1;
        *b = 2;
        return 1;
    case 5:
        *a = 1;
        *b = 0;
        return 1;
    case 6:
        *a = 0;
        *b = 1;
        return 1;
    case 7:
        *a = 4;
        *b = 1;
        return 1;
    case 8:
        *a = 0;
        *b = 3;
        return 1;
    case 9:
        *a = 0;
        *b = 4;
        return 1;
    case 0xa:
        *a = 3;
        *b = 2;
        return 1;
    case 0xb:
        *a = 2;
        *b = 3;
        return 1;
    case 0xc:
        *a = 3;
        *b = 2;
        return 1;
    case 0xd:
        *a = 2;
        *b = 0;
        return 1;
    }
}
