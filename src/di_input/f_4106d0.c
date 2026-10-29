/* 0x4106d0, format fmt and the HRESULT ("Error = %08x") into a local buffer; the original has no output call */

void di_Error(int unk, int hr, char *fmt, ...)
{
    char buf[1024];
    char *p;
    va_list ap;

    va_start(ap, fmt);
    p = buf + wsprintf(buf, fmt, ap);
    wsprintf(p, "\n\nError = %08x", hr);
}
