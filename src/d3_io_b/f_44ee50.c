/* 0x44ee50, "LOCAL" / "ANIM" keyword of the text format */
void agi_Kind(void *unused, char *out)
{
    char *tok = strtok(0, " ");

    *out = 0;
    if (!strcmp(tok, "LOCAL")) {
        *out = 1;
        return;
    }
    if (!strcmp(tok, "ANIM"))
        *out = 2;
}
