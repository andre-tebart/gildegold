/* 0x58fd40, the world position of a plot record's origin, written to out (79 B): d3if_SelectScene (0, 1) brackets
   the work; the zeroed 0x10-byte vector goes through d3_LocalToWorld (the plot, the vector, out), which applies
   the record's matrix and its parent chain; d3_CurrentScene is selected again afterwards */

int d3t_DropObject(); 

struct XOBJ;
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);  /* 0x458c10 (unk_458810.c) */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);    /* 0x458810 (unk_458810.c) */
float d3_VecAngle(struct D3VECTOR *a, struct D3VECTOR *b);      /* 0x4539f0 (d3_vec.c) */
extern struct D3VECTOR d3_ZAxis;                                /* 0x5ebb74, (0, 0, 1) */

void ai_ObjectPosition(void *out, int plot)
{
    float v[4];
    int scene;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    d3_LocalToWorld((struct XOBJ *)plot, v, (float *)out);
    d3if_SelectScene(scene, 1);
}
