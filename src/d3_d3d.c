/* d3_d3d
   unit of rea 8cef7d5b (src/game/d3_d3d.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\Units\\d3\\d3_d3d.c"
/* d3_d3d.c -- original path D:\develop\Units\d3\d3_d3d.c
   Direct3D 6 device setup, render states, texture surfaces. Extent in GildeGold.exe (inferred, not
   proven): 0x4359f0..0x438c30; the functions before it use d3_anim.c's data, the ones after it no D3D.
   Functions are kept in their original order. Names are ours except where the D3DCHECK texts give them.
   @named_by leaked
   @boundary proven */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these with int parameters; they take chars. */
#define d3_BeginScene d3_BeginScene_int
#define d3_SetBlendState d3_SetBlendState_int
#include "d3.h"
#undef d3_BeginScene
#undef d3_SetBlendState
#include "d3_d3d.h"
extern D3DTEXFMTCTX d3d_UsedTexFormat;
#include "surf.h"
#include "di_input.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRIGGER / REA_DUMP_BACKBUFFER */
#endif
#include "data_views.h"
#include "bmp.h"
#include "d3_tools.h"
#include "dd_vesa.h"
#include "err.h"
#include "mem.h"
#include "ts_texture.h"
#define dd_BBitsC (*(char *)&dd_BBits)  /* 0x1464fbd, dd_BBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
#define dd_GBitsC (*(char *)&dd_GBits)  /* 0x1463580, dd_GBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
#define dd_RBitsC (*(char *)&dd_RBits)  /* 0x1463570, dd_RBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
extern unsigned char d3_MipSkip;

