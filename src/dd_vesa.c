/* dd_vesa
   unit of rea 8cef7d5b (src/game/dd_vesa.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\Units\\dd_vesa\\Dd_vesa.c"
/* dd_vesa.c -- D:\develop\Units\dd_vesa\Dd_vesa.c (leaked assert path), the DirectDraw display layer:
   driver and device enumeration (with a selection dialog from d3_resource.dll), mode setup in a window,
   fullscreen or through GDI, locking, clearing, flipping and pixel format helpers.
   Extent in GildeGold.exe: 0x41bdd0..0x41ebc0. Functions are kept in their original order. Names are ours
   unless quoted in dd_vesa.h.
   @named_by leaked
   @boundary plausible */
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3.h"
#include "dd_vesa.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRIGGER / REA_DUMP_BACKBUFFER */


#endif
#include "data_views.h"
#include "ahm.h"
#include "di_input.h"
#include "err.h"
#include "mem.h"
#include "txt.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */

/* 0x418ce0..0x41bdd0: unk_418ce0.c until layout 2026-10-22 (character to key code table, DirectDraw / Direct3D
   error text); joined: the .data block starts with dd_vesa's -1 at 0x5f3c48, then d3d_Error's texts, then the
   Dd_vesa.c path 0x5f65e0 (one block). */
/* 0x418ce0, fills the 256-byte table p: character -> key code (1..0x5a), 0 for unmapped */

void d3_InitKeyCodes(unsigned char *p)
{
    if (p) {
        D3_ZERO_AT(p, 256);
        p[' '] = 1;
        p['A'] = 2;
        p['B'] = 3;
        p['C'] = 4;
        p['D'] = 5;
        p['E'] = 6;
        p['F'] = 7;
        p['G'] = 8;
        p['H'] = 9;
        p['I'] = 10;
        p['J'] = 11;
        p['K'] = 12;
        p['L'] = 13;
        p['M'] = 14;
        p['N'] = 15;
        p['O'] = 16;
        p['P'] = 17;
        p['Q'] = 18;
        p['R'] = 19;
        p['S'] = 20;
        p['T'] = 21;
        p['U'] = 22;
        p['V'] = 23;
        p['W'] = 24;
        p['X'] = 25;
        p['Y'] = 26;
        p['Z'] = 27;
        p['a'] = 28;
        p['b'] = 29;
        p['c'] = 30;
        p['d'] = 31;
        p['e'] = 32;
        p['f'] = 33;
        p['g'] = 34;
        p['h'] = 35;
        p['i'] = 36;
        p['j'] = 37;
        p['k'] = 38;
        p['l'] = 39;
        p['m'] = 40;
        p['n'] = 41;
        p['o'] = 42;
        p['p'] = 43;
        p['q'] = 44;
        p['r'] = 45;
        p['s'] = 46;
        p['t'] = 47;
        p['u'] = 48;
        p['v'] = 49;
        p['w'] = 50;
        p['x'] = 51;
        p['y'] = 52;
        p['z'] = 53;
        p['/'] = 54;
        p['\\'] = 55;
        p['\''] = 56;
        p['.'] = 57;
        p[','] = 58;
        p['-'] = 59;
        p['+'] = 60;
        p[':'] = 61;
        p['('] = 62;
        p[')'] = 63;
        p['['] = 64;
        p[']'] = 65;
        p['&'] = 66;
        p['<'] = 67;
        p['>'] = 68;
        p['!'] = 69;
        p['?'] = 70;
        p[';'] = 71;
        p['"'] = 72;
        p['#'] = 73;
        p['$'] = 74;
        p['%'] = 75;
        p['*'] = 76;
        p['='] = 77;
        p['@'] = 78;
        p['_'] = 79;
        p['0'] = 80;
        p['1'] = 81;
        p['2'] = 82;
        p['3'] = 83;
        p['4'] = 84;
        p['5'] = 85;
        p['6'] = 86;
        p['7'] = 87;
        p['8'] = 88;
        p['9'] = 89;
        p['~'] = 90;
    }
}
/* 0x418e90: formats the text of the last DirectDraw/Direct3D error (d3d_hr) into a local buffer that is
   never used (the output was probably compiled out) */

