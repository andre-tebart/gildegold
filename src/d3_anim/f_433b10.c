/* 0x433b10, frees the object's path animation. @deep Q4: D3_OBJPATH(obj) (raw cast of D3OBJECT+0x1d0) is used in the body; the typed-field fix (struct D3PATH *path in D3OBJECT plus (obj)->path) made d3_MirDrawObject (d3_mir.c) stop matching by 8 bytes - the same scheduling flip as the rotation union attempt - and moved 4 bytes in d3_anim.c, so it was reverted. Q1 fixed (d3_Unk433b10 -> d3anim_FreePath). */

void d3anim_FreePath(D3OBJECT *obj)
{
    if (D3_OBJPATH(obj)) {
        mem_Free(D3_OBJPATH(obj)->keys);
        D3_OBJPATH(obj)->keys = 0;
        mem_Free(D3_OBJPATH(obj));
        D3_OBJPATH(obj) = 0;
    }
}
