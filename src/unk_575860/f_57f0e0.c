/* 0x57f0e0, the cannon camera update (947 B, ROUGH: the placement calls and the aiming / recoil maths are
   written, the matrix details are comments): sc = the scene struct; the camera position sc + 0x90 / the object
   sc + 0x98 / the wimpel sc + 0x9c handles; the screen 0xfb9070 / 0xfb9072; the view 0xfc2dec. The three object
   handles sc + 0x8c / 0x90 / 0x94 get their positions through 0x43b860 (0x54560 the rotation); the mouse
   0x475980 (sc + 0xb4) * 0.01 as the speed; the target position from the object matrix (the 0x18c / 0x194 /
   entries * 30), 0x54560; the camera placement 0x43b790 (0xfc2dec, the position) / 0x43b860 (0xfc2dec, the
   angle); the recoil sc + 0x78 / sc + 0x7c timer with the fade 0x54560 and the wimpel at -70 / -50 / 120;
   0x43b790 (sc + 0x98, sc) as the default */

void d3_MatTranspose(); float d3_VecAngle2(); int ai_GetDefaultEffect();

void ai_Unk57f0e0(char *sc)
{
    float pos[3];
    float ang[3];
    float t[16];   /* t is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    float f1;
    float f2;
    int m;
    int i;
    int h;

    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    d3_SetObjectRot(*(int *)(sc + 0x98), (char *)pos);
    pos[0] = *(float *)(sc + 0x10);
    pos[1] = 0;
    pos[2] = 0;
    for (i = 0; i < 3; i++)
        d3_SetObjectRot(*(int *)(sc + 0x8c + i * 4), (char *)pos);
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    t[3] = 0;
    h = ai_GetDefaultEffect();
    d3_MatTranspose(*(int *)(sc + 0x98) + 0x18c, (char *)t);
    ang[0] = 0;
    ang[1] = 0;
    ang[2] = (float)(d3_VecAngle2((char *)(unsigned)&ai_AxisPair, h) - 1.5707964f) - *(float *)(sc + 0x14);
    d3_SetObjectRot(*(int *)(sc + 0x9c), (char *)ang);
    if (*(float *)(sc + 0x7c) == 0.0f)
        ai_Unk57f5d0(sc);
    /* the object's forward vector (the matrix +0x18c / +0x19c / +0x1ac, +0x194 / +0x1a0 / +0x1b0, +0x194 /
       +0x1a4 / +0x1b4 at * 30) -> 0x54560 -> the camera offset from sc + 0x10 / 0x14 / 0x18, the mouse x *
       0.01 as the speed and the pitch sc + 0x10 scaled */
    f1 = *(float *)(sc + 0x10);
    f2 = (f1 - -0.2617994f) / 1.0471976f;
    m = d2_GetObjValue(*(int *)(sc + 0xb4));
    d3_SetObjectPos(U_I((unsigned)&ai_Unkfc2dec), (char *)pos);
    d3_SetObjectRot(U_I((unsigned)&ai_Unkfc2dec), (char *)ang);
    /* the recoil: sc + 0x78 (timer) / sc + 0x7c (active): the fade 0x54560, the camera pushed back by the
       object direction * 50 * the fade, the wimpel to -70 / -50 / 120, else reset and 0x43b790 (sc + 0x9c,
       0x5eec80) / 0x43b790 (sc + 0x98, sc) */
    if (*(float *)(sc + 0x7c) != 0.0f) {
        if (*(float *)(sc + 0x78) < *(float *)(sc + 0x6c) + 0x54) {
            *(float *)(sc + 0x7c) = 0;
        } else {
            *(float *)(sc + 0x7c) = 0;
            *(float *)(sc + 0x78) = 0;
            d3_SetObjectPos(*(int *)(sc + 0x9c), (char *)((unsigned)&ai_Unk5eeab8 + 0x1c8));
        }
    }
    d3_SetObjectPos(*(int *)(sc + 0x98), sc);
}
