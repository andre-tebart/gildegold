/* shp_asm.c -- 0x47d600.., the shape drawing loops for 8 bit shapes, written in inline assembly; it ends with
   the darkening blit 0x47d960; AI code follows from 0x47d9e0. Uses unk_47c5e0.h. Names are ours. Kept apart from
   shp.c by subject only: the 0xcc before 0x47d600 is the linker's fill after d2_ShpBlit16ClipXY's unpadded section
   (its pragma), not an object end.
   unit of rea 8cef7d5b (src/game/unk_47d600.c); boundaries in decl/text.dcl */
#include "data_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d2.h"
#include "shp.h"
#include "unk_47c5e0.h"

/* 0x47d600, blits the runs of an 8 bit shape into an 8 bit surface (bits, row pitch d2_SurfPitch) at x, y:
 * y is clipped to d2_ClipTop / d2_ClipBottom (x is the caller's clip); leaves the shape size in d2_ShpW /
 * d2_ShpH, the offset of the first visible line's runs in d2_ShpDataOfs and its row count in d2_ShpRows */
void d2_ShpBlit8(int x, int y, void *bits, D2SHP *s)
{
    __asm {
                push edi
                push esi
                push eax
                push ebx
                push edx
                push ecx
                push ebp
                mov eax,dword ptr [ebp+0x8]
                mov edx,dword ptr [ebp+0xc]
                mov ebx,dword ptr [ebp+0x10]
                mov ecx,dword ptr [ebp+0x14]
                mov edi,ebx
                mov esi,ecx
                mov ebx,edx
                add bx,word ptr [esi+0xa]
                cmp ebx,dword ptr d2_ClipTop
                jle Lblit_done
                cmp edx,dword ptr d2_ClipBottom
                jge Lblit_done
                mov ebp,dword ptr d2_SurfPitch
                sub bp,word ptr [ecx+0x6]        ; pitch - s->w: end of a line to the next row
                xor ebx,ebx
                mov bx,word ptr [esi+0xa]
                mov dword ptr d2_ShpRows,ebx
                mov dword ptr d2_ShpDataOfs,0x32 ; 0x32 = sizeof(D2SHP): the runs follow the header
                add edi,eax
                mov ebx,edx
                mov ecx,dword ptr d2_ClipTop
                sub ecx,ebx
                cmp ecx,0x0
                jle Lblit_no_top_clip
                mov eax,esi
                add ax,word ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]    ; s->lineoff: line offset of the first visible line
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    Lblit_no_top_clip:
                mov ecx,dword ptr d2_ClipBottom
                mov eax,ebx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle Lblit_no_bottom_clip
                sub dword ptr d2_ShpRows,eax
    Lblit_no_bottom_clip:
                mov eax,d2_SurfPitch
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je Lblit_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                add esi,dword ptr d2_ShpDataOfs
    Lblit_next_row:                              ; a line: dword count of runs, then per run skip, byte
                mov edx,dword ptr [esi]          ; count and pixels (an odd pixel first, then words)
                add esi,0x4
    Lblit_next_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je Lblit_run_done
                bt ecx,0x0
                jae Lblit_copy_words
                mov al,BYTE ptr [esi]
                mov BYTE ptr [edi],al
                inc edi
                inc esi
    Lblit_copy_words:
                shr ecx,1
                rep movs word ptr es:[edi],word ptr ds:[esi]
    Lblit_run_done:
                dec edx
                jne Lblit_next_run
                add edi,ebp
                dec ebx
                jne Lblit_next_row
    Lblit_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}

#pragma optimize("", off)   /* the original has no alignment pad after this function: its section ends at the
                               last byte and the linker fills 0xcc; VC6 pads only under the default optimization, so a
                               pragma stood here (optimize("s", on) gives the same bytes) */
/* 0x47d710, blits the runs of an 8 bit shape into a 16 bit surface (bits, row pitch d2_SurfPitch, line gap
 * d2_ShpRowGap) at x, y, clipped to d2_ClipTop / d2_ClipBottom; s->mode picks the pixel loop: 0 palette
 * (d2_ShpPalTable), 2 half dark, 1 light table (d2_ShpLightTable) and 3 add source, of which the original
 * reaches 1 and 3 only through dead je +0 dispatches (kept verbatim) */
