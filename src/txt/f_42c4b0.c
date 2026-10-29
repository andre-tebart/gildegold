/* 0x42c4b0, one line of a text file: "(m|w|s)", then up to four texts of the form
   prefix[alt1/alt2]suffix (or prefix[alt1]alt2]suffix); appends both variants of each to the pool. */

int txt_ParseLine(char *line, int group)
{
    unsigned char i;
    unsigned char mode;
    char suffix[128];
    char alt2[128];
    char alt1[128];
    char prefix[128];
    char tmp[256];
    char out1[4][160];
    char out2[4][160];
    char *p;
    char *q;
    char *r;
    int k;

    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    memset(alt1, 0, sizeof(alt1));
    memset(alt2, 0, sizeof(alt2));
    mode = 0;
    i = 0;
    suffix[0] = 0;
    prefix[0] = 0;
    p = strtok(line, ",\";\n");
    while (p) {
        if (i == 0) {
            strcpy(tmp, p);
            p = txt_SkipSpaces(tmp);
            txt_TrimRight(p);
            if (!_strcmpi(p, "(m)"))
                mode = 0;
            else if (!_strcmpi(p, "(w)"))
                mode = 1;
            else if (!_strcmpi(p, "(s)"))
                mode = 2;
        } else {
            p = txt_SkipSpaces(p);
            if (p) {
                txt_TrimRight(p);
                q = strchr(p, '[');
                if (!q)
                    strcpy(prefix, p);
                else {
                    if (q != p) {
                        memset(prefix, 0, sizeof(prefix));
                        strncpy(prefix, p, q - p);
                    }
                    r = strchr(q, '/');
                    if (r) {
                        memset(alt1, 0, sizeof(alt1));
                        q++;
                        strncpy(alt1, q, r - q - 1);
                        q = strchr(r, ']');
                        if (!q)
                            return 0;
                        memset(alt2, 0, sizeof(alt2));
                        r++;
                        strncpy(alt2, r, q - r - 1);
                        if (strlen(q + 1)) {
                            memset(suffix, 0, sizeof(suffix));
                            strcpy(suffix, q + 1);
                        }
                    } else {
                        r = strchr(q, ']');
                        if (!r)
                            return 0;
                        q++;
                        memset(alt1, 0, sizeof(alt1));
                        strncpy(alt1, q, r - q - 1);
                        memset(alt2, 0, sizeof(alt2));
                        q++;
                        strncpy(alt2, q, r - q - 1);
                        if (strlen(r + 1)) {
                            memset(suffix, 0, sizeof(suffix));
                            strcpy(suffix, r + 1);
                        }
                    }
                }
                sprintf(out1[i - 1], "%s%s%s", prefix, alt1, suffix);
                sprintf(out2[i - 1], "%s%s%s", prefix, alt2, suffix);
            }
        }
        p = strtok(NULL, ",\";\n");
        i++;
    }
    for (k = 0; k < 4; k++) {
        strcat(out1[k], "|");
        strcat(txt_Buffer + txt_Count * 0x1000, out1[k]);
        txt_Size += strlen(out1[k]);
    }
    for (k = 0; k < 4; k++) {
        strcat(out2[k], "|");
        strcat(txt_Buffer + txt_Count * 0x1000, out2[k]);
        txt_Size += strlen(out2[k]);
    }
    txt_Random[txt_Count] = mode;
    sprintf(txt_Keys[txt_Count], "%s+%i", txt_Names[group - 1], txt_Count - txt_GroupFirst[group - 1]);
    txt_Size++;
    txt_Count++;
    return 1;
}
