/* 0x573760, the approach 0 of the event 0x9d: with the two words of 0x505230 (per, ..) a name (copied from 0x629038 when
   the figure is the event's own, else 0x62901c, 0x19 bytes) and the texts 0x41de / 0x41dd (else 0x41dc) are built and
   sent as the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573760(char *pl, char *per)
{
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    int a, b;

    if (gm_Unk505230(per, &a, &b) == 0)
        return;
    if (per == pl) {
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
        txt_Format(t1, 0x41de, a + 0x1632);
        txt_Format(t2, 0x41dd, t1);
    } else {
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
        txt_Format(t1, 0x41de, a + 0x1632);
        txt_Format(t2, 0x41dc, t1);
    }
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
