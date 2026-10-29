/* 0x4adec0, the same for the persons */

void hmt_NextPerson(void)
{
    int i;

    for (i = 0; i < hmt_File.np; i++) {
        if (hmt_File.p[i].idx == hmt_Person) {
            hmt_CurrentPerson = hmt_Person;
            if (i < hmt_File.np - 1)
                hmt_Person = hmt_File.p[i + 1].idx;
            else
                hmt_Person = -1;
            return;
        }
    }
}
