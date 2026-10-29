/* 0x426950, loads a sample bank file (a SBBANK header followed by its SBENTRY table) into the bank list,
   looking the file up in sb_Path and returning an already loaded bank with its reference count raised;
   preload loads the data of every entry. Returns the bank, &sb_DummyBank when the driver is closed and
   NULL when the file cannot be opened or read */

SBBANK *sb_LoadSampleBank(char *name, int preload)
{
    SBBANK hdr;
    char path[256];
    char msg[512];
    SBBANK *bank, *last;
    FILE *f;
    int i;

    if (!sb_Driver)
        return &sb_DummyBank;
    bank = sb_FindBankFile(name);
    if (!bank) {
        strcpy(path, sb_Path);
        strcat(path, name);
        f = fopen(path, "rb");
        if (f) {
        if (fread(&hdr, sizeof(SBBANK), 1, f) == 1
            && (bank = mem_Alloc(hdr.count * sizeof(SBENTRY) + sizeof(SBBANK), "sb_LoadSampleBank")) != NULL) {
            strncpy(bank->name, hdr.name, 50);
            strncpy(bank->path, path, 256);
            bank->count = hdr.count;
            bank->next = NULL;
            bank->entries = (SBENTRY *)(bank + 1);
            if (fread(bank->entries, sizeof(SBENTRY), bank->count, f) == bank->count) {
                last = sb_LastBank();
                if (last)
                    last->next = bank;
                else
                    sb_Banks = bank;
                sb_Used += hdr.count * sizeof(SBENTRY) + sizeof(SBBANK);
                sprintf(sb_Msg, "Samplebank %s loaded. MemUsed is now %d bytes.", name, sb_Used);
                bank->refs = 1;
                if (preload) {
                    for (i = 0; i < hdr.count; i++)
                        sb_LoadEntry(bank, &bank->entries[i]);
                }
            } else {
                mem_Free(bank);
                bank = NULL;
            }
        }
        fclose(f);
        } else
            sprintf(msg, "sb_LoadSampleBank: Samplebank %s konnte nicht geladen werden!", path);
    } else
        bank->refs++;
    return bank;
}
