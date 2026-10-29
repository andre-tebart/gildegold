/* 0x40b240 */

void ch_DeleteAll(void)
{
    int i;

    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i]) {
            ch_Delete(ch_List[i]);
            ch_List[i] = 0;
        }
    }
}
