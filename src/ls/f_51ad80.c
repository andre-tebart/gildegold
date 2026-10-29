/* 0x51ad80, the record r of a character scene (+0 / +8 .. , the names +0x170 / +0x130, the object's bounding box from +0x34, the word `id`, the 0x40 bytes of +0x64 / the 0x40 bytes of its parent's +0x68): writes the pieces; returns 1 when all was written @deep draft: one code hunk left, the 0x1c-byte zero substitute (7 mov DWORD PTR [esp+X],0 stores in the original at 0x51aea1..0x51aed1 vs our rep stosd; tried int[7]={0} (mov + rep stosd), char[0x1c]+memset (rep stosd ecx=7), a zero loop (rep stosd), two arrays, a struct with members and one with ={0} (all force ebp), seven separate ints (dead, frame 0x40) - VC6 always inlines the zero with rep stosd; the original's 7 immediate stores need the shape a vargen round can search). Everything else matches: 233/232 instructions, the field order/sizes, the blank[0x40] buffer, zero+0xc, the per-branch != 0x40 checks and the <= 0 LS_W checks; the two condition chains are written from the non-zero case (q != 0 && ...) as the original lays them out. Q1/Q2/Q4 remain: the function name, ch_EmptyName, and the raw pointer casts of the scene record field offsets. */


int unk_Unk51ad80(char *r, int id, int f)
{
    char blank[0x40];
    char *q;

    LS_W(r, 4)
    LS_W(r + 8, 0x20)
    LS_W(r + 0x2c, 4)
    LS_W(r + 0x30, 4)
    LS_W(r + 0x8c, 2)
    LS_W(r + 0x170, 0x30)
    LS_W(r + 0x130, 0x40)
    if (*(int *)(r + 0x88) == 0 || *(char *)(*(int *)(r + 0x88) + 0x3d5) != 0) {
        int zero[7] = {0};

        LS_W(zero, 0x10)
        LS_W((char *)zero + 0xc, 0xc)
    } else {
        LS_W(*(char **)(r + 0x34) + 0x4c, 0x10)
        LS_W(*(char **)(r + 0x34) + 0x84, 0xc)
    }
    LS_W(*(char **)(r + 0x34) + 0x218, 1)
    LS_W(&id, 4)
    q = *(char **)(r + 100);
    if (q != 0) {
        LS_W(q, 0x40)
    }
    else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        LS_W(blank, 0x40)
    }
    q = *(char **)(r + 0x74);
    if (q != 0 && q != *(char **)(r + 0x70) && *(int *)(q + 0x68) != 0) {
        if (vfs_fwrite(*(char **)(q + 0x68), 1, 0x40, f) != 0x40)
            return 0;
    } else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        if (vfs_fwrite(blank, 1, 0x40, f) != 0x40)
            return 0;
    }
    return 1;
}
