/* 0x520a30, sends the save game file `name` to the other players: opens it "rbn" (vfs_Tfopen), takes its length (vfs_ftell, rounded up to a multiple of 0x80), reads it into a mem_Alloc'd "ls_SendGame" buffer (vfs_fread) and hands it out in blocks of 0x80 bytes (cm_SendLoadBlock, the size first in cm_SendLoadSize; a Sleep(15) after every 16th block; the network pumps sv_PumpMessages / sv_PumpNet between), then waits until nothing is queued in cm_SendQueue. Closes the file and frees the buffer. Returns 1, 0 when the file or the buffer fails */


int ls_SendGame(char *name)
{
    VFSFILE *fh;
    int len, size, cnt = 0;
    char *buf, *p;

    fh = vfs_Tfopen(name, "rbn", (int)vfs_Root);
    if (fh == 0)
        return 0;
    vfs_fseek(fh, 0, SEEK_END);
    len = (int)vfs_ftell(fh);
    size = (len / 0x80 + 1) * 0x80;
    vfs_fseek(fh, 0, SEEK_SET);
    buf = (char *)mem_Alloc(size, "ls_SendGame");
    if (buf == 0) {
        vfs_fclose(fh);
        return 0;
    }
    if (vfs_fread(buf, len, 1, fh) == -1) {
        mem_Free(buf);
        vfs_fclose(fh);
        return 0;
    }
    cm_SendLoadSize(size);
    p = buf;
    if (size > 0) {
        int sent = 0;

        do {
            if (cm_SendLoadBlock(p) != -1) {
                p += 0x80;
                sent += 0x80;
                if (cnt++ % 16 == 0)
                    Sleep(0xf);
            }
            sv_PumpMessages();
            sv_PumpNet();
        } while (sent < size);
    }
    while (cm_SendQueue != 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    mem_Free(buf);
    vfs_fclose(fh);
    return 1;
}
