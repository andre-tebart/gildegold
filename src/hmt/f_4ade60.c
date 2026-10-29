/* 0x4ade60, hmt_Event becomes the previous one (hmt_CurrentEvent), the next event record the current one */

void hmt_NextEvent(void)
{
    int i;

    for (i = 0; i < hmt_File.nk; i++) {
        if (hmt_File.k[i].idx == hmt_Event) {
            hmt_CurrentEvent = hmt_Event;
            if (i < hmt_File.nk - 1)
                hmt_Event = hmt_File.k[i + 1].idx;
            else
                hmt_Event = -1;
            return;
        }
    }
}
