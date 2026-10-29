/* 0x47d960, darkens h rows of w 16 bit pixels to half brightness at dst (a shift, then the colour masks
 * 0x3def / 0x3defbdef); w and the source of an odd first pixel arrive in the caller's edx / esi (this asm
 * was one piece with its caller), the row pitch is d2_SurfPitch */

#pragma optimize("", on)

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
