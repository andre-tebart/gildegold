/* 0x52fb50, sets 0x6f22a0 to the distance (from the top of the 0x75f780 stack of 8-byte records, 0x6f229c = top) to the record whose first word is v */

void sv_PopTo(int v)
{
    int i;

    i = sv_FrameDepth;
    if (sv_FrameDepth >= 0) {
        while (SV_I(((unsigned)&ai_Unk75f280 + 0x500) + i * 8) != v) {
            i--;
            if (i < 0)
                return;
        }
        sv_PopRequest = sv_FrameDepth - i;
    }
}
