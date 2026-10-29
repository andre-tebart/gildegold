/* 0x413b00, inflates obj's geometry (d3_InflateObject) and then every object of its child chain
   (obj->child and the child's siblings), recursively; used after a model is loaded. */

typedef struct D3TRAINDROP {         /* 0x28 bytes */
    D3VECTOR4 pos;                   /* w = tail length */
    float windScale;                 /* 0x10, weight of the wind velocity */
    float fallScale;                 /* 0x14, weight of the fall velocity */
    float sx1, sy1;                  /* 0x18, projected head */
    float sx2, sy2;                  /* 0x20, projected tail */
} D3TRAINDROP;

typedef struct D3TRAIN {             /* 0x60 bytes */
    int count;
    int capacity;                    /* allocated drops */
    int oldcount;                    /* count fade: oldcount -> newcount over time30..time34 */
    int newcount;
    D3TRAINDROP *drops;              /* 0x10 */
    float oldwindx, oldwindz;        /* wind fade: old -> new over time38..time3c */
    float windx, windz;              /* 0x1c */
    float newwindx, newwindz;
    int time2c;                      /* last update */
    int time30, time34, time38, time3c;
    D3VECTOR4 campos;                /* 0x40 */
    D3VECTOR4 cam04c;                /* 0x50 */
} D3TRAIN;

void d3t_InflateObjectTree(D3OBJECT *obj)
{
    D3OBJECT *c;
    d3_InflateObject(obj);
    for (c = obj->child; c; c = c->next)
        d3t_InflateObjectTree(c);
}
