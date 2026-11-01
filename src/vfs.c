/* vfs.c -- original file name unknown: the virtual file system ("vfs_Tfopen", "vfs_init" strings).
   The translation unit starts at 0x42eca0 (after tr.c, "tr_startup"; its .data starts with
   gz_magic at 0x5f9004, right after "tr_startup", and "vfs:IGetSubDir" of 0x42ed70 follows) and ends at
   0x4315c0, where d3_anim.c starts. Functions are kept in their original order. Names are ours except
   vfs_IGetSubDir.
   unit of rea 8cef7d5b (src/game/vfs.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include "vfs.h"
#include "mem.h"

static int gz_magic[2] = {0x1f, 0x8b};   /* 0x5f9004, the first data of this file (as in gzio.c) */
struct MPOOL * vfs_EntryPool = 0;  /* 0x633400 */
struct MPOOL * vfs_FilePool = 0;  /* 0x633404 */
char vfs_IgnoreCase = 0;  /* 0x633408 */
int vfs_OpenCount = 0;  /* 0x63340c */
const char vfs_Ext[][6] = { ".BIN5", ".BIN4", ".BIN3", ".BIN2", ".BIN1", ".BIN0", ".BIN" };  /* 0x5eba40, ".BIN5", ".BIN4", ... */

/* 0x42eca0, compares two names, ignoring case if vfs_IgnoreCase is set. */
int vfs_strcmp(char *a, char *b)
{
    if (vfs_IgnoreCase)
        return stricmp(a, b);
    return strcmp(a, b);
}
/* 0x42ecf0, upper-cases a name unless vfs_IgnoreCase is set. */

char *vfs_UpperCase(char *path)
{
    if (vfs_IgnoreCase)
        return path;
    return strupr(path);
}
/* 0x42ed10, turns backslashes into slashes. */

void vfs_ToSlashes(char *path)
{
    int i, n;

    if (path) {
        n = strlen(path);
        for (i = 0; i < n; i++) {
            if (*path == '\\')
                *path = '/';
            path++;
        }
    }
}
/* 0x42ed40, turns slashes into backslashes. */

void vfs_ToBackslashes(char *path)
{
    int i, n;

    if (path) {
        n = strlen(path);
        for (i = 0; i < n; i++) {
            if (*path == '/')
                *path = '\\';
            path++;
        }
    }
}
/* 0x42ed70, returns the subdirectory `name` of parent, creating it if needed. */

VFSDIR *vfs_IGetSubDir(char *name, VFSDIR *parent)
{
    VFSDIR *d, *p;

    if (parent)
        for (d = parent->child; d; d = d->next)
            if (!vfs_strcmp(name, d->name))
                return d;
    d = mem_Alloc(sizeof(VFSDIR), "vfs:IGetSubDir");
    strcpy(d->name, name);
    d->sorted = 0;
    d->parent = parent;
    if (parent) {
        p = parent->child;
        if (!p) {
            parent->child = d;
            return d;
        }
        while (p->next)
            p = p->next;
        p->next = d;
    }
    return d;
}
/* 0x42ee10, bsearch comparator of a name with the name of an entry. */

int vfs_CmpName(const char *name, const VFSENTRY *e)
{
    return strcmp(name, e->name);
}
/* 0x42ee50, the same ignoring case. */

int vfs_CmpNameI(const char *name, const VFSENTRY *e)
{
    return stricmp(name, e->name);
}

/* ---- 0x42ee60..0x430130 (worker vfs2) ---- */

/* 0x42ee60, looks a file up in the sorted entries of d (and of its subdirectories if recurse). */
VFSENTRY *vfs_FindEntry(char *name, VFSDIR *d, VFSDIR **found, char recurse)
{
    VFSENTRY *e;

    if (name && *name && d) {
        if (d->nentries > 0 && d->sorted) {
            e = bsearch(name, d->entries, d->nentries, sizeof(VFSENTRY),
                        (int (*)(const void *, const void *))(vfs_IgnoreCase ? vfs_CmpNameI : vfs_CmpName));
            if (e) {
                *found = d;
                return e;
            }
        }
        if (recurse)
            for (d = d->child; d; d = d->next)
                if (e = vfs_FindEntry(name, d, found, 1))
                    return e;
    }
    return 0;
}
/* 0x42ef10, adds a file to a directory: to the node list while it is being filled, else into the sorted
   entries. Of two files with the same name the newer one (or the one with the lower extension index) wins.
   @deep behaviour: 41 code hunk lines are register roles (the new entry pointer e and the name copy in eax vs ecx, add eax,ecx vs add ecx,eax, the shared tail's store base eax vs ecx) plus 2 nop fill bytes at the end; every store, load and constant pair agrees, no statement is missing or extra (asmdiff; an earlier round found the same one-register difference for e). Register allocation - deep */

