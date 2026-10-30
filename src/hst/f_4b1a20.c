/* 0x4b1a20, looks for the key hst_Key in the list from hst_KeyNext on (entries of 6, up to today); found:
   sends message 0x52 with its index */

/* G12_COPY: AI_ZERO's copy twin (inline rep movsb / movsd from a global) */
#define G12_COPY(dst, src, size) {                                  \
    int _cn = (size); void *_cp = (dst);                            \
    __asm mov edi, _cp                                              \
    __asm mov esi, OFFSET src                                       \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

extern int hst_KeyFirst;
extern int ai_Unk601068;
extern int hst_KeyNext;
extern char hst_Key[0x20];
int hst_ParseDate(int k, unsigned char mode, int *year, char *name);              /* 0x4b2ed0 */

int ai_Unk4b1a20(void)
{
    AIMSG msg;
    char key[16];
    int day[4];
    char name[16];
    int pos;

    pos = hst_KeyNext;
    if (hst_KeyFirst <= -1 && (hst_KeyNext < hst_KeyFirst || hst_KeyNext >= ai_Unk601068))
        return 0;
    AI_ZERO(key, 16);
    G12_COPY(key, hst_Key, strlen(hst_Key));
    AI_ZERO(name, 16);
    if (!hst_ParseDate(pos, 4, day, name))
        return 0;
    do {
        if (memcmp(key, name, strlen(name)) == 0) {
            hst_KeyNext = pos + 6;
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x52;
            msg.to = *(int *)((char *)ai_PlayerChar + 4);
            msg.car = pos - hst_KeyFirst;
            msg.fromId = -1;
            msg.flags = 0x12;
            cm_SendLongMessage(&msg);
            return 1;
        }
        if (day[0] > ai_TimeT.year)
            break;
        pos += 6;
        if (!hst_ParseDate(pos, 4, day, name))
            break;
    } while (hst_KeyNext < ai_Unk601068);
    return 0;
}
