/* 0x467c60 */



/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
  return UNZ_OK if there is no problem.
*/
extern int ZEXPORT unzGetCurrentFileInfo (file,
                                                  pfile_info,
                                                  szFileName, fileNameBufferSize,
                                                  extraField, extraFieldBufferSize,
                                                  szComment,  commentBufferSize)
	unzFile file;
	unz_file_info *pfile_info;
	char *szFileName;
	uLong fileNameBufferSize;
	void *extraField;
	uLong extraFieldBufferSize;
	char *szComment;
	uLong commentBufferSize;
{
	return unzlocal_GetCurrentFileInfoInternal(file,pfile_info,NULL,
												szFileName,fileNameBufferSize,
												extraField,extraFieldBufferSize,
												szComment,commentBufferSize);
}