/* 0x4359f0, picks the texture formats closest to the wanted bit depths. */
HRESULT CALLBACK d3d_EnumTextureFormatsCallback(LPDDPIXELFORMAT pf, LPVOID lpctx)
{
    D3DTEXFMTCTX *ctx = lpctx;
    DWORD bpp = pf->dwRGBBitCount;
    char r = 0, g = 0, b = 0;
    DWORD m;
    int i;

    if (!ctx->found && (pf->dwFlags & DDPF_FOURCC) && ctx->fourcc && pf->dwFourCC == ctx->fourcc) {
        ctx->found = 1;
        ctx->bpp = pf->dwRGBBitCount;
        D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
    }
    if (!(pf->dwFlags & (DDPF_ALPHAPIXELS | DDPF_ALPHA)) && bpp >= 8) {
        if (bpp == 8) {
            if (!(pf->dwFlags & DDPF_PALETTEINDEXED8))
                return DDENUMRET_OK;
        } else if (bpp > 8 && !(pf->dwFlags & DDPF_RGB))
            return DDENUMRET_OK;
        if (!ctx->found
            && (!ctx->pf.dwRGBBitCount
                || bpp >= ctx->bpp && abs(bpp - ctx->bpp) < abs(ctx->pf.dwRGBBitCount - ctx->bpp)
                || bpp == ctx->pf.dwRGBBitCount
                   && pf->dwRBitMask + pf->dwGBitMask + pf->dwBBitMask
                      > ctx->pf.dwRBitMask + ctx->pf.dwGBitMask + ctx->pf.dwBBitMask))
            D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
        for (i = 0, m = pf->dwRBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                r++;
        for (i = 0, m = pf->dwGBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                g++;
        for (i = 0, m = pf->dwBBitMask; i < 32; i++, m >>= 1)
            if (m & 1)
                b++;
        if (!ctx->pf2.dwRGBBitCount
            || bpp >= ctx->bpp2 && abs(bpp - ctx->bpp2) < abs(ctx->pf2.dwRGBBitCount - ctx->bpp2)
            || bpp == ctx->pf2.dwRGBBitCount
               && pf->dwRBitMask + pf->dwGBitMask + pf->dwBBitMask
                  > ctx->pf2.dwRBitMask + ctx->pf2.dwGBitMask + ctx->pf2.dwBBitMask
               && !d3d_UsedTexFormat.exact) {
            if (r == dd_RBitsC && g == dd_GBitsC && b == dd_BBitsC)
                d3d_UsedTexFormat.exact = 1;
            D3_COPY(&ctx->pf2, pf, sizeof(DDPIXELFORMAT));
        }
    }
    return DDENUMRET_OK;
}
/* 0x435c50, fills the global d3d_UsedTexFormat for later texture loads: enumerates the device's
   texture formats into pf (closest to bpp, or the wanted fourcc when the device has it) and pf2 (the
   second choice for bpp2), then stores the resulting bit counts (4 for DXT1) and whether pf and pf2
   agree; returns nothing */

void d3d_FindTextureFormats(DWORD bpp, DWORD fourcc, DWORD bpp2)
{
    D3_ZERO_AT(offset d3d_UsedTexFormat, sizeof(d3d_UsedTexFormat));
    d3d_UsedTexFormat.bpp = bpp;
    d3d_UsedTexFormat.fourcc = fourcc;
    d3d_UsedTexFormat.bpp2 = bpp2;
    d3d_UsedTexFormat.found = 0;
    d3d_UsedTexFormat.exact = 0;
#line 195
    D3DCHECK(d3d_Device->lpVtbl->EnumTextureFormats(d3d_Device, d3d_EnumTextureFormatsCallback, &d3d_UsedTexFormat));
    if (d3d_UsedTexFormat.fourcc && !d3d_UsedTexFormat.found)
        d3d_UsedTexFormat.fourcc = 0;
    d3d_UsedTexFormat.bpp = d3d_UsedTexFormat.pf.dwRGBBitCount;
    if (d3d_UsedTexFormat.fourcc == MAKEFOURCC('D', 'X', 'T', '1'))
        d3d_UsedTexFormat.bpp = 4;
    d3d_UsedTexFormat.bpp2 = d3d_UsedTexFormat.pf2.dwRGBBitCount;
    if (d3d_UsedTexFormat.pf.dwRGBBitCount == d3d_UsedTexFormat.pf2.dwRGBBitCount
        && d3d_UsedTexFormat.pf.dwRBitMask == d3d_UsedTexFormat.pf2.dwRBitMask
        && d3d_UsedTexFormat.pf.dwGBitMask == d3d_UsedTexFormat.pf2.dwGBitMask
        && d3d_UsedTexFormat.pf.dwBBitMask == d3d_UsedTexFormat.pf2.dwBBitMask)
        d3d_UsedTexFormat.same = 1;
    else
        d3d_UsedTexFormat.same = 0;
}
/* 0x435d70, gets IDirect3D3 and picks the device: the one saved in the registry, else asks. */

char d3d_Init(int bitplanes, int a, int b)
{
    char guidstr[256];
    GUID guid;
    int h;
    int sel;
    char ask;
    char again;
    int i;

    if (dd_DD) {
#line 228
        D3DCHECK(dd_DD->lpVtbl->QueryInterface(dd_DD, &IID_IDirect3D3, &d3d_D3D));
        if (!d3d_hr) {
            d3d_NumDevices = 0;
#line 233
            D3DCHECK(d3d_D3D->lpVtbl->EnumDevices(d3d_D3D, d3d_EnumDeviceCallback, (void *)bitplanes));
            if (!d3d_hr && d3d_NumDevices) {
                for (sel = 0; sel < d3d_NumDevices; sel++)
                    if (!(d3d_Cards[sel].info.flags & 4))
                        break;
                if (sel >= d3d_NumDevices)
                    sel = 0;
                if (d3d_NumDevices > 1) {
                    ask = 1;
                    again = 0;
                    for (;;) {
                        if (again)
                            ask = 0;
                        h = reg_OpenKey("dd_vesa", 1);
                        if (h != -1) {
                            if (reg_GetString(h, "d3d_Guid", guidstr) && dd_StringToGuid(guidstr, &guid)) {
                                for (i = 0; i < d3d_NumDevices; i++) {
                                    if (!memcmp(&guid, &d3d_Cards[i].Guid, sizeof(GUID))) {
                                        ask = 0;
                                        sel = i;
                                    }
                                }
                            }
                            reg_CloseKey(h);
                        }
                        if (!ask)
                            break;
                        dd_DeviceDialog(bitplanes, b, a, 1, d3d_D3D);
                        again = 1;
                    }
                }
                D3_COPY_TO(offset d3d_UsedCard, &d3d_Cards[sel], sizeof(D3DCARD));
                d3d_Mipmaps = d3d_UsedCard.info.maxMipFilter > 1;
                d3d_ZBufferCaps = (d3d_UsedCard.info.flags & 4) ? 0x800 : 0x6000;
                return 1;
            }
        }
    }
    return 0;
}
/* 0x435fb0, clears the z buffer and the back buffer unless already done this frame. */

void d3d_Clear(D3RECT *r)
{
    D3DRECT mr;
    DWORD fl;

    if (!dd_ZCleared || !dd_Cleared) {
        fl = 0;
        if (d3d_ZBuffer && !dd_ZCleared)
            fl = D3DCLEAR_ZBUFFER;
        if (dd_BackBuffer && !dd_Cleared)
            fl |= D3DCLEAR_TARGET;
        if (fl) {
            mr.x1 = d3_Viewport.left;
            mr.y1 = d3_Viewport.top;
            mr.x2 = d3_Viewport.right;
            mr.y2 = d3_Viewport.bottom;
#line 325
            D3DCHECK(d3d_Viewport->lpVtbl->Clear2(d3d_Viewport, 1, &mr, fl, d3_fog_color.col, 1.0f, 0));
        }
        dd_Cleared = 1;
        dd_ZCleared = 1;
    }
}
/* 0x436070, picks the z buffer format closest to the wanted depth. */

HRESULT CALLBACK d3d_EnumZBufferFormatsCallback(LPDDPIXELFORMAT pf, LPVOID lpctx)
{
    D3DZBUFCTX *ctx = lpctx;
    DWORD bits = pf->dwZBufferBitDepth;

    if (pf->dwFlags == DDPF_ZBUFFER
        && (!ctx->pf.dwZBufferBitDepth
            || bits >= ctx->bpp && bits - ctx->bpp <= ctx->pf.dwZBufferBitDepth - ctx->bpp))
        D3_COPY(&ctx->pf, pf, sizeof(DDPIXELFORMAT));
    return DDENUMRET_OK;
}
/* 0x4360e0, draws one invisible triangle with the texture so that the driver loads it. */

void d3d_PrecacheTexture(LPDIRECT3DTEXTURE2 t)
{
    if (t) {
#line 359
        D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, t));
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3D_VERTEX_STRUCT, (void*)d3d_PrecacheBuffer, 3, 0));
    }
}
/* 0x436160, creates the z buffer, the device and the viewport. */

