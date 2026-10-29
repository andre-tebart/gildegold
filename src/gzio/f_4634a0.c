/* 0x4634a0 */

/* ===========================================================================
      Sets the starting position for the next gzread or gzwrite on the given
   compressed file. The offset represents a number of bytes in the
      gzseek returns the resulting offset location as measured in bytes from
   the beginning of the uncompressed stream, or -1 in case of error.
      SEEK_END is not implemented, returns error.
      In this version of the library, gzseek can be extremely slow.
*/
z_off_t ZEXPORT gzseek (file, offset, whence)
    gzFile file;
    z_off_t offset;
    int whence;
{
    gz_stream *s = (gz_stream*)file;

    if (s == NULL || whence == SEEK_END ||
	s->z_err == Z_ERRNO || s->z_err == Z_DATA_ERROR) {
	return -1L;
    }
    
    if (s->mode == 'w') {
#ifdef NO_DEFLATE
	return -1L;
#else
	if (whence == SEEK_SET) {
	    offset -= s->stream.total_in;
	}
	if (offset < 0) return -1L;

	/* At this point, offset is the number of zero bytes to write. */
	if (s->inbuf == Z_NULL) {
	    s->inbuf = (Byte*)ALLOC(Z_BUFSIZE); /* for seeking */
	    zmemzero(s->inbuf, Z_BUFSIZE);
	}
	while (offset > 0)  {
	    uInt size = Z_BUFSIZE;
	    if (offset < Z_BUFSIZE) size = (uInt)offset;

	    size = gzwrite(file, s->inbuf, size);
	    if (size == 0) return -1L;

	    offset -= size;
	}
	return (z_off_t)s->stream.total_in;
#endif
    }
    /* Rest of function is for reading only */

    /* compute absolute position */
    if (whence == SEEK_CUR) {
	offset += s->stream.total_out;
    }
    if (offset < 0) return -1L;

    if (s->transparent) {
	/* map to fseek */
	s->stream.avail_in = 0;
	s->stream.next_in = s->inbuf;
        if (fseek(s->file, offset, SEEK_SET) < 0) return -1L;

	s->stream.total_in = s->stream.total_out = (uLong)offset;
	return offset;
    }

    /* For a negative seek, rewind and use positive seek */
    if ((uLong)offset >= s->stream.total_out) {
	offset -= s->stream.total_out;
    } else if (gzrewind(file) < 0) {
	return -1L;
    }
    /* offset is now the number of bytes to skip. */

    if (offset != 0 && s->outbuf == Z_NULL) {
	s->outbuf = (Byte*)ALLOC(Z_BUFSIZE);
    }
    while (offset > 0)  {
	int size = Z_BUFSIZE;
	if (offset < Z_BUFSIZE) size = (int)offset;

	size = gzread(file, s->outbuf, (uInt)size);
	if (size <= 0) return -1L;
	offset -= size;
    }
    return (z_off_t)s->stream.total_out;
}
