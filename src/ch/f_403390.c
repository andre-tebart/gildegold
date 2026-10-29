/* 0x403390 */

CHARACTER *ch_Create(char *name, D3VECTOR *pos)
{
    CHARACTER *ch = ch_Alloc();

    if (!ch_CreateMesh(ch, name, pos)) {
        ch_Delete(ch);
        return 0;
    }
    return ch;
}
