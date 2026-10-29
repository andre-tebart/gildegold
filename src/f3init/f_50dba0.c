/* 0x50dba0, the name of the building kind k into out ("FRIEDHOF" for the type 0x1d, "ZIGEUNERLAGER" for 0x1f, else the prototype's name) @deep close (47/47 instructions, 12 differing, 1 code hunk at 50dc18): the only difference is the inline strcpy form - the original copies with inc eax / sub edx,eax / mov cl,[eax] / mov [edx+eax],cl / inc eax / test cl,cl / jne (one running index, the dest base folded into edx), ours with lea ecx,[eax+1] / mov al,[ecx] / inc ecx / mov [edx],al / inc edx / test al,al (two increments). The meaning was read: out gets the record name at ai_BTypes + k*0x24d + 1, the same string, same result 1; the local src was removed and the expression written directly in the call (no change). spell.py: no hunk matched the menu; diffclass: code 12, meaning checked. */


int init_BuildingKindName(int k, char *out)
{
    char t;

    t = ai_BTypes[k + k * 0x24c];
    if (t == 0x1d) {
        strcpy(out, "FRIEDHOF");
        return 1;
    }
    if (t == 0x1f) {
        strcpy(out, "ZIGEUNERLAGER");
        return 1;
    }
    strcpy(out, ai_BTypes + k + k * 0x24c + 1);
    return 1;
}
