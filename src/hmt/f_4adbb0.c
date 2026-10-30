/* 0x4adbb0, makes c the historical event (type 0x15, bit 0x1000) or person (bit 0x2000) hmt_Event / hmt_Person:
   new relations to everybody, default values, skills 210..251, age 9, the names of the record; tells the first
   human player and moves on to the next record
   @deep B: 17 code hunks left after the one meaning fix (the character scan's i was unsigned short; the original strength-reduces an int counter to a pointer walk with a signed cmp/jl and cmp cx,0x300/jb does not come from a 16-bit induction variable: now int i, 43 -> 17 hunks). The rest are spelling/order: the hmt_Find argument is a ?: here (mov eax,evt; cmp; je; mov eax,per) against the original's if/else jump shape (cmp; jne; mov ecx,evt; lea; push; push; jmp), lea edi,[ebp+0x5c] before the rep stos here vs after the or, xor ecx,ecx earlier; Q2 (amt_SetEventOwner/amt_SetPersonOwner/500970/505d30/5043d0, HMT_ macros, the 0x5176d0/0x53dda0 rename block) still open */

#define HMT_I(c, o) (*(int *)((char *)(c) + (o)))
#define HMT_F(c, o) (*(float *)((char *)(c) + (o)))
#define HMT_S(c, o) (*(short *)((char *)(c) + (o)))
/* the word at +0x14 of the 0x2f4-byte player records that start at 0x7e3f80 */
#define HMT_PLAYERW(i) (*(unsigned short *)((char *)&ai_Unk7e3f80 + 0x14 + (i) * 0x2f4))

void hmt_Create(int type, int unk, unsigned short v, AICHAR *c)
{
    char text[256];
    int kind;
    HMTREC *r;
    unsigned short pl;
    int i, j, k;

    if (type == 0x15) {
        c->flags1cc |= 0x1000;
        amt_SetEventOwner(c->netId);
    } else {
        c->flags1cc |= 0x2000;
        amt_SetPersonOwner(c->netId);
    }
    HMT_S(c, 0x0a) = v;
    HMT_B(c, 0x0c) = 0;
    for (j = 0; j < 768; j++) {
        for (k = 0; k < 768; k++) {
            ai_Relations[j][k] = (char)util_Random(0x40) - 0x20;
            ai_RelationMemory[j][k] = 0;
        }
    }
    HMT_S(c, 0x50) = 0x7fff;
    HMT_I(c, 0x7c) = 0;
    HMT_B(c, 0x58) = 0;
    memset((char *)c + 0x5c, 0xff, 0x20);
    HMT_I(c, 0x170) = 0;
    HMT_I(c, 0x16c) = 0;
    HMT_F(c, 0x1c) = 1000.0f;
    HMT_F(c, 0x14) = 1.0f;
    HMT_F(c, 0x20) = 12.0f;
    HMT_F(c, 0x10) = 330.0f;
    HMT_F(c, 0x18) = 1000.0f;
    for (j = 0; j < 5; j++)
        HMT_B(c, 0x80 + j) = (unsigned char)(util_Random(0x2a) + 210.0f);
    HMT_B(c, 0x0d) = 9;
    gm_AddObjekt(c->netId, HMT_PLAYERW(ai_OwnPlayer), &ai_Unkf42400);
    pl = 0xffff;
    for (i = 0; i < 768; i++) {
        if (pl != 0xffff)
            break;
        if (ai_Chars[i].kind == 6)
            pl = i;
    }
    r = hmt_Find(type == 0x15 ? hmt_Event : hmt_Person, &kind);
    strcpy(c->name, r->vname);
    strcpy(c->name + 0x10, r->nname);
    if (type == 0x15) {
        if (ai_TimeT.year > 0 && pl != 0xffff) {
            txt_Format(text, 0x42b3, c->id);
            he_ShowNews(ai_Chars[pl].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_01");
        }
        HMT_I(c, 0x18c) = hmt_RandomHeadIcon();
        hmt_NextEvent();
    } else {
        if (ai_TimeT.year > 0 && pl != 0xffff) {
            txt_Format(text, 0x42b5, c->id);
            he_ShowNews(ai_Chars[pl].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_03");
        }
        HMT_I(c, 0x18c) = hmt_RandomHeadIcon();
        hmt_NextPerson();
    }
    c->kind = 3;
}
