/* 0x47c410, converts a bank of shapes to 16 bit (a = 1); b = 0 frees the old bank @deep B3, 17/464 bytes with 32 code + 32 regs hunks: only the size / offset-pointer register pair differs (original size in ecx, pointer in edx); local copies, pointer and countdown loops, declaration order and size+total were tried in the d2s round (unit note), so what is left is the allocator, not the C */

void *d2_BankConvertTo16(void *pic, int a, int b)
{
    D2BANK *bank, *nb;
    int i, total, size;
    D2SHP *sh;

    bank = (D2BANK *)pic;
    size = bank->size;
    total = 0;
    if (size == 0)
        return 0;
    for (i = 0; i < bank->n; i++)
        total += ((D2SHP *)((char *)bank + bank->offs[i]))->npix;
    if (bank->kind == 2 && a == 1) {
        nb = mem_Alloc(size - total, "d2:shp:NewBank");
        memcpy(nb, bank, 0x845);
        nb->size = 0;
        nb->kind = 1;
        nb->n = 0;
        for (i = 0; i < bank->n; i++) {
            sh = d2_ShpConvertTo16((D2SHP *)((char *)bank + bank->offs[i]), 1);
            d2_BankAppendShape(sh, nb);
            if (sh != 0)
                mem_Free(sh);
        }
        if (bank->pal != 0) {
            nb->pal = nb->size;
            memcpy((char *)nb + nb->size, (char *)bank + bank->pal, bank->npal * 8);
        }
        if (b == 0) {
            mem_Free(bank);
            return nb;
        }
    } else {
        if (bank->kind != 0 || a != 1)
            return bank;
        nb = mem_Alloc(total + size, "d2:shp:NewBank");
        memcpy(nb, bank, 0x845);
        nb->size = 0;
        nb->kind = 1;
        nb->n = 0;
        for (i = 0; i < bank->n; i++) {
            sh = d2_ShpConvertTo16((D2SHP *)((char *)bank + bank->offs[i]), 1);
            d2_BankAppendShape(sh, nb);
            if (sh != 0)
                mem_Free(sh);
        }
        if (bank->pal != 0) {
            nb->pal = nb->size;
            memcpy((char *)nb + nb->size, (char *)bank + bank->pal, bank->npal * 8);
        }
        if (b == 0)
            mem_Free(bank);
    }
    return nb;
}
