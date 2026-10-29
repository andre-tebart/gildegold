/* 0x4027b0 */

void ch_Unk4027b0(void)
{
    int i;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && ch->flags.dirty && ch->scene == d3_DefaultScene)
            d3_SetObject078(ch->obj, &ch->offset);
    }
}
