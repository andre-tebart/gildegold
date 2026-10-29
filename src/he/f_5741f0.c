/* 0x5741f0, the trial of the event 0xa0 (ROUGH): the building +0x10 (0x4fea50) and the figure +0xc (0x505df0) must exist; by the
   building kind (0x50d060) 3 / 2 / else the approach +0xac gives a base value (0x14 / 0xa / 0 for a below 4 / below 7) plus the
   float constants 0x5ebad4 / 0x5eb7f4 and a bonus count (0xa / 0x14 / 0x19); a random amount (0x42ec00 (0x18 / 0x10 / 8) + 0xf /
   0xc / 0xa, times 0x20) and the other characters of the kind below 0xa whose 0x50c550 (word, per's word, 1) reaches the base
   are counted; text 0x41fa (0x5176d0), the message 0x5fa (0x53dda0) and the payment -amount from per +4 (0x4c4830) */

int gm_Unk50d060();                 /* 0x50d060 */

void he_Unk5741f0(int ev)
{
    char text[0x200];
    char *bld, *per, *ch;
    int a, kind, cnt, n, i, x;
    float base;

    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    a = HM_I(ev, 0xac);
    kind = gm_Unk50d060(bld);
    cnt = 0;
    x = 0;
    if (kind == 3) {
        if (a < 4) {
            base = 0x14 + (40.0f);
            x = 0xa;
        } else if (a < 7) {
            base = 0xa + (40.0f);
            x = 0x14;
        } else {
            base = (40.0f);
            x = 0x19;
        }
        n = (util_Random(0x18) & 0xffff) + 0xf;
    } else if (kind == 2) {
        if (a < 4) {
            base = 0xa + (50.0f);
            x = 0xf;
        } else {
            base = (50.0f);
            x = 0xf;
        }
        n = (util_Random(0x10) & 0xffff) + 0xc;
    } else {
        base = 70.0f;
        n = (util_Random(8) & 0xffff) + 0xa;
    }
    n = n * 0x20;
    for (i = 0; i < 0x300; i++) {
        ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
        if (*(unsigned short *)ch == 0xffff || ch == per || HM_B(ch, 2) >= 0xa)
            continue;
        if (((float (*)(unsigned short, unsigned short, int))gm_Liking)(*(unsigned short *)ch, *(unsigned short *)per, 1) >= base)
            cnt++;
    }
    cnt += x;
    txt_Format(text, 0x41fa, 0, cnt, n * cnt);
    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, 0);
    cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n * cnt);
}
