/* 0x51e750, reads one ch.c CHARACTER record from the save (allocated by ch_AllocAt from the id read first): its name (+8, 0x20 bytes), the group / script group (+0x2c / +0x30), the flag word +0x8c (bit 5 set when the flag byte read is 1), the full name +0x170 (0x30) and the body name +0x130 (0x40), +0x38 (0x10), +0x48 (0xc), the texts +0x1a8 (from 0x10013) and +0x1e8 (from 0x1005a), the flags &= 0xdffb, then the reference index read last goes to +0x12c (0x51e900 turns it into a pointer). Returns the record */


int ls_ReadCharacter(int f)
{
    int id, ent, v;
    unsigned short *fl;         /* the flag word at +0x8c */
    unsigned char *flagByte;    /* its low byte (bit 5 = 0x20) */
    int *ref;                   /* the saved reference index at +0x12c */
    char b;

    LS_R(&id, 4)
    ent = (int)ch_AllocAt(id);
    LS_R(ent + 8, 0x20)
    LS_R(ent + 0x2c, 4)
    LS_R(ent + 0x30, 4)
    fl = (unsigned short *)(ent + 0x8c);
    LS_R(fl, 2)
    LS_R(ent + 0x170, 0x30)
    LS_R(ent + 0x130, 0x40)
    LS_R(ent + 0x38, 0x10)
    LS_R(ent + 0x48, 0xc)
    LS_R(&b, 1)
    if (b == 1) {
        flagByte = (unsigned char *)(ent + 0x8c);
        *flagByte |= 0x20;
    }
    LS_R(&id, 4)
    if (ls_Version >= 0x10013)
        LS_R(ent + 0x1a8, 0x40)
    if (ls_Version >= 0x1005a)
        LS_R(ent + 0x1e8, 0x40)
    v = id;
    *fl &= 0xdffb;
    ref = (int *)(ent + 0x12c);
    *ref = v;
    return ent;
}
