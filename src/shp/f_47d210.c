/* 0x47d210, draws a 16 bit shape, clipped left and right too (its dead variant blocks, je +0 dispatch, are in the asm verbatim) */

#pragma optimize("", off)   /* the original has no alignment pad after this function: its section ends at the
                               last byte and the linker fills 0xcc; VC6 pads only under the default optimization, so a
                               pragma stood here (optimize("s", on) gives the same bytes) */
void d2_ShpBlit16ClipXY(int x, int y, void *bits, D2SHP *s)
{
#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d surfpitch=%d\n", GetTickCount(), "ClipXY", s->w, s->h, x, y, s->mode, d2_SurfPitch));
#endif
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
                mov d2_ShpX,eax
                mov dword ptr d2_ShpY,edx
                mov d2_ShpRowX,eax
                shl eax,1
                xor ecx,ecx
                mov ebx,edx
                mov cx,word ptr [esi+0xa]
                add ebx,ecx
                cmp ebx,dword ptr d2_ClipTop
                jle LclipXY_done
                cmp edx,dword ptr d2_ClipBottom
                jge LclipXY_done
                push edx
                mov ebp,dword ptr d2_SurfPitch
                sub bp,word ptr [esi+0x6]
                shl ebp,1
                mov dword ptr d2_ShpRowGap,ebp
                xor ebx,ebx
                mov bx,word ptr [esi+0xa]
                mov dword ptr d2_ShpRows,ebx
                mov dword ptr d2_ShpDataOfs,0x32
                add edi,eax
                mov ebx,edx
                mov ecx,dword ptr d2_ClipTop
                sub ecx,ebx
                cmp ecx,0x0
                jle LclipXY_afterTop
                mov eax,esi
                add eax,dword ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    LclipXY_afterTop:
                mov ecx,dword ptr d2_ClipBottom
                pop edx
                mov eax,edx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle LclipXY_afterBottom
                sub dword ptr d2_ShpRows,eax
                cmp dword ptr d2_ShpRows,0x0
                jle LclipXY_done
    LclipXY_afterBottom:
                mov eax,d2_SurfPitch
                shl eax,1
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je LclipXY_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                mov al,BYTE ptr [esi+0xd]
                add esi,dword ptr d2_ShpDataOfs
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
                je LclipXY_mode2_entry
                cmp al,0x3
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x4
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x5
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
    LclipXY_mode0_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode0_run:
                add edi,dword ptr [esi]
                mov eax,dword ptr [esi]
                shr eax,1
                add dword ptr d2_ShpX,eax
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode0_nextRun
    LclipXY_mode0_pixel:
                mov eax,d2_ClipLeft
                cmp dword ptr d2_ShpX,eax
                jle LclipXY_mode0_pixelDone
                mov eax,d2_ClipRight
                cmp dword ptr d2_ShpX,eax
                jge LclipXY_mode0_pixelDone
                mov ax,word ptr [esi]
                mov word ptr [edi],ax
    LclipXY_mode0_pixelDone:
                add edi,0x2
                add esi,0x2
                add dword ptr d2_ShpX,0x1
                dec ecx
                jne LclipXY_mode0_pixel
    LclipXY_mode0_nextRun:
                dec edx
                jne LclipXY_mode0_run
                add edi,ebp
                mov eax,d2_ShpRowX
                mov d2_ShpX,eax
                dec ebx
                jne LclipXY_mode0_row
                jmp LclipXY_done
                xor ebp,ebp
                mov eax,dword ptr [d2_ClipTop+4]
    LclipXY_mode3_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode3_row:
                add edi,dword ptr [esi]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode3_nextRun
    LclipXY_mode3_pixel:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode3_pixelDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode3_pixelDone
                mov bp,word ptr [edi]
                mov bp,word ptr [eax+ebp*2]
                mov word ptr [edi],bp
    LclipXY_mode3_pixelDone:
                add dword ptr d2_ShpX,0x1
                add edi,0x2
                dec ecx
                jne LclipXY_mode3_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipXY_mode3_nextRun:
                dec edx
                jne LclipXY_mode3_row
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode3_entry
                jmp LclipXY_done
                xor ebp,ebp
    LclipXY_mode1_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode1_row:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode1_nextRun
                bt ecx,0x0
                jae LclipXY_mode1_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipXY_mode1_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode1_nextRun
    LclipXY_mode1_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipXY_mode1_pair
    LclipXY_mode1_nextRun:
                dec edx
                jne LclipXY_mode1_row
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipXY_mode1_entry
                jmp LclipXY_done
                xor ebp,ebp
    LclipXY_mode4_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode4_row:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode4_nextRun
                bt ecx,0x0
                jae LclipXY_mode4_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipXY_mode4_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode4_nextRun
    LclipXY_mode4_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipXY_mode4_pair
    LclipXY_mode4_nextRun:
                dec edx
                jne LclipXY_mode4_row
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipXY_mode4_entry
                jmp LclipXY_done
    LclipXY_mode2_entry:
                xor ebp,ebp
    LclipXY_mode2_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode2_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode2_nextRun
                bt ecx,0x0
                jae LclipXY_mode2_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,word ptr d2_HighBitMask
                mov word ptr [edi],bp
                add edi,0x2
    LclipXY_mode2_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode2_nextRun
    LclipXY_mode2_pair:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode2_pairDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode2_pairDone
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,dword ptr d2_HighBitMask32
                mov dword ptr [edi],ebp
    LclipXY_mode2_pairDone:
                add edi,0x4
                add dword ptr d2_ShpX,0x1
                dec ecx
                jne LclipXY_mode2_pair
    LclipXY_mode2_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipXY_mode2_run
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode2_row
                jmp LclipXY_done
                xor ebp,ebp
                mov eax,d2_Mode5Colour
    LclipXY_mode5_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode5_run:
                add edi,dword ptr [esi]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode5_nextRun
    LclipXY_mode5_pixel:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode5_pixelDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode5_pixelDone
                mov word ptr [edi],ax
    LclipXY_mode5_pixelDone:
                add dword ptr d2_ShpX,0x1
                add edi,0x2
                dec ecx
                jne LclipXY_mode5_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipXY_mode5_nextRun:
                dec edx
                jne LclipXY_mode5_run
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode5_row
    LclipXY_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}
