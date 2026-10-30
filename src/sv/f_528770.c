/* 0x528770, in scene 0 goes over the objects of kind 6 (0x501190 (1, 6)): for those whose owner (+0x27) is another character of type 5 / 6 / 7 and which are not flagged (+0x5a & 1) the banner is tinted (0x5286a0) and coloured (0x5282f0) */

void sv_Unk528770(void)
{
    int scene;
    char *o;
    char *c;
    char t;

    scene = d3_CurrentScene;
    c = 0;
    d3if_SelectScene(0, 1);
    o = (char *)gm_BuildQuery(1, 6);
    while (o != 0) {
        if (*(unsigned short *)(o + 0x27) != 0xffff)
            c = ai_Chars[*(unsigned short *)(o + 0x27)];
        if (c != 0 && c != ai_PlayerCharC && ((t = c[2]) == 6 || t == 7 || t == 5) && (o[0x5a] & 1) == 0) {
            sv_Unk5286a0(o);
            sv_Unk5282f0(o);
        }
        o = (char *)gm_BuildNext();
    }
    d3if_SelectScene(scene, 1);
}
