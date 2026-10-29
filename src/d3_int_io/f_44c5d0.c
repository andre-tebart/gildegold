/* 0x44c5d0, the loaded stock object of that name */

int d3io_FindStock(char *name)
{
    D3IOSTOCK *s;

    for (s = (D3IOSTOCK *)d3_StockListHead; s != (D3IOSTOCK *)&d3_StockListEnd; s = s->next)
        if (!_strnicmp(name, s->name, 0x3f))
            return (int)s;
    return 0;
}
