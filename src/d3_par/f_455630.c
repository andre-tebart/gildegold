/* 0x455630, empties the default scene's particle list (the head d3_ParHead becomes the end marker d3_ParEnd,
   the tail d3_ParTail the head node d3if_ParList) and sets the two quad UV tables */
void par_Init(void)
{
    d3_ParHead = &d3_ParEnd;
    d3_ParEnd.next = 0;
    D3PAR_HEADNODE->prev = 0;
    d3_ParTail = D3PAR_HEADNODE;
    d3_ParUV0[0] = 0.0f;
    d3_ParUV0[1] = 0.0f;
    d3_ParUV0[2] = 1.0f;
    d3_ParUV0[3] = 1.0f;
    d3_ParUV0[4] = 0.0f;
    d3_ParUV0[5] = 1.0f;
    d3_ParUV1[0] = 0.0f;
    d3_ParUV1[1] = 0.0f;
    d3_ParUV1[2] = 1.0f;
    d3_ParUV1[3] = 0.0f;
    d3_ParUV1[4] = 1.0f;
    d3_ParUV1[5] = 1.0f;
}
