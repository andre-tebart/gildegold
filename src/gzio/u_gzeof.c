/* unlinked */

/* ===========================================================================
     Returns 1 when EOF has previously been detected reading the given
   input stream, otherwise zero.
*/
int ZEXPORT gzeof (file)
    gzFile file;
{
    gz_stream *s = (gz_stream*)file;
    
    return (s == NULL || s->mode != 'r') ? 0 : s->z_eof;
}
