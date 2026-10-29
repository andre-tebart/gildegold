/* 0x40b4d0, drops the frame callbacks of all actions of characters in the scene */

void ch_ClearSceneCallbacks(D3SCENE *scene)
{
    int i;

    for (i = 0; i < CH_QUEUE; i++) {
        if (ch_Queue[i].type && ch_Queue[i].func && ch_Queue[i].ch && ch_Queue[i].ch->scene == scene
            && ch_Queue[i].func2)
            ch_Queue[i].func2 = 0;
    }
}