void vfs_AddFile(char *name, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, unsigned int ext, uLong date)
{
    char msg[0x400];
    char path[0x400];
/* */    VFSNODE *prev = 0;
    VFSNODE *n;
    VFSDIR *dir;
    VFSENTRY *e, *entries, *old, *dst;
    int lo, hi, mid, len, i, off, cnt;
    char *s, *src;

    if (d->sorted) {
        e = vfs_FindEntry(name, d, &dir, 0);
        if (!e) {
            if (d->entries) {
                lo = 0;
                hi = d->nentries - 1;
                while (hi >= lo) {
                    mid = (lo + hi) >> 1;
                    if (vfs_strcmp(name, d->entries[mid].name) > 0)
                        lo = mid + 1;
                    else
                        hi = mid - 1;
                }
                if (mid >= d->nentries)
                    mid = d->nentries;
                else if (vfs_strcmp(name, d->entries[mid].name) > 0)
                    mid++;
                len = strlen(name) + 1;
                for (i = 0; i < d->nentries; i++)
                    len += strlen(d->entries[i].name) + 1;
                entries = mem_Alloc(len + (d->nentries + 1) * sizeof(VFSENTRY), "vfs:AddFile(sorted, insert)");
                s = (char *)(entries + d->nentries + 1);
                for (lo = 0; lo < d->nentries; lo++) {
                    strcpy(s, d->entries[lo].name);
                    d->entries[lo].name = s;
                    s += strlen(d->entries[lo].name) + 1;
                }
                off = mid * sizeof(VFSENTRY);
                cnt = off;
                old = d->entries;
                VFS_COPY(entries, old, cnt);
                hi = d->nentries * sizeof(VFSENTRY) - off;
                src = (char *)d->entries + off;
                dst = entries + mid + 1;
                VFS_COPY(dst, src, hi);
                mem_Free(d->entries);
                d->entries = entries;
                e = (VFSENTRY *)((char *)entries + off);
            } else {
                len = strlen(name) + 1;
                d->nentries = 0;
                e = d->entries = mem_Alloc(len + sizeof(VFSENTRY), "vfs:AddFile(sorted, first)");
                s = (char *)(e + 1);
            }
            strcpy(s, name);
            e->name = s;
            e->date = date;
            e->zip = zip;
            e->pos = pos;
            e->num = num;
            e->ext = ext;
            d->nentries++;
        } else if (date > e->date || date == -1 || ext < (unsigned int)e->ext) {
            e->date = date;
            e->ext = ext;
            e->zip = zip;
            e->pos = pos;
            e->num = num;
        } else if (date == e->date && !zip) {
            e->zip = 0;
            e->pos = 0;
            e->num = 0;
            e->ext = 0;
        }
    } else {
        for (n = d->files; n; prev = n, n = n->next) {
            if (!vfs_strcmp(name, n->name)) {
                char mine[0x400] = "";
                char other[0x400] = "";
                vfs_DirPath(d, vfs_Root, path);
                if (n->zip) {
                    vfs_DirPath(n->zip, vfs_Root, mine);
                    *strrchr(mine, '/') = 0;
                    strcat(mine, vfs_Ext[n->ext]);
                    strcat(mine, " ");
                }
                if (zip) {
                    vfs_DirPath(zip, vfs_Root, other);
                    *strrchr(other, '/') = 0;
                    strcat(other, vfs_Ext[ext]);
                }
                sprintf(msg, "vfs: name collision: \"%s%s\" %s%s", path, name, mine, other);
                if (date > n->date || date == -1 || ext < (unsigned int)n->ext) {
                    n->date = date;
                    n->zip = zip;
                    n->pos = pos;
                    n->num = num;
                    n->ext = ext;
                } else if (date == n->date && !zip) {
                    n->zip = 0;
                    n->pos = 0;
                    n->num = 0;
                    n->ext = 0;
                }
                return;
            }
        }
        if (!d->files) {
            n = pool_Alloc(&vfs_EntryPool, sizeof(VFSNODE), 128);
            d->files = n;
        } else {
            n = pool_Alloc(&vfs_EntryPool, sizeof(VFSNODE), 128);
            prev->next = n;
        }
        strncpy(n->name, name, 0xff);
        n->zip = zip;
        n->pos = pos;
        n->num = num;
        n->ext = ext;
        n->date = date;
        n->next = 0;
    }
}
/* 0x42f4d0, adds a file given by its path below d, creating the directories on the way. */

void vfs_AddFilePath(char *path, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, int ext, uLong date)
{
    char buf[0x100];
    char sub[0x100];
    char *p, *q;

    if (path[strlen(path) - 1] != '/') {
        VFSDIR *cur = d;
        strcpy(buf, path);
        p = buf;
        for (q = strchr(buf, '/'); q; q = strchr(p, '/')) {
            strcpy(sub, p);
            strchr(sub, '/')[1] = 0;
            cur = vfs_IGetSubDir(sub, cur);
            p = q + 1;
        }
        vfs_AddFile(p, cur, zip, pos, num, ext, date);
    }
}
/* 0x42f5c0, qsort comparator of two entries by name. */

int vfs_CmpEntry(const VFSENTRY *a, const VFSENTRY *b)
{
    return strcmp(a->name, b->name);
}
/* 0x42f600, the same ignoring case. */

int vfs_CmpEntryI(const VFSENTRY *a, const VFSENTRY *b)
{
    return stricmp(a->name, b->name);
}
/* 0x42f620, moves the file nodes of a directory into one sorted array of entries (names stored behind the
   records), and with recurse also of its subdirectories and following siblings. */

