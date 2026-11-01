/* bio.c -- original file name unknown: binary file i/o helpers on top of the vfs
   ("bio:bio_rd_array_quick" strings), plus crc32, a base-name helper and a config line reader.
   Extent in GildeGold.exe (inferred): 0x401000..0x401410; the bmp loader starts at 0x401410
   ("!bmp:" strings follow the "bio:" ones in .data). Functions are kept in their original order.
   Names are ours.
   unit of rea 8cef7d5b (src/game/bio.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bio.h"
#include "mem.h"

static char util_Line[0x100];            /* 0x630f50 */
const unsigned int bio_CrcTable[256] = { 0, 1996959894, 3993919788, 2567524794, 124634137, 1886057615, 3915621685, 2657392035, 249268274, 2044508324, 3772115230, 2547177864, 162941995, 2125561021, 3887607047, 2428444049, 498536548, 1789927666, 4089016648, 2227061214, 450548861, 1843258603, 4107580753, 2211677639, 325883990, 1684777152, 4251122042, 2321926636, 335633487, 1661365465, 4195302755, 2366115317, 997073096, 1281953886, 3579855332, 2724688242, 1006888145, 1258607687, 3524101629, 2768942443, 901097722, 1119000684, 3686517206, 2898065728, 853044451, 1172266101, 3705015759, 2882616665, 651767980, 1373503546, 3369554304, 3218104598, 565507253, 1454621731, 3485111705, 3099436303, 671266974, 1594198024, 3322730930, 2970347812, 795835527, 1483230225, 3244367275, 3060149565, 1994146192, 31158534, 2563907772, 4023717930, 1907459465, 112637215, 2680153253, 3904427059, 2013776290, 251722036, 2517215374, 3775830040, 2137656763, 141376813, 2439277719, 3865271297, 1802195444, 476864866, 2238001368, 4066508878, 1812370925, 453092731, 2181625025, 4111451223, 1706088902, 314042704, 2344532202, 4240017532, 1658658271, 366619977, 2362670323, 4224994405, 1303535960, 984961486, 2747007092, 3569037538, 1256170817, 1037604311, 2765210733, 3554079995, 1131014506, 879679996, 2909243462, 3663771856, 1141124467, 855842277, 2852801631, 3708648649, 1342533948, 654459306, 3188396048, 3373015174, 1466479909, 544179635, 3110523913, 3462522015, 1591671054, 702138776, 2966460450, 3352799412, 1504918807, 783551873, 3082640443, 3233442989, 3988292384, 2596254646, 62317068, 1957810842, 3939845945, 2647816111, 81470997, 1943803523, 3814918930, 2489596804, 225274430, 2053790376, 3826175755, 2466906013, 167816743, 2097651377, 4027552580, 2265490386, 503444072, 1762050814, 4150417245, 2154129355, 426522225, 1852507879, 4275313526, 2312317920, 282753626, 1742555852, 4189708143, 2394877945, 397917763, 1622183637, 3604390888, 2714866558, 953729732, 1340076626, 3518719985, 2797360999, 1068828381, 1219638859, 3624741850, 2936675148, 906185462, 1090812512, 3747672003, 2825379669, 829329135, 1181335161, 3412177804, 3160834842, 628085408, 1382605366, 3423369109, 3138078467, 570562233, 1426400815, 3317316542, 2998733608, 733239954, 1555261956, 3268935591, 3050360625, 752459403, 1541320221, 2607071920, 3965973030, 1969922972, 40735498, 2617837225, 3943577151, 1913087877, 83908371, 2512341634, 3803740692, 2075208622, 213261112, 2463272603, 3855990285, 2094854071, 198958881, 2262029012, 4057260610, 1759359992, 534414190, 2176718541, 4139329115, 1873836001, 414664567, 2282248934, 4279200368, 1711684554, 285281116, 2405801727, 4167216745, 1634467795, 376229701, 2685067896, 3608007406, 1308918612, 956543938, 2808555105, 3495958263, 1231636301, 1047427035, 2932959818, 3654703836, 1088359270, 936918000, 2847714899, 3736837829, 1202900863, 817233897, 3183342108, 3401237130, 1404277552, 615818150, 3134207493, 3453421203, 1423857449, 601450431, 3009837614, 3294710456, 1567103746, 711928724, 3020668471, 3272380065, 1510334235, 755167117 };  /* 0x5eb380, the zlib crc32 table */

/* 0x401000, CRC-32 of buf (table bio_CrcTable, initial value and final xor 0xffffffff) */
unsigned int util_Hash(unsigned char *buf, unsigned int len)
{
    unsigned int crc = 0xffffffff;

    for (; len > 0; len--) {
        crc = (crc >> 8) ^ bio_CrcTable[(crc & 0xff) ^ *buf];
        buf++;
    }
    return ~crc;
}
/* 0x401040, strips the directory and the extension (in place) */