char d3d_CreateDevice(DWORD texbpp, DWORD fourcc, DWORD texbpp2, DWORD zbpp)
{
    DDSURFACEDESC2 ddsd;
    int i;

    d3d_ZEnabled = 0;
    d3d_MinMipSize = 4;
    d3_MipSkip = 0;
    d3d_balpha = 0;
    d3d_glow = 0;
    d3d_colorkey = 0;
    d3d_alphatest = 0;
    d3d_fog = 0;
    d3d_zbuffer = 1;
    d3d_Near = d3d_Far = d3d_ViewW = d3d_ViewH = d3d_ViewX = d3d_ViewY = -1.0f;
    if (d3d_UsedCard.info.caps & 0x20) {
        d3d_UsedZBufferFormat.bpp = zbpp;
#line 397
        D3DCHECK(d3d_D3D->lpVtbl->EnumZBufferFormats(d3d_D3D, &d3d_UsedCard.Guid, d3d_EnumZBufferFormatsCallback, (void *) &d3d_UsedZBufferFormat));
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.ddsCaps.dwCaps = d3d_ZBufferCaps | DDSCAPS_ZBUFFER;
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        ddsd.dwWidth = dd_Width;
        ddsd.dwHeight = dd_Height;
        D3_COPY(&ddsd.ddpfPixelFormat, offset d3d_UsedZBufferFormat.pf, sizeof(DDPIXELFORMAT));
#line 405
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &d3d_ZBuffer, NULL));
        if (d3d_hr)
            return 0;
        dd_SetSurfaceName(d3d_ZBuffer, "d3d_ZBuffer", 0);
#line 409
        D3DCHECK(dd_BackBuffer->lpVtbl->AddAttachedSurface(dd_BackBuffer, d3d_ZBuffer));
        if (d3d_hr)
            return 0;
    }
#line 413
    D3DCHECK(d3d_D3D->lpVtbl->CreateDevice(d3d_D3D, &d3d_UsedCard.Guid, dd_BackBuffer, &d3d_Device, NULL));
    if (d3d_hr)
        return 0;
#line 417
    D3DCHECK(d3d_D3D->lpVtbl->CreateViewport(d3d_D3D, &d3d_Viewport, NULL));
    if (d3d_hr)
        return 0;
#line 420
    D3DCHECK(d3d_Device->lpVtbl->AddViewport(d3d_Device, d3d_Viewport));
    if (d3d_hr)
        return 0;
#line 423
    D3DCHECK(d3d_Device->lpVtbl->SetCurrentViewport(d3d_Device, d3d_Viewport));
    if (d3d_hr)
        return 0;
    for (i = 0; i < 3; i++) {
        d3d_PrecacheBuffer[i].sx = (float)i;
        d3d_PrecacheBuffer[i].sy = (float)i;
        d3d_PrecacheBuffer[i].sz = 0.0f;
        d3d_PrecacheBuffer[i].rhw = 1.0f;
        d3d_PrecacheBuffer[i].color = 0xffffffff;
        d3d_PrecacheBuffer[i].tu = (float)i;
        d3d_PrecacheBuffer[i].tv = (float)i;
    }
    d3d_VertexBuffer = mem_Alloc(0x10000, "d3d:VertexBuffer");
    d3d_StatesSet = 0;
    d3d_FindTextureFormats(texbpp, fourcc, texbpp2);
    return 1;
}
/* 0x4364f0, the vertex buffer and its size in vertices. */

int d3d_GetVertexBuffer(void **buf, int *n)
{
    *buf = d3d_VertexBuffer;
    *n = 0x800;
    return 0;
}
/* 0x436510, draws a triangle list from the vertex buffer. */

void d3d_DrawTriangles(void *unk, D3DTLVERTEX *buf, DWORD num)
{
    if (num > 2) {
#line 496
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3D_VERTEX_STRUCT, buf, num, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTCLIP | D3DDP_DONOTLIGHT));
    }
}
/* 0x436560, applies the render options; the first call sets all states. */

void d3d_SetConfig(D3DCONFIG *c)
{
    int mipfilter;

    if (d3d_StatesSet) {
#line 515
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        if (d3d_Config.fillmode != c->fillmode)
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FILLMODE, c->fillmode));
        if (d3d_Config.mipfilter != c->mipfilter) {
            mipfilter = d3d_UsedCard.info.maxMipFilter;
            if (c->mipfilter < mipfilter)
                mipfilter = c->mipfilter;
#line 524
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MIPFILTER, mipfilter));
        }
        if (d3d_Config.perspective != c->perspective) {
            if ((d3d_UsedCard.info.caps & 4) && c->perspective) {
#line 530
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE));
            } else {
#line 532
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE));
            }
        }
        if (d3d_Config.filter != c->filter && (d3d_UsedCard.info.caps & 0x10)) {
            if (c->filter) {
#line 539
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR));
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_LINEAR));
            } else {
#line 544
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_POINT));
                D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_POINT));
            }
        }
        if (d3d_Config.antialias != c->antialias && (d3d_UsedCard.info.caps & 0x40)) {
            if (c->antialias) {
#line 552
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT));
            } else {
#line 554
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE));
            }
        }
        if (d3d_Config.dither != c->dither && (d3d_UsedCard.info.caps & 0x80))
#line 558
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DITHERENABLE, c->dither));
        if (d3d_Config.subpixel != c->subpixel && (d3d_UsedCard.info.flags & 1))
#line 561
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SUBPIXEL, c->subpixel));
        D3_COPY_AT(offset d3d_Config, c, sizeof(D3DCONFIG));
    } else {
#line 568
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FILLMODE, c->fillmode));
#line 574
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE));
#line 578
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE));
        if ((d3d_UsedCard.info.caps & 4) && c->perspective) {
#line 585
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE));
        } else {
#line 587
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, FALSE));
        }
        mipfilter = d3d_UsedCard.info.maxMipFilter;
        if (c->mipfilter < mipfilter)
            mipfilter = c->mipfilter;
