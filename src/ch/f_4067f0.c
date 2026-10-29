/* 0x4067f0, action callback: moves the character to scene args[0] (ids args[1], args[2]) */

void ch_ActMove2Universe(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    int cur;
    D3OBJECT *obj;
    D3VECTOR4 pos;
    char name[0x80];

    if (a->args[0] < 0) {
        err_Message("ch_Move2Universe(): Invalid universe (<0)!");
        ch_RemoveAction(a);
        return;
    }
    if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
        if (!a->args[0] && a->args[1] != -1)
            err_Message("ch_Move2Universe(): invalid combination of ids and universeindex (Action->Data[0]==0 && Action->Data[1]!=CH_ID_NONE)");
        else
            err_Message("ch_Move2Universe(): invalid combination of ids and universeindex (Action->Data[0]!=0 && Action->Data[1]==CH_ID_NONE)");
        ch_RemoveAction(a);
        return;
    }
    if (!ch_MoveToScene(ch, &d3_Scenes[a->args[0]])) {
        err_Message("ch_Move2Universe(): Could not change universe...");
        ch_RemoveAction(a);
        return;
    }
    cur = d3_CurrentScene;
    d3if_SelectScene(a->args[0], 1);
    if (!a->args[0] && ch_CurrentGroup == ch->group)
        d3if_SelectTextureSet(ch->obj, ch->obj->material->animCtl, 0, 0);
    else if (ch_CurrentGroup == ch->group)
        d3if_SelectTextureSet(ch->obj, ch->obj->material->animCtl, CH_UNK23A(ch), 1);
    ch_ReleaseSit(ch);
    if (CHACT_HASPOS(a))
        ch_SetPos(ch, CHACT_POS(a), 0);
    else {
        if (a->u.name[0])
            strcpy(name, a->u.name);
        else
            strcpy(name, "dummy_EINGANG");
        obj = d3_FindObject(0, 0x100, 0, name);
        if (!obj)
            obj = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (!obj)
            obj = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (obj) {
            d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
            ch_SetPos(ch, (D3VECTOR *)&pos, 0);
        }
    }
    ch->group = a->args[1];
    ch->scriptGroup = a->args[2];
    CH_UNK038(ch)[0] = 0;
    CH_UNK038(ch)[1] = 0;
    CH_UNK038(ch)[2] = 0;
    ch_SetVisible(ch, 1);
    if (cur == a->args[0]) {
        if (ch->group != -1 && ch_CurrentGroup != ch->group || ch_CurrentScriptGroup != -1 && ch_CurrentScriptGroup != ch->scriptGroup)
            ch_SetVisible(ch, 0);
    }
    if (a->next && a->next->type == 0x38)
        ch_SetVisible(ch, 0);
    d3if_SelectScene(cur, 1);
    ch_RemoveAction(a);
}
