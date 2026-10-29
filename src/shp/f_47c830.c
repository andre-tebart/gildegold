/* 0x47c830, draws shape i of a bank scaled down (mode < 0) @deep B3, 150 code hunks: pure register allocation and scheduling - the original pushes edi at entry (keeps the surface pointer / bank in edi across the switch) and puts eax=1 early, ours uses esi/ ebp and shifts every stack slot by the missing push; the read hunks are the same operations with other registers (and eax,0xff vs and ecx,0xff, dec eax vs dec edx, mov [..],edx vs mov [..],edi) while the callees, constants, the switch on s->type and the mode / on conditions correspond; the ret=1 variable form gave the same bytes */

int d2_DrawShapeScaled(int x, int y, void *surf, void *pic, char i, char mode, int on)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    int n, old;
    char buf[128];

    if (bank == 0)
        return 0;
    if ((unsigned char)i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", (char *)bank + 10);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    if (d2_LightTableExternal == 0) {
        old = d2_SurfPitch;
        d2_SurfPitch = ((D2SURF *)surf)->pitch;
        switch (s->type) {
        case 0:
            return 0;
        case 1:
            if (mode >= 0)
                return 1;
            if (on != 0)
                return d2_ShpBlit16ScaledLit(x, y, (D2SURF *)surf, s, abs(mode));
            return d2_ShpBlit16Scaled(x, y, (D2SURF *)surf, s, abs(mode));
        case 2:
            return 1;
        }
        d2_SurfPitch = old;
    }
    n = abs(mode);
    d2_ShpW = s->w / n;
    d2_ShpH = s->h / n;
    return 1;
}
