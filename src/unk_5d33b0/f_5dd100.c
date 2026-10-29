/* 0x5dd100, the trajectory / angle record (890 B): the direction param_2 - param_1 normalized
   (d3_Normalize 0x454560), the angle d3_VecAngle2 (0x453b40) into param_5, the pitch ABS (dy) *
   0.0254, the sin / cos loop over the curve record param_3, the height 0x45f170 and the out records
   param_4 .. param_9.
   ROUGH SKELETON (layout 2026-10-24): the nine out pointers and the curve layout are not decoded. */

int ai_Unk5dd100(float *param_1, float *param_2, float *param_3, float *param_4, float *param_5,
                 float *param_6, float *param_7, float *param_8, float *param_9)
{
    return 0;
}
