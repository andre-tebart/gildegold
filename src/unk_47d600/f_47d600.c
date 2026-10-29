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
