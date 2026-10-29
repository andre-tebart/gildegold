/* 0x520fe0, checks the character that works for the master id: when its flag +0x1b4 & 1 is set (gm_FindCharacterByMaster,
   0x50a810) a message box with the text 0x78 (the short id) in the pergament style; 1 when shown */

int sv_CheckCharacterFlag(int id)
{
    unsigned short *p;
    char buf[0x100];

    p = (unsigned short *)gm_FindCharacterByMaster(id);
    if (p != 0 && (*((unsigned char *)p + 0x1b4) & 1) != 0) {
        txt_Format(buf, 0x78, *p);
        sv_MessageBox(buf, 0x100);
        return 1;
    }
    return 0;
}
