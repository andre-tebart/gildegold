/* 0x42b5b0 */

int evt_ImportCommand(char *name, int (*fn)(), int rettype, int nargs, ...)
{
    int i = 0;
    int k;
    char *s;
    char *d;
    char c;

    if (strlen(name) > 0x1f) {
        err_Message("evt_ImportCommand: Commandnamelength is invalid...");
        return 0;
    }
    if (evt_FindBuiltin(name)) {
        err_Message("evt_ImportCommand: Commandname already exists...");
        return 0;
    }
    for (; i < 256; i++)
        if (!evt_Builtins[i].fn)
            break;
    if (i >= 256) {
        err_Message("evt_ImportCommand: Too many commands!");
        return 0;
    }
    evt_Builtins[i].fn = fn;
    evt_Builtins[i].nargs = nargs;
    evt_Builtins[i].rettype = rettype;
    {
        va_list ap;
        va_start(ap, nargs);
        for (k = 0; k < nargs; k++)
            evt_Builtins[i].argtype[k] = va_arg(ap, int);
        va_end(ap);
    }
    s = name;
    d = evt_Builtins[i].name;
    do {
        c = *s++;
        *d++ = c;
    } while (c);
    return 1;
}
