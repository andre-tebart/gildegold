/* 0x406c60, creates the object name in a hand of the character (name 0: removes it) */

void ch_SetHandObject(CHARACTER *ch, int hand, char *name)
{
    D3OBJECT **slot = 0;
    int cur;
    D3VECTOR4 pos = {0.0f, 0.0f, 0.0f, 0.0f};
    D3VECTOR rot = {0.0f, 0.0f, 0.0f};

    if (hand == 1)
        slot = CH_HAND(ch, 0x68);
    else if (hand == 2)
        slot = CH_HAND(ch, 0x64);
    else if (hand == 3)
        slot = CH_HAND(ch, 0x6c);
    if (name) {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        if (*slot) {
            d3if_Delete(*slot);
            *slot = 0;
        }
        *slot = d3_CreateObject((int)ch->obj, (D3VECTOR *)&pos, &rot, name);
        if (!*slot) {
            d3if_SelectScene(cur, 1);
            return;
        }
        (*slot)->stateFlags |= 0xc;
        (*slot)->objectFlags &= ~2;
        (*slot)->flags &= ~4;
        (*slot)->kind = 5;
        d3_RelightObject(*slot);
        if (hand == 1)
            d3_AttachToBone(*slot, "d3_LeftHand");
        else if (hand == 2)
            d3_AttachToBone(*slot, "d3_RightHand");
        else if (hand == 3)
            d3_AttachToBone(*slot, "d3_Head");
        d3if_SelectScene(cur, 1);
    } else {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        if (*slot) {
            d3if_Delete(*slot);
            *slot = 0;
        }
        d3if_SelectScene(cur, 1);
    }
}