void vfs_FinishedFiles(VFSDIR *d, char recurse)
{
    VFSNODE *n, *next;
    char *s;
    int count, len, i;

    if (d->files && !d->sorted) {
        len = 0;
        count = 0;
        for (n = d->files; n; n = n->next) {
            count++;
            len += strlen(n->name) + 1;
        }
        d->nentries = count;
        d->entries = mem_Alloc(len + count * sizeof(VFSENTRY), "vfs:FinishedFiles");
        s = (char *)(d->entries + d->nentries);
        if (n = d->files) {
            i = 0;
            do {
                strcpy(s, n->name);
                d->entries[i].name = s;
                d->entries[i].date = n->date;
                d->entries[i].zip = n->zip;
                d->entries[i].pos = n->pos;
                d->entries[i].num = n->num;
                d->entries[i].ext = n->ext;
                i++;
                s += strlen(n->name) + 1;
                next = n->next;
                pool_Free(&vfs_EntryPool, n);
            } while (n = next);
        }
        d->files = 0;
        if (vfs_IgnoreCase)
            qsort(d->entries, d->nentries, sizeof(VFSENTRY), (int (*)(const void *, const void *))vfs_CmpEntryI);
        else
            qsort(d->entries, d->nentries, sizeof(VFSENTRY), (int (*)(const void *, const void *))vfs_CmpEntry);
    } else if (!d->files)
        d->nentries = 0;
    d->sorted = 1;
    if (recurse) {
        if (d->child)
            vfs_FinishedFiles(d->child, 1);
        if (d->next)
            vfs_FinishedFiles(d->next, 1);
    }
}
/* 0x42f7e0, walks a directory path ("a/b/") down from d; 0 if a directory is missing. @deep behaviour: 26 code hunk lines are register roles and stack slots (dir ebx vs esi, the name copy edi/esi swapped, push edi moved, the string buffers at [esp+0xc/0x10] vs [esp+0x8/0xc] after the different push set), 64 regs hunk lines; the same calls (strcpy, vfs_strcmp/vfs_FindEntry in the loop) and constants agree, no statement differs (asmdiff, listing). Register/slot allocation - deep */

VFSDIR *vfs_FindDir(char *path, VFSDIR *d)
{
    char buf[0x100];
    char name[0x100];
    char *p;
    VFSDIR *dir = d;

    if (!path)
        return 0;
    if (*path) {
        strcpy(buf, path);
        vfs_UpperCase(buf);
        if (buf[strlen(buf) - 1] != '/')
            strcat(buf, "/");
        strcpy(name, buf);
        while ((p = strchr(name, '/')) && dir) {
            p++;
            strcpy(buf, p);
            *p = 0;
            for (dir = dir->child; dir && vfs_strcmp(dir->name, name); dir = dir->next)
                ;
            strcpy(name, buf);
        }
    }
    return dir;
}
/* 0x42f8f0, looks a file path up below d; *found gets the directory of the entry. A file name starting with
   '*' is searched in all subdirectories. */

VFSENTRY *vfs_FindFile(char *path, VFSDIR *d, VFSDIR **found)
{
    VFSDIR *dir = d;
    char buf[0x100];
    char name[0x100];
    VFSENTRY *e;
    char *p;

    strcpy(buf, path);
    vfs_UpperCase(buf);
    vfs_ToSlashes(buf);
    for (p = buf; *p && *p == '/'; p++)
        ;
    strcpy(name, p);
    if (p != buf)
        strcpy(buf, name);
    p = strrchr(buf, '/');
    if (p) {
        p++;
        strcpy(name, p);
        *p = 0;
        dir = vfs_FindDir(buf, dir);
        if (!dir)
            return 0;
    }
    if (name[0] == '*')
        e = vfs_FindEntry(name + 1, dir, &dir, 1);
    else
        e = vfs_FindEntry(name, dir, &dir, 0);
    if (e)
        *found = dir;
    return e;
}
/* 0x42fa10, builds the path of entry e (or of directory d without the trailing slash if e is 0) relative to
   the directory stop.
   @deep behaviour: 56 code hunk lines are the ebp/ebx role swap of the out buffer and the loop directory pointer (every differing instruction is mov/cmp/lea/rep with the other register, e.g. [ebp+0x110] vs [ebx+0x110], mov eax,ebx vs mov eax,ebp, plus that mov sitting one slot later between pop edi and pop esi in the epilogue = order 18); no statement, constant, callee or operand differs (asmdiff, listing). Register/slot allocation - deep */

char *vfs_EntryPath(VFSENTRY *e, VFSDIR *d, VFSDIR *stop, char *out)
{
    char tmp[0x100];
    VFSDIR *p = d;
    char *r;

    if (!p)
        r = 0;
    else {
    r = out;
    if (p != stop) {
        strcpy(out, p->name);
        if (e) {
            strcat(out, e->name);
            while (p->parent && (p = p->parent) != stop) {
                strcpy(tmp, p->name);
                strcat(tmp, out);
                strcpy(out, tmp);
            }
        } else {
            out[strlen(out) - 1] = 0;
            while (p->parent && (p = p->parent) != stop) {
                strcpy(tmp, p->name);
                strcat(tmp, out);
                strcpy(out, tmp);
            }
        }
    } else
        strcpy(out, e->name);
    }
    return r;
}
/* 0x42fbf0, builds the path of directory d relative to the directory stop. */

