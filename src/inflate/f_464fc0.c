/* 0x464fc0, resets the decompressor to the head of a new stream (counters, message, block state)
   without freeing the state; Z_OK, or Z_STREAM_ERROR for a NULL stream. */
int ZEXPORT inflateReset(z)
z_streamp z;
{
  if (z == Z_NULL || z->state == Z_NULL)
    return Z_STREAM_ERROR;
  z->total_in = z->total_out = 0;
  z->msg = Z_NULL;
  z->state->mode = z->state->nowrap ? BLOCKS : METHOD;
  inflate_blocks_reset(z->state->blocks, z, Z_NULL);
  Tracev((stderr, "inflate: reset\n"));
  return Z_OK;
}
