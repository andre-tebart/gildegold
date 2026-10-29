/* 0x4c3ba0, starts a field block about object o (id) */

void cm_BeginFieldBlock(char *o, int id)
{
    int a, b;

    ai_Unka68c20.id = id;
    ai_Unka68c20.n = 0;
    memset(ai_Unka68c20.buf, 0, sizeof(ai_Unka68c20.buf));
    ai_Unka68c08 = 0;
    if (gm_FindById(&a, &b, &id, id) != 0)
        cm_FieldObject = o;
}
