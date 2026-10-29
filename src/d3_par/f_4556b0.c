/* 0x4556b0, deletes every particle system of the default scene (from the head until the end marker) */

void par_DeleteAll(void)
{
    while (d3_ParHead != &d3_ParEnd)
        par_Delete(d3_ParHead);
}