char *util_BaseName(char *path)
{
    char *p, *name;

    p = strrchr(path, '\\');
    name = p + 1;
    if (!p)
        name = path;
    p = strrchr(name, '/');
    if (p)
        name = p + 1;
    p = strrchr(path, '.');
    if (p)
        *p = 0;
    return name;
}
/* 0x401090, 1 if the file exists in the vfs */

int file_Exists(char *path)
{
    return vfs_FindFile(path, (int)vfs_Root, (int *)&path) != 0;
}
/* 0x4010b0, the next line that is neither empty nor a ';' comment, trimmed and upper-cased */

char *util_ReadLine(VFSFILE *f)
{
    int i;
    char *p;

    while (vfs_fgets(util_Line, 0x100, f)) {
        for (i = strlen(util_Line) - 1; i > -1; i--) {
            if (util_Line[i] != '\n' && util_Line[i] != '\r' && util_Line[i] != '\t' && util_Line[i] != ' ')
                break;
        }
        if (i >= 0) {
            util_Line[i + 1] = 0;
            for (p = util_Line; *p; p++) {
                if (*p != '\n' && *p != '\r' && *p != '\t' && *p != ' ')
                    break;
            }
            if (*p && *p != ';') {
                _strupr(p);
                return p;
            }
        }
    }
    return 0;
}
/* 0x401180, reads one byte */

void bio_rd_c(VFSFILE *f, char *c)
{
    vfs_fread(c, 1, 1, f);
}
/* 0x4011a0, reads a zero-terminated string byte by byte (including the terminator) into s.
   stuck: the original tests the byte through s (cmp [eax],0 after mov eax,esi), VC6 tests it through p
   @deep B3: only code hunks 4011b5/4011ba (ours mov cl,[esi]; add esp; test cl,cl; mov eax,esi vs orig mov eax,esi; add esp; cmp [eax],0) + fill nop 4011cf; spell.py: no hunk matched the menu (escalate with diffclass verdict); this session tried *(s=p) as condition, bio_rd_c(f,++s), bio_rd_c(f,s=s+1), s=p before the call - same or shorter inc loop; note already tried while(*s)/*p/s[0]/!(*s==0)/uchar/volatile cast/for(;;)break/&&1/s++/++s/s=s+1/return s/extra pointer q=p. Left: VC6 CSE forwards the test load to p - register allocation/scheduling. */

void bio_rd_s(VFSFILE *f, char *s)
{
    char *p;

    s--;
    do {
        p = s + 1;
        bio_rd_c(f, p);
        s = p;
    } while (*s != 0);
}
/* 0x4011d0, reads a 4-byte value */

void bio_rd_i(VFSFILE *f, int *i)
{
    vfs_fread(i, 4, 1, f);
}
/* 0x4011f0, writes one byte */

void bio_wr_c(VFSFILE *f, char c)
{
    vfs_fwrite(&c, 1, 1, f);
}
/* 0x401210, writes a string with its terminator */

void bio_wr_s(VFSFILE *f, char *s)
{
    vfs_fwrite(s, strlen(s) + 1, 1, f);
}
/* 0x401240, writes a 4-byte value */

void bio_wr_i(VFSFILE *f, int i)
{
    vfs_fwrite(&i, 4, 1, f);
}
/* 0x401260, reads three 4-byte values (a vector, as raw bits) */

void bio_rd_v3(VFSFILE *f, int *v)
{
    bio_rd_i(f, v);
    bio_rd_i(f, v + 1);
    bio_rd_i(f, v + 2);
}
/* 0x401290, reads four 4-byte values */

void bio_rd_v4(VFSFILE *f, int *v)
{
    bio_rd_i(f, v);
    bio_rd_i(f, v + 1);
    bio_rd_i(f, v + 2);
    bio_rd_i(f, v + 3);
}
/* 0x4012d0, writes three 4-byte values */

void bio_wr_v3(VFSFILE *f, int *v)
{
    bio_wr_i(f, v[0]);
    bio_wr_i(f, v[1]);
    bio_wr_i(f, v[2]);
}
/* 0x401300, writes four 4-byte values */

void bio_wr_v4(VFSFILE *f, int *v)
{
    bio_wr_i(f, v[0]);
    bio_wr_i(f, v[1]);
    bio_wr_i(f, v[2]);
    bio_wr_i(f, v[3]);
}
/* 0x401340, writes an array: element count, element size, then the data */

void bio_wr_array(VFSFILE *f, int size, int n, void *data)
{
    bio_wr_i(f, n);
    bio_wr_i(f, size);
    vfs_fwrite(data, size, n, f);
}
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
