/* 0x57bc30, the tutorial info block (168 B, ROUGH): the bytes 0x75e5a0 = 0x4b, 0x75e5a1 = 0x18, 0x75e5a2 =
   0x10, 0x75e5a3 = 0x33, the dword 0x75e5a4 = 0x59a, 0x75e5a8 / 0x75e5a9 = 0; 0x75e628 = 0x4fb770 (0x624ee0)
   + 0x639; the strings U_I(0x1449e48) / U_I(0x1449e4c) copied to 0x75e5aa / 0x75e5ca; 0x503ee0 (the local
   buffer, the byte 0x75e5a3) returns a record whose dword +0 goes to 0x75e5f0 and whose word +4 (low byte) to the
   local and 0x75e5f4 */

int unk_Unk4fb770();
void gm_Unk503ee0(unsigned *out, int n);

void ai_Unk57bc30(void)
{
    char local[0x10];
    char *r;

    *(unsigned char *)(unsigned)&init_PlayerProfile = 0x4b;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0x18;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x2) = 0x10;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x3) = 0x33;
    U_I(((unsigned)&init_PlayerProfile + 0x4)) = 0x59a;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8) = 0;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9) = 0;
    U_I((unsigned)&full_data_75e628) = unk_Unk4fb770((char *)(unsigned)&full_data_624ee0) + 0x639;
    strcpy((char *)((unsigned)&init_PlayerProfile + 0xa), *(char **)(unsigned)&full_data_1449e48);
    strcpy((char *)((unsigned)&init_PlayerProfile + 0x2a), *(char **)(unsigned)&full_data_1449e4c);
    gm_Unk503ee0((unsigned *)local, *(unsigned char *)((unsigned)&init_PlayerProfile + 0x3));
    r = local;
    U_I((unsigned)&full_data_75e5f0) = *(int *)r;
    *(unsigned short *)(local + 0xc) = *(unsigned short *)(r + 4);
    *(unsigned char *)(unsigned)&full_data_75e5f4 = *(unsigned char *)(r + 4);
}
