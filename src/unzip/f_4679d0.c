/* 0x4679d0 */


/* ===========================================================================
   Reads a long in LSB order from the given gz_stream. Sets 
*/
local int unzlocal_getShort (fin,pX)
	FILE* fin;
    uLong *pX;
{
    uLong x ;
    int i;
    int err;

    err = unzlocal_getByte(fin,&i);
    x = (uLong)i;
    
    if (err==UNZ_OK)
        err = unzlocal_getByte(fin,&i);
    x += ((uLong)i)<<8;
   
    if (err==UNZ_OK)
        *pX = x;
    else
        *pX = 0;
    return err;
}
