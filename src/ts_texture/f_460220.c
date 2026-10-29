/* 0x460220, loads the 8-bit pixels for the software renderer @deep B3: single code hunk at 0x46033c, 14 hunks / 8 asm lines, 11/352 bytes: write-address reassociation only - orig lea edx,[eax+ecx]; add eax,ecx (destructive row+x, store [eax+data]); ours lea edx,[ecx+eax]; add edx,ecx (data+x+row). spell.py: no menu rule on the hunk, verdict 'escalate with the diffclass verdict'. Read: full disasm of the inner loop (row from imul, size reloaded at the bottom in both), listing var map, notes/units/ts_texture.md. Tried by earlier match sessions (all canonicalise to these 11 B): ~900 spellings - index operand orders, pointer types/casts (360), byte temp, IV forms, i=y*size reuse (already in), x++ in index, declaration orders, n=i+x, while form. Base means exhausted; readability already readable. Remaining is VC6 register allocation of the two sums. */

void ts_LoadSoft(char *file, TSTEXTURE *tex)
{
    unsigned char remap[0x100];
    unsigned char used[0x100];
    unsigned char pal[0x300];
    unsigned char *data;
    unsigned int i, n, x, y;
    int count;

    if (ts_MaxHiCol) {
        data = mem_Alloc(tex->size * tex->size, "d3_ts:tLoadTextureSoft");
        bmp_Load(file, data, pal, 7);
        n = tex->size * tex->size;
        for (i = 0; i < n; i++)
            used[data[i]] = 1;
        count = 0;
        for (i = 0; i < 0x100; i++) {
            if (used[i]) {
                pal[count * 3] = pal[i * 3];
                pal[count * 3 + 1] = pal[i * 3 + 1];
                pal[count * 3 + 2] = pal[i * 3 + 2];
                remap[i] = count;
                count++;
            }
        }
        for (i = 0; i < tex->size * tex->size; i++)
            data[i] = remap[data[i]];
        tex->hicol = ts_GetHiColTab(pal, count, remap);
        tex->data = mem_Alloc(tex->size * tex->size, "d3_ts:LoadTextureSoft(data)");
        for (y = 0; y < tex->size; y++) {
            for (x = 0; x < tex->size; x++) {
                /* the row offset in the reused i (not a fresh local, which VC6 forward-substitutes)
                   keeps the remapped byte in bl like the original */
                i = y * tex->size;
                tex->data[i + x] = remap[data[i + x]];
            }
        }
        mem_Free(data);
    }
}
