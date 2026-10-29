/* 0x401380, reads an array written by bio_wr_array into a new buffer; returns the element count.
   A stored element size different from `size` skips the data, sets *out = 0 and reports an error (returns 0). */

int bio_rd_array_quick(VFSFILE *f, int size, void **out)
{
    int n, sz;

    bio_rd_i(f, &n);
    bio_rd_i(f, &sz);
    if (sz != size) {
        vfs_fseek(f, sz * n, SEEK_CUR);
        *out = 0;
        err_Message("bio_rd_array_quick: Tried to load invalid array-sizes...");
        return 0;
    }
    *out = mem_Alloc(n * size, "bio:bio_rd_array_quick");
    vfs_fread(*out, size, n, f);
    return n;
}
