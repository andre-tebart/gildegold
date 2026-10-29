/* 0x427c70 */

int evt_TypeOf(char *name)
{
    char buf[512];
    char *p;

    strcpy(buf, name);
    if ((p = strchr(buf, ' ')) || (p = strchr(buf, ')')))
        *p = 0;
    if (!strcmp(buf, "int"))
        return 1;
    if (!strcmp(buf, "float"))
        return 7;
    if (!strcmp(buf, "void"))
        return 5;
    if (!strcmp(buf, "string"))
        return 6;
    if (!strcmp(buf, "{"))
        return 8;
    if (!strcmp(buf, "}"))
        return 9;
    if (!strcmp(buf, "byte") || !strcmp(buf, "char"))
        return 2;
    return 0;
}
