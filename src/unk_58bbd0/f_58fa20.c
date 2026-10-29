/* 0x58fa20, whether a building site may be built on, shown for its object (310 B): a site is blocked (0) when a
   character of the build query (sm_BuildQuery (1, 6) .. gm_BuildNext) stands within 100.0 of the site object's
   owner (+0x1f8) or of one of its "bk_" / "!bk_" named children (+0x1fc list); 1 when free. The result is passed
   to ai_ShowVgObjects (obj, result), which shows / hides the "vg_" markers of the object */

void ai_CheckBuildable(int obj)
{
    AIOBJVIEW *view;
    AIOBJVIEW *owner;
    AIOBJVIEW *site;
    AIBLD *bld;
    float here[4];
    float there[4];
    int buildable;

    buildable = 1;
    owner = ((AIOBJVIEW *)obj)->parent;
    if (owner != 0) {
        d3_PointToWorld((struct D3OBJECT *)owner, (struct D3VECTOR *)((char *)owner + 0x4c),
                        (struct D3VECTOR *)here);
        for (bld = (AIBLD *)gm_BuildQuery(1, 6); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
            site = (AIOBJVIEW *)bld->object;
            if (site != 0) {
                d3_PointToWorld((struct D3OBJECT *)site, (struct D3VECTOR *)((char *)site + 0x4c),
                                (struct D3VECTOR *)there);
                if (d3_VecEqual((struct D3VECTOR *)there, (struct D3VECTOR *)here, 100.0f) != 0) {
                    buildable = 0;
                    break;
                }
            }
        }
    }
    for (view = ((AIOBJVIEW *)obj)->child; view != 0; view = view->next) {
        if (strncmp((char *)view, "bk_", 3) == 0 || strncmp((char *)view, "!bk_", 4) == 0) {
            d3_PointToWorld((struct D3OBJECT *)view, (struct D3VECTOR *)((char *)view + 0x4c),
                            (struct D3VECTOR *)here);
            for (bld = (AIBLD *)gm_BuildQuery(1, 6); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
                site = (AIOBJVIEW *)bld->object;
                if (site != 0) {
                    d3_PointToWorld((struct D3OBJECT *)site, (struct D3VECTOR *)((char *)site + 0x4c),
                                    (struct D3VECTOR *)there);
                    if (d3_VecEqual((struct D3VECTOR *)there, (struct D3VECTOR *)here, 100.0f) != 0) {
                        buildable = 0;
                        goto done;
                    }
                }
            }
        }
    }
done:
    ai_ShowVgObjects(obj, buildable);
}