#line 592
        D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MIPFILTER, mipfilter));
        if ((d3d_UsedCard.info.caps & 0x10) && c->filter) {
#line 596
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_LINEAR));
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR));
        } else {
#line 601
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MINFILTER, D3DTFG_POINT));
            D3DCHECK(d3d_Device->lpVtbl->SetTextureStageState(d3d_Device, 0, D3DTSS_MAGFILTER, D3DTFG_POINT));
        }
#line 605
        D3DCHECK(d3d_Device->lpVtbl->SetLightState(d3d_Device, D3DLIGHTSTATE_FOGMODE, D3DFOG_NONE));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE));
        if (c->antialias && (d3d_UsedCard.info.caps & 0x40)) {
#line 609
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT));
        } else {
#line 611
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE));
        }
        if (c->dither && (d3d_UsedCard.info.caps & 0x80))
#line 614
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DITHERENABLE, TRUE));
        if (c->subpixel && (d3d_UsedCard.info.flags & 1))
#line 617
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SUBPIXEL, c->subpixel));
        d3d_StatesSet = 1;
        D3_COPY_AT(offset d3d_Config, c, sizeof(D3DCONFIG));
    }
}
/* 0x436de0, sets the viewport and the projection; both are cached. */

void d3d_SetViewport(float w, float h, float x, float y, float zn, float zf)
{
    if (d3d_Device && d3d_Viewport) {
        if (w != d3d_ViewW || h != d3d_ViewH || x != d3d_ViewX || y != d3d_ViewY) {
            D3DVIEWPORT2 viewData;

            D3_ZERO(&viewData, sizeof(viewData));
            viewData.dwSize = sizeof(viewData);
            viewData.dwX = (DWORD)x;
            viewData.dwY = (DWORD)y;
            viewData.dwWidth = (DWORD)w;
            viewData.dwHeight = (DWORD)h;
            viewData.dvClipX = -1.0f;
            viewData.dvClipY = h / w;
            viewData.dvClipWidth = 2.0f;
            viewData.dvClipHeight = viewData.dvClipY * 2.0f;
            viewData.dvMinZ = 0.0f;
            viewData.dvMaxZ = 1.0f;
#line 660
            D3DCHECK(d3d_Viewport->lpVtbl->SetViewport2(d3d_Viewport, &viewData));
            d3d_ViewW = w;
            d3d_ViewH = h;
            d3d_ViewX = x;
            d3d_ViewY = y;
        }
        if (zn != d3d_Near || zf != d3d_Far) {
            D3DMATRIX mat;

            D3_ZERO(&mat, sizeof(mat));
            mat._44 = 1.0f;
            mat._33 = 1.0f;
            mat._22 = 1.0f;
            mat._11 = 1.0f;
#line 673
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_WORLD, &mat));
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_VIEW, &mat));
            mat._44 = zn;
            mat._34 = 1.0f;
            mat._33 = zn / (zf - zn) + 1.0f;
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_PROJECTION, &mat));
            d3d_Near = zn;
            d3d_Far = zf;
        }
    }
}
/* 0x437060, switches between z buffer (D3DZB_TRUE) and w buffer (D3DZB_USEW). */

void d3d_SetWBuffer(char w)
{
    if (d3d_Device && d3d_ZBuffer && (d3d_UsedCard.info.flags & 0x10) && w != d3d_ZEnabled) {
        d3d_UsedCard.info.zmode = (w != 0) + 1;
#line 699
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        d3d_ZEnabled = w;
    }
}
/* 0x4370d0, D3D device restore after the window is activated (main.c's WM_ACTIVATEAPP loop calls
   it after dd_Restore): a stub that always reports success, so the loop never waits; returns 1 */

char d3d_Restore(void)
{
    return 1;
}
/* 0x4370e0 */

void d3d_Exit(void)
{
    mem_Free(d3d_VertexBuffer);
    d3d_VertexBuffer = 0;
    if (d3d_ZBuffer) {
        if (dd_BackBuffer) {
#line 737
            D3DCHECK(dd_BackBuffer->lpVtbl->DeleteAttachedSurface(dd_BackBuffer, 0, d3d_ZBuffer));
        }
        d3d_RefCount = d3d_ZBuffer->lpVtbl->Release(d3d_ZBuffer);
        d3d_ZBuffer = 0;
    }
    if (d3d_Viewport) {
#line 742
        D3DCHECK(d3d_Device->lpVtbl->DeleteViewport(d3d_Device, d3d_Viewport));
        d3d_RefCount = d3d_Viewport->lpVtbl->Release(d3d_Viewport);
        d3d_Viewport = 0;
    }
    if (d3d_Device) {
        d3d_RefCount = d3d_Device->lpVtbl->Release(d3d_Device);
        d3d_Device = 0;
    }
    if (d3d_D3D) {
        d3d_RefCount = d3d_D3D->lpVtbl->Release(d3d_D3D);
        d3d_D3D = 0;
    }
    d3d_ZEnabled = 0;
}
/* 0x4371d0, fills the attached mip levels by scaling each level down into the next. */

