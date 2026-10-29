/* 0x47cf00, draws a 16 bit shape, clipped at top and bottom only */

void d2_ShpBlit16ClipY(int x, int y, void *bits, D2SHP *s)
{
#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d surfpitch=%d\n", GetTickCount(), "ClipY", s->w, s->h, x, y, s->mode, d2_SurfPitch));
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
                shl eax,1
                xor ecx,ecx
                mov ebx,edx
                mov cx,word ptr [esi+0xa]
                add ebx,ecx
                cmp ebx,dword ptr d2_ClipTop
                jle LclipY_done
                cmp edx,dword ptr d2_ClipBottom
                jge LclipY_done
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
                jle LclipY_afterTop
                mov eax,esi
                add eax,dword ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    LclipY_afterTop:
                mov ecx,dword ptr d2_ClipBottom
                pop edx
                mov eax,edx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle LclipY_afterBottom
                sub dword ptr d2_ShpRows,eax
                cmp dword ptr d2_ShpRows,0x0
                jle LclipY_done
    LclipY_afterBottom:
                mov eax,d2_SurfPitch
                shl eax,1
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je LclipY_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                xor eax,eax
                mov al,BYTE ptr [esi+0xd]
                add esi,dword ptr d2_ShpDataOfs
                cmp al,0x0
                je LclipY_mode0_row
                cmp al,0x1
                je LclipY_mode1_entry
                cmp al,0x2
                je LclipY_mode2_entry
                cmp al,0x3
                je LclipY_mode3_entry
                cmp al,0x4
                je LclipY_mode4_entry
                cmp al,0x5
                je LclipY_mode5_entry
    LclipY_mode0_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode0_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode0_nextRun
                bt ecx,0x0
                jae LclipY_mode0_oddDone
                mov ax,word ptr [esi]
                mov word ptr [edi],ax
                add edi,0x2
                add esi,0x2
    LclipY_mode0_oddDone:
                shr ecx,1
                rep movsd
    LclipY_mode0_nextRun:
                dec edx
                jne LclipY_mode0_run
                add edi,ebp
                dec ebx
                jne LclipY_mode0_row
                jmp LclipY_done
    LclipY_mode3_entry:
                xor ebp,ebp
                mov eax,dword ptr [d2_ClipTop+4]
    LclipY_mode3_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode3_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode3_nextRun
    LclipY_mode3_pixel:
                mov bp,word ptr [edi]
                mov bp,word ptr [eax+ebp*2]
                mov word ptr [edi],bp
                add edi,0x2
                dec ecx
                jne LclipY_mode3_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipY_mode3_nextRun:
                dec edx
                jne LclipY_mode3_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode3_row
                jmp LclipY_done
    LclipY_mode1_entry:
                xor ebp,ebp
    LclipY_mode1_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode1_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode1_nextRun
                bt ecx,0x0
                jae LclipY_mode1_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipY_mode1_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode1_nextRun
    LclipY_mode1_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipY_mode1_pair
    LclipY_mode1_nextRun:
                dec edx
                jne LclipY_mode1_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode1_row
                jmp LclipY_done
    LclipY_mode4_entry:
                xor ebp,ebp
    LclipY_mode4_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode4_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode4_nextRun
                bt ecx,0x0
                jae LclipY_mode4_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipY_mode4_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode4_nextRun
    LclipY_mode4_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipY_mode4_pair
    LclipY_mode4_nextRun:
                dec edx
                jne LclipY_mode4_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode4_row
                jmp LclipY_done
    LclipY_mode2_entry:
                xor ebp,ebp
    LclipY_mode2_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode2_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode2_nextRun
                bt ecx,0x0
                jae LclipY_mode2_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,word ptr d2_HighBitMask
                mov word ptr [edi],bp
                add edi,0x2
    LclipY_mode2_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode2_nextRun
    LclipY_mode2_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,dword ptr d2_HighBitMask32
                mov dword ptr [edi],ebp
                add edi,0x4
                dec ecx
                jne LclipY_mode2_pair
    LclipY_mode2_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipY_mode2_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode2_row
                jmp LclipY_done
    LclipY_mode5_entry:
                xor ebp,ebp
                mov ebp,dword ptr d2_Mode5Colour
    LclipY_mode5_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode5_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode5_nextRun
                bt ecx,0x0
                jae LclipY_mode5_oddDone
                mov word ptr [edi],bp
                add edi,0x2
    LclipY_mode5_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode5_nextRun
    LclipY_mode5_pair:
                mov dword ptr [edi],ebp
                add edi,0x4
                dec ecx
                jne LclipY_mode5_pair
    LclipY_mode5_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipY_mode5_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode5_row
                jmp LclipY_done
    LclipY_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}
