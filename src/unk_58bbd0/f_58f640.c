/* 0x58f640, the building name of a kind id (83 B): ai_BuildNames holds 15 groups of 0x50 bytes (the 12 kind ids
   after the 0x20-byte name); the name of the matching id is copied to out (inlined strcpy); 1 when found, 0 else
   @deep B: one code hunk left, add eax,X (ours) vs lea eax,[eax+X] (original) at the group base 0x62aae8; the same address addition. Tried the struct field ai_BuildNames[g].name, pointer arithmetic ai_BuildNames + g, a 2D char-array view and char* cast + g * 0x50: all compile to add. VC6 address-form choice -> variant round. */

char *strstr(); unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void);

int ai_BuildingName(int id, char *out)
{
    int i;
    int g;
    char *s;
    int d;
    char c;

    for (g = 0; g < 0xf; g++) {
        for (i = 0; i < 0xc; i++) {
            if (ai_BuildNames[g].ids[i] == id) {
                s = ai_BuildNames[g].name;
                d = out - s;
                do {
                    c = *s;
                    s[d] = c;
                    s++;
                } while (c != '\0');
                return 1;
            }
        }
    }
    return 0;
}
