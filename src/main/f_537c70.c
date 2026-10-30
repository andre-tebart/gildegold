/* 0x537c70, "main_PreloadSound": reads the sound definition file f (lines "#include "<bank>"", comments "//") and loads each named sound bank (0x426950, or the language bank 0x4c2830 when the path starts with
   "sprache\") into the table 0x75df20 (0x80 slots); returns 1 when done, 0 on errors (a message box "main_PreloadSound(): Could not open sound-definition!") */

void sv_UpdateClock(void);

int main_PreloadSound(char *f)
{
    int *slot;
    int *line;
    char *fp, *q, *inc;
    char buf[0x100];
    int i;

    memset((char *)(unsigned)&full_data_75df20, 0, 0x200);
    fp = (char *)fopen(f, "rt");
    if (fp == 0) {
        MessageBoxA(0, "main_PreloadSound(): Could not open sound-definition!", "Error", 0x30);
        return 0;
    }
    slot = (int *)(unsigned)&full_data_75df20;
    do {
        memset(buf, 0, 0x100);
        fgets(buf, 0x100, fp);
        if (strstr(buf, "//") == 0 && (inc = strstr(buf, "#include")) != 0) {
            q = strchr(inc, 0x22);
            if (q == 0)
                return 0;
            q++;
            inc = strchr(q, 0x22);
            if (inc == 0)
                return 0;
            *inc = 0;
            if (_strnicmp(q, "sprache\\", 8) == 0) {
                q = strchr(q, 0x5c);
                *slot = snd_LoadVoiceBank(q + 1, 0);
            } else
                *slot = sb_LoadSampleBank(q, 0);
            slot++;
            if ((int)slot > (int)((unsigned)&ai_MissionSeed))
                return 0;
        }
        if ((*(unsigned char *)(fp + 0xc) & 0x10) != 0) {
            fclose(fp);
            return 1;
        }
    } while (1);
}
