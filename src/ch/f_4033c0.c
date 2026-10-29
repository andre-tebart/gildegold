/* 0x4033c0, ch_MoveCharacter2Universe */

int ch_MoveToScene(CHARACTER *ch, D3SCENE *scene)
{
    char msg[0x100];

    if (ch && ch->scene && scene && d3if_MoveObject(ch->obj, ch->scene, scene)) {
        d3_SetObject078(ch->obj, &d3_Origin);
        if (ch->transport)
            d3if_MoveObject(*ch->transport, ch->scene, scene);
        if (ch->lowPolyBody)
            d3if_MoveObject(ch->lowPolyBody, ch->scene, scene);
        ch->scene = scene;
        if (!d3t_SceneIndex(scene))
            D3OBJ_BITS(ch->obj, 0x211)->b3 = ch_ShadowEnabled;
        else
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
        if (scene == d3_DefaultScene && !ch->obj->info) {
            sprintf(msg, "ch_MoveCharacter2Universe(): character deflated, will be inflated:%s", ch->fullname);
            ch_Unk402540(ch);
        }
        return 1;
    }
    return 0;
}