char *vfs_DirPath(VFSDIR *d, VFSDIR *stop, char *out)
{
    char tmp[0x100];
    VFSDIR *p = d;

    if (!p)
        return 0;
    strcpy(out, p->name);
    do {
        if (p->parent == stop)
            break;
        p = p->parent;
        strcpy(tmp, p->name);
        strcat(tmp, out);
        strcpy(out, tmp);
    } while (p);
    return out;
}
/* 0x42fca0, the path of a file relative to stop, 0 if it is not found. */

char *vfs_GetPath(char *name, VFSDIR *d, VFSDIR *stop, char *out)
{
    VFSENTRY *e;

    e = vfs_FindFile(name, d, &d);
    if (e) {
        vfs_EntryPath(e, d, stop, out);
        return out;
    }
    return 0;
}
/* 0x42fce0, adds a real directory (recursively) or an archive (path with one of the vfs_Ext extensions) to
   the directory tree below parent. With parent 0 it creates the root and finishes it. */

VFSDIR *vfs_AddDir(char *path, VFSDIR *parent)
{
    SYSTEMTIME st;
    FILETIME ft;
    char buf[0x100];
    char dir[0x100];
    WIN32_FIND_DATA fd;
    char name[0x100];
    char sub[0x100];
    VFSDIR *d = 0;
    HANDLE h;
    char *ext, *p;
    char found;
    int i, n;

    strcpy(dir, path);
    vfs_UpperCase(dir);
    if (!parent)
        vfs_ToSlashes(dir);
    ext = strrchr(dir, '.');
    if (((n = strlen(dir) - 1) <= 0 || dir[n] != '/' && dir[n] != '\\') && ext) {
        for (i = 0; i <= 6; i++)
            if (!vfs_strcmp(ext, vfs_Ext[i]))
                vfs_AddZip(dir, parent, i);
    } else {
        if (dir[strlen(dir) - 1] != '/')
            strcat(dir, "/");
        strcpy(buf, dir);
        strcat(buf, "*");
        strcpy(sub, buf);
        h = FindFirstFile(buf, &fd);
        if (h != INVALID_HANDLE_VALUE) {
            if (parent) {
                strcpy(buf, dir);
                if ((p = strrchr(buf, '/')) && (*p = 0, p = strrchr(buf, '/'))) {
                    strcpy(name, p + 1);
                    strcat(name, "/");
                } else
                    strcpy(name, "");
            } else
                strcpy(name, dir);
            d = vfs_IGetSubDir(name, parent);
            do {
                if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        strcpy(buf, dir);
                        strcat(buf, fd.cFileName);
                        strcat(buf, "/");
                        vfs_AddDir(buf, d);
                    } else {
                        found = 0;
                        strcpy(buf, fd.cFileName);
                        vfs_UpperCase(buf);
                        ext = strrchr(buf, '.');
                        if (ext) {
                            for (i = 0; i <= 6; i++) {
                                if (!vfs_strcmp(ext, vfs_Ext[i])) {
                                    strcpy(sub, dir);
                                    strcat(sub, buf);
                                    vfs_AddDir(sub, d);
                                    found = 1;
                                }
                            }
                        }
                        if (!found) {
                            FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft);
                            FileTimeToSystemTime(&ft, &st);
                            vfs_AddFile(buf, d, 0, 0, 0, 0,
                                        ((((st.wMonth << 5 | st.wDay) << 5 | st.wHour) << 6 | st.wMinute) << 5)
                                        | (st.wYear - 60) << 25 | st.wSecond >> 1);
                        }
                    }
                }
            } while (FindNextFile(h, &fd));
            FindClose(h);
        }
    }
    if (!parent && d)
        vfs_FinishedFiles(d, 1);
    return d;
}
/* 0x430130, adds the contents of a zip archive to the directory tree. */

void vfs_AddZip(char *path, VFSDIR *d, int ext)
{
    uLong pos, num;
    unz_file_info info;
    char name[0x100];
    unzFile zip;
    char *p;
    VFSDIR *r;

    zip = unzOpen(path);
    if (zip) {
        strcpy(name, path);
        *strrchr(name, '.') = 0;
        p = strrchr(name, '/');
        if (!p)
            p = name;
        else
            p++;
        strcat(p, "/");
        r = vfs_IGetSubDir(p, d);
        do {
            if (unzGetCurrentFileInfo(zip, &info, name, 0x100, 0, 0, 0, 0) == 0) {
                vfs_UpperCase(name);
                vfs_ToSlashes(name);
                if (info.uncompressed_size > 0) {
                    unzGetFilePos(zip, &pos, &num);
                    vfs_AddFilePath(name, r, r, pos, num, ext, info.dosDate);
                }
            }
        } while (unzGoToNextFile(zip) == 0);
        unzClose(zip);
    }
}
/* 0x430260, frees a directory list with its files and subdirectories. */

