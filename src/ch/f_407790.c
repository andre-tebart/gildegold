/* 0x407790, the character's cells in ch_PathMap (transport, body, the next path cells), then a corner
   cell for each diagonal step
   @deep behaviour: close, 10 differing instructions. Two hunks: (1) 4077d7 slots - the address-taken x and z locals of ch_PosToCell are swapped (ours z,x at 0x1c/0x20; original x,z) and the same pair again at 0x2c/0x30; round-4 attempts in notes/units/ch.md (declaration order, inner block, int xz[2], reusing i/n, (void)z;, if(0) z=0;, all 6 store orders); (2) 40783c moved - original mov ebp,ecx; add ebp,edi for n = i + (ch->transport ? 2 : 1), ours add ecx,edi; mov ebp,ecx; spell moved prescriptions tried this session: split the init into two statements before the loop (byte-identical) and swapped the operand order (byte-identical). Statements read against 407790..4078a0: same bounds and calls. Readability done this session: u.walk.unk0f0 -> pathLen (cell count, indexes *2 into path) and u.walk.unk154 -> retryAt (ch_Time of the last failed path search + 200/225 ms wait, -1: none), Q2 clean. */

int ch_GetWaypoints(D3SM *h, CHARACTER *ch, CHWAYPT *out)
{
    CHACTION *a = CH_ACTIONS(ch);
    CHWAYPT *start = out;
    CHWAYPT *p;
    int x, z, i, n;
    unsigned char *c;

    if (ch->transport) {
        ch_PosToCell(ch_PathMap, &CH_TRANSPORT(ch)->obj->pos, &out->x, &out->z);
        out->flags = 8;
        out++;
    }
    ch_PosToCell(ch_PathMap, &ch->obj->pos, &x, &z);
    out->x = x;
    out->z = z;
    out->flags = 1;
    out++;
    if (a && a->type == '-' && a->u.walk.path && a->u.walk.retryAt == -1) {
        for (i = a->u.walk.cur, n = i + (ch->transport ? 2 : 1); i < n && i < a->u.walk.pathLen; i++) {
            c = (unsigned char *)a->u.walk.path + i * 2;
            ch_CellToCell(h, c[0], c[1], ch_PathMap, &out->x, &out->z);
            out->flags = 2;
            out++;
        }
    }
    out->flags = 0;
    for (p = start; p->flags; p++) {
        if (p->flags & 4 || !p[1].flags || p[1].flags & 4)
            break;
        if (p[1].x > p->x && p[1].z < p->z) {
            out->flags = p->flags | 4;
            out->x = p->x + 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z - 1;
            out++;
        } else if (p[1].x > p->x && p[1].z > p->z) {
            out->flags = p->flags | 4;
            out->x = p->x + 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z + 1;
            out++;
        } else if (p[1].x < p->x && p[1].z > p->z) {
            out->flags = p->flags | 4;
            out->x = p->x - 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z + 1;
            out++;
        } else if (p[1].x < p->x && p[1].z < p->z) {
            out->flags = p->flags | 4;
            out->x = p->x - 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z - 1;
            out++;
        }
    }
    out->flags = 0;
    return 1;
}