void d2_ShpBlit8To16(int x, int y, void *bits, D2SHP *s)
{
    __asm {
                push edi
                push esi
                push eax
                push ebx
                push edx
                push ecx
                push ebp
                mov eax,dword ptr [ebp+0x8]
                mov edx,dword ptr [ebp+0xc]
                mov ebx,dword ptr [ebp+0x10]
                mov ecx,dword ptr [ebp+0x14]
                mov edi,ebx
                mov esi,ecx
                shl eax,1
                xor ecx,ecx
                mov ebx,edx
                mov cx,word ptr [esi+0xa]
                add ebx,ecx
                cmp ebx,dword ptr d2_ClipTop
                jle Lblit16_done
                cmp edx,dword ptr d2_ClipBottom
                jge Lblit16_done
                mov ebp,dword ptr d2_SurfPitch
                sub bp,word ptr [esi+0x6]
                shl ebp,1                          ; (pitch - s->w) * 2: the bytes a row leaves over
                mov dword ptr d2_ShpRowGap,ebp
                xor ebx,ebx
                mov bx,word ptr [esi+0xa]
                mov dword ptr d2_ShpRows,ebx
                mov dword ptr d2_ShpDataOfs,0x32   ; 0x32 = sizeof(D2SHP): the runs follow the header
                add edi,eax
                mov ebx,edx
                mov ecx,dword ptr d2_ClipTop
                sub ecx,ebx
                cmp ecx,0x0
                jle Lblit16_no_top_clip
                mov eax,esi
                add eax,dword ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    Lblit16_no_top_clip:
                mov ecx,dword ptr d2_ClipBottom
                mov eax,ebx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle Lblit16_no_bottom_clip
                sub dword ptr d2_ShpRows,eax
    Lblit16_no_bottom_clip:
                mov eax,d2_SurfPitch
                shl eax,1
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je Lblit16_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                mov al,BYTE ptr [esi+0xd]          ; s->mode: the je + 0 blocks are the original's dead
                add esi,dword ptr d2_ShpDataOfs   ; dispatches; what is not 2 runs the palette loop
                cmp al,0x0
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x1
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x2
                je Lblit16_m2_start
                cmp al,0x3
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                xor eax,eax
                xor edx,edx
    Lblit16_m0_row:
                mov ebp,dword ptr [esi]
                add esi,0x4
    Lblit16_m0_run:
                mov ecx,dword ptr [esi]
                shl ecx,1
                add edi,ecx
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je Lblit16_m0_run_done
    Lblit16_m0_pixel:
                mov ebx,dword ptr d2_ShpPalTable  ; 8 bit pixel -> 16 bit colour
                mov dl,BYTE ptr [esi]
                mov bx,word ptr [ebx+edx*2]
                mov word ptr [edi],bx
                add edi,0x2
                inc esi
                dec ecx
                jne Lblit16_m0_pixel
    Lblit16_m0_run_done:
                dec ebp
                jne Lblit16_m0_run
                add edi,dword ptr d2_ShpRowGap
                dec dword ptr d2_ShpRows
                cmp dword ptr d2_ShpRows,0x0
                jne Lblit16_m0_row
                jmp Lblit16_done
                xor ebp,ebp
                mov eax,dword ptr d2_ShpLightTable        ; mode 1 (dead): brightens the surface pixel
    Lblit16_m1_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    Lblit16_m1_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je Lblit16_m1_run_done
    Lblit16_m1_pixel:
                mov bp,word ptr [edi]
                mov bp,word ptr [eax+ebp*2]
                mov word ptr [edi],bp
                add edi,0x2
                dec ecx
                jne Lblit16_m1_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    Lblit16_m1_run_done:
                dec edx
                jne Lblit16_m1_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne Lblit16_m1_row
                jmp Lblit16_done
                xor ebp,ebp
    Lblit16_m3_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    Lblit16_m3_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je Lblit16_m3_run_done
                bt ecx,0x0
                jae Lblit16_m3_copy_words
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    Lblit16_m3_copy_words:
                shr ecx,1
    Lblit16_m3_word:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne Lblit16_m3_word
    Lblit16_m3_run_done:
                dec edx
                jne Lblit16_m3_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne Lblit16_m3_row
                jmp Lblit16_done
    Lblit16_m2_start:
                xor ebp,ebp
    Lblit16_m2_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    Lblit16_m2_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je Lblit16_m2_run_done
                bt ecx,0x0
                jae Lblit16_m2_copy_words
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                mov word ptr [edi],bp
                add edi,0x2
    Lblit16_m2_copy_words:
                shr ecx,1
    Lblit16_m2_word:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                mov dword ptr [edi],ebp
                add edi,0x4
                dec ecx
                jne Lblit16_m2_word
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    Lblit16_m2_run_done:
                dec edx
                jne Lblit16_m2_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne Lblit16_m2_row
                jmp Lblit16_done
    Lblit16_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}

#pragma optimize("", on)

/* 0x47d960, darkens h rows of w 16 bit pixels to half brightness at dst (a shift, then the colour masks
 * 0x3def / 0x3defbdef); w and the source of an odd first pixel arrive in the caller's edx / esi (this asm
 * was one piece with its caller), the row pitch is d2_SurfPitch */
void d2_ShpDarken16(void *dst, int w, int h)
{
    __asm {
        push edi
        push esi
        push eax
        push ebx
        push edx
        push ecx
        push ebp
        mov d2_BlitW, edx
        mov eax, [ebp+8]
        mov ecx, [ebp+0xc]
        mov ebx, [ebp+0x10]
        mov edi, eax
        xor ecx, ecx
        xor edx, edx
    Lrow:
        mov ecx, d2_BlitW
        bt ecx, 0
        jae Leven
        mov dx, [esi]
        shr dx, 1
        and dx, 0x3def
        mov [edi], dx
        add edi, 2
        add esi, 2
    Leven:
        shr ecx, 1
    Lpair:
        mov edx, [edi]
        shr edx, 1
        and edx, 0x3defbdef
        mov [edi], edx
        add edi, 4
        dec ecx
        jne Lpair
        mov ecx, d2_BlitW
        shl ecx, 1
        mov edx, d2_SurfPitch
        shl edx, 1
        sub edx, ecx
        add edi, edx
        dec ebx
        jne Lrow
        pop ebp
        pop ecx
        pop edx
        pop ebx
        pop eax
        pop esi
        pop edi
    }
}

int d2_ShpDataOfs = 50;  /* 0x5fef00 */
unsigned int d2_ShpRows = 0;  /* 0xf8a118 */
unsigned int d2_BlitW = 0;  /* 0xf8a634 */
unsigned int d2_ShpPalTable = 0;  /* 0xf8a644 */