void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd)
{
    DDSURFACEDESC2 ddsd_back;
    DDSURFACEDESC2 ddsd2;
    DDSCAPS2 ddscaps2;
    LPDIRECTDRAWSURFACE4 tex2;
    char locked = 0;

    if (tex) {
        ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
        if (tex->lpVtbl->GetAttachedSurface(tex, &ddscaps2, &tex2) == DD_OK) {
            if (!ddsd) {
                D3_ZERO(&ddsd_back, sizeof(ddsd_back));
                ddsd_back.dwSize = sizeof(ddsd_back);
#line 782
                D3DCHECK(tex->lpVtbl->Lock(tex, 0, &ddsd_back, DDLOCK_WAIT, NULL));
                ddsd = &ddsd_back;
                locked = 1;
            }
            D3_ZERO(&ddsd2, sizeof(ddsd2));
            ddsd2.dwSize = sizeof(ddsd2);
#line 790
            D3DCHECK(tex2->lpVtbl->Lock(tex2, 0, &ddsd2, DDLOCK_WAIT, NULL));
            surf_ScaleBlit(&ddsd2, ddsd);
            d3d_BuildMipmaps(tex2, &ddsd2);
#line 795
            D3DCHECK(tex2->lpVtbl->Unlock(tex2, NULL));
            if (locked) {
#line 798
                D3DCHECK(tex->lpVtbl->Unlock(tex, NULL));
            }
        }
    }
}
/* 0x437390, loads an image file into a texture surface of format pf (scaled to the card's limits),
   with mip levels; palettized formats get the image's palette, others a color key from color 0.
   bpp (the file's bits per pixel, which ts_texture passes) is not read. @deep behaviour: every code hunk is shape - the loadddsd/pal/adw stack-slot assignment (loadddsd at [ebp-0xb8] vs [ebp-0x9c], its field stores at +0x48/+0x68 move with it) and the dead-parameter slot reuse (stretcht/tempt in the w/h slots, [+0x1c] vs [+0x20], flipping between the level loop and the i loop), operand order [esi+eax]/[eax+esi] in the name copy, moved stores, tail nops; control flow and the w != ddsd.dwHeight quirk follow the original; readability met */

void d3d_LoadTexture(char *name, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext,
                     unsigned char mips, DWORD w, DWORD h, int bpp)
{
    char msg[512];
    char namebuf[512];
    PALETTEENTRY adw[256];
    BYTE pal[768];
    DDSURFACEDESC2 tempddsd;
    DDSURFACEDESC2 dest;
    DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps2;
    char num[12];
    DDSURFACEDESC2 loadddsd;
    DDCOLORKEY ddk;
    LPDIRECTDRAWPALETTE Palette;
    LPDIRECTDRAWSURFACE4 loadt;
    LPDIRECTDRAWSURFACE4 texture;
    /* stretcht, tempt in blocks */
    DWORD x;
    int levels;
    int m;
    int i;
    char *s, *d;

    ddk.dwColorSpaceLowValue = 0;
    ddk.dwColorSpaceHighValue = 0;
    loadt = 0;
    if (!d3d_Mipmaps)
        mips = 0;
    if (*surf) {
        d3d_RefCount = (*surf)->lpVtbl->Release(*surf);
        *surf = 0;
    }
    if (*surftext) {
        d3d_RefCount = (*surftext)->lpVtbl->Release(*surftext);
        *surftext = 0;
    }
    D3_ZERO(pal, sizeof(pal));
    D3_ZERO(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    ddsd.ddpfPixelFormat = *pf;
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    x = w >> d3_MipSkip;
    if (x > d3d_UsedCard.info.maxw)
        ddsd.dwWidth = d3d_UsedCard.info.maxw;
    else if (x < d3d_UsedCard.info.minw)
        ddsd.dwWidth = d3d_UsedCard.info.minw;
    else
        ddsd.dwWidth = x;
    x = h >> d3_MipSkip;
    if (x > d3d_UsedCard.info.maxh)
        ddsd.dwHeight = d3d_UsedCard.info.maxh;
    else if (x < d3d_UsedCard.info.minh)
        ddsd.dwHeight = d3d_UsedCard.info.minh;
    else
        ddsd.dwHeight = x;
    if (pf->dwFlags & DDPF_PALETTEINDEXED8) {
        D3_ZERO(&loadddsd, sizeof(loadddsd));
        loadddsd.dwSize = sizeof(loadddsd);
        loadddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        loadddsd.dwHeight = h;
        loadddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        loadddsd.dwWidth = w;
        loadddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        loadddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
        loadddsd.ddpfPixelFormat.dwRGBBitCount = 8;
        D3_COPY3(&dest, &loadddsd, sizeof(dest));
#line 872
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &loadddsd, &loadt, NULL));
        dd_SetSurfaceName(loadt, "loadt8", 0);
        D3DCHECK(loadt->lpVtbl->Lock(loadt, 0, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL));
        bmp_Load(name, (unsigned char *)loadddsd.lpSurface, pal, 7);
    } else {
        D3_ZERO(&loadddsd, sizeof(loadddsd));
        loadddsd.dwSize = sizeof(loadddsd);
        loadddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        loadddsd.dwHeight = h;
        loadddsd.dwWidth = w;
        loadddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        loadddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        loadddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
        loadddsd.ddpfPixelFormat.dwRGBBitCount = 24;
        loadddsd.ddpfPixelFormat.dwRBitMask = D3D_RGB24_RMASK;
        loadddsd.ddpfPixelFormat.dwGBitMask = D3D_RGB24_GMASK;
        loadddsd.ddpfPixelFormat.dwBBitMask = D3D_RGB24_BMASK;
        D3_COPY3(&dest, &loadddsd, sizeof(dest));
#line 892
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &loadddsd, &loadt, NULL));
        dd_SetSurfaceName(loadt, "loadt24", 0);
        D3DCHECK(loadt->lpVtbl->Lock(loadt, 0, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL));
        bmp_Load(name, (unsigned char *)loadddsd.lpSurface, pal, 9);
    }
    if (w != ddsd.dwHeight || h != ddsd.dwHeight) {
        LPDIRECTDRAWSURFACE4 stretcht;

        dest.dwHeight = ddsd.dwHeight;
        dest.dwWidth = ddsd.dwWidth;
        dest.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
#line 904
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &dest, &stretcht, NULL));
        dd_SetSurfaceName(stretcht, "stretcht", 0);
        D3DCHECK(stretcht->lpVtbl->Lock(stretcht, 0, &dest, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
        surf_ScaleBlit(&dest, &loadddsd);
        D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
        d3d_RefCount = loadt->lpVtbl->Release(loadt);
        loadt = stretcht;
        D3_COPY3(&loadddsd, &dest, sizeof(loadddsd));
    }
#line 913
    D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
    if ((max(ddsd.dwHeight, ddsd.dwWidth) >> mips) >= d3d_MinMipSize)
        levels = mips + 1;
    else
        for (m = max(ddsd.dwHeight, ddsd.dwWidth), levels = 0; m >= d3d_MinMipSize; m >>= 1)
            levels++;
    if (levels > 1) {
        ddsd.ddsCaps.dwCaps |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
        ddsd.dwFlags |= DDSD_MIPMAPCOUNT;
        ddsd.dwMipMapCount = levels;
    }
    ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
#line 931
    D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, surf, NULL));
    texture = *surf;
    if (!texture) {
        sprintf(msg, "Could not load create surface for %s", name);
        err_Message(msg);
        return;
    }
    dd_SetSurfaceName(texture, name, 0);
