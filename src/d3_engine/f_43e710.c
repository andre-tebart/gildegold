/* 0x43e710, the name of an event id, "NONE" when the id is unknown */

char *d3e_EventName(int id)
{
    unsigned int i;

    for (i = 0; i < sizeof(d3e_Events) / sizeof(d3e_Events[0]); i++)
        if (id == d3e_Events[i].id)
            return d3e_Events[i].name;
    return "NONE";
}
