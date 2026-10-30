/* 0x4e32f0, sets up the scene's light / camera object 0x6f0db8 (0x45da30 -2.25, -0.2, 600.0) with the attachment 0x6f0db4 (0x45d880, 3.0, 0.8, -0.25;
   the colour 0 and a the tint) and fades it in (0x45d960, 0x45d9a0 255, 10.0) */

int ai_Unk4e32f0(int a)
{
    int v, id;

    if (ai_Unk6f0d8c == 0) {
        id = d3sky_CreateK(0xc0100000, 0xbe4ccccd, 0x44160000);
        v = 0;
        ai_Unk6f0db8 = ai_Unk633dd4 = id;
        ai_Unk6f0db4 = d3sky_CreateLayer(id, 0, a, (((char *)&v)[2] = 0, v), 0x40400000, 0, 0x3f4ccccd, 0xbe800000);
        d3sky_SetLayerSpeed(ai_Unk6f0db8, ai_Unk6f0db4, 0);
        d3sky_FadeLayer(ai_Unk6f0db8, ai_Unk6f0db4, 0xff, 0x41200000);
    }
}
