/* 0x40eec0, renames *obj to *name; 0 when *obj is NULL, else 1 */

int ocmd_RenameObject(D3OBJECT **obj, char **name)
{
    if (!*obj)
        return 0;
    strcpy((*obj)->name, *name);
    return 1;
}
