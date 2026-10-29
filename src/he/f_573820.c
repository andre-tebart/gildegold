/* 0x573820, the approach 1 of the event 0x9d: the relation of per changes (0x4ea550): to the figure of the byte +0x166
   (0x4e6f80), when there is one: by -0x14 - random (0x10); else, for a figure not of the kind 5 / 6 / 7: by 1 + random (3) and
   0x14 + random (0x10); else to every other figure of the kinds 5 .. 7 by -0x14 - random (0x10); then the texts 0x41df / 0x41dd
   (0x41dc when per is not pl) and the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573820(char *pl, char *per)
{
    char t1[0x400];
    char t2[0x400];
    char *ch;
    int a, x, i;

    if (amt_GetKind(HM_B(per, 0x166), &a) != 0 && a != 0) {
        amt_ChangeDistrictOpinion(per, a, -0x14 - (util_Random(0x10) & 0xffff));
    } else if (HM_B(per, 2) != 6 && HM_B(per, 2) != 7 && HM_B(per, 2) != 5) {
        amt_ChangeDistrictOpinion(per, 1 + (util_Random(3) & 0xffff), 0x14 + (util_Random(0x10) & 0xffff));
    } else {
        x = -0x14 - (util_Random(0x10) & 0xffff);
        for (i = 0; i < 0x300; i++) {
            if (i == *(unsigned short *)per)
                continue;
            ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
            if (HM_B(ch, 2) == 6 || HM_B(ch, 2) == 7 || HM_B(ch, 2) == 5)
                cm_SendRelations(HM_I(per, 4), HM_I(ch, 4), x, 1);
        }
    }
    txt_Format(t1, 0x41df);
    txt_Format(t2, per == pl ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, 0);
}