void d3d_Error(char *file, int line, char *call)
{
    char buf[1024];

    if (d3d_hr) {
        switch (d3d_hr) {
        case DDERR_GENERIC:
            strcpy(buf, "DDERR_GENERIC");
            strcat(buf, ": ");
            strcat(buf, "Generic failure.");
            break;
        case DDERR_UNSUPPORTED:
            strcpy(buf, "DDERR_UNSUPPORTED");
            strcat(buf, ": ");
            strcat(buf, "Action not supported.");
            break;
        case DDERR_NOTINITIALIZED:
            strcpy(buf, "DDERR_NOTINITIALIZED");
            strcat(buf, ": ");
            strcat(buf, "An attempt was made to invoke an interface member of a DirectDraw object created by CoCreateInstance() before it was initialized.");
            break;
        case DDERR_INVALIDPARAMS:
            strcpy(buf, "DDERR_INVALIDPARAMS");
            strcat(buf, ": ");
            strcat(buf, "One or more of the parameters passed to the callback function are incorrect.");
            break;
        case DDERR_OUTOFMEMORY:
            strcpy(buf, "DDERR_OUTOFMEMORY");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw does not have enough memory to perform the operation.");
            break;
        case DDERR_ALREADYINITIALIZED:
            strcpy(buf, "DDERR_ALREADYINITIALIZED");
            strcat(buf, ": ");
            strcat(buf, "This object is already initialized.");
            break;
        case DDERR_CANNOTATTACHSURFACE:
            strcpy(buf, "DDERR_CANNOTATTACHSURFACE");
            strcat(buf, ": ");
            strcat(buf, "This surface can not be attached to the requested surface.");
            break;
        case DDERR_CANNOTDETACHSURFACE:
            strcpy(buf, "DDERR_CANNOTDETACHSURFACE");
            strcat(buf, ": ");
            strcat(buf, "This surface can not be detached from the requested surface.");
            break;
        case DDERR_CURRENTLYNOTAVAIL:
            strcpy(buf, "DDERR_CURRENTLYNOTAVAIL");
            strcat(buf, ": ");
            strcat(buf, "Support is currently not available.");
            break;
        case DDERR_EXCEPTION:
            strcpy(buf, "DDERR_EXCEPTION");
            strcat(buf, ": ");
            strcat(buf, "An exception was encountered while performing the requested operation.");
            break;
        case DDERR_HEIGHTALIGN:
            strcpy(buf, "DDERR_HEIGHTALIGN");
            strcat(buf, ": ");
            strcat(buf, "Height of rectangle provided is not a multiple of reqd alignment.");
            break;
        case DDERR_INCOMPATIBLEPRIMARY:
            strcpy(buf, "DDERR_INCOMPATIBLEPRIMARY");
            strcat(buf, ": ");
            strcat(buf, "Unable to match primary surface creation request with existing primary surface.");
            break;
        case DDERR_INVALIDCAPS:
            strcpy(buf, "DDERR_INVALIDCAPS");
            strcat(buf, ": ");
            strcat(buf, "One or more of the caps bits passed to the callback are incorrect.");
            break;
        case DDERR_INVALIDCLIPLIST:
            strcpy(buf, "DDERR_INVALIDCLIPLIST");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw does not support provided Cliplist.");
            break;
        case DDERR_INVALIDMODE:
            strcpy(buf, "DDERR_INVALIDMODE");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw does not support the requested mode.");
            break;
        case DDERR_INVALIDOBJECT:
            strcpy(buf, "DDERR_INVALIDOBJECT");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw received a pointer that was an invalid DIRECTDRAW object.");
            break;
        case DDERR_INVALIDPIXELFORMAT:
            strcpy(buf, "DDERR_INVALIDPIXELFORMAT");
            strcat(buf, ": ");
            strcat(buf, "Pixel format was invalid as specified.");
            break;
        case DDERR_INVALIDRECT:
            strcpy(buf, "DDERR_INVALIDRECT");
            strcat(buf, ": ");
            strcat(buf, "Rectangle provided was invalid.");
            break;
        case DDERR_LOCKEDSURFACES:
            strcpy(buf, "DDERR_LOCKEDSURFACES");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because one or more surfaces are locked.");
            break;
        case DDERR_NO3D:
            strcpy(buf, "DDERR_NO3D");
            strcat(buf, ": ");
            strcat(buf, "There is no 3D present.");
            break;
        case DDERR_NOALPHAHW:
            strcpy(buf, "DDERR_NOALPHAHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no alpha accleration hardware present or available.");
            break;
        case DDERR_NOCLIPLIST:
            strcpy(buf, "DDERR_NOCLIPLIST");
            strcat(buf, ": ");
            strcat(buf, "No clip list available.");
            break;
        case DDERR_NOCOLORCONVHW:
            strcpy(buf, "DDERR_NOCOLORCONVHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no color conversion hardware present or available.");
            break;
        case DDERR_NOCOOPERATIVELEVELSET:
            strcpy(buf, "DDERR_NOCOOPERATIVELEVELSET");
            strcat(buf, ": ");
            strcat(buf, "Create function called without DirectDraw object method SetCooperativeLevel being called.");
            break;
        case DDERR_NOCOLORKEY:
            strcpy(buf, "DDERR_NOCOLORKEY");
            strcat(buf, ": ");
            strcat(buf, "Surface doesn't currently have a color key");
            break;
        case DDERR_NOCOLORKEYHW:
            strcpy(buf, "DDERR_NOCOLORKEYHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no hardware support of the dest color key.");
            break;
        case DDERR_NODIRECTDRAWSUPPORT:
            strcpy(buf, "DDERR_NODIRECTDRAWSUPPORT");
            strcat(buf, ": ");
            strcat(buf, "No DirectDraw support possible with current display driver.");
            break;
        case DDERR_NOEXCLUSIVEMODE:
            strcpy(buf, "DDERR_NOEXCLUSIVEMODE");
            strcat(buf, ": ");
            strcat(buf, "Operation requires the application to have exclusive mode but the application does not have exclusive mode.");
            break;
        case DDERR_NOFLIPHW:
            strcpy(buf, "DDERR_NOFLIPHW");
            strcat(buf, ": ");
            strcat(buf, "Flipping visible surfaces is not supported.");
            break;
        case DDERR_NOGDI:
            strcpy(buf, "DDERR_NOGDI");
            strcat(buf, ": ");
            strcat(buf, "There is no GDI present.");
            break;
        case DDERR_NOMIRRORHW:
            strcpy(buf, "DDERR_NOMIRRORHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because no mirroring hardware is present or available.");
            break;
        case DDERR_NOTFOUND:
            strcpy(buf, "DDERR_NOTFOUND");
            strcat(buf, ": ");
            strcat(buf, "Requested item was not found.");
            break;
        case DDERR_NOOVERLAYHW:
            strcpy(buf, "DDERR_NOOVERLAYHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no overlay hardware present or available.");
            break;
        case DDERR_OVERLAPPINGRECTS:
            strcpy(buf, "DDERR_OVERLAPPINGRECTS");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because the source and destination rectangles are on the same surface and overlap each other.");
            break;
        case DDERR_NORASTEROPHW:
            strcpy(buf, "DDERR_NORASTEROPHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no appropriate raster op hardware present or available.");
            break;
        case DDERR_NOROTATIONHW:
            strcpy(buf, "DDERR_NOROTATIONHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no rotation hardware present or available.");
            break;
        case DDERR_NOSTRETCHHW:
            strcpy(buf, "DDERR_NOSTRETCHHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no hardware support for stretching.");
            break;
        case DDERR_NOT4BITCOLOR:
            strcpy(buf, "DDERR_NOT4BITCOLOR");
            strcat(buf, ": ");
            strcat(buf, "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.");
            break;
        case DDERR_NOT4BITCOLORINDEX:
            strcpy(buf, "DDERR_NOT4BITCOLORINDEX");
            strcat(buf, ": ");
            strcat(buf, "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.");
            break;
        case DDERR_NOT8BITCOLOR:
            strcpy(buf, "DDERR_NOT8BITCOLOR");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw Surface is not in 8 bit color mode and the requested operation requires 8 bit color.");
            break;
        case DDERR_NOTEXTUREHW:
            strcpy(buf, "DDERR_NOTEXTUREHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no texture mapping hardware present or available.");
            break;
        case DDERR_NOVSYNCHW:
            strcpy(buf, "DDERR_NOVSYNCHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.");
            break;
        case DDERR_NOZBUFFERHW:
            strcpy(buf, "DDERR_NOZBUFFERHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no hardware support for zbuffer blting.");
            break;
        case DDERR_NOZOVERLAYHW:
            strcpy(buf, "DDERR_NOZOVERLAYHW");
            strcat(buf, ": ");
            strcat(buf, "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.");
            break;
        case DDERR_OUTOFCAPS:
            strcpy(buf, "DDERR_OUTOFCAPS");
            strcat(buf, ": ");
            strcat(buf, "The hardware needed for the requested operation has already been allocated.");
            break;
        case DDERR_OUTOFVIDEOMEMORY:
            strcpy(buf, "DDERR_OUTOFVIDEOMEMORY");
            strcat(buf, ": ");
            strcat(buf, "DirectDraw does not have enough video memory to perform the operation.");
            break;
        case DDERR_OVERLAYCANTCLIP:
            strcpy(buf, "DDERR_OVERLAYCANTCLIP");
            strcat(buf, ": ");
            strcat(buf, "Hardware does not support clipped overlays.");
            break;
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            strcpy(buf, "DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
            strcat(buf, ": ");
            strcat(buf, "Can only have ony color key active at one time for overlays.");
            break;
        case DDERR_PALETTEBUSY:
            strcpy(buf, "DDERR_PALETTEBUSY");
            strcat(buf, ": ");
            strcat(buf, "Access to this palette is being refused because the palette is already locked by another thread.");
            break;
        case DDERR_COLORKEYNOTSET:
            strcpy(buf, "DDERR_COLORKEYNOTSET");
            strcat(buf, ": ");
            strcat(buf, "No src color key specified for this operation.");
            break;
        case DDERR_SURFACEALREADYATTACHED:
            strcpy(buf, "DDERR_SURFACEALREADYATTACHED");
            strcat(buf, ": ");
            strcat(buf, "This surface is already attached to the surface it is being attached to.");
            break;
        case DDERR_SURFACEALREADYDEPENDENT:
            strcpy(buf, "DDERR_SURFACEALREADYDEPENDENT");
            strcat(buf, ": ");
            strcat(buf, "This surface is already a dependency of the surface it is being made a dependency of.");
            break;
        case DDERR_SURFACEBUSY:
            strcpy(buf, "DDERR_SURFACEBUSY");
            strcat(buf, ": ");
            strcat(buf, "Access to this surface is being refused because the surface is already locked by another thread.");
            break;
        case DDERR_CANTLOCKSURFACE:
            strcpy(buf, "DDERR_CANTLOCKSURFACE");
            strcat(buf, ": ");
            strcat(buf, "Access to this surface is being refused because no driver exists which can supply a pointer to the surface (No DCI-provider or optimized surface).");
            break;
        case DDERR_SURFACEISOBSCURED:
            strcpy(buf, "DDERR_SURFACEISOBSCURED");
            strcat(buf, ": ");
            strcat(buf, "Access to Surface refused because Surface is obscured.");
            break;
        case DDERR_SURFACELOST:
            strcpy(buf, "DDERR_SURFACELOST");
            strcat(buf, ": ");
            strcat(buf, "Access to this surface is being refused because the surface is gone. The DIRECTDRAWSURFACE object representing this surface should have Restore called on it.");
            break;
        case DDERR_SURFACENOTATTACHED:
            strcpy(buf, "DDERR_SURFACENOTATTACHED");
            strcat(buf, ": ");
            strcat(buf, "The requested surface is not attached.");
            break;
        case DDERR_TOOBIGHEIGHT:
            strcpy(buf, "DDERR_TOOBIGHEIGHT");
            strcat(buf, ": ");
            strcat(buf, "Height requested by DirectDraw is too large.");
            break;
        case DDERR_TOOBIGSIZE:
            strcpy(buf, "DDERR_TOOBIGSIZE");
            strcat(buf, ": ");
            strcat(buf, "Size requested by DirectDraw is too large --  The individual height and width are OK.");
            break;
        case DDERR_TOOBIGWIDTH:
            strcpy(buf, "DDERR_TOOBIGWIDTH");
            strcat(buf, ": ");
            strcat(buf, "Width requested by DirectDraw is too large.");
            break;
        case DDERR_UNSUPPORTEDFORMAT:
            strcpy(buf, "DDERR_UNSUPPORTEDFORMAT");
            strcat(buf, ": ");
            strcat(buf, "FOURCC format requested is unsupported by DirectDraw.");
            break;
        case DDERR_UNSUPPORTEDMASK:
            strcpy(buf, "DDERR_UNSUPPORTEDMASK");
            strcat(buf, ": ");
            strcat(buf, "Bitmask in the pixel format requested is unsupported by DirectDraw.");
            break;
        case DDERR_INVALIDSTREAM:
            strcpy(buf, "DDERR_INVALIDSTREAM");
            strcat(buf, ": ");
            strcat(buf, "The specified stream contains invalid data.");
            break;
        case DDERR_VERTICALBLANKINPROGRESS:
            strcpy(buf, "DDERR_VERTICALBLANKINPROGRESS");
            strcat(buf, ": ");
            strcat(buf, "Vertical blank is in progress.");
            break;
        case DDERR_WASSTILLDRAWING:
            strcpy(buf, "DDERR_WASSTILLDRAWING");
            strcat(buf, ": ");
            strcat(buf, "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.");
            break;
        case DDERR_XALIGN:
            strcpy(buf, "DDERR_XALIGN");
            strcat(buf, ": ");
            strcat(buf, "Rectangle provided was not horizontally aligned on reqd. boundary.");
            break;
        case DDERR_INVALIDDIRECTDRAWGUID:
            strcpy(buf, "DDERR_INVALIDDIRECTDRAWGUID");
            strcat(buf, ": ");
            strcat(buf, "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.");
            break;
        case DDERR_DIRECTDRAWALREADYCREATED:
            strcpy(buf, "DDERR_DIRECTDRAWALREADYCREATED");
            strcat(buf, ": ");
            strcat(buf, "A DirectDraw object representing this driver has already been created for this process.");
            break;
        case DDERR_NODIRECTDRAWHW:
            strcpy(buf, "DDERR_NODIRECTDRAWHW");
            strcat(buf, ": ");
            strcat(buf, "A hardware only DirectDraw object creation was attempted but the driver did not support any hardware.");
            break;
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            strcpy(buf, "DDERR_PRIMARYSURFACEALREADYEXISTS");
            strcat(buf, ": ");
            strcat(buf, "This process already has created a primary surface.");
            break;
        case DDERR_NOEMULATION:
            strcpy(buf, "DDERR_NOEMULATION");
            strcat(buf, ": ");
            strcat(buf, "Software emulation not available.");
            break;
        case DDERR_REGIONTOOSMALL:
            strcpy(buf, "DDERR_REGIONTOOSMALL");
            strcat(buf, ": ");
            strcat(buf, "Region passed to Clipper::GetClipList is too small.");
            break;
        case DDERR_CLIPPERISUSINGHWND:
            strcpy(buf, "DDERR_CLIPPERISUSINGHWND");
            strcat(buf, ": ");
            strcat(buf, "An attempt was made to set a clip list for a clipper object that is already monitoring an hwnd.");
            break;
        case DDERR_NOCLIPPERATTACHED:
            strcpy(buf, "DDERR_NOCLIPPERATTACHED");
            strcat(buf, ": ");
            strcat(buf, "No clipper object attached to surface object.");
            break;
        case DDERR_NOHWND:
            strcpy(buf, "DDERR_NOHWND");
            strcat(buf, ": ");
            strcat(buf, "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.");
            break;
        case DDERR_HWNDSUBCLASSED:
            strcpy(buf, "DDERR_HWNDSUBCLASSED");
            strcat(buf, ": ");
            strcat(buf, "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.");
            break;
        case DDERR_HWNDALREADYSET:
            strcpy(buf, "DDERR_HWNDALREADYSET");
            strcat(buf, ": ");
            strcat(buf, "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.");
            break;
        case DDERR_NOPALETTEATTACHED:
            strcpy(buf, "DDERR_NOPALETTEATTACHED");
            strcat(buf, ": ");
            strcat(buf, "No palette object attached to this surface.");
            break;
        case DDERR_NOPALETTEHW:
            strcpy(buf, "DDERR_NOPALETTEHW");
            strcat(buf, ": ");
            strcat(buf, "No hardware support for 16 or 256 color palettes.");
            break;
        case DDERR_BLTFASTCANTCLIP:
            strcpy(buf, "DDERR_BLTFASTCANTCLIP");
            strcat(buf, ": ");
            strcat(buf, "Cannot BltFast if a clipper object is attached to the source surface.");
            break;
        case DDERR_NOBLTHW:
            strcpy(buf, "DDERR_NOBLTHW");
            strcat(buf, ": ");
            strcat(buf, "No blter.");
            break;
        case DDERR_NODDROPSHW:
            strcpy(buf, "DDERR_NODDROPSHW");
            strcat(buf, ": ");
            strcat(buf, "No DirectDraw ROP hardware.");
            break;
        case DDERR_OVERLAYNOTVISIBLE:
            strcpy(buf, "DDERR_OVERLAYNOTVISIBLE");
            strcat(buf, ": ");
            strcat(buf, "Overlay is not visible (GetOverlayPosition).");
            break;
        case DDERR_NOOVERLAYDEST:
            strcpy(buf, "DDERR_NOOVERLAYDEST");
            strcat(buf, ": ");
            strcat(buf, "Overlay has no destination (No UpdateOverlay).");
            break;
        case DDERR_INVALIDPOSITION:
            strcpy(buf, "DDERR_INVALIDPOSITION");
            strcat(buf, ": ");
            strcat(buf, "Overlay has invalid position.");
            break;
        case DDERR_NOTAOVERLAYSURFACE:
            strcpy(buf, "DDERR_NOTAOVERLAYSURFACE");
            strcat(buf, ": ");
            strcat(buf, "Overlay member called for a non-overlay surface.");
            break;
        case DDERR_EXCLUSIVEMODEALREADYSET:
            strcpy(buf, "DDERR_EXCLUSIVEMODEALREADYSET");
            strcat(buf, ": ");
            strcat(buf, "An attempt was made to set the cooperative level when it was already set to exclusive.");
            break;
        case DDERR_NOTFLIPPABLE:
            strcpy(buf, "DDERR_NOTFLIPPABLE");
            strcat(buf, ": ");
            strcat(buf, "An attempt has been made to flip a surface that is not flippable.");
            break;
        case DDERR_CANTDUPLICATE:
            strcpy(buf, "DDERR_CANTDUPLICATE");
            strcat(buf, ": ");
            strcat(buf, "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.");
            break;
        case DDERR_NOTLOCKED:
            strcpy(buf, "DDERR_NOTLOCKED");
            strcat(buf, ": ");
            strcat(buf, "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.");
            break;
        case DDERR_CANTCREATEDC:
            strcpy(buf, "DDERR_CANTCREATEDC");
            strcat(buf, ": ");
            strcat(buf, "Windows can not create any more DCs, or a DC was requested for a palette-indexed surface with an unset palette.");
            break;
        case DDERR_NODC:
            strcpy(buf, "DDERR_NODC");
            strcat(buf, ": ");
            strcat(buf, "No DC was ever created for this surface.");
            break;
        case DDERR_WRONGMODE:
            strcpy(buf, "DDERR_WRONGMODE");
            strcat(buf, ": ");
            strcat(buf, "This surface can not be restored because it was created in a different mode.");
            break;
        case DDERR_IMPLICITLYCREATED:
            strcpy(buf, "DDERR_IMPLICITLYCREATED");
            strcat(buf, ": ");
            strcat(buf, "This surface can not be restored because it is an implicitly created surface.");
            break;
        case DDERR_NOTPALETTIZED:
            strcpy(buf, "DDERR_NOTPALETTIZED");
            strcat(buf, ": ");
            strcat(buf, "The surface being used is not a palette-based surface.");
            break;
        case DDERR_UNSUPPORTEDMODE:
            strcpy(buf, "DDERR_UNSUPPORTEDMODE");
            strcat(buf, ": ");
            strcat(buf, "The display is currently in an unsupported mode.");
            break;
        case DDERR_NOMIPMAPHW:
            strcpy(buf, "DDERR_NOMIPMAPHW");
            strcat(buf, ": ");
            strcat(buf, "Operation could not be carried out because there is no mip-map texture mapping hardware present or available.");
            break;
        case DDERR_INVALIDSURFACETYPE:
            strcpy(buf, "DDERR_INVALIDSURFACETYPE");
            strcat(buf, ": ");
            strcat(buf, "The requested action could not be performed because the surface was of the wrong type.");
            break;
        case DDERR_NOOPTIMIZEHW:
            strcpy(buf, "DDERR_NOOPTIMIZEHW");
            strcat(buf, ": ");
            strcat(buf, "Device does not support optimized surfaces, therefore no video memory optimized surfaces.");
            break;
        case DDERR_NOTLOADED:
            strcpy(buf, "DDERR_NOTLOADED");
            strcat(buf, ": ");
            strcat(buf, "Surface is an optimized surface, but has not yet been allocated any memory.");
            break;
        case DDERR_NOFOCUSWINDOW:
            strcpy(buf, "DDERR_NOFOCUSWINDOW");
            strcat(buf, ": ");
            strcat(buf, "Attempt was made to create or set a device window without first setting the focus window.");
            break;
        case DDERR_DCALREADYCREATED:
            strcpy(buf, "DDERR_DCALREADYCREATED");
            strcat(buf, ": ");
            strcat(buf, "A DC has already been returned for this surface. Only one DC can be retrieved per surface.");
            break;
        case DDERR_NONONLOCALVIDMEM:
            strcpy(buf, "DDERR_NONONLOCALVIDMEM");
            strcat(buf, ": ");
            strcat(buf, "An attempt was made to allocate non-local video memory from a device that does not support non-local video memory.");
            break;
        case DDERR_CANTPAGELOCK:
            strcpy(buf, "DDERR_CANTPAGELOCK");
            strcat(buf, ": ");
            strcat(buf, "The attempt to page lock a surface failed.");
            break;
        case DDERR_CANTPAGEUNLOCK:
            strcpy(buf, "DDERR_CANTPAGEUNLOCK");
            strcat(buf, ": ");
            strcat(buf, "The attempt to page unlock a surface failed.");
            break;
        case DDERR_NOTPAGELOCKED:
            strcpy(buf, "DDERR_NOTPAGELOCKED");
            strcat(buf, ": ");
            strcat(buf, "An attempt was made to page unlock a surface with no outstanding page locks.");
            break;
        case DDERR_MOREDATA:
            strcpy(buf, "DDERR_MOREDATA");
            strcat(buf, ": ");
            strcat(buf, "There is more data available than the specified buffer size could hold.");
            break;
        case DDERR_EXPIRED:
            strcpy(buf, "DDERR_EXPIRED");
            strcat(buf, ": ");
            strcat(buf, "The data has expired and is therefore no longer valid.");
            break;
        case DDERR_VIDEONOTACTIVE:
            strcpy(buf, "DDERR_VIDEONOTACTIVE");
            strcat(buf, ": ");
            strcat(buf, "The video port is not active.");
            break;
        case DDERR_DEVICEDOESNTOWNSURFACE:
            strcpy(buf, "DDERR_DEVICEDOESNTOWNSURFACE");
            strcat(buf, ": ");
            strcat(buf, "Surfaces created by one direct draw device cannot be used directly by another direct draw device.");
            break;
        /* The original switch has a label for every D3DERR_* code of d3d.h, all with an empty body.
           VC6 removes an empty case before lowering the switch, but a folded `if (0) { }` keeps the
           label in the compare tree and the jump tables and still emits no code. Without these 75
           labels the dispatch tree shifts and nothing after it matches. */
        case D3DERR_BADMAJORVERSION: if (0) { } break;
        case D3DERR_BADMINORVERSION: if (0) { } break;
        case D3DERR_INVALID_DEVICE: if (0) { } break;
        case D3DERR_INITFAILED: if (0) { } break;
        case D3DERR_DEVICEAGGREGATED: if (0) { } break;
        case D3DERR_EXECUTE_CREATE_FAILED: if (0) { } break;
        case D3DERR_EXECUTE_DESTROY_FAILED: if (0) { } break;
        case D3DERR_EXECUTE_LOCK_FAILED: if (0) { } break;
        case D3DERR_EXECUTE_UNLOCK_FAILED: if (0) { } break;
        case D3DERR_EXECUTE_LOCKED: if (0) { } break;
        case D3DERR_EXECUTE_NOT_LOCKED: if (0) { } break;
        case D3DERR_EXECUTE_FAILED: if (0) { } break;
        case D3DERR_EXECUTE_CLIPPED_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_NO_SUPPORT: if (0) { } break;
        case D3DERR_TEXTURE_CREATE_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_DESTROY_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_LOCK_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_UNLOCK_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_LOAD_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_SWAP_FAILED: if (0) { } break;
        case D3DERR_TEXTURE_LOCKED: if (0) { } break;
        case D3DERR_TEXTURE_NOT_LOCKED: if (0) { } break;
        case D3DERR_TEXTURE_GETSURF_FAILED: if (0) { } break;
        case D3DERR_MATRIX_CREATE_FAILED: if (0) { } break;
        case D3DERR_MATRIX_DESTROY_FAILED: if (0) { } break;
        case D3DERR_MATRIX_SETDATA_FAILED: if (0) { } break;
        case D3DERR_MATRIX_GETDATA_FAILED: if (0) { } break;
        case D3DERR_SETVIEWPORTDATA_FAILED: if (0) { } break;
        case D3DERR_INVALIDCURRENTVIEWPORT: if (0) { } break;
        case D3DERR_INVALIDPRIMITIVETYPE: if (0) { } break;
        case D3DERR_INVALIDVERTEXTYPE: if (0) { } break;
        case D3DERR_TEXTURE_BADSIZE: if (0) { } break;
        case D3DERR_INVALIDRAMPTEXTURE: if (0) { } break;
        case D3DERR_MATERIAL_CREATE_FAILED: if (0) { } break;
        case D3DERR_MATERIAL_DESTROY_FAILED: if (0) { } break;
        case D3DERR_MATERIAL_SETDATA_FAILED: if (0) { } break;
        case D3DERR_MATERIAL_GETDATA_FAILED: if (0) { } break;
        case D3DERR_INVALIDPALETTE: if (0) { } break;
        case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY: if (0) { } break;
        case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY: if (0) { } break;
        case D3DERR_SURFACENOTINVIDMEM: if (0) { } break;
        case D3DERR_LIGHT_SET_FAILED: if (0) { } break;
        case D3DERR_LIGHTHASVIEWPORT: if (0) { } break;
        case D3DERR_LIGHTNOTINTHISVIEWPORT: if (0) { } break;
        case D3DERR_SCENE_IN_SCENE: if (0) { } break;
        case D3DERR_SCENE_NOT_IN_SCENE: if (0) { } break;
        case D3DERR_SCENE_BEGIN_FAILED: if (0) { } break;
        case D3DERR_SCENE_END_FAILED: if (0) { } break;
        case D3DERR_INBEGIN: if (0) { } break;
        case D3DERR_NOTINBEGIN: if (0) { } break;
        case D3DERR_NOVIEWPORTS: if (0) { } break;
        case D3DERR_VIEWPORTDATANOTSET: if (0) { } break;
        case D3DERR_VIEWPORTHASNODEVICE: if (0) { } break;
        case D3DERR_NOCURRENTVIEWPORT: if (0) { } break;
        case D3DERR_INVALIDVERTEXFORMAT: if (0) { } break;
        case D3DERR_COLORKEYATTACHED: if (0) { } break;
        case D3DERR_VERTEXBUFFEROPTIMIZED: if (0) { } break;
        case D3DERR_VBUF_CREATE_FAILED: if (0) { } break;
        case D3DERR_VERTEXBUFFERLOCKED: if (0) { } break;
        case D3DERR_ZBUFFER_NOTPRESENT: if (0) { } break;
        case D3DERR_STENCILBUFFER_NOTPRESENT: if (0) { } break;
        case D3DERR_WRONGTEXTUREFORMAT: if (0) { } break;
        case D3DERR_UNSUPPORTEDCOLOROPERATION: if (0) { } break;
        case D3DERR_UNSUPPORTEDCOLORARG: if (0) { } break;
        case D3DERR_UNSUPPORTEDALPHAOPERATION: if (0) { } break;
        case D3DERR_UNSUPPORTEDALPHAARG: if (0) { } break;
        case D3DERR_TOOMANYOPERATIONS: if (0) { } break;
        case D3DERR_CONFLICTINGTEXTUREFILTER: if (0) { } break;
        case D3DERR_UNSUPPORTEDFACTORVALUE: if (0) { } break;
        case D3DERR_CONFLICTINGRENDERSTATE: if (0) { } break;
        case D3DERR_UNSUPPORTEDTEXTUREFILTER: if (0) { } break;
        case D3DERR_TOOMANYPRIMITIVES: if (0) { } break;
        case D3DERR_INVALIDMATRIX: if (0) { } break;
        case D3DERR_TOOMANYVERTICES: if (0) { } break;
        case D3DERR_CONFLICTINGTEXTUREPALETTE: if (0) { } break;
        }
    }
}

HWND dd_DevCombo = 0;  /* 0x6311f8 */
HWND dd_DrvCombo = 0;  /* 0x6311fc */
int dd_LastDriver = -1;  /* 0x5f3c48 */
LPDIRECTDRAW dd_DDBase = 0;  /* 0x631200 */
LPDIRECTDRAW4 dd_DD = 0;  /* 0x631204 */
LPDIRECTDRAWSURFACE4 dd_BackBuffer = 0;  /* 0x631208 */
int dd_Page = 0;  /* 0x63120c */
LPDIRECTDRAWSURFACE4 dd_FrontBuffer = 0;  /* 0x631210 */
LPDIRECTDRAWCLIPPER lpClipper = 0;  /* 0x631214 */
LPDIRECT3D3 dd_D3D3 = 0;  /* 0x631218 */
int dd_BitPlanes = 0;  /* 0x63121c */
char dd_HasMMX = 0;  /* 0x631220 */
char dd_TripleBuffer = 0;  /* 0x631221 */
char dd_Cleared = 0;  /* 0x631222 */
char dd_ZCleared = 0;  /* 0x631223 */
char dd_DlgFlag = 0;  /* 0x631224 */
char dd_Has800x600 = 0;  /* 0x631225 */
char dd_Has1024x768 = 0;  /* 0x631226 */
char dd_Has1152x864 = 0;  /* 0x631227 */

/* txt_DlgTitle: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DlgLabel104: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DlgLabel105: view of txt_Ptrs (build/mk_txt_views.py) */
int dd_BytesPerPixel = 0;  /* 0x1463568 */
ULONG dd_Ref = {0}; /* 0x146356c */
unsigned char dd_RBits = {0}; /* 0x1463570 */
HWND dd_Window = 0;  /* 0x1463574 */
unsigned char dd_GShift = 0;  /* 0x1463578 */
unsigned char dd_RShift = 0;  /* 0x1463579 */
int dd_LockPitch = 0;  /* 0x146357c */
unsigned char dd_GBits = {0}; /* 0x1463580 */
unsigned char dd_GLoss = 0;  /* 0x1463581, an object of its own after dd_GBits (the original reads it as a byte) */
void *dd_MemBuff = 0;  /* 0x1463584 */
int dd_Pitch = {0}; /* 0x1463588 */
int dd_MemSize = 0;  /* 0x146358c */
void *dd_Surface = {0}; /* 0x1463590 */
int dd_Height = 0;  /* 0x1463594 */
int dd_Width = 0;  /* 0x1463598 */
unsigned int dd_BMask = 0;  /* 0x146359c */
void *dd_MemBottom = 0;  /* 0x14635a0 */
int dd_PixelFormat = 0;  /* 0x14635a4 */
unsigned int dd_ColorBitMask = 0;  /* 0x14635a8, the pixel format's colour bits below each channel's top bit at their shifts, high half set */
int dd_Mode = 0;  /* 0x14635ac */
int dd_NumDevices = 0;  /* 0x14635b0 */
RECT dd_ClipRect = {0};  /* 0x14635c0 */
D3DCARD d3d_Devices[8] = {0}; /* 0x14635e0 */
int dd_LockBytes = 0;  /* 0x1464fa0 */
unsigned int dd_RMask = 0;  /* 0x1464fa4 */
int d3d_NumDevices = {0}; /* 0x1464fa8 */
int dd_MemPitch = 0;  /* 0x1464fac */
unsigned int dd_GMask = 0;  /* 0x1464fb0 */
int dd_LockBits = 0;  /* 0x1464fb4 */
unsigned int dd_ColorBitMaskDword = 0;  /* 0x1464fb8, dd_ColorBitMask in both 16-bit halves (dword write) */
unsigned char dd_BShift = 0;  /* 0x1464fbc */
unsigned char dd_BBits = {0}; /* 0x1464fbd */
int dd_SurfaceFlags = 0;  /* 0x1464fc0, cleared with the surfaces on every mode set (own object: dd_BBits is one byte; a reasoned guess, the matched code never reads it) */
unsigned char dd_BLoss = 0;  /* 0x1464fc4 */
char dd_KeyCodes[0x100] = {0};  /* 0x1464fe0, character -> key code table (d3_InitKeyCodes) */
HBITMAP dd_Bitmap = {0};  /* 0x14650e0 */
unsigned char dd_RLoss = 0;  /* 0x14650e4 */
DDDEVICE dd_Devices[8] = {0};  /* 0x1465100 */
int dd_PitchPixels = {0}; /* 0x14661a0 */
long d3d_hr = 0;  /* 0x14661a4, result of the last checked D3D call */

/* 0x41bdd0, restores all lost surfaces; nonzero on success (or when there is nothing to restore) */
char dd_Restore(void)
{
    if (dd_Mode && dd_DD)
        return dd_DD->lpVtbl->RestoreAllSurfaces(dd_DD) == DD_OK;
    return 1;
}
/* 0x41be00, releases everything dd_SetMode created */

void dd_Exit(void)
{
    dd_Surface = NULL;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    if (dd_Mode == DD_FULLMEM)
        dd_Mode = DD_FULLSCREEN;
    else if (dd_Mode == DD_WINDOWMEM)
        dd_Mode = DD_WINDOW;
    switch (dd_Mode) {
    case DD_GDI:
        DeleteObject(dd_Bitmap);
        break;
    case DD_WINDOW:
        if (lpClipper) {
            dd_Ref = lpClipper->lpVtbl->Release(lpClipper);
            lpClipper = NULL;
        }
        if (dd_FrontBuffer) {
            dd_Ref = dd_FrontBuffer->lpVtbl->Release(dd_FrontBuffer);
            dd_FrontBuffer = NULL;
        }
        if (dd_BackBuffer) {
            dd_Ref = dd_BackBuffer->lpVtbl->Release(dd_BackBuffer);
            dd_BackBuffer = NULL;
        }
        if (dd_DD) {
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        if (dd_DDBase) {
            dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
            dd_DDBase = NULL;
        }
        if (dd_MemBuff) {
            mem_Free(dd_MemBuff);
            dd_MemBuff = NULL;
        }
        break;
    case DD_FULLSCREEN:
        if (dd_FrontBuffer) {
            dd_Ref = dd_FrontBuffer->lpVtbl->Release(dd_FrontBuffer);
            dd_FrontBuffer = NULL;
        }
        if (dd_DD) {
#line 473
            D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_NORMAL));
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        if (dd_DDBase) {
            dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
            dd_DDBase = NULL;
        }
        if (dd_MemBuff) {
            mem_Free(dd_MemBuff);
            dd_MemBuff = NULL;
        }
        break;
    }
}
/* 0x41bf80, DirectDrawEnumerateEx callback: collects up to 8 drivers in dd_Devices */

BOOL WINAPI dd_EnumDriverCallback(GUID *guid, LPSTR desc, LPSTR name, LPVOID ctx, HMONITOR mon)
{
    strcpy(dd_Devices[dd_NumDevices].desc, desc);
    strcpy(dd_Devices[dd_NumDevices].name, name);
    if (!guid) {
        dd_Devices[dd_NumDevices].guid = NULL;
        D3_ZERO(&dd_Devices[dd_NumDevices].guiddata, sizeof(GUID));
    } else {
        D3_COPY(&dd_Devices[dd_NumDevices].guiddata, guid, sizeof(GUID));
        dd_Devices[dd_NumDevices].guid = &dd_Devices[dd_NumDevices].guiddata;
    }
    dd_NumDevices++;
    return dd_NumDevices != 8;
}
/* 0x41c080, condenses a device description into the flags and limits of card->info */

void d3d_GetDeviceInfo(D3DDEVICEDESC *caps, D3DCARD *card)
{
    int sw = card->info.software;
    int tex;

    D3_ZERO(&card->info, 0x2c);
    card->info.software = sw;
    if ((caps->dwFlags & 0x300) && (caps->dwFlags & D3DDD_TRICAPS) && (caps->dpcTriCaps.dwTextureBlendCaps & 0x82))
        tex = 1;
    else
        tex = 0;
    card->info.clipping = (caps->dwFlags & 1) && caps->bClipping == 1 ? 1 : 0;
    if (caps->dwFlags & D3DDD_TRICAPS) {
        card->info.zbuffer = ~(caps->dpcTriCaps.dwRasterCaps >> 15);
        if (card->info.zbuffer) {
            card->info.zmode = 1;
            card->info.wbuffer = caps->dpcTriCaps.dwRasterCaps >> 18;
        } else
            card->info.zmode = 0;
        card->info.dither = caps->dpcTriCaps.dwRasterCaps;
        card->info.antialias = caps->dpcTriCaps.dwRasterCaps >> 11;
        /* written as two tests: VC6 merges them into one `test dh,0x50` but keeps the branchy 0/1 result
           (a single `& 0x5000` compiles to setne and shifts the whole register allocation) */
        card->info.alphablend = (caps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND)
                      || (caps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND) ? 1 : 0;
        if (!card->info.alphablend)
            card->info.stipple = (caps->dpcTriCaps.dwShadeCaps & 0x2000) || (caps->dpcTriCaps.dwRasterCaps & 0x200) ? 1 : 0;
        card->info.stippled = !card->info.alphablend && card->info.stipple ? 1 : 0;
        card->info.fogvertex = caps->dpcTriCaps.dwRasterCaps >> 7;
    }
    if (tex) {
        card->info.perspective = caps->dpcTriCaps.dwTextureCaps;
        card->info.bilinear = caps->dpcTriCaps.dwTextureFilterCaps >> 1;
        card->info.maxMipFilter = 1;
        if (card->info.bilinear) {
            if (caps->dpcTriCaps.dwTextureFilterCaps & 0x20)
                card->info.maxMipFilter = 3;
            else if (caps->dpcTriCaps.dwTextureFilterCaps & 8)
                card->info.maxMipFilter = 2;
        } else {
            if (caps->dpcTriCaps.dwTextureFilterCaps & 0x10)
                card->info.maxMipFilter = 3;
            else if (caps->dpcTriCaps.dwTextureFilterCaps & 4)
                card->info.maxMipFilter = 2;
        }
        card->info.subpixel = caps->dpcTriCaps.dwRasterCaps >> 5;
        card->info.multitexture = caps->wMaxSimultaneousTextures >= 2 && (caps->dwTextureOpCaps & 8) && !card->info.software ? 1 : 0;
        card->info.minw = caps->dwMinTextureWidth;
        card->info.minh = caps->dwMinTextureHeight;
        card->info.maxw = caps->dwMaxTextureWidth;
        if (!card->info.maxw)
            card->info.maxw = 0x400;
        card->info.maxh = caps->dwMaxTextureHeight;
        if (!card->info.maxh)
            card->info.maxh = 0x400;
        if ((caps->dpcTriCaps.dwDestBlendCaps & 0x20) && (caps->dpcTriCaps.dwSrcBlendCaps & 0x10)) {
            card->info.trans_destblend = 6;
            card->info.trans_srcblend = 5;
        } else {
            if (caps->dpcTriCaps.dwDestBlendCaps & 0x20)
                card->info.trans_destblend = 6;
            else if ((caps->dpcTriCaps.dwDestBlendCaps & 2) && (caps->dpcTriCaps.dwSrcBlendCaps & 2))
                card->info.trans_destblend = 2;
            else if (caps->dpcTriCaps.dwDestBlendCaps & 4)
                card->info.trans_destblend = 3;
            else if (caps->dpcTriCaps.dwDestBlendCaps & 0x100)
                card->info.trans_destblend = 9;
            else {
                card->info.trans_destblend = 1;
                card->info.alphablend = 0;
            }
            card->info.trans_srcblend = 2;
        }
        if ((caps->dpcTriCaps.dwDestBlendCaps & 2) && (caps->dpcTriCaps.dwSrcBlendCaps & 2)) {
            card->info.glow_destblend = 2;
            card->info.glow_srcblend = 2;
        } else {
            card->info.glow_destblend = card->info.trans_destblend;
            card->info.glow_srcblend = card->info.trans_srcblend;
        }
    }
}
/* 0x41c420, DDBD_ bit depth flag of a pixel size */

DWORD dd_BppToDDBD(int bpp)
{
    if (bpp == 8)
        return DDBD_8;
    if (bpp == 16)
        return DDBD_16;
    if (bpp == 24)
        return DDBD_24;
    return bpp == 32 ? DDBD_32 : 0;
}
/* 0x41c460, "d3d_EnumDeviceCallback": collects up to 8 Direct3D devices that can render at ctx bits @deep B2/B3: the remaining code hunks are the same instructions with exchanged scratch registers and stack slots (instr 110 test DWORD PTR [ecx+0x9c],eax vs [eax+0x9c],ecx; instr 140 the lea *3/*2/*8 chain with eax/ecx/edx exchanged); the slot-20/24/28 allocation and every attempt (scope experiments, declaration order, D3_COPY mixes, c2prio) leave it at 279 B - VC6 allocation, not a meaning error */

HRESULT WINAPI d3d_EnumDeviceCallback(GUID *guid, LPSTR desc, LPSTR name,
                                      LPD3DDEVICEDESC hw, LPD3DDEVICEDESC hel, LPVOID ctx)
{
    D3DCOLORMODEL model = hw->dcmColorModel;
    DWORD depth = dd_BppToDDBD((int)ctx);

    if (model) {
        if (!(hw->dwDeviceRenderBitDepth & depth))
            return D3DENUMRET_OK;
        if (model == D3DCOLOR_MONO)
            return D3DENUMRET_OK;
        if (!(hw->dwFlags & D3DDD_TRICAPS))
            return D3DENUMRET_OK;
        D3_COPY(&d3d_Devices[d3d_NumDevices].Guid, guid, sizeof(GUID));
        strcpy(d3d_Devices[d3d_NumDevices].desc, desc);
        strcpy(d3d_Devices[d3d_NumDevices].name, name);
        D3_COPY(&d3d_Devices[d3d_NumDevices].caps, hw, sizeof(D3DDEVICEDESC));
        d3d_Devices[d3d_NumDevices].info.software = 0;
        d3d_GetDeviceInfo(&d3d_Devices[d3d_NumDevices].caps, &d3d_Devices[d3d_NumDevices]);
        d3d_NumDevices++;
    } else {
        if (!(hel->dwDeviceRenderBitDepth & depth))
            return D3DENUMRET_OK;
        if (hel->dcmColorModel == D3DCOLOR_MONO)
            return D3DENUMRET_OK;
        if (!(hel->dwFlags & D3DDD_TRICAPS))
            return D3DENUMRET_OK;
        D3_COPY(&d3d_Devices[d3d_NumDevices].Guid, guid, sizeof(GUID));
        strcpy(d3d_Devices[d3d_NumDevices].desc, desc);
        strcpy(d3d_Devices[d3d_NumDevices].name, name);
        D3_COPY(&d3d_Devices[d3d_NumDevices].caps, hel, sizeof(D3DDEVICEDESC));
        d3d_Devices[d3d_NumDevices].info.software = 1;
        d3d_GetDeviceInfo(&d3d_Devices[d3d_NumDevices].caps, &d3d_Devices[d3d_NumDevices]);
        d3d_NumDevices++;
    }
    return d3d_NumDevices != 8;
}
/* 0x41c6f0, for the "dd_Guid"/"d3d_Guid" settings */

char *dd_GuidToString(GUID *guid, char *s)
{
    char *ret = NULL;

    if (guid && s) {
        sprintf(s, "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", guid->Data1, guid->Data2, guid->Data3,
                guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
                guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
        ret = s;
    }
    return ret;
}
/* 0x41c760, parses "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx" into guid (1, or 0 for a NULL or empty string) */

char dd_StringToGuid(char *s, GUID *guid)
{
    int d1, d2, d3;
    int b0, b1, b2, b3, b4, b5, b6, b7;
    char ok = 0;

    if (s && *s && guid) {
        sscanf(s, "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", &d1, &d2, &d3,
               &b0, &b1, &b2, &b3, &b4, &b5, &b6, &b7);
        guid->Data1 = d1;
        guid->Data2 = d2;
        guid->Data3 = d3;
        guid->Data4[0] = b0;
        guid->Data4[1] = b1;
        guid->Data4[2] = b2;
        guid->Data4[3] = b3;
        guid->Data4[4] = b4;
        guid->Data4[5] = b5;
        guid->Data4[6] = b6;
        guid->Data4[7] = b7;
        ok = 1;
    }
    return ok;
}
/* 0x41c830, driver (102) and device (103) selection; the choice is stored as "dd_Guid"/"d3d_Guid" */

BOOL CALLBACK dd_DlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    LPDIRECTDRAW ddraw1;
    LPDIRECTDRAW4 ddraw4;
    char buf[0x200];
    char guidstr[0x100];
    int i, idx, sel = 0;
    HKEY cfg;
    GUID *guid;
    char *s;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case 102:
            i = SendMessage(dd_DrvCombo, CB_GETCURSEL, 0, 0);
            if (i == dd_LastDriver)
                return 1;
            dd_LastDriver = i;
            guid = (GUID *)SendMessage(dd_DrvCombo, CB_GETITEMDATA, i, 0);
            SendMessage(dd_DevCombo, CB_RESETCONTENT, 0, 0);
            d3d_NumDevices = 0;
            if (DirectDrawCreate(guid, &ddraw1, NULL) == DD_OK) {
#line 939
                D3DCHECK(ddraw1->lpVtbl->QueryInterface(ddraw1, &IID_IDirectDraw4, &ddraw4));
                if (!d3d_hr) {
                    LPDIRECT3D3 d3d3;
#line 944
                    D3DCHECK(ddraw4->lpVtbl->QueryInterface(ddraw4, &IID_IDirect3D3, &d3d3));
                    if (!d3d_hr) {
#line 947
                        D3DCHECK(d3d3->lpVtbl->EnumDevices(d3d3, d3d_EnumDeviceCallback, (void *)dd_BitPlanes));
                        dd_Ref = d3d3->lpVtbl->Release(d3d3);
                        d3d3 = NULL;
                    }
                    dd_Ref = ddraw4->lpVtbl->Release(ddraw4);
                    ddraw4 = NULL;
                }
                dd_Ref = ddraw1->lpVtbl->Release(ddraw1);
                ddraw1 = NULL;
            }
            for (i = 0; i < d3d_NumDevices; i++) {
                strcpy(buf, d3d_Devices[i].desc);
                strcat(buf, " (");
                strcat(buf, d3d_Devices[i].name);
                strcat(buf, ")");
                idx = SendMessage(dd_DevCombo, CB_ADDSTRING, 0, (LPARAM)buf);
                SendMessage(dd_DevCombo, CB_SETITEMDATA, idx, (LPARAM)&d3d_Devices[i].Guid);
                if (!d3d_Devices[i].info.software)
                    sel = idx;
            }
            SendMessage(dd_DevCombo, CB_SETCURSEL, sel, 0);
            return 1;
        case IDOK:
            PostMessage(dlg, WM_CLOSE, 0, 0);
            return 1;
        }
        break;
    case WM_INITDIALOG:
        dd_DrvCombo = GetDlgItem(dlg, 102);
        dd_DevCombo = GetDlgItem(dlg, 103);
        SetWindowText(dlg, txt_DlgTitle);
        SetDlgItemText(dlg, 104, txt_DlgLabel104);
        SetDlgItemText(dlg, 105, txt_DlgLabel105);
        if (dd_DlgFlag) {
            SetWindowLong(dlg, GWL_STYLE, GetWindowLong(dlg, GWL_STYLE) & ~WS_CAPTION);
            EnableWindow(dd_DrvCombo, FALSE);
            while (ShowCursor(TRUE) < 1)
                ;
            SendMessage(dd_DevCombo, CB_RESETCONTENT, 0, 0);
            d3d_NumDevices = 0;
#line 889
            D3DCHECK(dd_D3D3->lpVtbl->EnumDevices(dd_D3D3, d3d_EnumDeviceCallback, (void *)dd_BitPlanes));
            for (i = 0; i < d3d_NumDevices; i++) {
                strcpy(buf, d3d_Devices[i].desc);
                strcat(buf, " (");
                strcat(buf, d3d_Devices[i].name);
                strcat(buf, ")");
                idx = SendMessage(dd_DevCombo, CB_ADDSTRING, 0, (LPARAM)buf);
                SendMessage(dd_DevCombo, CB_SETITEMDATA, idx, (LPARAM)&d3d_Devices[i].Guid);
                if (!d3d_Devices[i].info.software)
                    sel = idx;
            }
            SendMessage(dd_DevCombo, CB_SETCURSEL, sel, 0);
            BringWindowToTop(dlg);
            return 1;
        }
        for (i = 0; i < dd_NumDevices; i++) {
            strcpy(buf, dd_Devices[i].desc);
            strcat(buf, " (");
            strcat(buf, dd_Devices[i].name);
            strcat(buf, ")");
            idx = SendMessage(dd_DrvCombo, CB_ADDSTRING, 0, (LPARAM)buf);
            SendMessage(dd_DrvCombo, CB_SETITEMDATA, idx, (LPARAM)dd_Devices[i].guid);
        }
        SendMessage(dd_DrvCombo, CB_SETCURSEL, 0, 0);
        SendMessage(dlg, WM_COMMAND, 102, (LPARAM)dd_DrvCombo);
        BringWindowToTop(dlg);
        return 1;
    case WM_CLOSE:
        cfg = reg_OpenKey("dd_vesa", 2);
        if (cfg) {
            if (!dd_DlgFlag) {
                i = SendMessage(dd_DrvCombo, CB_GETCURSEL, 0, 0);
                guid = (GUID *)SendMessage(dd_DrvCombo, CB_GETITEMDATA, i, 0);
                if (guid) {
                    if (s = dd_GuidToString(guid, guidstr))
                        reg_SetString(cfg, "dd_Guid", s);
                } else
                    reg_SetString(cfg, "dd_Guid", "00000000-0000-0000-0000000000000000");
            }
            i = SendMessage(dd_DevCombo, CB_GETCURSEL, 0, 0);
            guid = (GUID *)SendMessage(dd_DevCombo, CB_GETITEMDATA, i, 0);
            if (guid) {
                if (s = dd_GuidToString(guid, guidstr))
                    reg_SetString(cfg, "d3d_Guid", s);
            } else
                reg_SetString(cfg, "d3d_Guid", "00000000-0000-0000-0000000000000000");
            reg_CloseKey(cfg);
        }
        while (ShowCursor(FALSE) > 0)
            ;
        EndDialog(dlg, 0);
        return 1;
    }
    return 0;
}
/* 0x41cf60, driver/device selection dialog (resource 101 in d3_resource.dll) */

