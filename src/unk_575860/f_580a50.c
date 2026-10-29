/* 0x580a50, resets the three 0x38-byte shot records of 0x6f9f60 (57 B): each record: +0 = -1 and its three
   0x10-byte entries: +8 = -1, +0xc = 100000.0, +0x10 = 0, +0x14 = 0 */

typedef struct SHOTENTRY {   /* 0x10 bytes */
    int obj;                 /* +0x0, -1 when empty */
    float dist;              /* +0x4 */
    int a;                   /* +0x8 */
    int b;                   /* +0xc */
} SHOTENTRY;

typedef struct SHOTREC {     /* 0x38 bytes, three of them at 0x6f9f60 (full_data_6f9f60) */
    int id;                  /* +0x0, -1 when empty */
    int unk4;                /* +0x4 */
    SHOTENTRY e[3];          /* +0x8 */
} SHOTREC;

void ai_Unk580a50(void)
{
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        ((SHOTREC *)&full_data_6f9f60)[i].id = -1;
        for (j = 0; j < 3; j++) {
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].obj = -1;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].dist = 100000.0f;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].a = 0;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].b = 0;
        }
    }
}
