/* 0x53e5f0, creates the gfx object of the info `e` (name b, parent obj): a 0x446e10 object at the top of `obj`, flags, 0x433d70 with a 0x78 byte record (ROUGH) */

int he_Unk53e5f0(int e, int name, int obj)
{
    int o;
    float vec[3];
    float mn, mx;
    int st[0x1e];

    d3_PointToWorld(obj, obj + 0x4c, vec);
    d3t_GetHeightRange(obj, &mn, &mx);
    vec[1] = mx;
    o = d3_CreateObject(0, vec, (char *)(unsigned)&ai_Unk5ebba4, name);
    *(int *)(e + 0xc) = o;
    *(int *)(e + 8) = obj;
    if (o != 0) {
        *(unsigned char *)(o + 0x212) |= 0xc;
        *(unsigned char *)(o + 0x211) &= 0xfd;
        *(unsigned char *)(o + 0x213) &= 0xfb;
        *(int *)(o + 0x220) = 5;
        ((void (*)(void *, void *, int))d3_SetObjectAlpha)(o, (void *)(o + 0x1cc), 0x10080);
        d3t_SetColor(o, 0xc0, 0xa0, 0x80);
        st[0] = 0x8c0;
        st[0x1c] = 0x43490fdb;
        st[5] = 0;
        st[6] = 0;
        st[7] = 0;
        st[1] = 0;
        st[2] = 0;
        st[3] = 0;
        st[0x1b] = 0;
        st[0x1d] = 0;
        st[0x17] = 0;
        st[0x18] = 0;
        st[0x19] = 0;
        ((D3ANIM *(*)(void *, int, int, int, int, void *))d3anim_CreatePath)(o, 0, 0, 2, 2, st);
        he_Unk53e4e0(obj);
    }
    return o;
}