void dd_DeviceDialog(int bpp, int unused, HWND parent, char flag, LPDIRECT3D3 d3d3)
{
    HINSTANCE lib = LoadLibrary("d3_resource.dll");

    dd_DlgFlag = flag;
    dd_D3D3 = d3d3;
    dd_BitPlanes = bpp;
    if (lib) {
        DialogBoxParam(lib, MAKEINTRESOURCE(101), parent, dd_DlgProc, 0);
        FreeLibrary(lib);
    }
}
/* 0x41cfb0, creates dd_DDBase/dd_DD on the driver stored as "dd_Guid" (asks with the dialog if unset) */

char dd_Init(int mode, int bpp, HWND parent, int unk)
{
    GUID *guid;
    HKEY cfg;
    GUID g;
    char buf[0x100];
    char ask, shown;

    dd_NumDevices = 0;
    if (DirectDrawEnumerateEx(dd_EnumDriverCallback, NULL, mode == DD_FULLSCREEN || mode == DD_FULLMEM ? 7 : 0) != DD_OK
        || !dd_NumDevices)
        return 0;
    guid = NULL;
    if (dd_NumDevices > 1) {
        ask = 1;
        shown = 0;
        while (1) {
            if (shown)
                ask = 0;
            cfg = reg_OpenKey("dd_vesa", 1);
            if (cfg != -1) {
                if (reg_GetString(cfg, "dd_Guid", buf)) {
                    if (!strcmp("00000000-0000-0000-0000000000000000", buf)) {
                        guid = NULL;
                        ask = 0;
                    } else if (dd_StringToGuid(buf, &g)) {
                        int i;
                        for (i = 0; i < dd_NumDevices; i++)
                            if (!memcmp(&g, &dd_Devices[i].guiddata, sizeof(GUID))) {
                                guid = dd_Devices[i].guid;
                                ask = 0;
                            }
                    }
                }
                reg_CloseKey(cfg);
            }
            if (!ask)
                break;
            dd_DeviceDialog(bpp, unk, parent, 0, 0);
            shown = 1;
        }
    } else
        guid = dd_Devices[0].guid;
    if (DirectDrawCreate(guid, &dd_DDBase, NULL) != DD_OK) {
        err_Message("Unable to create DirectDraw-Object");
        return 0;
    }
#line 1073
    D3DCHECK(dd_DDBase->lpVtbl->QueryInterface(dd_DDBase, &IID_IDirectDraw4, &dd_DD));
    if (d3d_hr == 0)
        return 1;
    dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
    dd_DDBase = NULL;
    return 0;
}
/* 0x41d190, sets the clip rectangle's bounds */

