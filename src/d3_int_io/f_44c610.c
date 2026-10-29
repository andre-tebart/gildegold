/* 0x44c610, builds surf from the stock object `name`: vertices and triangles, vertex colours from the colour
   key, the texture set; counts the surface (unless it is the shadow surface 3)
   @deep spell.py menu: both hunks 'no menu rule' (44c737 add ecx,edx / mov esi,ecx vs lea esi,[ecx+edx*1]; 44c9cf is the alignment nop that follows from the 1-byte length difference), diffclass: code. Read the hunk against tools/disasm.py 0x44c720 0x40: same value, only the add/lea spelling differs - VC6 computed '(D3IOTEX *)d3_Textures + tri->src->tex' into a temp (ecx, the dead index register) and copied it to tex (esi) at the store. Tried this session: the double-assignment form 'tri->tex = tex = ...' (byte-identical, reverted). Already tried in the 2026-10-22 session (unit note): int ti = tri->src->tex before the test, ti + (D3IOTEX *)d3_Textures, a D3TEXTURE * local with casts at the uses, &((D3IOTEX *)d3_Textures)[i], tex += i, (int)d3_Textures + (i << 7) / * 0x80 - all identical or worse. Readability done: Q4 via D3IOCOL union (all / b,g,r,a) and D3IOVERT.col40/col44 typed D3IOCOL, D3IO_214 macro body now the typed field D3_X(o)->unk214 (byte-neutral, check 2567). Left: the register-allocation choice that makes the sum's target esi instead of the dead operand - match-deep's toolbox. */

void d3io_AttachStock(D3OBJECT *obj, D3SURF *surf, char *name)
{
    D3IOCOL col;
    D3IOSTOCK *stock;
    D3IOSURF *s;
    D3IOTRI *tri;
    D3IOTEX *tex;
    D3MESH *mesh;
    char buf[0x100];
    int i, k;

    stock = (D3IOSTOCK *)d3io_FindStock(name);
    if (stock) {
        s = (D3IOSURF *)surf;
        s->nkeys = stock->npoints;
        s->ntris = stock->npolys;
        s->stock = stock;
        s->f194 = 0;
        d3_AllocObjectPolys((D3OBJINFO *)s);
        col.all = -1;
        if (s->nkeys > 0)
            for (i = 0; i < s->nkeys + 8; i++) {
                s->keys[i].src = stock->points + i * 0x18;
                s->keys[i].f4c = 0;
                s->keys[i].col40.all = -1;
                s->keys[i].col44.all = -1;
            }
        tri = s->tris;
        for (i = 0; i < s->ntris; i++, tri++) {
            tri->f20 = 0;
            tri->fl.b0 = 0;
            tri->f1c = 0;
            tri->f18 = 0;
            tri->src = (D3IOPOLY *)stock->polys + i;
            for (k = 0; k < 3; k++) {
                D3IOVERT *v = &s->keys[tri->src->idx[k]];
                tri->v[k] = v;
                v->f4c |= 0x80;
            }
            tri->f24 = 0;
            if (tri->src->tex >= 0) {
                tex = (D3IOTEX *)d3_Textures + tri->src->tex;
                tri->tex = tex;
                if (tex->colorkey < 0xff || (tex->flags & 3)) {
                    col.a = tex->colorkey;
                    if (tex->flags & 1) {
                        col.b = tex->colorkey;
                        col.g = tex->colorkey;
                        col.r = tex->colorkey;
                    } else {
                        col.b = 0xff;
                        col.g = 0xff;
                        col.r = 0xff;
                    }
                    for (k = 0; k < 3; k++) {
                        tri->v[k]->col40 = col;
                        tri->v[k]->col44 = col;
                    }
                }
                tri->fl.b3 = tex->f68;
                for (k = 0; k < 3; k++)
                    tri->v[k]->f4d = tex->f68 & 1;
            } else
                tri->tex = 0;
        }
        s->texSet = 0;
        s->texset = mem_Alloc(stock->ntex * 4, "d3_io:CurTexSet");
        for (i = 0; i < stock->ntex; i++)
            for (k = 0; k < s->ntris; k++)
                if (s->tris[k].src && s->tris[k].src->slot == i) {
                    if (s->tris[k].tex) {
                        s->texset[i] = s->tris[k].tex;
                        tex_AddRef((D3FLTEX *)s->texset[i], 1);
                    } else
                        sprintf(buf, "stock object %s, texture %s not found", stock, stock->texnames[i]);
                    break;
                }
        for (i = 0; i < stock->ntex; i++)
            if (!s->texset[i]) {
                strcpy(buf, "Error attaching Stock-Object \"");
                strcat(buf, stock->name);
                strcat(buf, "\": Not all Textures used/found!");
                break;
            }
        mesh = D3_MESH(obj);
        if (surf != &mesh->surf[3]) {
            mesh->nsurf++;
            D3IO_214(obj) = 4;
            if (!obj->info) {
                obj->info = d3_SelectSurf(obj);
                if (!obj->info) {
                    k = 0;
                    do {
                        if (k >= 3)
                            break;
                        if (D3_MESH(obj)->surf[k].info.ntris > 0 && D3_MESH(obj)->surf[k].info.nkeys > 0 &&
                            D3_MESH(obj)->surf[k].info.mesh)
                            obj->info = &D3_MESH(obj)->surf[k].info;
                        k++;
                    } while (!obj->info);
                }
            }
        }
        stock->refs++;
    }
}
