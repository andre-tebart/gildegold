/* 0x40b860, script command CallUserFunctionExtended: calls the given user function with two arguments; a
   null function is a script error ("CallUserFunctionExtended(): Invalid userfunction") and returns 0, else 1 */

int evtc_CallUserFunctionExt(EVTUSERFUNC2 *f, int *a, int *b)
{
    if (!*f) {
        evt_Error(evt_Cur, evt_Cur->pc, "CallUserFunctionExtended(): Invalid userfunction");
        return 0;
    }
    (*f)(*a, *b);
    return 1;
}