void dd_SetClipRect(int left, int top, int right, int bottom)
{
    dd_ClipRect.left = left;
    dd_ClipRect.top = top;
    dd_ClipRect.right = right;
    dd_ClipRect.bottom = bottom;
}
/* 0x41d1c0, "dd_EnumVideoModesCallback": notes which of the larger resolutions exist at ctx bits */

HRESULT WINAPI dd_EnumVideoModesCallback(LPDDSURFACEDESC2 ddsd, LPVOID ctx)
{
    if (ddsd->ddpfPixelFormat.dwFlags & DDPF_RGB) {
        if (ddsd->ddpfPixelFormat.dwRGBBitCount == (DWORD)ctx) {
            if (ddsd->dwWidth == 800 && ddsd->dwHeight == 600)
                dd_Has800x600 = 1;
            if (ddsd->dwWidth == 1024 && ddsd->dwHeight == 768)
                dd_Has1024x768 = 1;
            if (ddsd->dwWidth == 1152 && ddsd->dwHeight == 864)
                dd_Has1152x864 = 1;
        }
    }
    return DDENUMRET_OK;
}
/* 0x41d230, probes the large display modes: clears dd_Has800x600 / dd_Has1024x768 / dd_Has1152x864, opens
   DirectDraw through dd_Init when it is not open yet and releases it again afterwards, then enumerates the
   standard VGA modes (0x41d1c0 notes the flags); returns 1 when DirectDraw was available, 0 otherwise */

