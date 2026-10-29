/* 0x528390, colours the banners of all buildings of the character c (index): in scene 0 every object (0x501190 (1, 4, c) / 0x501030) with a model is walked (0x438d40, 0x340, 0x528290), and the buildings of the
   list 0x4ffe90 (0, 3, 7, 3, c, 4, 0x1d) get the model scene (0x411be0) and the banner number (the byte +0x54 of the character's record - 0x599) through 0x446f30; the scene is restored */

void sv_Unk528390(int c)
{
    int scene, b, n;
    char *o;
    char *ch;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    o = (char *)gm_BuildQuery(1, 4, c);
    while (o != 0) {
        if (*(int *)(o + 0x61) != 0)
            d3_ForEachObject(ai_Unk5fb5b0, *(int *)(o + 0x61), 0x340, sv_Unk528290, o);
        o = (char *)gm_BuildNext();
    }
    o = gm_Query(0, 3, 7, 3, c, 4, 0x1d);
    while (o != 0) {
        b = *(int *)(o + 0x3b);
        if (b != 0) {
            d3if_SelectScene(d3t_SceneIndex(*(int *)(b + 0x88), 1), 1);
            ch = ai_Chars[c];
            if (c * 0x21c != -(unsigned)&ai_Chars && *(int *)(ch + 0x54) != 0 && (ch[2] == 5 || ch[2] == 6 || ch[2] == 7)) {
                n = *(int *)(ch + 0x54) - 0x599;
                if (n != 0)
                    d3if_SelectTextureSet(**(int **)(b + 0x124), *(int *)(**(int **)(b + 0x124) + 0x1cc), n, 1);
            }
        }
        o = gm_QueryNext();
    }
    d3if_SelectScene(scene, 1);
}
