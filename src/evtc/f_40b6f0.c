/* 0x40b6f0, script command StopScript: stops the script whose handle (evt_Load result) was passed; always
   returns 0 */

int evtc_StopScript(int *sc)
{
    evt_Stop((EVTSCRIPT *)*sc);
    return 0;
}