char dd_CheckModes(int mode, int bpp, HWND parent, int c)
{
    char created = 0;

    dd_Has800x600 = 0;
    dd_Has1024x768 = 0;
    dd_Has1152x864 = 0;
    if (!dd_DD) {
        dd_Init(mode, bpp, parent, c);
        created = 1;
    }
    if (dd_DD) {
#line 1209
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, (LPVOID)bpp, dd_EnumVideoModesCallback));
        if (dd_DD && created) {
            dd_Ref = dd_DD->lpVtbl->Release(dd_DD);
            dd_DD = NULL;
        }
        return 1;
    }
    return 0;
}

/* CPUID feature bit 23 (MMX), 0 if the CPU has no CPUID */
__inline char dd_CpuHasMMX(void)
{
    __asm {
        pushfd
        pop ecx
        mov eax, ecx
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        test eax, 0x200000
        mov eax, 0
        je nocpuid
        inc eax
        cpuid
        test edx, 0x800000
        setne al
    nocpuid:
    }
}

/* the 888 pixel format's red mask (dd_RMask = ...; the original stores 0xff0000 to dd_RMask here) */
#define DD_RMASK888 0xff0000u

/* 0x41d2e0, sets up the display: mode DD_GDI..DD_FULLMEM, xres x yres at bpp bits
   @deep read the whole function against the original: 853 vs 855 instructions, 2753 vs 2742 bytes, the
   same call set (EnumDisplayModes, SetCooperativeLevel, SetDisplayMode, CreateSurface, GetAttachedSurface,
   CreateClipper, SetHWnd, SetClipper, GetPixelFormat, GetDC, CreateDIBSection, ReleaseDC, dd_Exit,
   dd_Restore, dd_Clear, dd_SetClipRect, dd_SetSurfaceName) and the same store set. The meaning differences
   are fixed: the BShift/GShift/RShift reset is one chained assignment, the loop tail stores the same six
   values, and the bpp guard is bpp > 8 (the original's mov eax,8 / cmp / jle). What is left is register
   allocation and schedule in the two bit-scan loops: the original keeps r,g,b in ecx/edi/esi and spills
   the loop counters to the dead parameter slots [ebp+0x20] and [ebp+0x10]; this source keeps r=edx, g=ecx,
   b=edi and the loop-2 counter in esi, so the eight 8-RLoss/GBits stores come in another order. Tried: one
   loop variable and two (i / j), split vs inline shifts, the loop tail in the original's order, the
   pixel-format expression flat and parenthesized, bpp > 8 and bpp >= 9; none moves the loop allocation,
   which is the remaining 130 of the 134 differing instructions. (references: M8 at +0x56e: ours refers to
   dd_RLoss (0x14650e4), the original's field to 0x1463570 (dd_RBits, dd_RBitsC) (and 4 more), explained:
   the 5 ref lines are the loop tail's crossed stores: ours writes dd_RBits, dd_RLoss, dd_GLoss, dd_GBits,
   dd_BBits, dd_BLoss, the original dd_RBits, dd_GBits, dd_RLoss, dd_BBits, dd_GLoss, dd_BLoss; each of the
   six fields is written exactly once on both sides with the same value (RBits=rb, GBits=gb, BBits=bb,
   RLoss=8-rb, GLoss=8-gb, BLoss=8-bb), and the reference check pairs each shifted store with the next
   field of the other side) */
