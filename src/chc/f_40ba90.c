/* 0x40ba90, CreateCharacterAtDummy(mesh, dummy, name): creates the character at the dummy's world
   position, turns it into the dummy's direction, gives it `name` (body and object) and the group of the
   running script; returns 0 after an error when the dummy is missing or the mesh cannot be loaded. */

int chc_CreateCharacterAtDummy(char **mesh, D3OBJECT **dummy, char **name)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;
    CHARACTER *ch;

    if (!*dummy) {
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacterAtDummy(): invalid dummy");
        return 0;
    }
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    ch = ch_Create(*mesh, (D3VECTOR *)&pos);
    if (ch) {
        strcpy(ch->name, *name);
        strcpy(ch->obj->name, *name);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(*dummy, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot(ch->obj, &rot);
        d3_SetObjectPos(ch->obj, (D3VECTOR *)&pos);
        ch->scriptGroup = evt_Cur->group;
    } else
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacterAtDummy(): Could not load character");
    return (int)ch;
}
