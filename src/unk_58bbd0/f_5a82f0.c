/* 0x5a82f0, the map window's close (78 B): the saved screen 0x6ed930 (+0x27a8) is restored, the view 0x4758f0,
   the 0x300 controls +0x18dc are removed (0x46e710) and the view is cleared */

void ai_Unk5a82f0(int *m_)
{
    char *p = (char *)m_;
    int i;

    *(int *)((unsigned)&d2_BackgroundTile) = m_[0x9ea];
    d2_CloseView(m_[0]);
    for (i = 0; i < 0x300; i++) {
        if (*(int *)(p + 0x18dc + i * 4) != -1) {
            d2_RemoveObj(*(int *)(p + 0x18dc + i * 4));
            *(int *)(p + 0x18dc + i * 4) = -1;
        }
    }
    m_[0] = -1;
}