char dd_SetMode(int xres, int yres, int bpp, int mode, HWND wnd, int notused, char triple)
{
    DDSURFACEDESC2 ddsd;
    BITMAPINFO bmi;
    DDPIXELFORMAT ddpf;
    DDSCAPS2 ddscaps;
    HDC dc;
    DWORD r, g, b;
    unsigned char rb, gb, bb;
    int i;

    if (!dd_DD)
        return 0;
    dd_Mode = mode;
    dd_Surface = NULL;
    dd_SurfaceFlags = 0;
    dd_MemBuff = NULL;
    dd_MemBottom = NULL;
    dd_Width = xres;
    dd_Height = yres;
    dd_RShift = dd_GShift = dd_BShift = 0;
    dd_BitPlanes = bpp;
    dd_Window = wnd;
    for (i = bpp; i & 7; i++)
        ;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    dd_BytesPerPixel = i / 8;
    dd_MemPitch = dd_BytesPerPixel * xres;
    dd_MemSize = dd_MemPitch * yres;
    dd_HasMMX = dd_CpuHasMMX();
    dd_Has800x600 = 0;
    dd_Has1024x768 = 0;
    dd_Has1152x864 = 0;
    if (!dd_HasMMX)
        return 0;
    d3_InitKeyCodes(dd_KeyCodes);
    dd_TripleBuffer = 0;
    if (dd_Mode == DD_FULLMEM || dd_Mode == DD_WINDOWMEM) {
        dd_MemBuff = mem_Alloc(dd_MemSize, "dd:membuff");
        DD_ZERO(dd_MemBuff, dd_MemSize);
    }
    switch (dd_Mode) {
    case DD_FULLSCREEN:
    case DD_FULLMEM:
#line 1283
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, (LPVOID)bpp, dd_EnumVideoModesCallback));
        D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_FPUSETUP));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
#line 1291
        D3DCHECK(dd_DD->lpVtbl->SetDisplayMode(dd_DD, xres, yres, bpp, 0, DDSDM_STANDARDVGAMODE));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
        if (triple) {
            ddsd.dwBackBufferCount = 2;
            if (d3d_hr = dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL)) {
                ddsd.dwBackBufferCount = 1;
#line 1308
                D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
                if (d3d_hr) {
                    dd_Exit();
                    return 0;
                }
            } else
                dd_TripleBuffer = 1;
        } else {
            ddsd.dwBackBufferCount = 1;
#line 1321
            D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
            if (d3d_hr) {
                dd_Exit();
                return 0;
            }
        }
        dd_SetSurfaceName(dd_FrontBuffer, "dd_FrontBuffer", 0);
        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
#line 1331
        D3DCHECK(dd_FrontBuffer->lpVtbl->GetAttachedSurface(dd_FrontBuffer, &ddscaps, &dd_BackBuffer));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        if (!dd_Restore()) {
            dd_Exit();
            err_Message("Could not restore dd_FrontBuffer!");
            return 0;
        }
        dd_SetSurfaceName(dd_BackBuffer, "dd_BackBuffer", 0);
        break;
    case DD_WINDOW:
    case DD_WINDOWMEM:
#line 1348
        D3DCHECK(dd_DD->lpVtbl->EnumDisplayModes(dd_DD, DDEDM_STANDARDVGAMODES, NULL, NULL, dd_EnumVideoModesCallback));
        D3DCHECK(dd_DD->lpVtbl->SetCooperativeLevel(dd_DD, dd_Window, DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES | DDSCL_FPUSETUP));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
