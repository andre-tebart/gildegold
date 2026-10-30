/* 0x43a160, d3_ForEachObject callback: transforms, clips and projects a visible mesh and adds its
   triangles to the sort buffer (by texture with Direct3D, by depth in software).
   @deep behaviour draft: 408 code hunks - the register allocation differs from the first hunk (orig keeps the object in esi, the surf in edi, the mesh in ebp and the state byte in bl at 0x43a172..0x43a1d6; ours uses ebx/ecx) and whole blocks sit at other addresses (orig 0x43a256 state/info stores, 0x43a2be the z range and 0x43a3e8 the sort entry); reading also shows meaning differences in the draft (ours clears obj->state bit 2 with D3_OBJBITS(obj).dirty = 0 at 0x43a1cc -> and cl,0xfb, while the original only clears bit 3 -> and cl,0xf3 and keeps bit 2). The function is the round-3 first draft (notes/units/d3_engine.md: 'first draft, structure right, not iterated'); re-deriving it is match-deep work, not a reading fix */

char d3_DrawObjectCB(D3OBJECT *obj)
{
    D3SURF *s;
    D3MESH *m;
    unsigned char dirty;
    unsigned char clip;
    int base;
    int n;
    D3PTRI *t;
    float z;

    d3_Unk633494++;
    s = (D3SURF *)d3_SelectSurf(obj);
    if (!s)
        return 1;
    m = D3_MESH(obj);
    d3_TriCount += s->info.ntris;
    d3_VertCount += s->info.nkeys;
    obj->state &= 0xef;
    dirty = D3_OBJBITS(obj).dirty;
    D3_OBJBITS(obj).dirty = 0;
    ((D3CLIP *)&obj->state)->bottom = dirty;
    if (m->frameStamp == d3_FrameCount)
        m->flags |= 0x40;
    else if (dirty || (obj->state & 0x40))
        m->flags = d3_ClipObject(obj, &s->info, d3_Camera);
    clip = m->flags;
    if (clip & 0x40) {
        if ((obj->flags >> 1 | dirty) & obj->objectFlags >> 2 & d3_ShadowMode.mode & d3_Hardware & 1) {
            d3s_CountU = 0;
            if (dirty)
                d3s_UpdateObjectAt(obj);
            else
                d3s_UpdateObject(obj);
        }
        if (obj != d3s_Object && obj->type == 4 && m->lightGeneration < d3_LightGeneration && d3_LightKeysCulled < d3_LightKeyBudgetCulled) {
            d3_LightKeysCulled += s->info.nkeys;
            obj->state |= 4;
            d3_RelightObject(obj);
        }
        return 1;
    }
    d3_DrawnVerts += s->info.nkeys;
    d3_Unk633498++;
    base = 1;
    if (obj->state & 0x40) {
        obj->info = &s->info;
        dirty = 1;
        D3_OBJBITS(obj).dirty = 1;
        if (obj->type == 4 && obj != d3s_Object && (D3_MESH(obj)->lightGeneration < d3_LightGeneration || d3_LightsChanged))
            d3_RelightObject(obj);
        obj->state &= 0xbf;
    } else if (!(obj->state & 4)) {
        if (m->zMin <= d3_MinZ)
            d3_MinZ = m->zMin;
        if (m->zMax >= d3_MaxZ)
            d3_MaxZ = m->zMax;
    }
    if (obj->objectFlags >> 2 & d3_ShadowMode.mode & d3_Hardware & 1) {
        d3s_CountU = 0;
        d3s_UpdateObject(obj);
        if (d3s_CountU > 0)
            goto draw;
    }
    if (dirty || s->unk194) {
draw:
        if (s->info.mesh)
            D3_OPS(&s->info)->Transform(obj, &s->info, m->cameraMatrix);
        else
            d3_TransformVerts(obj, &s->info, m->cameraMatrix);
        d3_ClipMesh(*(D3CLIP *)&clip, (D3PVERT *)s->info.keys, s->info.nkeys, (D3PTRI *)s->info.tris, s->info.ntris);
        d3_ProjectMesh(obj);
    }
    if (obj != d3s_Object && obj->type == 4 && m->lightGeneration < d3_LightGeneration && d3_LightKeysVisible < d3_LightKeyBudgetVisible) {
        d3_LightKeysVisible += s->info.nkeys;
        d3_RelightObject(obj);
    }
    d3_MirBeginFrameE(obj, (D3OBJECT **)&d3_Mirror);
    if (obj == d3s_Object || s->flags.f.b[0] < 0xff || (s->flags.f.b[2] & 1))
        base = 0x4000;
    n = min(s->info.ntris, d3_SortMax - (int)d3_SortCount);
    t = (D3PTRI *)s->info.tris;
    if (d3_Hardware) {
        for (; n > 0; n--, t++) {
            if (t->flags & 0x80) {
                d3_SortPtr->item = t;
                if (t->tex) {
                    d3_SortPtr->key = (t->tex - d3_Textures) + base;
                    *(int *)((char *)t->tex + 0x54) = d3_FrameCount;
                } else
                    d3_SortPtr->key = 0;
                t->clip = d3_ClipInfo[t->flags & 0x3f];
                d3_SortPtr++;
                d3_SortCount++;
            }
        }
    } else {
        for (; n > 0; n--, t++) {
            if (t->flags & 0x80) {
                d3_SortPtr->item = t;
                z = t->v[0]->z;
                if (z < t->v[1]->z)
                    z = t->v[1]->z;
                if (z < t->v[2]->z)
                    z = t->v[2]->z;
                d3_FtoI((int *)&d3_SortPtr->key, d3_InvZFar * 16777216.0f * z * 3.0f);
                if (t->tex)
                    *(int *)((char *)t->tex + 0x54) = d3_FrameCount;
                t->clip = d3_ClipInfo[t->flags & 0x3f];
                d3_SortPtr++;
                d3_SortCount++;
            }
        }
    }
    return 1;
}
