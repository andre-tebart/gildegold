/* 0x52f340, the page keys of the book: the button 0x767 turns forward (0x52f9f0), 0x768 backward (0x52fa00) */

void sv_Unk52f340(int *book)
{
    if (SV_I((unsigned)&d2_ClickedId) == 0x767) {
        sv_Unk52f9f0(book);
        return;
    }
    if (SV_I((unsigned)&d2_ClickedId) == 0x768)
        sv_Unk52fa00(book);
}