void vfs_FreeDir(VFSDIR *d)
{
    VFSDIR *next;
    VFSNODE *e, *enext;

    while (d) {
        next = d->next;
        for (e = d->files; e; e = enext) {
            enext = e->next;
            pool_Free(&vfs_EntryPool, e);
        }
        if (d->entries) {
            mem_Free(d->entries);
            d->entries = 0;
        }
        if (d->child)
            vfs_FreeDir(d->child);
        mem_Free(d);
        d = next;
    }
}
/* 0x4302e0, creates the real directories of a path, one level at a time. */

char vfs_MakeDirs(char *name, int unk)
{
    char path[0x100];
    char tmp[0x100];
    char dir[0x100];
    char base[0x100];
    struct _finddata_t fd;
    char *p;
    long h;

    if (!name || !*name)
        return 0;
    strcpy(path, name);
    p = strrchr(path, '/');
    if (p) {
        vfs_DirPath((VFSDIR *)unk, 0, base);
        p[1] = 0;
        strcpy(tmp, path);
        p = strchr(tmp, '/');
        while (p) {
            p++;
            strcpy(path, p);
            *p = 0;
            strcat(base, tmp);
            strcpy(dir, base);
            p = strrchr(dir, '/');
            if (p)
                *p = 0;
            h = _findfirst(dir, &fd);
            if (h == -1) {
                if (_mkdir(dir))
                    return 0;
            } else
                _findclose(h);
            strcpy(tmp, path);
            p = strchr(tmp, '/');
        }
    }
    return 1;
}
/* 0x430470, opens a file of the directory tree. mode: R read (default), W write, B binary (default), T text,
   Z gzip (default), N plain. Files inside zip archives can only be read, and only in binary mode. @deep match-base 2026-10-31: read against the original. The early argument check, the read-success tail and the write path !fp all reach one shared return f epilogue at the end (mov eax,ebp) in the original: rewritten as if (name && *name && mode && unk) { ... } return f; with the write success as if (fp) { ... } (486 vs 490 now, was 472). The two return 0 sites (zip text mode, MakeDirs) keep their own xor eax,eax epilogues on both sides. Left: f has a stack home in the original (frame 0x368 vs ours 0x364: mov [esp+0x24],ebp at entry, mov [esp+0x24],eax after the zip pool_Alloc, mov ebp,[esp+0x30] in notfound) and is cached in ebp, so VC6 does not fold f to 0: notfound and Could-write return f (mov eax,ebp / the reload) where ours returns xor eax,eax, and the notfound block stays after unzClose where ours moves it behind the fopen test; name is in edi / e in ebp in the original vs ebp / edi in ours (regs hunks 0x430527, 0x4309e8). Tried: f forced into memory (&f, experiment) 508 ins, worse; the zip loop with its own unzOpenCurrentFile / pool_Alloc copy instead of goto found 492, worse; early return 0 / MakeDirs return f: no change. */

