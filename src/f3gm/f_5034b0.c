/* 0x5034b0, tears down the extra parts of the object o: its record at +0x24 (flag 2 at +0x78: 0x4c4c60 message), its model at +0x3b (0x4028a0 in the scene 0x411be0 gives), and unlinks it from the owner's list (0x50a0f0, 0x5000c0 by id). ROUGH */

void gm_Unk5034b0(int o)
{
    int saved, r, m, sc;

    saved = d3_CurrentScene;
    r = *(int *)(o + 0x24);
    if (r != 0 && (*(unsigned char *)(r + 0x78) & 2) != 0)
        *(int *)(r + 0x84) = ((int (*)(int, int))cm_SendExSetHE)(-1, r);
    m = *(int *)(o + 0x3b);
    if (m != 0) {
        sc = d3t_SceneIndex(*(int *)(m + 0x88), 1);
        d3if_SelectScene(sc);
        ch_Delete(m);
        d3if_SelectScene(saved, 1);
    }
    r = gm_Unk50a0f0(o);
    if (r != 0)
        gm_RemoveOwned((int *)(r + 0x5d), *(int *)(o + 2));
}
