/* unlinked */


/* Returns true if inflate is currently at the end of a block generated
 * by Z_SYNC_FLUSH or Z_FULL_FLUSH. 
 * IN assertion: s != Z_NULL
 */
int inflate_blocks_sync_point(s)
inflate_blocks_statef *s;
{
  return s->mode == LENS;
}