VFSFILE *vfs_Tfopen(char *name, char *mode, int unk)
{
    VFSDIR *r;
    char *modes[2];
    VFSFILE *f = 0;
    char tmp[0x100];
    unz_file_info info;
    char path[0x100];
    char buf[0x100];
    unsigned char binary, z, read;
    VFSENTRY *e;
    unzFile zip;
    void *fp;
    char *p;
    int err;

    if (name && *name && mode && unk) {
        read = 1;
        binary = 1;
        z = 1;
        for (p = mode; *p; p++) {
            switch (toupper(*p)) {
            case 'B':
                binary = 1;
                break;
            case 'N':
                z = 0;
                break;
            case 'R':
                read = 1;
                break;
            case 'T':
                binary = 0;
                break;
            case 'W':
                read = 0;
                break;
            case 'Z':
                z = 1;
                break;
            }
        }
        if (read) {
            e = vfs_FindFile(name, (VFSDIR *)unk, &r);
            if (!e)
                goto notfound;
            if (e->zip) {
                if (!binary) {
                    err_Message("Unsupported Read-Mode in vfs:Open (textfile inside zip)");
                    return 0;
                }
                vfs_EntryPath(0, e->zip, 0, path);
                strcat(path, vfs_Ext[e->ext]);
                vfs_ToBackslashes(path);
                zip = unzOpen(path);
                if (!zip)
                    goto notfound;
                vfs_EntryPath(e, r, e->zip, buf);
                _strupr(buf);
                if (e->pos > 0) {
                    err = unzGoToFilePos(zip, e->pos, e->num);
                    if (unzGetCurrentFileInfo(zip, &info, tmp, 0x100, 0, 0, 0, 0) == 0) {
                        _strupr(tmp);
                        vfs_ToSlashes(tmp);
                        if (info.uncompressed_size > 0 && !strcmp(buf, tmp)) {
found:
                            err = unzOpenCurrentFile(zip);
                        }
                        if (!err) {
                            f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
                            f->zip = zip;
                            f->flags.checkgz = 1;
                            f->flags.inzip = 1;
                            goto opened;
                        }
                    }
                } else {
                    do {
                        if (unzGetCurrentFileInfo(zip, &info, tmp, 0x100, 0, 0, 0, 0) == 0) {
                            _strupr(tmp);
                            vfs_ToSlashes(tmp);
                            if (info.uncompressed_size > 0 && !strcmp(buf, tmp))
                                goto found;
                        }
                    } while (unzGoToNextFile(zip) == 0);
                }
                unzClose(zip);
notfound:
                sprintf(buf, "vfs_Tfopen: Could not find \"%s\"!!", name);
                return f;
            }
            modes[0] = "rt";
            modes[1] = "rb";
            vfs_EntryPath(e, r, 0, path);
            vfs_ToBackslashes(path);
            if (z)
                fp = gzopen(path, modes[binary]);
            else
                fp = fopen(path, modes[binary]);
            if (!fp)
                goto notfound;
            f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
            f->fp = fp;
            f->flags.inzip = 0;
opened:
            strcpy(f->name, name);
            f->flags.memory = 0;
            f->flags.read = 1;
            f->flags.binary = binary;
            f->flags.compressed = z;
            vfs_OpenCount++;
        } else {
            for (p = name; *p && (*p == '\\' || *p == '/'); p++)
                ;
            strcpy(tmp, p);
            vfs_ToSlashes(tmp);
            strcpy(buf, tmp);
            vfs_UpperCase(tmp);
            e = vfs_FindFile(tmp, (VFSDIR *)unk, &r);
            if (!e || e->zip) {
                if (!vfs_MakeDirs(buf, unk))
                    return 0;
                if (!e) {
                    vfs_AddFilePath(tmp, (VFSDIR *)unk, 0, 0, 0, 0, -1);
                    e = vfs_FindFile(tmp, (VFSDIR *)unk, &r);
                }
            }
            if (!e) {
                sprintf(buf, "vfs_Tfopen: Could write to \"%s\"!!", name);
                err_Message(buf);
                return f;
            }
            modes[0] = "wt";
            modes[1] = "wb";
            e->date = -1;
            e->zip = 0;
            e->pos = 0;
            vfs_EntryPath(e, r, 0, path);
            vfs_ToBackslashes(path);
            if (z)
                fp = gzopen(path, modes[binary]);
            else
                fp = fopen(path, modes[binary]);
            if (fp) {
                f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
                f->fp = fp;
                strcpy(f->name, name);
                f->flags.memory = 0;
                f->flags.read = 0;
                f->flags.binary = binary;
                f->flags.compressed = z;
                vfs_OpenCount++;
            }
        }
    }
    return f;
}
/* 0x430aa0, returns the size of a written memory file. */

int vfs_fclose(VFSFILE *f)
{
    int ret = 0;
    unsigned char *p;
    uLong x;
    int i;

    if (f) {
        if (f->flags.memory) {
            if (f->flags.compressed) {
                if (f->flags.read)
                    inflateEnd(&f->zs);
                else {
                    deflate(&f->zs, Z_FINISH);
                    deflateEnd(&f->zs);
                    if (f->zs.avail_out >= 8) {
                        p = f->zs.next_out;
                        x = f->crc;
                        for (i = 0; i < 4; i++) {
                            *p = (unsigned char)x;
                            x >>= 8;
                            p++;
                        }
                        x = f->zs.total_in;
                        for (i = 0; i < 4; i++) {
                            *p = (unsigned char)x;
                            x >>= 8;
                            p++;
                        }
                        f->zs.total_out += 8;
                        f->zs.next_out += 8;
                        f->zs.avail_out -= 8;
                    }
                }
            }
            if (!f->flags.read)
                ret = f->zs.total_out;
        } else if (f->fp) {
            if (f->flags.read && f->flags.inzip) {
                unzCloseCurrentFile(f->zip);
                unzClose(f->zip);
            } else if (f->flags.compressed)
                gzclose(f->gz);
            else
                fclose(f->fp);
        }
        vfs_OpenCount--;
        pool_Free(&vfs_FilePool, f);
    }
    return ret;
}
/* 0x430bd0, reads size * n bytes of an open VFS file into buf: from the memory image, from inside a zip
   archive (unzReadCurrentFile), inflated (gzread) or plain (fread); returns the number of bytes read (0 at
   the end of a memory image), (unsigned)-1 for a closed or write-only file */

int vfs_fread(void *buf, int size, int n, VFSFILE *f)
{
    char msg[0x100];
    unsigned int ret = -1;
    unsigned int len;
    z_stream *zs;
    void *fp;
    char mem;

    if (f && f->flags.read) {
        fp = f->fp;
        zs = &f->zs;
        if (fp || f->flags.memory) {
            len = size * n;
            mem = f->flags.memory;
            if (mem && !f->flags.compressed && f->zs.avail_in < len)
                len = f->zs.avail_in;
            if (len == 0)
                ret = len;
            else if (buf) {
                if (mem) {
                    if (f->flags.compressed) {
                        f->zs.next_out = buf;
                        f->zs.avail_out = len;
                        inflate(zs, Z_SYNC_FLUSH);
                        ret = len - f->zs.avail_out;
                    } else {
                    n = (int)fp;
                    VFS_COPY(buf, n, len);
                    ret = len;
                    zs->next_in += len;
                    f->zs.avail_in -= len;
                    f->zs.total_in += len;
                    }
                } else if (f->flags.inzip) {
                    ret = unzReadCurrentFile(fp, buf, len);
                    if (f->flags.checkgz && ret >= 2) {
                        f->flags.checkgz = 0;
                        if (((unsigned char *)buf)[0] == gz_magic[0] && ((unsigned char *)buf)[1] == gz_magic[1]) {
                            sprintf(msg, "vfs: gz-compressed file (\"%s\") in zip found!", f);
                        }
                    }
                } else if (f->flags.compressed)
                    ret = gzread(fp, buf, len);
                else
                    ret = fread(buf, size, n, fp);
            }
        }
    }
    return ret;
}
/* 0x430d90, reads one byte through vfs_fread; returns it as 0..255, or -1 at the end of the file */

