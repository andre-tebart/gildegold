/* 0x53e840, removes the gfx info `e` (frees its object, re-spreads the others on the same parent) */

void he_Unk53e840(int *e)
{
    int sv = U_I((unsigned)&d3_CurrentScene);
    int par = e[2];

    if (e[3] != 0) {
        e[2] = 0;
        d3if_SelectScene(0, 1);
        if (par != 0) {
            if (d3_FindObject(0, 0x1c0, par, 0) != 0)
                he_Unk53e4e0(par);
        }
        d3if_Delete((struct D3OBJECT *)e[3]);
        e[3] = 0;
        d3if_SelectScene(sv, 1);
    }
    e[1] = 0;
    e[0] = -1;
}
