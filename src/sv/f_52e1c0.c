/* 0x52e1c0, registers (or finds) the plate for the object called name (entry: +0 the object, +4 its name, +0x44 b, +0x48 the text, 200 bytes each); returns the object or 0 */

int sv_RegisterNamePlate(char *name, char *text, int b)
{
    int i, o;
    char *e;
    char msg[0x100];

    i = 0;
    e = (char *)((unsigned)&sv_NamePlates + 0x4);
    do {
        if (*(int *)(e - 4) != 0 && strcmp(e, name) == 0) {
            *(int *)(SV_I((unsigned)&sv_NamePlates + i * 200) + 0x224) = 1;
            return SV_I((unsigned)&sv_NamePlates + i * 200);
        }
        e += 200;
        i++;
    } while ((int)e < ((unsigned)((unsigned)sv_NamePlates + 0x1900) + 0x4));
    i = 0;
    e = (char *)(unsigned)&sv_NamePlates;
    do {
        if (*(int *)e == 0) {
            if (i < 0x20) {
                o = d3t_FindObject(name, 2);
                SV_I((unsigned)&sv_NamePlates + i * 200) = o;
                if (o != 0) {
                    SV_I(((unsigned)&sv_NamePlates + 0x44) + i * 200) = b;
                    strncpy((char *)(((unsigned)&sv_NamePlates + 0x48) + i * 200), text, 0x80);
                    strncpy((char *)(((unsigned)&sv_NamePlates + 0x4) + i * 200), name, 0x40);
                    *(char *)(((unsigned)&sv_NamePlates + 0xc7) + i * 200) = 0;
                    *(char *)(((unsigned)&sv_NamePlates + 0x43) + i * 200) = 0;
                    *(int *)(SV_I((unsigned)&sv_NamePlates + i * 200) + 0x224) = 1;
                    return SV_I((unsigned)&sv_NamePlates + i * 200);
                }
                sprintf(msg, "RegisterStatusText(): Object not found: %s", name);
                return 0;
            }
            break;
        }
        e += 200;
        i++;
    } while ((int)e < (unsigned)((unsigned)sv_NamePlates + 0x1900));
    sprintf(msg, "RegisterStatusText(): Too many Objects: %s", name);
    return 0;
}
