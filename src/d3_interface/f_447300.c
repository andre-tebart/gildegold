/* 0x447300, d3if_Show for an object and everything below it */

char d3if_ShowTree(D3OBJECT *obj, char show)
{
    if (obj) {
        d3_ForEachObject(d3_DefaultScene, obj, 0x3ff, (D3OBJECTCB)d3if_ShowCB, (void *)(unsigned char)show);
        return 1;
    }
    return 0;
}
