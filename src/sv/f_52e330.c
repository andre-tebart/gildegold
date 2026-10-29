/* 0x52e330, sets the status line text sv_StatusText and its time stamp sv_StatusTime (d3_Time) */

void sv_SetStatusText(char *s)
{
    strcpy(sv_StatusText, s);
    sv_StatusTime = d3_Time;
}
