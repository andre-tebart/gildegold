/* 0x51c920, reads count items of size bytes into buf (vfs_fread, 0x430bd0, the counterpart of vfs_fwrite):
   true when the read returned a positive count */


int ls_Read(void *buf, int size, int count, int f)
{
    unsigned got = vfs_fread(buf, size, count, f);

    return got > 0;
}
