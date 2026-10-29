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
