/* 0x418030, frees the water-ambience ray arrays allocated by d3sndw_Init */

void d3sndw_Exit(void)
{
    if (d3sndw_Sin) {
        mem_Free(d3sndw_Sin);
        d3sndw_Sin = NULL;
        mem_Free(d3sndw_Type);
        d3sndw_Type = NULL;
        mem_Free(d3sndw_State);
        d3sndw_State = NULL;
        d3sndw_Type = NULL;
        d3sndw_Sin = NULL;
    }
}
