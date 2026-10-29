/* 0x47cb70, shape i of a bank */

D2FRAME *d2_GetFrame(void *pic, short i)
{
    D2BANK *bank = (D2BANK *)pic;

    if (bank == 0)
        return 0;
    return (D2FRAME *)((char *)bank + bank->offs[(unsigned short)i]);
}
