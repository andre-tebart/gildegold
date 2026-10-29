/* 0x421f00, whether prot marks memory the stack walk may scan as code: executable pages always; a
   read-only or read-write page only while the exe's own code page is read-only or read-write too */

char err_IsCodeProtect(unsigned char prot)
{
    MEMORY_BASIC_INFORMATION mbi;

    VirtualQuery(err_IsCodeProtect, &mbi, sizeof(mbi));
    switch (prot) {
    case PAGE_READONLY:
    case PAGE_READWRITE:
        return mbi.Protect == PAGE_READONLY || mbi.Protect == PAGE_READWRITE;
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
        return 1;
    }
    return 0;
}
