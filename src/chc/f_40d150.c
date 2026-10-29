/* 0x40d150, SetCharacterTransperancy(ch, alpha): sets the alpha of the body and of its transport; 1 after an error, else 0 */

int chc_SetCharacterTransperancy(CHARACTER **ch, int *alpha)
{
    CHCALPHA f;

    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAni(): invalid character or dummy");
        return 1;
    }
    memset(&f, 0, sizeof(f));
    f.f0 = 0;
    f.alpha = *alpha;
    d3_SetObjectAlphaC((*ch)->obj, (*ch)->obj->info, f);
    if ((*ch)->transport)
        d3_SetObjectAlphaC(*(*ch)->transport, (*(*ch)->transport)->info, f);
    return 0;
}
