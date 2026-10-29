/* 0x428ca0 */

void evt_Return(void)
{
    int r = evt_Eval(0);

    if (!evt_LeaveFunction(evt_Cur))
        evt_Stop(evt_Cur);
    evt_ReturnValue = r;
}