int vfs_fgetc(VFSFILE *f)
{
    char c;

    if (vfs_fread(&c, 1, 1, f) == 1)
        return c;
    return -1;
}
/* 0x430dc0, reads a line; the line end (any run of CR/LF) is skipped. */

char *vfs_fgets(char *buf, int n, VFSFILE *f)
{
    int c, i;
    char *p;

    c = vfs_fgetc(f);
    if (c == -1)
        return 0;
    p = buf;
    i = 0;
    while (c != 13 && c != 10 && c != -1 && i < n) {
        *p++ = c;
        i++;
        c = vfs_fgetc(f);
    }
    *p = 0;
    if (c != -1) {
        while (c == 13 || c == 10)
            c = vfs_fgetc(f);
    }
    return buf;
}
/* 0x430e30 */

unsigned vfs_fwrite(void *buf, int size, int n, VFSFILE *f)
{
    unsigned int len;
    void *fp;
    char mem;
    int ret = -1;

    if (f && !f->flags.read) {
        fp = f->fp;
        if (fp || f->flags.memory) {
            len = size * n;
            mem = f->flags.memory;
            if (mem && !f->flags.compressed && f->zs.avail_out < len)
                len = f->zs.avail_out;
            if (len == 0)
                ret = 0;
            else if (buf) {
                if (mem) {
                    if (f->flags.compressed) {
                        f->zs.avail_in = len;
                        f->zs.next_in = buf;
                        deflate(&f->zs, Z_NO_FLUSH);
                        f->crc = crc32(f->crc, buf, len);
                        ret = len - f->zs.avail_in;
                    } else {
                    n = (int)f->zs.next_out;
                    VFS_COPY(n, buf, len);
                    f->zs.next_out += len;
                    f->zs.avail_out -= len;
                    f->zs.total_out += len;
                    ret = len;
                    }
                } else if (f->flags.compressed)
                    ret = gzwrite(fp, buf, len);
                else
                    ret = fwrite(buf, size, n, fp);
            }
        }
    }
    return ret;
}
/* 0x430fa0, files in a zip can only be seeked forward (by reading), or reopened for a backward SEEK_SET. */

int vfs_fseek(VFSFILE *f, long offset, int origin)
{
    int ret = -1;
    long cur;
    void *p;

    if (f && f->fp && !f->flags.memory) {
        if (f->flags.read) {
            if (f->flags.inzip) {
                f->flags.checkgz = 0;
                if (origin == SEEK_SET) {
                    cur = unztell(f->zip);
                    if (cur <= offset) {
                        if (cur == offset)
                            ret = 0;
                        else {
                        offset -= cur;
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_SET,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                        }
                    } else {
                    unzCloseCurrentFile(f->zip);
                    if (unzOpenCurrentFile(f->zip) == 0) {
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_SET,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                    }
                    }
                } else if (origin == SEEK_CUR) {
                    if (offset >= 0) {
                        if (offset == 0)
                            ret = 0;
                        else {
                        p = mem_Alloc(offset, "vfs:fseek(inzip,SEEK_CUR,skip)");
                        if (unzReadCurrentFile(f->zip, p, offset) == offset)
                            ret = 0;
                        mem_Free(p);
                        }
                    } else
                    err_Message("Unsupported vfs_fseek(inzip,SEEK_CUR)");
                } else {
                    err_Message("Unsupported vfs_fseek(inzip,SEEK_END)");
                }
            } else if (f->flags.compressed)
                ret = gzseek(f->gz, offset, origin) == -1 ? -1 : 0;
            else
                ret = fseek(f->fp, offset, origin) ? -1 : 0;
        } else
            err_Message("Seeking only supported in read-only files");
    }
    return ret;
}
/* 0x431170 */

long vfs_ftell(VFSFILE *f)
{
    long r = -1;

    if (f && (f->fp || f->flags.memory)) {
        if (f->flags.memory)
            r = f->zs.total_in;
        else if (f->flags.read) {
            if (f->flags.inzip)
                r = unztell(f->zip);
            else if (f->flags.compressed)
                r = gztell(f->gz);
            else
                r = ftell(f->fp);
        } else if (f->flags.compressed)
            r = gztell(f->gz);
        else
            r = ftell(f->fp);
    }
    return r;
}
/* 0x4311d0, opens a memory buffer as a file. mode: R read (default), W write, Z deflated, N not deflated;
   without Z or N a read buffer is deflated if it starts with the gzip magic.
   @deep behaviour: 39 code hunk lines are block layout and slots - the original merges the detect block's z=0 path with the else of the later if(z) (xor bl,bl; mov [esp+0x14],bl; the three f->zs store, then jmp over the deflated setup; the if(z) else jumps into the middle at 0x43129a), while ours lays the else out separately; the two branch polarities (size<=10: ours jbe to the else block, original ja over it; and jne/je at instruction 98) are the same tests, changed and unchanged statements and every constant (0xa, 0x1, gz_magic, 0x38, 0xf1) agree; local init order ([esp+0x14]/[esp+0x18]/[esp+0x13]) and 16 slot hunk lines are the frame. Tried and reverted: the if (size > 10 && magic) orientation (same layout). Block layout / slot allocation - deep */

