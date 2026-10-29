/* 0x40b820, script command CallUserFunction: calls the given user function with one argument; a null
   function is a script error ("CallUserFunction(): Invalid userfunction") and returns 0, else 1 */

int evtc_CallUserFunction(EVTUSERFUNC1 *f, int *a)
{
    if (!*f) {
        evt_Error(evt_Cur, evt_Cur->pc, "CallUserFunction(): Invalid userfunction");
        return 0;
    }
    (*f)(*a);
    return 1;
}
