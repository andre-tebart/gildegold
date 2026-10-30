/* 0x5690c0, "he_AllocProduktion()": prepares the production event record r (the building +0xbc, the good +0xac): both clocks (+0x52 / +0x60) = the start clock (+0x44), the amount (+0xb4 / +0xb8) = the 0x22 field of the good's record in 0x91aaf0 (0x41 bytes each), due 1 unit later, state +0xc4 = -1, +0xc8 = 0, every character record that holds this event (+0x17c) is handed to 0x524260 with the building, +0xd4 = 1.0 and +0xcc = (the 0x50c980 value of the owner (+8) - 0.5) * 0.25. A building that is not found with the owner unset (+8 = 0xffff) prints the debug text. ROUGH */

void he_Unk5690c0(int r)
{
    char buf[0x200];
    int b, o, rec, id;
    char *t;
    float v;

    if (*(unsigned short *)(r + 8) == 0xffff) {
        o = gm_FindBuildingById(*(int *)(r + 0xbc));
        if (o != 0)
            txt_Format(buf, "he_AllocProduktion(): geb:%G", o);
    }
    b = gm_FindBuildingById(*(int *)(r + 0xbc));
    t = (char *)U_I((unsigned)&ai_Goods);
    id = *(unsigned short *)(r + 0xac);
    v = (float)*(unsigned int *)(t + (id << 6) + id + 0x22);
    *(HETIME *)(r + 0x52) = *(HETIME *)(r + 0x44);
    *(HETIME *)(r + 0x60) = *(HETIME *)(r + 0x44);
    *(float *)(r + 0xb4) = v;
    *(float *)(r + 0xb8) = v;
    gm_AddTime(r + 0x52, 0, 0, 1);
    *(int *)(r + 0xc4) = -1;
    *(int *)(r + 0xc8) = 0;
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == r)
            sv_MoveToGroup(b, *(unsigned short *)rec, 0, r);
    }
    *(float *)(r + 0xcc) = (gm_Unk50c980(*(unsigned short *)(r + 8), 8, r + 0x8c) - 0.5) * 0.25;
    *(int *)(r + 0xd4) = 0x3f800000;
}