#line 1360
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_FrontBuffer, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        dd_SetSurfaceName(dd_FrontBuffer, "dd_FrontBuffer", 0);
#line 1367
        D3DCHECK(dd_DD->lpVtbl->CreateClipper(dd_DD, 0, &lpClipper, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
#line 1373
        D3DCHECK(lpClipper->lpVtbl->SetHWnd(lpClipper, 0, dd_Window));
        D3DCHECK(dd_FrontBuffer->lpVtbl->SetClipper(dd_FrontBuffer, lpClipper));
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
        ddsd.dwWidth = xres;
#ifdef REA_FIX_16BPP
        /* REA_FIX_16BPP=1 (tools/build.py): request a 16 bit back buffer, the format the 2D
           blitters draw, instead of the desktop's 32 bit format that an offscreen surface without
           DDSD_PIXELFORMAT picks up in windowed mode (the fullscreen path gets 16 bit from SetDisplayMode) */
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
        ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
        ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
        ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
        ddsd.ddpfPixelFormat.dwGBitMask = 0x07e0;
        ddsd.ddpfPixelFormat.dwBBitMask = 0x001f;
#else
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
#endif
        ddsd.dwHeight = yres;
#line 1392
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &dd_BackBuffer, NULL));
        if (d3d_hr) {
            dd_Exit();
            return 0;
        }
        if (!dd_Restore()) {
            dd_Exit();
            err_Message("Could not restore dd_FrontBuffer(window)!");
            return 0;
        }
        dd_SetSurfaceName(dd_BackBuffer, "dd_BackBuffer", 0);
        break;
    case DD_GDI:
        dc = GetDC(dd_Window);
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = xres;
        bmi.bmiHeader.biHeight = yres;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = bpp;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biXPelsPerMeter = 0;
        bmi.bmiHeader.biYPelsPerMeter = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        dd_Bitmap = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &dd_MemBuff, NULL, 0);
        if (!dd_Bitmap)
            return 0;
        ReleaseDC(dd_Window, dc);
        D3_ZERO_AT(dd_MemBuff, bpp / 8 * xres * yres);
        if (bmi.bmiHeader.biHeight > 0)
            dd_MemBottom = (unsigned char *)dd_MemBuff + (yres - 1) * dd_MemPitch;
        else
            dd_MemBottom = NULL;
        if (bpp != 16) {
            if (bpp == 24 || bpp == 32) {
                dd_RShift = 16;
                dd_GShift = 8;
                dd_BShift = 0;
                dd_RLoss = 8;
                dd_GLoss = 8;
                dd_BLoss = 8;
                dd_BBits = 8;
                dd_GBits = 8;
                dd_RBits = 8;
                dd_RMask = DD_RMASK888;
                dd_GMask = 0xff00;
                dd_BMask = 0xff;
            }
        } else {
            dd_BShift = 0;
            dd_RShift = 10;
            dd_GShift = 5;
            dd_RLoss = 3;
            dd_GLoss = 3;
            dd_BLoss = 3;
            dd_BBits = 5;
            dd_GBits = 5;
            dd_RBits = 5;
            dd_RMask = 0x7c00;
            dd_GMask = 0x3e0;
            dd_BMask = 0x1f;
        }
        break;
    }
    if (bpp > 8 && dd_Mode != DD_GDI) {
        D3_ZERO(&ddpf, sizeof(ddpf));
        ddpf.dwSize = sizeof(ddpf);
#line 1463
        D3DCHECK(dd_FrontBuffer->lpVtbl->GetPixelFormat(dd_BackBuffer, &ddpf));
        dd_RMask = r = ddpf.dwRBitMask;
        dd_GMask = g = ddpf.dwGBitMask;
        dd_BMask = b = ddpf.dwBBitMask;
        for (i = 0; i < bpp; i++) {
            if ((r & 1) != 1) {
                r >>= 1;
                dd_RShift++;
            }
            if ((g & 1) != 1) {
                g >>= 1;
                dd_GShift++;
            }
            if ((b & 1) != 1) {
                b >>= 1;
                dd_BShift++;
            }
        }
        rb = 0;
        gb = 0;
        bb = 0;
        for (i = 0; i < bpp; i++) {
            if ((r & 1) == 1) {
                r >>= 1;
                rb++;
            }
            if ((g & 1) == 1) {
                g >>= 1;
                gb++;
            }
            if ((b & 1) == 1) {
                b >>= 1;
                bb++;
            }
        }
        dd_RBits = rb;
        dd_GBits = gb;
        dd_RLoss = 8 - rb;
        dd_BBits = bb;
        dd_GLoss = 8 - gb;
        dd_BLoss = 8 - bb;
    }
    dd_PixelFormat = 888 - dd_RLoss * 100 - dd_GLoss * 10 - dd_BLoss;
    dd_Pitch = 0;
    dd_PitchPixels = 0;
    dd_ColorBitMask = ((1 << (7 - dd_BLoss)) - 1 << dd_BShift) + ((1 << (7 - dd_GLoss)) - 1 << dd_GShift)
                  + ((1 << (7 - dd_RLoss)) - 1 << dd_RShift);
    dd_ColorBitMaskDword = dd_ColorBitMask << 16 | dd_ColorBitMask;
    dd_ColorBitMask |= 0xffff0000;
    dd_Clear(NULL, 0, 0, 0);
    dd_SetClipRect(0, 0, xres - 1, yres - 1);
    return 1;
}
/* 0x41ddb0, locks the surface (DDLOCK_WAIT; on DDERR_SURFACELOST it restores and retries), caches the pixel
   format of the lock in dd_LockBits / dd_LockPitch / dd_Pitch / dd_LockBytes / dd_PitchPixels and returns the
   locked pixels, NULL when the lock failed */

void *dd_LockSurface(LPDIRECTDRAWSURFACE4 surf)
{
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    ddsd.dwSize = sizeof(ddsd);
    while (1) {
        hr = surf->lpVtbl->Lock(surf, NULL, &ddsd, DDLOCK_WAIT, NULL);
        if (hr == DD_OK) {
            dd_LockBits = ddsd.ddpfPixelFormat.dwRGBBitCount;
            dd_LockPitch = ddsd.lPitch;
            dd_Pitch = ddsd.lPitch;
            dd_LockBytes = ddsd.ddpfPixelFormat.dwRGBBitCount >> 3;
            dd_PitchPixels = (DWORD)ddsd.lPitch / dd_BytesPerPixel;
            return ddsd.lpSurface;
        }
        if (hr != DDERR_SURFACELOST)
            return NULL;
        if (surf->lpVtbl->Restore(surf) != DD_OK)
            return NULL;
    }
}
/* 0x41de30, locks surf with the extra lock flags (NOSYSLOCK | WAIT always): sets the lock's bits / pitch / bytes
   globals and returns the surface memory; a lost surface is restored and locked again, any other error gives NULL */

void *dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags)
{
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    ddsd.dwSize = sizeof(ddsd);
    while (1) {
        hr = surf->lpVtbl->Lock(surf, NULL, &ddsd, flags | DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL);
        if (hr == DD_OK) {
            dd_LockBits = ddsd.ddpfPixelFormat.dwRGBBitCount;
            dd_LockPitch = ddsd.lPitch;
            dd_Pitch = ddsd.lPitch;
            dd_LockBytes = ddsd.ddpfPixelFormat.dwRGBBitCount >> 3;
            dd_PitchPixels = (DWORD)ddsd.lPitch / dd_BytesPerPixel;
            return ddsd.lpSurface;
        }
        if (hr != DDERR_SURFACELOST)
            return NULL;
        if (surf->lpVtbl->Restore(surf) != DD_OK)
            return NULL;
    }
}
/* 0x41dec0, sets dd_Surface/dd_Pitch/dd_PitchPixels for drawing */

char dd_Lock(void)
{
    switch (dd_Mode) {
    case DD_WINDOWMEM:
    case DD_FULLMEM:
        dd_Surface = dd_MemBuff;
        dd_Pitch = dd_MemPitch;
        dd_PitchPixels = dd_Width;
        return 1;
    case DD_GDI:
        if (!dd_MemBottom) {
            dd_Surface = dd_MemBuff;
            dd_Pitch = dd_MemPitch;
            dd_PitchPixels = dd_Width;
        } else {
            dd_Surface = dd_MemBottom;
            dd_Pitch = -dd_MemPitch;
            dd_PitchPixels = -dd_Width;
        }
        return 1;
    case DD_WINDOW:
    case DD_FULLSCREEN:
        dd_Surface = dd_LockSurface(dd_BackBuffer);
        return dd_Surface != NULL;
    }
    return 0;
}
/* 0x41df80, locks the drawing surface now (no wait), by display mode */

char dd_LockNoWait(void)
{
    switch (dd_Mode) {
    case DD_WINDOWMEM:
    case DD_FULLMEM:
        dd_Surface = dd_MemBuff;
        dd_Pitch = dd_MemPitch;
        dd_PitchPixels = dd_Width;
        return 1;
    case DD_GDI:
        if (!dd_MemBottom) {
            dd_Surface = dd_MemBuff;
            dd_Pitch = dd_MemPitch;
            dd_PitchPixels = dd_Width;
        } else {
            dd_Surface = dd_MemBottom;
            dd_Pitch = -dd_MemPitch;
            dd_PitchPixels = -dd_Width;
        }
        return 1;
    case DD_WINDOW:
    case DD_FULLSCREEN:
        dd_Surface = dd_LockSurfaceFlags(dd_BackBuffer, 0);
        return dd_Surface != NULL;
    }
    return 0;
}
/* 0x41e050, unlocks the drawing surface (releases the back buffer in fullscreen modes) */

void dd_Unlock(void)
{
    if (dd_Surface) {
        switch (dd_Mode) {
        case DD_WINDOW:
        case DD_FULLSCREEN:
            dd_BackBuffer->lpVtbl->Unlock(dd_BackBuffer, NULL);
        case DD_GDI:
        case DD_WINDOWMEM:
        case DD_FULLMEM:
            dd_Surface = NULL;
        }
    }
}
/* 0x41e0a0, color-fills rect (NULL = the whole surface) of surf with a DDBLT_COLORFILL Blt, restoring the
   surface and retrying after DDERR_SURFACELOST; returns 1 when the fill went through, 0 otherwise */

