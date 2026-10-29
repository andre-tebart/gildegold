/* 0x578440, frees the tutorial record chain (82 B, recursive): nothing for a null record; else its +0x70 next
   record, its +0x58 and +0x64 tables (mem_Free, both cleared) and then the record itself */

void ai_Unk578440(char *rec)
{
    if (!rec)
        return;
    if (*(char **)(rec + 0x70))
        ai_Unk578440(*(char **)(rec + 0x70));
    if (*(int *)(rec + 0x58)) {
        mem_Free(*(int *)(rec + 0x58));
        *(int *)(rec + 0x58) = 0;
    }
    if (*(int *)(rec + 0x64)) {
        mem_Free(*(int *)(rec + 0x64));
        *(int *)(rec + 0x64) = 0;
    }
    mem_Free(rec);
}
