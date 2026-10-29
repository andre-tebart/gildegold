/* 0x405310, per frame: runs the characters' actions */

int ch_Update(void)
{
    int i;
    CHACTION *act;
    D3SM *h;
    int x, z;
    float y;
    D3VECTOR4 pos;
    CHARACTER *found[32];

    ch_Time = d3_Time;
    ch_Unk631060 = 0;
    if (!ch_Enabled)
        return 0;
    if (ch_Saved0 != d3_DefaultScene || ch_Saved1 != d3_DefaultScene) {
        if (d3_Scenes[0].hmap && ch_Saved0 != d3_DefaultScene) {
            ch_SaveGround(d3_Scenes[0].hmap);
            ch_Saved0 = d3_DefaultScene;
        }
        if (d3_DefaultScene && d3_CurrentScene && d3_DefaultScene->hmap && ch_Saved1 != d3_DefaultScene) {
            ch_SaveGround(d3_DefaultScene->hmap);
            ch_Saved1 = d3_DefaultScene;
        }
    }
    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i] && !(ch_List[i]->flags.all & 4)) {
            act = CH_ACTIONS(ch_List[i]);
            ch_CheckAniMorph(ch_List[i]);
            if (act) {
                if ((ch_List[i]->flags.all & 0x2000) && ch_List[i]->slot) {
                    d3_ReleaseTexture(ch_List[i]->obj->material->animCtl, ch_List[i]->slot->data->name);
                    ch_List[i]->slot = 0;
                    ch_List[i]->flags.all &= ~0x2000;
                }
                ch_UpdateAction(ch_List[i]);
            } else if (ch_List[i]->scene == d3_DefaultScene && !(ch_List[i]->flags.all & 0x32)
                       && !(ch_List[i]->flags.all & 0x8000) && !ch_List[i]->transport) {
                if (ch_FindNear(ch_List[i], found, 20.0f)) {
                    h = (D3SM *)ch_GetHMap(ch_List[i]);
                    ch_RandomNear(&ch_List[i]->obj->pos, (D3VECTOR *)&pos);
                    if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y))
                        ch_Unk40b2f0(ch_List[i], 0x2d, x, z, d3_CurrentScene);
                }
            } else
                ch_List[i]->flags.all &= ~8;
            if (ch_List[i]->transport)
                ch_PlaceTransport(ch_List[i], 0);
            if (ch_List[i]->lowPolyBody)
                ch_UpdateLowPoly(ch_List[i]);
            if (!act && d3_DefaultScene == ch_List[i]->scene && d3_CurrentScene && !ch_List[i]->slot
                && (ch_List[i]->flags.all & 0x1000) && !(ch_List[i]->flags.all & 2)
                && !(D3MATERIAL_UNK96D(ch_List[i]->obj->material) & 0x40)) {
                if (ch_List[i]->flags.all & 0x10) {
                    ch_AttachAni(ch_List[i], "sitzend/sitz_newnoise", 0);
                    if (ch_List[i]->slot) {
                        ch_List[i]->slot->flags.b[0] = 0;
                        ch_List[i]->slot->flags.b[1] &= ~0x10;
                        ch_List[i]->flags.all |= 0x2000;
                    } else
                        ch_List[i]->flags.all &= ~0x1000;
                } else {
                    ch_AttachAni(ch_List[i], "stehen/stehen_newnoise", 0);
                    if (ch_List[i]->slot) {
                        ch_List[i]->slot->flags.b[0] = 0;
                        ch_List[i]->slot->flags.b[1] &= ~0x10;
                        ch_List[i]->flags.all |= 0x2000;
                    } else
                        ch_List[i]->flags.all &= ~0x1000;
                }
            }
            if (ch_List[i] && ch_List[i]->scene != ch_List[i]->obj->scene)
                err_Message("ch_Update(): invalid c->universe!");
        }
    }
    ch_UpdateFades();
    ch_LastUpdate = d3_Time;
    return 1;
}
