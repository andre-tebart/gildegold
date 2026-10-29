/* 0x525dd0, in scene 0 sets the camera height factors 0x6f22d4 / 0x6f22d8 to f and moves the camera (0x43b790: the x / z of its position and the height 0x623800 .. 0x623808 blended by f over the terrain height
   0x412550) and the camera angle (0x43b860: +0x88 / +0x8c and 0x623804 .. 0x62380c blended); stores the camera position and direction (0x75fb80 / 0x75fb90) */

char * ai_Unkfc2dec = {0}; /* 0xfc2dec, the camera object */
extern float sv_CameraHeightFactor, sv_CameraAngleFactor, ai_Unk6f2374, ai_Unk6f2388, ai_Unk6f2390, ai_Unk6f238c, ai_Unk6f1c44;  /* 0x6f22d4, 0x6f22d8, 0x6f2374, 0x6f2388, 0x6f2390, 0x6f238c, 0x6f1c44 */
extern float sv_CameraHeightNear, sv_CameraAngleNear, sv_CameraHeightFar, sv_CameraAngleFar, ai_Unk623818;  /* 0x623800, 0x623804, 0x623808, 0x62380c, 0x623818 */
extern int ai_Unk6f2378, ai_Unk6f22cc, ai_Unk6f22d0, ai_Unk6310e4, d3t_CamMove, ai_Unk6f2394;  /* 0x6f2378, 0x6f22cc, 0x6f22d0, 0x6310e4, 0x6310e8, 0x6f2394 */
extern int ai_Unk75fb80, ai_Unk75fb84, ai_Unk75fb88, ai_Unk75fb90, ai_Unk75fb94, ai_Unk75fb98;  /* 0x75fb80, 0x75fb84, 0x75fb88, 0x75fb90, 0x75fb94, 0x75fb98 */
float ai_Unk6f1c74[3] = {0};  /* 0x6f1c74 */
float d3t_GetMeanHeight();                   /* 0x412550 */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */
int d3t_PlaceObjectV();                     /* 0x411860 */  /* @original: sv.c's call at 2780 passes 4 arguments to ecmd.h's 5-parameter prototype */
void d3_MatTranspose();                     /* 0x454040 */
float d3_VecAngle2();                   /* 0x453b40 */

void sv_PlaceCamera(float f)
{
    float pos[3], ang[3];
    double h;

    if (d3_CurrentScene != 0)
        return;
    sv_CameraHeightFactor = f;
    sv_CameraAngleFactor = f;
    h = d3t_GetMeanHeight(ai_Unkfc2dec + 0x4c);
    pos[0] = *(float *)(ai_Unkfc2dec + 0x4c);
    pos[2] = *(float *)(ai_Unkfc2dec + 0x54);
    pos[1] = (float)((sv_CameraHeightFar - sv_CameraHeightNear) * sv_CameraHeightFactor + sv_CameraHeightNear + h);
    d3_SetObjectPos(ai_Unkfc2dec, pos);
    ang[1] = *(float *)(ai_Unkfc2dec + 0x88);
    ang[2] = *(float *)(ai_Unkfc2dec + 0x8c);
    ang[0] = (sv_CameraAngleFar - sv_CameraAngleNear) * sv_CameraAngleFactor + sv_CameraAngleNear;
    d3_SetObjectRot(ai_Unkfc2dec, ang);
    ai_Unk75fb90 = *(int *)(ai_Unkfc2dec + 0x84);
    ai_Unk75fb94 = *(int *)(ai_Unkfc2dec + 0x88);
    ai_Unk75fb98 = *(int *)(ai_Unkfc2dec + 0x8c);
    ai_Unk75fb80 = *(int *)(ai_Unkfc2dec + 0x4c);
    ai_Unk75fb84 = *(int *)(ai_Unkfc2dec + 0x50);
    ai_Unk75fb88 = *(int *)(ai_Unkfc2dec + 0x54);
}
