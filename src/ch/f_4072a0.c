/* 0x4072a0, finds a walkable cell (type not 0 or 0xd) near x/z, searching rings of growing radius up to max;
   flags & 1 or 0x10: the one with the shortest way from -> cell -> to (0x10: x/z itself if walkable)
   @deep behaviour: no meaning difference left; the 54 code instructions and 24 fpu are the stack-slot permutation of the three D3VECTOR4 locals (ours dst,v,src at 0x18/0x28/0x38; original src,dst,v at 0x18/0x28/0x38) plus the loop temporaries living in the dead parameter slots; the fpu hunks are the same subtractions with the swapped slots (checked: fld src/fsub v both sides, 40759d). Round-4 attempts already in notes/units/ch.md: declaration order, renaming, struct copies, 12-byte src, scalar sx/sy/sz, dead init, swapped loop order. This session re-read 4072a0..407690 statement by statement: bounds min(1,z-r)/min(h->size-1,..) and the type tests are identical. Needs slotmap/vargen. Readability already clean (grade readable). */


char ch_FindFreeCell(D3SM *h, D3VECTOR *from, D3VECTOR *to, int x, int z, int *px, int *pz, int max, int flags)
{
    int r = 0;
    float best = 1e11f;
    D3VECTOR4 src, dst, v;
    int t, d, zi, xi, zmax, xmax;
    float dist, dx, dy, dz;

    if (h && h->entries && px && pz) {
        if (flags & 0x10) {
            t = h->entries[h->size * z + x].type;
            if (t && t != 0xd) {
                *px = x;
                *pz = z;
                return 1;
            }
            dst.x = to->x;
            dst.y = to->y;
            dst.z = to->z;
        } else
            d3sm_GetVertex(h, x, z, (D3VECTOR *)&dst);
        src.x = from->x;
        src.y = from->y;
        src.z = from->z;
        *px = -1;
        *pz = -1;
        if (!(flags & 1) && !(flags & 0x10)) {
            for (; r < h->size; r++) {
                if (r >= max)
                    break;
                zmax = min(h->size - 1, r + z);
                for (zi = max(1, z - r); zi <= zmax; zi++) {
                    d = r - abs(z - zi);
                    xmax = min(h->size - 1, d + x);
                    for (xi = max(0, x - d); xi <= xmax; xi++) {
                        if (zi >= 0 && zi < h->size && xi >= 0 && xi < h->size
                            && (t = h->entries[h->size * zi + xi].type) && t != 0xd) {
                            *px = xi;
                            *pz = zi;
                            return 1;
                        }
                    }
                }
            }
        } else {
            for (; r < h->size; r++) {
                if (r >= max)
                    break;
                zmax = min(h->size - 1, r + z);
                for (zi = max(1, z - r); zi <= zmax; zi++) {
                    d = r - abs(z - zi);
                    xmax = min(h->size - 1, d + x);
                    for (xi = max(0, x - d); xi <= xmax; xi++) {
                        if (zi >= 0 && zi < h->size && xi >= 0 && xi < h->size
                            && (t = h->entries[h->size * zi + xi].type) && t != 0xd
                            && d3sm_GetVertex(h, xi, zi, (D3VECTOR *)&v)) {
                            dx = src.x - v.x;
                            dy = src.y - v.y;
                            dz = src.z - v.z;
                            dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
                            dx = dst.x - v.x;
                            dy = dst.y - v.y;
                            dz = dst.z - v.z;
                            dist += (float)sqrt(dx * dx + dy * dy + dz * dz);
                            if (dist < best) {
                                best = dist;
                                *px = xi;
                                *pz = zi;
                            }
                        }
                    }
                }
            }
        }
        if (*px != -1 || *pz != -1)
            return 1;
        *px = x;
        *pz = z;
    }
    return 0;
}