#line 944
    D3DCHECK(texture->lpVtbl->QueryInterface(texture, &IID_IDirect3DTexture2, (LPVOID *)surftext));
    for (i = 0; i < 256; i++) {
        adw[i].peFlags = 0xff;
        adw[i].peRed = pal[i * 3];
        adw[i].peGreen = pal[i * 3 + 1];
        adw[i].peBlue = pal[i * 3 + 2];
    }
    adw[0].peFlags = 0;
    if (pf->dwRGBBitCount <= 8 && (pf->dwFlags & DDPF_PALETTEINDEXED8)) {
        Palette = 0;
#line 957
        D3DCHECK(dd_DD->lpVtbl->CreatePalette(dd_DD, DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_ALPHA, adw, &Palette, NULL));
        D3DCHECK(texture->lpVtbl->SetPalette(texture, Palette));
        d3d_RefCount = Palette->lpVtbl->Release(Palette);
        Palette = 0;
    } else
        ddk.dwColorSpaceHighValue = ddk.dwColorSpaceLowValue = RGB_MAKE(pal[0], pal[1], pal[2]);
#line 967
    D3DCHECK(loadt->lpVtbl->SetColorKey(loadt, DDCKEY_SRCBLT, &ddk));
    if (pf->dwFlags & DDPF_FOURCC) {
#line 970
        D3DCHECK(texture->lpVtbl->Blt(texture, NULL, loadt, NULL, DDBLT_WAIT, NULL));
    } else {
#line 973
        D3DCHECK(texture->lpVtbl->SetColorKey(texture, DDCKEY_SRCBLT, &ddk));
        D3DCHECK(loadt->lpVtbl->Lock(loadt, NULL, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
        if (!d3d_hr) {
#line 977
            D3DCHECK(texture->lpVtbl->Lock(texture, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
            if (!d3d_hr) {
                surf_Blit(&ddsd, 0, &loadddsd, adw);
#line 981
                D3DCHECK(texture->lpVtbl->Unlock(texture, NULL));
            }
#line 983
            D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
        }
    }
    if (levels > 1) {
        ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
#line 992
        D3DCHECK(texture->lpVtbl->GetAttachedSurface(texture, &ddscaps2, &texture));
        if (!d3d_hr)
            texture->lpVtbl->Release(texture);
        for (i = 1; i < levels; i++) {
            LPDIRECTDRAWSURFACE4 tempt;

            _itoa(i, num, 10);
            for (s = name, d = namebuf; (*d++ = *s++) != 0; )
                ;
            strcat(namebuf, "_");
            strcat(namebuf, num);
            dd_SetSurfaceName(texture, namebuf, 0);
            D3_COPY3(&tempddsd, &dest, sizeof(tempddsd));
            tempddsd.dwWidth = loadddsd.dwWidth >> i;
            tempddsd.dwHeight = loadddsd.dwHeight >> i;
            tempddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
#line 1013
            D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &tempddsd, &tempt, NULL));
            dd_SetSurfaceName(tempt, "tempt(mipmap)", 0);
#line 1016
            D3DCHECK(loadt->lpVtbl->Lock(loadt, NULL, &loadddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
            if (!d3d_hr) {
#line 1019
                D3DCHECK(tempt->lpVtbl->Lock(tempt, NULL, &tempddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
                if (!d3d_hr) {
                    surf_ScaleBlit(&tempddsd, &loadddsd);
#line 1023
                    D3DCHECK(tempt->lpVtbl->Unlock(tempt, NULL));
                }
#line 1025
                D3DCHECK(loadt->lpVtbl->Unlock(loadt, NULL));
            }
#line 1027
            D3DCHECK(tempt->lpVtbl->SetColorKey(tempt, DDCKEY_SRCBLT, &ddk));
            if (ddsd.ddpfPixelFormat.dwFourCC) {
#line 1029
                D3DCHECK(texture->lpVtbl->Blt(texture, NULL, tempt, NULL, DDBLT_WAIT, NULL));
            } else {
#line 1032
                D3DCHECK(texture->lpVtbl->SetColorKey(texture, DDCKEY_SRCBLT, &ddk));
                D3DCHECK(texture->lpVtbl->Lock(texture, NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, NULL));
                if (!d3d_hr) {
#line 1036
                    D3DCHECK(tempt->lpVtbl->Lock(tempt, NULL, &tempddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL));
                    if (!d3d_hr) {
                        surf_Blit(&ddsd, 0, &tempddsd, adw);
#line 1040
                        D3DCHECK(tempt->lpVtbl->Unlock(tempt, NULL));
                    }
#line 1042
                    D3DCHECK(texture->lpVtbl->Unlock(texture, NULL));
                }
            }
            d3d_RefCount = tempt->lpVtbl->Release(tempt);
            tempt = 0;
            if (i < levels - 1) {
                ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
#line 1054
                D3DCHECK(texture->lpVtbl->GetAttachedSurface(texture, &ddscaps2, &texture));
                if (!d3d_hr)
                    texture->lpVtbl->Release(texture);
            }
        }
    }
    d3d_RefCount = loadt->lpVtbl->Release(loadt);
}
/* 0x438180, gives a palettized surface and its mip levels the palette and the color key. */

void d3d_SetSurfacePalette(DDSURFACEDESC2 *desc, LPDIRECTDRAWSURFACE4 surf, DWORD n, BYTE *pal)
{
    PALETTEENTRY adw[256];
    DDSURFACEDESC2 ddsd_back;
    DDSCAPS2 ddscaps2;
    DDCOLORKEY ddk;
    LPDIRECTDRAWPALETTE Palette;
    DWORD i;

    ddk.dwColorSpaceLowValue = 0;
    ddk.dwColorSpaceHighValue = 0;
    if (surf) {
        if (!desc) {
            ddsd_back.dwSize = sizeof(ddsd_back);
#line 1089
            D3DCHECK(surf->lpVtbl->GetSurfaceDesc(surf, &ddsd_back));
            desc = &ddsd_back;
        }
        if (n && desc->ddpfPixelFormat.dwRGBBitCount <= 8 && (desc->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)) {
            D3_ZERO(adw, sizeof(adw));
            for (i = 0; i < n; i++) {
                adw[i].peFlags = 0xff;
                adw[i].peRed = pal[i * 3];
                adw[i].peGreen = pal[i * 3 + 1];
                adw[i].peBlue = pal[i * 3 + 2];
            }
            adw[0].peFlags = 0;
#line 1106
            D3DCHECK(dd_DD->lpVtbl->CreatePalette(dd_DD, DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_ALPHA, adw, &Palette, NULL));
            D3DCHECK(surf->lpVtbl->SetPalette(surf, Palette));
            d3d_RefCount = Palette->lpVtbl->Release(Palette);
        }
        while (surf) {
#line 1116
            D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, NULL));
            if (desc->ddpfPixelFormat.dwRGBBitCount <= 8 && (desc->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)) {
                if (n) {
#line 1120
                    D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, &ddk));
                }
            } else {
                ddk.dwColorSpaceLowValue = 0;
                ddk.dwColorSpaceHighValue = 0;
#line 1126
                D3DCHECK(surf->lpVtbl->SetColorKey(surf, DDCKEY_SRCBLT, &ddk));
            }
            ddscaps2.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
            if (surf->lpVtbl->GetAttachedSurface(surf, &ddscaps2, &surf) != DD_OK)
                return;
            surf->lpVtbl->Release(surf);
        }
    }
}
/* 0x4383d0, creates a (dynamic) texture surface of size x size with mip levels down to 1 >> mips. */

void d3d_CreateTexture(DWORD size, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext,
                       DWORD n, BYTE *pal, unsigned char mips)
{
    DDSURFACEDESC2 ddsd;
    int levels;
    int m;

    if (*surf) {
        d3d_RefCount = (*surf)->lpVtbl->Release(*surf);
        *surf = 0;
    }
    if (*surftext) {
        d3d_RefCount = (*surftext)->lpVtbl->Release(*surftext);
        *surftext = 0;
    }
    if (!d3d_Mipmaps)
        mips = 0;
    if (pf->dwFlags & DDPF_FOURCC)
        err_Message("Bad dynamic pixel format...");
    D3_ZERO(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    if (size > d3d_UsedCard.info.maxw) {
        ddsd.dwWidth = d3d_UsedCard.info.maxw;
        ddsd.dwHeight = d3d_UsedCard.info.maxw;
    } else if (size < d3d_UsedCard.info.minw) {
        ddsd.dwWidth = d3d_UsedCard.info.minw;
        ddsd.dwHeight = d3d_UsedCard.info.minw;
    } else {
        ddsd.dwWidth = size;
        ddsd.dwHeight = size;
    }
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddpfPixelFormat = *pf;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    if ((int)(max(ddsd.dwHeight, ddsd.dwWidth) >> mips) > 0)
        levels = mips + 1;
    else
        for (m = max(ddsd.dwHeight, ddsd.dwWidth), levels = 0; m > 0; m >>= 1)
            levels++;
    if (levels > 1) {
        ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
        ddsd.dwMipMapCount = levels;
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    } else
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE | DDSCAPS2_HINTDYNAMIC;
#line 1208
    D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, surf, NULL));
    dd_SetSurfaceName(*surf, "texture(createtexture(dyn))", 0);
    d3d_SetSurfacePalette(&ddsd, *surf, n, pal);
    D3DCHECK((*surf)->lpVtbl->QueryInterface(*surf, &IID_IDirect3DTexture2, (LPVOID *) surftext));
}
/* 0x4385a0, d3.h: d3_BeginScene */

