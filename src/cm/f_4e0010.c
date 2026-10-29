/* 0x4e0010, plays the scene "cutscene_toby.ed3" and closes it */

int ai_Unk4e0010(int scene)
{
    ai_Unk4e2e60("cutscene_toby.ed3");
    ai_Unk4e3830(scene, 0);
    ai_Unk4e3830(scene, 0);
    ai_Unk4e3420(5);
    return ai_Unk4e30c0();
}
