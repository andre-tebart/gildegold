/* 0x4c2510, the voice sample text of speaker id (a character: "_M%i" / "_W%i" by sex, -1.. -6: "_W2", "_W1", "_M1",
   "_M2", "_M3", "_HS"; -7: text itself, -8: none) in bank, variation var if >= 0 ("%02i" appended)
   @deep B draft, 268 code hunks / 2 slots. Read the whole original 0x4c2510..0x4c275d against the source: no missing statement, constant, callee or condition - the id < 0x300 table sex/voice read, the six strcpy suffixes, id -7 sprintf(buf, text), -8 the empty first byte, both %s%s + sb_IsVariation (0x426f60), the inlined strcat, the second %s%s, sb_GetVariation (0x426190) and the two not-found messages, sb_GetSample (0x426030) all correspond; our call multiset is the same. Left is codegen: our frame is 0x12c vs the original's 0x128 (a 4-byte unused local slot shifts every local), the character table is read through the G16CHAR view at 0x770b84 (sex +5, voice +0) as lea chain + shl eax,2 + [eax+disp] where the original reads the record base 0x770b80 (sex +9, voice +4) with the folded [eax*4+disp], and the rest is register allocation / basic-block layout (the early return inlined and moved after the body). Lead: the real AICHAR view at 0x770b80 (ai.h) for the table access, and the extra 4-byte slot. Readability: Q1 done (snd_FindVoiceSample); Q2 ch_EmptyName, ai_CharInts, unk00. */

int snd_FindVoiceSample(int id, int bank, char *text, int var)
{
    char sfx[0x14];
    char num[0x10];
    char buf[0x104];
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    if (id == -7)
        sprintf(buf, text);
    else {
        if (id < 0x300 && id >= 0) {
            if (((G16CHAR *)ai_CharInts)[id].sex == 1)
                sprintf(sfx, "_W%i", (((G16CHAR *)ai_CharInts)[id].unk00 & 1) + 1);
            else if (((G16CHAR *)ai_CharInts)[id].sex == 0)
                sprintf(sfx, "_M%i", ((G16CHAR *)ai_CharInts)[id].unk00 % 3 + 1);
        } else if (id == -2)
            strcpy(sfx, "_W1");
        else if (id == -1)
            strcpy(sfx, "_W2");
        else if (id == -5)
            strcpy(sfx, "_M1");
        else if (id == -4)
            strcpy(sfx, "_M2");
        else if (id == -3)
            strcpy(sfx, "_M3");
        else if (id == -6)
            strcpy(sfx, "_HS");
        else if (id == -8)
            sfx[0] = ch_EmptyName[0];
        sprintf(buf, "%s%s", text, sfx);
        s = sb_IsVariationI(bank, buf);
        if (var >= 0 && s == 0) {
            sprintf(num, "%02i", var);
            strcat(sfx, num);
        }
        sprintf(buf, "%s%s", text, sfx);
        if (s != 0 && var >= 0) {
            s = sb_GetVariationI(bank, buf, var);
            if (s != 0)
                return s;
            sprintf(buf, "Voicesample not found in variation: %s %i", text, var);
            return 0;
        }
    }
    s = sb_GetSample(bank, buf);
    if (s == 0)
        sprintf(buf, "Voicesample not found: %s", text);
    return s;
}