void d3_BeginScene(unsigned char fog, DWORD fogcol, unsigned char zbuffer)
{
#line 1227
    D3DCHECK(d3d_Device->lpVtbl->BeginScene(d3d_Device));
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, NULL));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHAREF, 0xbf));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER));
    if (fog) {
#line 1233
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGCOLOR, fogcol));
    }
#line 1234
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, fog));
#line 1236
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, zbuffer ? d3d_UsedCard.info.zmode : D3DZB_FALSE));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, zbuffer && !d3d_balpha));
    d3d_zbuffer = zbuffer;
    d3d_fog = fog;
}
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
/* 0x4387b0, d3.h: d3_SetBlendState; alpha blending (balpha), additive blending (glow), color key. */

void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey)
{
    if (balpha || glow) {
        if (!d3d_balpha) {
#line 1285
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZBIAS, 16));
            if (d3d_zbuffer) {
#line 1287
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, FALSE));
            }
#line 1288
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHABLENDENABLE, TRUE));
            if (d3d_UsedCard.info.flags & 8) {
#line 1290
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_STIPPLEDALPHA, TRUE));
            }
            d3d_balpha = 1;
        }
        if (glow) {
            if (!d3d_glow) {
#line 1295
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, d3d_UsedCard.info.glow_destblend));
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, d3d_UsedCard.info.glow_srcblend));
                if (d3d_fog) {
#line 1298
                    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, FALSE));
                }
                d3d_glow = 1;
            }
        } else {
#line 1303
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, d3d_UsedCard.info.trans_destblend));
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, d3d_UsedCard.info.trans_srcblend));
            if (d3d_fog) {
#line 1306
                D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, TRUE));
            }
            d3d_glow = 0;
        }
    } else if (d3d_balpha) {
#line 1312
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZBIAS, 0));
        if (d3d_zbuffer) {
#line 1314
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, TRUE));
        }
