/* 0x474ca0, sets the two textures of a view from two picture names: clears the old textures, fills in the new
   pictures and redraws the view; returns 0 when the view is not active or not loaded, else 1 */

int d2_SetViewTextures(int view, char *tex1, char *tex2)
{
    D2VIEW *v = &d2_Views[view];

    if (v->active == 0)
        return 0;
    if (v->loaded == 0)
        return 0;
    if (tex1 != 0 && tex2 != 0) {
        d2_ClearViewTextures(view);
        d2_FillViewTextures(view, tex1, tex2);
        d2_RedrawView(view);
    }
    return 1;
}
