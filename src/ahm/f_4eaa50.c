/* 0x4eaa50, the character the alchemist object p (building kind 0x1e) belongs to: the character with the id of the record at +0x65; 0 when p is another kind or has no record */
char *ahm_OwnerChar(unsigned char *p)
{
    AHMOBJ *o = (AHMOBJ *)p;

    if (ai_BTypes[o->kind * 0x24d] == 0x1e && o->rec != 0)
        return (char *)gm_FindChar(o->rec->id);
    return 0;
}
