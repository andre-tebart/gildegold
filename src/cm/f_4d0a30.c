/* 0x4d0a30, fills ai_Unk7603a0[] (32 pointers) with the records of ai_Unk930600 whose object belongs to the scene ai_Unk5fb5b0 */

void ai_Unk4d0a30()
{
    char **p;
    char *r;

    memset(ai_Unk7603a0V, 0, 0x80);
    p = ai_Unk7603a0V;
    for (r = ai_Unk930600; (int)p < ((unsigned)&ai_Unk7603a0 + 0x7c) && (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (*(int *)(r + 0x184) != 0 && *(char **)(*(char **)(r + 0x184) + 0x88) == ai_Unk5fb5b0)
            *p++ = r;
    }
}
