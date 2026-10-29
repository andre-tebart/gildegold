/* 0x4ebf50, the object id of the name without its "ob_" prefix (0x4fe900; the tail call) */

int ahm_ObjectId(char *name)
{
    return gm_FindObjectProto(name + 3);
}