#line 1315
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHABLENDENABLE, FALSE));
        if (d3d_UsedCard.info.flags & 8) {
#line 1317
            D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_STIPPLEDALPHA, FALSE));
        }
#line 1318
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO));
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
        d3d_balpha = 0;
        d3d_glow = 0;
    }
    if (colorkey != d3d_colorkey) {
#line 1324
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_COLORKEYENABLE, colorkey));
        d3d_colorkey = colorkey;
    }
    if ((colorkey && !d3d_balpha) != d3d_alphatest) {
#line 1329
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHATESTENABLE, colorkey && !d3d_balpha));
        d3d_alphatest = colorkey && !d3d_balpha;
    }
}

extern int d3d_ZBufferCaps;
extern D3DZBUFCTX d3d_UsedZBufferFormat;
extern D3DCONFIG d3d_Config;
extern D3DCARD d3d_UsedCard;
extern D3DTLVERTEX d3d_PrecacheBuffer[3];
DWORD d3d_MinMipSize = 4;  /* 0x5f942c, smallest mip dimension kept (4) */
float d3d_ViewW = -1.0f;  /* 0x5f9430, last viewport set, -1 = none */
float d3d_ViewH = -1.0f;  /* 0x5f9434 */
float d3d_ViewX = -1.0f;  /* 0x5f9438 */
float d3d_ViewY = -1.0f;  /* 0x5f943c */
float d3d_Near = -1.0f;  /* 0x5f9440, last projection set */
float d3d_Far = -1.0f;  /* 0x5f9444 */
unsigned char d3d_zbuffer = 1;  /* 0x5f9448, z buffer enabled (render state cache) */
void * d3d_VertexBuffer = 0;  /* 0x633410 */
LPDIRECT3D3 d3d_D3D = {0};  /* 0x633414 */
struct IDirect3DDevice3 * d3d_Device = 0;  /* 0x633418 */
LPDIRECT3DVIEWPORT3 d3d_Viewport = {0};  /* 0x63341c */
LPDIRECTDRAWSURFACE4 d3d_ZBuffer = {0};  /* 0x633420 */
/* d3d_ZEnabled: view of d3_MipSkip (tools/data_views.py) */
/* d3d_balpha: view of d3_MipSkip (tools/data_views.py) */
/* d3d_glow: view of d3_MipSkip (tools/data_views.py) */
unsigned char d3d_colorkey = 0;  /* 0x633430 */
unsigned char d3d_alphatest = 0;  /* 0x633431 */
unsigned char d3d_fog = 0;  /* 0x633432 */
char d3d_StatesSet = 0;  /* 0x633433 */
char d3d_Mipmaps = 0;  /* 0x633434, mipmapping usable: d3d_UsedCard.info.maxMipFilter > 1 */
int d3d_ZBufferCaps = 0;  /* 0xfe7608, memory caps for the z buffer surface: */
D3DZBUFCTX d3d_UsedZBufferFormat = {0};  /* 0xfe7620 */
D3DCONFIG d3d_Config = {0}; /* 0xfe7670 */
D3DCARD d3d_UsedCard = {0};  /* 0xfe7680 */
/* 0xfe79c0, the 0x50-byte texture-format context the callback fills (its fields are the d3_TexFormat*
   names ts_texture.c used to define separately). */
D3DTEXFMTCTX d3d_UsedTexFormat = {0};  /* 0xfe79c0 */
D3DTLVERTEX d3d_PrecacheBuffer[3] = {0};  /* 0xfe7a20 */
