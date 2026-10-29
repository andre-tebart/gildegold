/* 0x58f980, shows or hides the player's "vg_" objects (146 B): the 0x84-byte local list (count at +0x80) is
   filled by d3_ForEachObject (mask 0x2c0) with ai_VgFilter, then every entry is shown by d3if_ShowTree (obj, 0) or
   hidden by d3if_ShowTree (obj, 1); showAll == 0 shows all, else the entries from the index ai_VgHideFrom (when
   that byte is not 2) are hidden */

char d3if_ShowTree(struct D3OBJECT *obj, char show);

char d3_ForEachObject(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, char (*cb)(struct D3OBJECT *, void *), void *ctx);  /* 0x438d40 (d3.h) */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                /* 0x447300 (d3_interface.c) */

void ai_ShowVgObjects(int start, int showAll)
{
    int list[0x21];
    int i;
    int v;

    memset(list, 0, 0x84);
    d3_ForEachObject(d3_DefaultScene, (struct D3OBJECT *)start, 0x2c0,
                     (char (*)(struct D3OBJECT *, void *))ai_VgFilter, (AIVGLIST *)list);
    for (i = 0; i < list[0x20]; i++) {
        v = list[i];
        if (showAll == 0 ||
            (i >= (unsigned char)ai_VgHideFrom && (char)ai_VgHideFrom != 2))
            d3if_ShowTree((struct D3OBJECT *)v, 0);
        else
            d3if_ShowTree((struct D3OBJECT *)v, 1);
    }
}
