/* 0x58f6a0, the placement-candidate filter (250 B, a d3_ForEachObject callback found through the pointer in
   0x58f7a0): obj passes when its name (or its name + 1) starts with the prefix (case-insensitive; "bk_" when the
   prefix is 0); it is dropped when a building of the build query (gm_BuildQuery (1, 6) .. gm_BuildNext) stands
   within 100.0 of it; otherwise it is appended to ai_BauplatzList (count ai_BauplatzCount); always returns 1
   @deep B: code hunk = the original inlines strlen(prefix) (mov edi,ebx; or ecx,-1; repnz scas al; not ecx; dec ecx; push ecx) where ours calls strlen. Tried #pragma intrinsic(strlen) before and after <string.h>; VC6 SP5+PP /O2 keeps the call. /Oi for the whole TU (480 functions) would inline it - a variant/flag round, not a source meaning error. The other hunks are regs and fill. */

struct D3OBJECT;
struct D3VECTOR;
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out); /* 0x458a20 (unk_458810.c) */
unsigned char *gm_BuildQuery(int n, ...);                  /* 0x501190 (f3gm.c) */
unsigned char *gm_BuildNext(void);                         /* 0x501030 (f3gm.c) */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); /* 0x453e80 (d3_vec.c) */

char ai_BauplatzFilter(int obj, char *prefix)
{
    AIBLD *b;
    int c;
    float pos[4];
    float q[4];

    if (prefix == 0) {
        if (_strnicmp((char *)obj, "bk_", 3) != 0 && _strnicmp((char *)(obj + 1), "bk_", 3) != 0)
            return 1;
    } else {
        c = strlen(prefix);
        if (_strnicmp((char *)obj, prefix, c) != 0 &&
            _strnicmp((char *)(obj + 1), prefix, c) != 0)
            return 1;
    }
    d3_PointToWorld((struct D3OBJECT *)obj, (struct D3VECTOR *)(obj + 0x4c), (struct D3VECTOR *)pos);
    for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
        c = b->object;
        if (c != 0) {
            d3_PointToWorld((struct D3OBJECT *)c, (struct D3VECTOR *)(c + 0x4c), (struct D3VECTOR *)q);
            if (d3_VecEqual((struct D3VECTOR *)q, (struct D3VECTOR *)pos, 100.0f) != 0)
                return 1;
        }
    }
    ai_BauplatzList[ai_BauplatzCount] = obj;
    ai_BauplatzCount++;
    return 1;
}
