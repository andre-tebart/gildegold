/* 0x468710, jumps to the file whose position and index unzGetFilePos saved and re-reads its central-directory
 * record; returns UNZ_PARAMERROR (NULL file, position 0 or a file open) or the record's status */

extern int ZEXPORT unzGoToFilePos (file, pos_in_central_dir, num_file)
	unzFile file;
	uLong pos_in_central_dir;
	uLong num_file;
{
	int err;
	unz_s* s;
	s=(unz_s*)file;
	if (file==NULL || pos_in_central_dir==0 || s->pfile_in_zip_read!=NULL)
		return UNZ_PARAMERROR;
	s->pos_in_central_dir = pos_in_central_dir;
	s->num_file = num_file;
	err = unzlocal_GetCurrentFileInfoInternal(file,&s->cur_file_info,
											 &s->cur_file_info_internal,
											 NULL,0,NULL,0,NULL,0);
	s->current_file_ok = (err == UNZ_OK);
	return err;
}
