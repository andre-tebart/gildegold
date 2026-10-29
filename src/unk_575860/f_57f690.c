/* 0x57f690, the shot result table (260 B): every 0x15e ticks (0x6333cc vs sc + 0x70) the three 0x38-byte records
   of 0x6f9f60 (rec 0 .. 2, the record index in the first dword; three 0x10-byte entries each: the object 0x4fea50
   (0, 0, the id), the flag +8, the value +0xc) are turned into the texts 0x406c / 0x406d (the character word +0x27
   of the object, 0xffff / 0x6f1bf8 = the playerless case) with 0x4071 / 0x4072 (<= 25) / 0x4073 and the name block
   0x602568 / 0x60c6c0; the first record with any entry: 0x6f2664 = 0, its index = -1, 0x475590 (sc + 0x74, 1);
   with 0x6f2664 set but no entry: the texts 0x602568 / 0x406e, the same; none at all: 0x475590 (sc + 0x74, 0) and
   0x6f2664 = 0 */

void ai_Unk57f690(char *sc)
{
    int n;
    int rec;
    int i;
    int h;
    int v;
    unsigned short w;

    n = 0;
    if ((unsigned int)(*(int *)(sc + 0x70) + 0x15e) <= (unsigned int)U_I((unsigned)&ai_Unk6333cc)) {
        *(int *)(sc + 0x70) = U_I((unsigned)&ai_Unk6333cc);
        d2_SetForm(*(int *)(sc + 0x74), 0);
        for (rec = 0; rec < 3; rec++) {
            i = 0;
            if (*(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) == rec) {
                U_I((unsigned)&full_data_6f2664) = 1;
                for (i = 0; i < 3; i++) {
                    h = 0;
                    gm_FindById(&h, 0, 0, *(int *)((unsigned)&full_data_6f9f68 + rec * 0x38 + i * 0x10));
                    if (h == 0)
                        break;
                    v = 0x4071;
                    if (*(int *)((unsigned)&full_data_6f9f70 + rec * 0x38 + i * 0x10) == 1) {
                        v = 0x4073;
                        if (*(float *)((unsigned)&full_data_6f9f74 + rec * 0x38 + i * 0x10) <= 25.0f)
                            v = 0x4072;
                    }
                    io_PrintText(i == 0 ? (char *)(unsigned)"$C" : (char *)(unsigned)&ai_Unk60c6c0);
                    w = *(unsigned short *)(h + 0x27);
                    if (w == 0xffff || (char *)(unsigned)&ai_Chars + w * 0x21c == (char *)U_I((unsigned)&ai_PlayerChar))
                        io_PrintText(0x406c, h, v);
                    else
                        io_PrintText(0x406d, (int)w, h, v);
                    n++;
                }
                if (n != 0) {
                    U_I((unsigned)&full_data_6f2664) = 0;
                    *(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) = -1;
                    d2_SetViewEnabled(*(int *)(sc + 0x74), 1);
                    return;
                }
                if (U_I((unsigned)&full_data_6f2664) != 0) {
                    U_I((unsigned)&full_data_6f2664) = 0;
                    *(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) = -1;
                    io_PrintText((char *)(unsigned)"$C");
                    io_PrintText(0x406e);
                    d2_SetViewEnabled(*(int *)(sc + 0x74), 1);
                    return;
                }
            }
        }
        d2_SetViewEnabled(*(int *)(sc + 0x74), 0);
        U_I((unsigned)&full_data_6f2664) = 0;
    }
}
