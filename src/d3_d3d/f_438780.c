/* 0x438780, d3.h: d3_EndScene */

void d3_EndScene(void)
{
#ifdef REA_FRAME_DUMP
    int rd_n;
#endif
#line 1264
    D3DCHECK(d3d_Device->lpVtbl->EndScene(d3d_Device));
#ifdef REA_FRAME_DUMP
    /* debug build (dd_vesa.h): while dump.trigger holds a count > 0, dump the back buffer after every scene */
    REA_DUMP_TRIGGER(rd_n);
    if (rd_n > 0)
        REA_DUMP_BACKBUFFER("scene");
#endif
}
