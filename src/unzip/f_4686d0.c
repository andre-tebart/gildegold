/* 0x4686d0 */

/* GildeGold: the two functions below are not in upstream unzip 0.15 (0x4686d0, 0x468710).
   They save and restore the position of the current file in the central directory,
   like unzGetFilePos/unzGoToFilePos of later minizip versions. Names are ours. */

extern int ZEXPORT unzGetFilePos (file, pos_in_central_dir, num_file)
	unzFile file;
	uLong *pos_in_central_dir;
	uLong *num_file;
{
	unz_s* s;
	*num_file = 0;
	*pos_in_central_dir = 0;
	s=(unz_s*)file;
	if (file==NULL || !s->current_file_ok || s->pfile_in_zip_read!=NULL)
		return UNZ_PARAMERROR;
	*pos_in_central_dir = s->pos_in_central_dir;
	*num_file = s->num_file;
	return UNZ_OK;
}
