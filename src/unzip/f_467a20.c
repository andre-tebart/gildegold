/* 0x467a20 */
local int unzlocal_getByte(fin,pi)
	FILE *fin;
	int *pi;
{
    unsigned char c;
	int err = fread(&c, 1, 1, fin);
    if (err==1)
    {
        *pi = (int)c;
        return UNZ_OK;
    }
    else
    {
        if (ferror(fin)) 
            return UNZ_ERRNO;
        else
            return UNZ_EOF;
    }
}
