/* 0x40d010, GetCharacterSubObjectHandle(ch, name): the character's sub-object (0x140: hands, head) with that name, or 0 */

int chc_GetCharacterSubObjectHandle(CHARACTER **ch, char **name)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "GetCharacterSubObjectHandle(): invalid character");
        return 0;
    }
    return (int)d3_FindObject((int)(*ch)->obj, 0x140, 0, *name);
}
