/* 0x4028a0 */

int ch_Delete(CHARACTER *ch)
{
    int i = 0;
    int cur;

    if (ch) {
        for (; ch_List[i] != ch && i < CH_MAX; i++)
            ;
        if (i < CH_MAX) {
            ch_List[i] = 0;
            ch->id = -1;
            ch_ClearActions(ch);
            if (ch->obj) {
                cur = -1;
                ch_ReleaseMorph(ch);
                if (d3_DefaultScene != ch->scene) {
                    cur = d3_CurrentScene;
                    d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
                }
                if (ch->unk228)
                    ch->unk228(ch);
                d3if_Delete(ch->obj);
                ch->obj = 0;
                if (ch->transport && *ch->transport)
                    d3if_Delete(*ch->transport);
                if (ch->lowPolyBody)
                    d3if_Delete(ch->lowPolyBody);
                if (cur != -1)
                    d3if_SelectScene(cur, 1);
            }
            if (ch->transport)
                ch_DetachTransport(ch);
            mem_Free(ch);
            return 1;
        }
    }
    return 0;
}
