/* 0x4e58e0, the debug dump of a person (log of the cutscenes): one line of tab separated fields (time, +4, id, the name of the kind (0x60eae0, 24 bytes each) ...) */

void ai_Unk4e58e0(char *c, char *buf)
{
    char *name;

    if ((unsigned char)c[2] < 10)
        name = ai_Unk60eae0[(unsigned char)c[2]];
    else
        name = "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t";
    sprintf(buf, "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t", *(int *)&ai_Time, *(int *)(c + 4), *(unsigned short *)c, name, c + 0x30, (unsigned char)c[8],
            ai_Unk143b3e4[(unsigned char)c[9]], *(unsigned short *)(c + 0xa), ai_Unk143b3d8[(unsigned char)c[0xc]], ai_Unk143a610[(unsigned char)c[0xd]],
            ai_Unk143a680[(unsigned char)c[0x164]], ai_Unk143a9f4[(unsigned char)c[0x165]]);
}
