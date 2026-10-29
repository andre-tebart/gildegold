/* 0x4f8890, copies the kind record i (below 0x58) of ord_KindTable to out; 0 for a bad i */

/* the kind table 0x617468: one 8-byte record per kind (ORD_KIND reads the same pair as four shorts) */
typedef struct ORDKIND {
    int value;                      /* 0x0, the kind's value */
    int flags;                      /* 0x4, the kind's flags */
} ORDKIND;
#define ORD_KINDS ((ORDKIND *)ord_KindTable)

int ord_GetKindRecord(int i, int *out)
{
    if (i >= 0x58)
        return 0;
    out[0] = ORD_KINDS[i].value;
    out[1] = ORD_KINDS[i].flags;
    return 1;
}