VFSFILE *vfs_MemOpen(unsigned char *buf, unsigned int size, char *mode)
{
    VFSFILE *f;
    unsigned char z = 1;
    unsigned char read = 1;
    char detect = 1;
    char *p;

    if (!buf || size <= 0)
        return 0;
    for (p = mode; *p; p++) {
        switch (toupper(*p)) {
        case 'N':
            z = 0;
            detect = 0;
            break;
        case 'R':
            read = 1;
            break;
        case 'W':
            read = 0;
            break;
        case 'Z':
            z = 1;
            detect = 0;
            break;
        }
    }
    f = pool_Alloc(&vfs_FilePool, sizeof(VFSFILE), 16);
    f->flags.checkgz = 0;
    f->flags.inzip = 0;
    f->flags.binary = 1;
    f->flags.memory = 1;
    if (read) {
        if (detect) {
            if (size <= 10 || buf[0] != gz_magic[0] || buf[1] != gz_magic[1])
                z = 0;
            else
                z = 1;
        }
        if (z) {
            f->zs.next_in = buf + 10;
            f->zs.avail_in = size - 10;
            f->zs.next_out = 0;
            f->zs.avail_out = 0;
            f->zs.zalloc = 0;
            f->zs.zfree = 0;
            if (inflateInit2(&f->zs, -MAX_WBITS) != Z_OK)
                goto fail;
        } else {
            f->zs.next_in = buf;
            f->zs.avail_in = size;
            f->zs.total_in = 0;
        }
    } else if (z) {
        if (size < 18)
            goto fail;
        buf[0] = gz_magic[0];
        buf[1] = gz_magic[1];
        buf[2] = Z_DEFLATED;
        buf[3] = buf[4] = buf[5] = buf[6] = buf[7] = buf[8] = 0;
        buf[9] = 0x0b;
        f->zs.next_in = 0;
        f->zs.avail_in = 0;
        f->zs.next_out = buf + 10;
        f->zs.avail_out = size - 10;
        f->zs.zalloc = 0;
        f->zs.zfree = 0;
        f->crc = crc32(0, 0, 0);
        if (deflateInit2(&f->zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
fail:
            pool_Free(&vfs_FilePool, f);
            return 0;
        }
        f->zs.total_out = 10;
    } else {
        f->zs.next_out = buf;
        f->zs.avail_out = size;
        f->zs.total_out = 0;
    }
    vfs_OpenCount++;
    f->flags.read = read;
    f->flags.compressed = z;
    sprintf(f->name, "0x%08X -> length: %u, read: %u compressed: %u", buf, size, read, z);
    return f;
}
/* 0x431490, initializes the virtual file system: resets the open count, stores the caller's
   case-insensitivity flag in vfs_IgnoreCase and builds the directory tree from root (vfs_AddDir), logging
   root and the current directory; side effects: vfs_OpenCount / vfs_Root / vfs_EntryPool. Returns 1 when the
   root was added, 0 otherwise */

char vfs_Init(char *root, char unk)
{
    char cwd[0x100];
    char msg[0x100];

    vfs_OpenCount = 0;
    vfs_IgnoreCase = unk;
    vfs_Root = vfs_AddDir(root, 0);
    _getcwd(cwd, 0x100);
    sprintf(msg, "vfs_init: root -> \"%s\" relative to \"%s\"", root, cwd);
    vfs_EntryPool = 0;
    return vfs_Root != 0;
}
/* 0x431510, shuts the virtual file system down: frees the directory tree, warns (err_Message) about the
   files still open when warn is set, closes them, clears the entry pool and the case-insensitivity flag;
   side effects: vfs_Root / vfs_OpenCount / vfs_EntryPool / vfs_IgnoreCase */

void vfs_Exit(char warn)
{
    MPOOLITER it;
    char msg[0x100];
    VFSFILE *f;

    if (vfs_Root) {
        vfs_FreeDir(vfs_Root);
        vfs_Root = 0;
    }
    if (vfs_OpenCount && warn) {
        sprintf(msg, "%i vfs_file(s) were left open!", vfs_OpenCount);
        err_Message(msg);
    }
    if (vfs_FilePool) {
        for (f = pool_First(vfs_FilePool, &it); f; f = pool_Next(&it))
            vfs_fclose(f);
    }
    vfs_OpenCount = 0;
    if (vfs_EntryPool)
        pool_Destroy(&vfs_EntryPool);
    vfs_IgnoreCase = 0;
}
