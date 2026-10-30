/* 0x514e90 @deep behaviour: no meaning error found, the same compiler shape as io_CharacterName (the original 2752 B against our 2336: G30_ENDS_S expands 29 inline strlens against our 23, buffers initialised in the order A, C, B, N/T recomputed per case, ebx/ebp pushed after the early return while ours pushes ebp at entry and shifts every stack slot by 4); the id range 0 .. 0x2ff, the player path with "s" and the double genitive of a kind 4 name without words are reproduced; readability Q4/Q5 and Q2: rec is read through the raw offsets 0x9/0xd/0x164..0x166/0x1cc of the 0x21c character record and the globals are the shared tables declared under placeholder names in ai.h/ahm.h/am.h */


/* 0x514e90, the same text builder as 0x5143d0 for the ids 0 .. 0x2ff (anything else: the empty text 0x143b3f4, returns 0), but with the markup "$A" between rank word and name ("%s$A%s", "%s$A%ss", "%s$A%s'") in the kinds 2, 4, 7 and 8; kinds 3 and 9 keep the plain space. ROUGH like 0x5143d0 (the original appends the genitive twice for a kind 4 name without words: sprintf with "%s'" / "%ss", then the append again). */
/* ai_Unk143b3f4: view of txt_Ptrs (build/mk_txt_views.py) */
int io_CharacterNameMarkup(int id, unsigned char c, unsigned char kind, char *out)
{
    char A[0x100], B[0x100], C[0x100];
    char tmp[0x21c];
    char *rec, *N, *T;
    char *w;
    int v;

    A[0] = ch_EmptyName[0];
    memset(A + 1, 0, 0xff);
    C[0] = ch_EmptyName[0];
    memset(C + 1, 0, 0xff);
    B[0] = ch_EmptyName[0];
    memset(B + 1, 0, 0xff);
    if (id < 0 || id >= 0x300) {
        strcpy(out, ai_Unk143b3f4);
        return 0;
    }
    if (id != 0xfffe) {
        rec = ai_Chars[id];
    } else {
        memset(tmp, 0, 0x21c);
        strcpy(tmp + 0x30, ai_Unk143a618);
        rec = tmp;
    }
    N = rec + 0x30;
    T = rec + 0x40;
    if (rec == ai_PlayerCharC) {
        strcpy(out, ai_PlayerSlotsV[ai_OwnPlayer]);
        if (c == 2)
            strcat(out, "s");
        return 1;
    }
    switch (kind) {
    case 1:
        strcpy(out, N);
        if (c == 2) {
            if (G30_ENDS_S(N))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 2:
        if (rec[0xd]) {
            w = txt_Field(txt_Ptrs[0x114 + (rec[9] ? 0xa : 0) + (unsigned char)rec[0xd]], 1, 4);
            sprintf(out, "%s$A%s", w, N);
        } else {
            sprintf(out, "%s", N);
        }
        break;
    case 3:
        if (rec[0x166]) {
            w = txt_Field(txt_Ptrs[0x253 + (rec[9] ? 0x28 : 0) + (unsigned char)rec[0x166]], 1, 4);
            if (*T)
                sprintf(out, "%s %s %s", w, N, T);
            else
                sprintf(out, "%s %s", w, N);
        } else if (*T) {
            sprintf(out, "%s %s", N, T);
        } else {
            sprintf(out, "%s", N);
        }
        if (c == 2) {
            if (G30_ENDS_S(T))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 4:
        if (rec[0x164]) {
            w = txt_Field(txt_Ptrs[0x130 + (rec[9] ? 0x5f : 0) + (unsigned char)rec[0x164]], 1, 4);
            sprintf(out, "%s$A%s", w, N);
        } else if (rec[0x165]) {
            w = txt_Field(txt_Ptrs[0x20d + (rec[9] ? 0x23 : 0) + (unsigned char)rec[0x165]], 1, 4);
            sprintf(out, "%s$A%s", w, N);
        } else if (c == 2) {
            if (G30_ENDS_S(N))
                sprintf(out, "%s'", N);
            else
                sprintf(out, "%ss", N);
        } else {
            sprintf(out, "%s", N);
        }
        if (c == 2) {
            if (G30_ENDS_S(N))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 5:
        w = txt_Field(txt_Ptrs[0x253 + (rec[9] ? 0x28 : 0) + (unsigned char)rec[0x166]], 1, 4);
        sprintf(out, "%s", w);
        break;
    case 6:
        if (*T)
            strcpy(out, T);
        else
            strcpy(out, N);
        break;
    case 7:
        if (c == 2) {
            if (G30_ENDS_S(T)) {
                if (*T)
                    sprintf(out, "%s$A%s'", N, T);
                else
                    sprintf(out, "%s'", N);
            } else {
                if (*T)
                    sprintf(out, "%s$A%ss", N, T);
                else
                    sprintf(out, "%ss", N);
            }
        } else if (*T) {
            sprintf(out, "%s$A%s", N, T);
        } else {
            sprintf(out, "%s", N);
        }
        break;
    case 8:
    case 9:
        if (kind == 8) {
            v = (unsigned char)rec[0x166];
        } else {
            unsigned f = *(unsigned *)(rec + 0x1cc);
            if (f & 0x3e00) {
                if (f & 0x1000)
                    v = 0x26;
                else if (f & 0x2000)
                    v = 0x27;
                else if (f & 0x800)
                    v = 0x25;
                else if (f & 0x400)
                    v = 0x24;
                else
                    v = (f & 0x200) ? 0x23 : 0;
            } else {
                v = (unsigned char)rec[0x166];
            }
        }
        if (rec[0xd])
            strcpy(C, txt_Field(txt_Ptrs[0x114 + (rec[9] ? 0xa : 0) + (unsigned char)rec[0xd]], c, 4));
        if (v)
            strcpy(B, txt_Field(txt_Ptrs[0x253 + (rec[9] ? 0x28 : 0) + v], c, 4));
        if (c == 2) {
            if (G30_ENDS_S(N))
                sprintf(A, "%s'", N);
            else
                sprintf(A, "%ss", N);
        } else {
            strcpy(A, N);
        }
        if (kind == 8)
            sprintf(out, "%s$A%s", v ? B : C, A);
        else
            sprintf(out, "%s %s", v ? B : C, A);
        break;
    }
    return 1;
}