char dd_FillSurface(LPDIRECTDRAWSURFACE4 surf, RECT *rect, DWORD color)
{
    DDBLTFX fx;
    HRESULT hr;

    fx.dwSize = sizeof(fx);
    fx.dwFillColor = color;
    while (1) {
        hr = surf->lpVtbl->Blt(surf, rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
        if (hr == DD_OK)
            return 1;
        if (hr != DDERR_SURFACELOST)
            break;
        surf->lpVtbl->Restore(surf);
    }
    return 0;
}
/* 0x41e100, clears rect (NULL = all) of the drawing surface, once per frame */

void dd_Clear(RECT *rect, int r, int g, int b)
{
    unsigned char *p;
    int w;

    if (dd_Cleared)
        return;
    switch (dd_Mode) {
    case DD_WINDOWMEM:
    case DD_FULLMEM:
        if (!dd_MemBuff)
            return;
        if (!rect) {
            DD_ZERO(dd_MemBuff, dd_MemSize);
            break;
        }
        p = (unsigned char *)dd_MemBuff + rect->left * dd_BytesPerPixel + rect->top * dd_MemPitch;
        w = (rect->right - rect->left) * dd_BytesPerPixel;
        for (b = rect->top; b < rect->bottom; b++) {
            DD_ZERO(p, w);
            p += dd_MemPitch;
        }
        break;
    case DD_GDI:
        if (!dd_MemBuff)
            return;
        if (!rect) {
            DD_ZERO(dd_MemBuff, dd_MemSize);
            break;
        }
        if (!dd_MemBottom) {
            p = (unsigned char *)dd_MemBuff + rect->left * dd_BytesPerPixel + rect->top * dd_MemPitch;
            w = (rect->right - rect->left) * dd_BytesPerPixel;
            for (b = rect->top; b < rect->bottom; b++) {
                DD_ZERO(p, w);
                p += dd_MemPitch;
            }
        } else {
            p = (unsigned char *)dd_MemBottom + rect->left * dd_BytesPerPixel - rect->top * dd_MemPitch;
            w = (rect->right - rect->left) * dd_BytesPerPixel;
            for (b = rect->top; b < rect->bottom; b++) {
                DD_ZERO(p, w);
                p -= dd_MemPitch;
            }
        }
        break;
    case DD_WINDOW:
    case DD_FULLSCREEN:
        if (!dd_BackBuffer)
            return;
        dd_FillSurface(dd_BackBuffer, rect, dd_RGB(r, g, b));
        break;
    }
    dd_Cleared = 1;
}
/* 0x41e390, shows the finished frame */

void dd_Flip(void)
{
    DWORD flags = DDFLIP_WAIT;
    void *p;
    RECT rc;
    POINT pt;
    RECT src;
    HRESULT hr;
    HDC dc, mem;
    HGDIOBJ old;

#ifdef REA_FRAME_DUMP
    /* debug build (dd_vesa.h): while dump.trigger holds a count > 0, dump the finished frame and count it down */
    {
        FILE *rd_t;
        int rd_n;
        REA_DUMP_TRIGGER(rd_n);
        if (rd_n > 0) {
            REA_DUMP_BACKBUFFER("final");
            rd_t = fopen("dump.trigger", "w");
            if (rd_t) {
                fprintf(rd_t, "%d\n", rd_n - 1);
                fclose(rd_t);
            }
        }
    }
#endif
    if (!dd_TripleBuffer)
        flags = DDFLIP_WAIT | DDFLIP_NOVSYNC;
    dd_Cleared = 0;
    dd_ZCleared = 0;
    switch (dd_Mode) {
    case DD_FULLMEM:
        p = dd_LockSurface(dd_BackBuffer);
        if (p) {
            DD_COPY(p, dd_MemBuff, dd_MemSize);
            dd_UnlockSurface(dd_BackBuffer);
            while (1) {
                hr = dd_FrontBuffer->lpVtbl->Flip(dd_FrontBuffer, NULL, flags);
                if (hr == DD_OK || hr != DDERR_SURFACELOST)
                    break;
                if (!dd_Restore())
                    return;
            }
        }
        break;
    case DD_WINDOWMEM:
        p = dd_LockSurface(dd_BackBuffer);
        if (p) {
            DD_COPY(p, dd_MemBuff, dd_MemSize);
            dd_UnlockSurface(dd_BackBuffer);
            GetClientRect(dd_Window, &rc);
            pt.y = 0;
            pt.x = 0;
            ClientToScreen(dd_Window, &pt);
            OffsetRect(&rc, pt.x, pt.y);
            while (1) {
                hr = dd_FrontBuffer->lpVtbl->Blt(dd_FrontBuffer, &rc, dd_BackBuffer, NULL, DDBLT_WAIT, NULL);
                if (hr == DD_OK)
                    return;
                if (hr != DDERR_SURFACELOST)
                    return;
                if (!dd_Restore())
                    return;
            }
        }
    case DD_FULLSCREEN:
        while (1) {
            hr = dd_FrontBuffer->lpVtbl->Flip(dd_FrontBuffer, NULL, flags);
            if (hr == DD_OK) {
                dd_Page = !dd_Page;
                return;
            }
            if (hr != DDERR_SURFACELOST)
                return;
            if (!dd_Restore())
                return;
        }
    case DD_WINDOW:
        GetClientRect(dd_Window, &rc);
        pt.y = 0;
        pt.x = 0;
        ClientToScreen(dd_Window, &pt);
        OffsetRect(&rc, pt.x, pt.y);
        src.bottom = dd_Height;
        src.right = dd_Width;
        src.top = 0;
        src.left = 0;
        dd_FrontBuffer->lpVtbl->Blt(dd_FrontBuffer, &rc, dd_BackBuffer, &src, DDBLT_WAIT | DDBLT_ASYNC, NULL);
        break;
    case DD_GDI:
        dc = GetDC(dd_Window);
        mem = CreateCompatibleDC(dc);
        old = SelectObject(mem, dd_Bitmap);
        BitBlt(dc, 0, 0, dd_Width, dd_Height, mem, 0, 0, SRCCOPY);
        ReleaseDC(dd_Window, dc);
        SelectObject(mem, old);
        DeleteDC(mem);
        break;
    }
}
/* 0x41e670, packs r/g/b into the current display's pixel format */

DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b)
{
    return (r >> dd_RLoss << dd_RShift) | (g >> dd_GLoss << dd_GShift) | (b >> dd_BLoss << dd_BShift);
}
/* 0x41e6d0, splits the packed color c into r / g / b through the pixel format's shifts and losses
   (dd_RShift / dd_GShift / dd_BShift, dd_RLoss / dd_GLoss / dd_BLoss) */

void dd_SplitRGB(DWORD c, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = (unsigned char)(c >> dd_RShift) << dd_RLoss;
    *g = (unsigned char)(c >> dd_GShift) << dd_GLoss;
    *b = (unsigned char)(c >> dd_BShift) << dd_BLoss;
}
/* 0x41e720, releases the surface (dd_Ref = the Release result, the last one is the ComRefCount) */

void dd_ReleaseSurface(LPDIRECTDRAWSURFACE4 surf)
{
    if (surf)
        dd_Ref = surf->lpVtbl->Release(surf);
}
/* 0x41e740, unlocks the surface, nothing when surf is NULL */

void dd_UnlockSurface(LPDIRECTDRAWSURFACE4 surf)
{
    if (surf)
        surf->lpVtbl->Unlock(surf, NULL);
}
/* 0x41e760, Bresenham into the locked 16-bit dd_Surface */

void dd_DrawLine16(int x0, int y0, int x1, int y1, int color)
{
    int dx, dy, sx, sy, e, n, p, inc1, inc2;

    if (y0 == y1) {
        if (x1 < x0) {
            p = dd_PitchPixels * y0 + x1;
            n = x0 - x1;
        } else {
            p = dd_PitchPixels * y0 + x0;
            n = x1 - x0;
        }
        for (; n > 0; n--)
            ((unsigned short *)dd_Surface)[p++] = color;
    } else if (x0 == x1) {
        if (y1 < y0) {
            p = dd_PitchPixels * y1 + x0;
            n = y0 - y1;
        } else {
            p = dd_PitchPixels * y0 + x0;
            n = y1 - y0;
        }
        for (; n > 0; n--) {
            ((unsigned short *)dd_Surface)[p] = color;
            p += dd_PitchPixels;
        }
    } else {
        if (x0 < x1) {
            sx = 1;
            dx = x1 - x0;
        } else {
            sx = -1;
            dx = x0 - x1;
        }
        if (y0 < y1) {
            sy = 1;
            dy = y1 - y0;
        } else {
            sy = -1;
            dy = y0 - y1;
        }
        ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
        if (dx > dy) {
            inc2 = 2 * (dy - dx);
            inc1 = 2 * dy;
            e = inc1 - dx;
            do {
                x0 += sx;
                if (e >= 0) {
                    y0 += sy;
                    e += inc2;
                } else
                    e += inc1;
                ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
            } while (x0 != x1);
        } else {
            inc2 = 2 * (dx - dy);
            inc1 = 2 * dx;
            e = inc1 - dy;
            do {
                y0 += sy;
                if (e >= 0) {
                    x0 += sx;
                    e += inc2;
                } else
                    e += inc1;
                ((unsigned short *)dd_Surface)[dd_PitchPixels * y0 + x0] = color;
            } while (y0 != y1);
        }
    }
}
/* 0x41e8f0, draws a line (0x41e760) into the drawing surface when the lock succeeds */

void dd_Line(int x0, int y0, int x1, int y1, int color)
{
    if (dd_Lock()) {
        dd_DrawLine16(x0, y0, x1, y1, color);
        dd_Unlock();
    }
}
/* 0x41e920, shift and lost bits of each channel of a pixel format */

void dd_GetMaskShifts(DWORD rmask, DWORD gmask, DWORD bmask, unsigned char *rloss, unsigned char *rshift,
                      unsigned char *gloss, unsigned char *gshift, unsigned char *bloss, unsigned char *bshift)
{
    unsigned char bs = 0, gs = 0, rs = 0;
    unsigned char rb, gb, bb;
    int i;

    for (i = 32; i; i--) {
        if (!(rmask & 1)) {
            rmask >>= 1;
            rs++;
        }
        if (!(gmask & 1)) {
            gmask >>= 1;
            gs++;
        }
        if (!(bmask & 1)) {
            bmask >>= 1;
            bs++;
        }
    }
    rb = gb = bb = 0;
    for (i = 32; i; i--) {
        if ((rmask & 1) == 1) {
            rmask >>= 1;
            rb++;
        }
        if ((gmask & 1) == 1) {
            gmask >>= 1;
            gb++;
        }
        if ((bmask & 1) == 1) {
            bmask >>= 1;
            bb++;
        }
    }
    *rloss = 8 - rb;
    *gloss = 8 - gb;
    *bloss = 8 - bb;
    *rshift = rs;
    *gshift = gs;
    *bshift = bs;
}
/* 0x41ea20, three 256-entry tables mapping 8-bit channel values to pixel bits */

void dd_MakeColorTables(DWORD rmask, DWORD gmask, DWORD bmask, DWORD *tab)
{
    unsigned char rl, rs, gl, gs, bl, bs;
    DWORD i;

    dd_GetMaskShifts(rmask, gmask, bmask, &rl, &rs, &gl, &gs, &bl, &bs);
    for (i = 0; i < 256; i++) {
        tab[i] = i >> rl << rs;
        tab[256 + i] = i >> gl << gs;
        tab[512 + i] = i >> bl << bs;
    }
}
/* 0x41eb00, tags a surface with a name for debugging */

char dd_SetSurfaceName(LPDIRECTDRAWSURFACE4 surf, char *data, char unk)
{
    GUID userdata = {0xf0eb1210, 0xaa9c, 0x4919, {0x9c, 0x15, 0x65, 0x99, 0x04, 0xd9, 0x8f, 0x51}};

    if (!surf)
        return 0;
    userdata.Data4[7] += unk;
#line 2609
    D3DCHECK(surf->lpVtbl->SetPrivateData(surf, &userdata, (void *)data, strlen(data) + 1, 0));
    return d3d_hr == 0;
}
