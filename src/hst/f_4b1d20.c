/* 0x4b1d20, resets the lists hst_Lists and opens the player's history texts "historie\text_H_<name>"
   (txt.c); none: hst_TextsOn = 0 */

/* G12_ZERO_G: AI_ZERO on a global (its address as an immediate) */
#define G12_ZERO_G(dst, size) {                                         int _zn = (size);                                                   __asm mov edi, OFFSET dst                                           __asm mov ebx, _zn                                                  __asm xor eax, eax                                                  __asm lea ecx, [edi+edi*2]                                          __asm and ecx, 3                                                    __asm sub ebx, ecx                                                  __asm jle AI_ZERO_LABEL(__LINE__)                                   __asm rep stosb                                                     __asm mov ecx, ebx                                                  __asm and ebx, 3                                                    __asm shr ecx, 2                                                    __asm rep stosd                                                     __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                         __asm rep stosb                                                 }


int hst_TextFirst = -1;  /* 0x601058, first text of the history file */
int hst_TextLast = -1;  /* 0x60105c, last text of the history file */
int hst_TextCurrent = -1;  /* 0x601060, the current text */
char ai_PlayerSlots[4][0x2f4];  /* 0x7e3f40, per player: starts with the name of its history file */
unsigned char hst_TextsOn = 0;  /* 0x6f0724, history texts on */

void ai_Unk4b1d20(void)
{
    char path[0x40];
    int i, j, k;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 0;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    hst_TextFirst = -1;
    hst_TextLast = -1;
    sprintf(path, "historie\\text_H_%s", ai_PlayerSlots[ai_OwnPlayer]);
    if (txt_Files[txt_FindFile(path)].buf == 0 && !txt_ReloadTextFile(path)) {
        hst_TextsOn = 0;
        return;
    }
    k = txt_FindFile(path);
    hst_TextFirst = txt_Files[k].first;
    hst_TextLast = txt_Files[k].last;
    hst_TextCurrent = txt_Files[k].first;
    G12_ZERO_G(hst_Key, 16);
    hst_KeyFirst = -1;
    ai_Unk601068 = -1;
}
