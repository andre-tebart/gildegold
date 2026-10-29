/* 0x402af0, 0/1 create the low-poly bodies, 2 deletes them */

void ch_SetLowPoly(unsigned char mode)
{
    int cur = d3_CurrentScene;
    int i;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch) {
            d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
            if (mode == 2) {
                if (ch->lowPolyBody) {
                    d3t_ReleaseObjectTextures(ch->lowPolyBody);
                    d3if_Delete(ch->lowPolyBody);
                    ch->lowPolyBody = 0;
                }
            } else if ((mode == 0 || mode == 1) && !ch->lowPolyBody) {
                sv_ReturnZero(ch);
                ch_UpdateLowPoly(ch);
            }
        }
    }
    ch_LowPoly = 2 - mode;
    d3if_SelectScene(cur, 1);
}
