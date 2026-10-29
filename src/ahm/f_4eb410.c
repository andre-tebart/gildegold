/* 0x4eb410, restarts the research events (kinds 0x95 .. 0x97) of the object o: the due clock (+0x52) is set to the game clock ai_Time and the event is told
   (0x4c4c60, -2) */

void ahm_RescheduleResearch(char *o)
{
    AHMRESEARCH *ev;

    ev = (AHMRESEARCH *)he_FindEvent(1, 3, ((AHMOBJ *)o)->id);
    while (ev != 0) {
        if (0x95 <= ev->kind && ev->kind <= 0x97) {
            ev->due = ai_Time;
            cm_SendExSetHE(-2, (char *)ev);
        }
        ev = (AHMRESEARCH *)he_NextEvent();
    }
}
