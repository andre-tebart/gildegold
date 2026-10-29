/* 0x43ed70, the same with MMX (all inline asm) */

void d3fl_BlendMMX(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n)
{
    __asm {
        mov eax, dst
        mov edx, w
        mov ebx, src
        mov ecx, n
        push ebp
        push esi
        push edi
        mov edi, eax
        mov ebp, edx
        mov esi, ebx
        mov edx, [esi]
        cmp [esi+4], edx
        jne blend
        cmp [esi+8], edx
        jne blend
        cmp [esi+12], edx
        jne blend
        mov esi, edx
        mov edx, ecx
        xor ebx, ebx
    row1:
        push edi
        push esi
        mov ebp, d3fl_TileN
        align 16
    pix1:
        mov eax, [esi]
        mov ebx, eax
        and ebx, 0xff
        mov ecx, d3fl_ColorTab[ebx*4]
        shr eax, 8
        mov ebx, eax
        and ebx, 0xff
        add ecx, d3fl_ColorTab[ebx*4+0x400]
        shr eax, 8
        add esi, d3fl_SrcStep
        add ecx, d3fl_ColorTab[eax*4+0x800]
        mov [edi], cx
        add edi, 2
        dec ebp
        jne pix1
        pop esi
        add esi, d3fl_SrcPitch
        pop edi
        add edi, d3fl_DstPitch
        dec edx
        jne row1
        pop edi
        pop esi
        pop ebp
        jmp done
    blend:
        xor edx, edx
        pcmpeqb mm7, mm7
        psrlw mm7, 8
    row2:
        push ecx
        push edi
        push edx
        push ebp
        mov ecx, d3fl_TileN
        align 16
    pix2:
        mov eax, [esi]
        movd mm0, [eax+edx]
        mov eax, [esi+4]
        movd mm1, [eax+edx]
        mov eax, [esi+8]
        movd mm2, [eax+edx]
        mov eax, [esi+12]
        movd mm3, [eax+edx]
        punpcklwd mm0, mm1
        punpcklwd mm2, mm3
        movq mm1, mm0
        movq mm3, mm2
        punpckldq mm0, mm2
        punpckhdq mm1, mm3
        movq mm2, mm0
        pand mm2, mm7
        psrlw mm0, 8
        punpcklbw mm6, [ebp]
        add edx, d3fl_SrcStep
        add ebp, d3fl_SrcStep
        psrlw mm6, 8
        pmaddwd mm2, mm6
        movq mm3, mm2
        psrlq mm2, 32
        paddd mm3, mm2
        psrld mm3, 8
        movd ebx, mm3
        mov eax, d3fl_ColorTab[ebx*4]
        pmaddwd mm0, mm6
        movq mm4, mm0
        psrlq mm0, 32
        paddd mm4, mm0
        psrld mm4, 8
        movd ebx, mm4
        add eax, d3fl_ColorTab[ebx*4+0x400]
        pmaddwd mm1, mm6
        movq mm5, mm1
        psrlq mm1, 32
        paddd mm5, mm1
        psrld mm5, 8
        movd ebx, mm5
        add eax, d3fl_ColorTab[ebx*4+0x800]
        mov [edi], ax
        add edi, 2
        dec ecx
        jne pix2
        pop ebp
        add ebp, d3fl_WPitch
        pop edx
        add edx, d3fl_SrcPitch
        pop edi
        add edi, d3fl_DstPitch
        pop ecx
        dec ecx
        jne row2
        pop edi
        pop esi
        pop ebp
    done:
    }
}
