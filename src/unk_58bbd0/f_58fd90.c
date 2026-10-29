/* 0x58fd90, the rotation angle of a plot record (123 B): d3if_SelectScene (0, 1) brackets the work; out[0..2] is
   cleared, the zeroed vector takes the record's world direction (d3_DirToWorld on d3_ZAxis) and its angle to
   d3_ZAxis (d3_VecAngle) is stored to out[1]; d3_CurrentScene is selected again afterwards */

void ai_ObjectAngle(void *out_, int plot)
{
    float v[3];
    float *out = (float *)out_;
    int scene;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    d3_DirToWorld((struct XOBJ *)plot, (float *)&d3_ZAxis, v);
    out[1] = d3_VecAngle((struct D3VECTOR *)&d3_ZAxis, (struct D3VECTOR *)v);
    d3if_SelectScene(scene, 1);
}
