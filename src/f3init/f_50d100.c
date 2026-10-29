/* 0x50d100, maps the name of the scene object o (copied: "ob_NAME_X" / "gb_NAME") to a prototype: a variant suffix "_A" .. "_H" is cut off, "ob" looks the name up in the object prototypes (the result 0x1000000 | number at SCENEOBJ.proto), "gb" in the building prototypes (0x2000000 | number), a name containing "KONTOR" is the building 0x47 (0x2000047). Returns 1 when found. */

int init_SceneNameToPrototype(char *o)
{
    SCENEOBJ *obj = (SCENEOBJ *)o;
    char head[0x100], rest[0x100], tail[0x80];
    char *u, *t;
    unsigned n;
    int i;

    strcpy(head, o);
    u = strchr(head, '_');
    if (u == 0)
        return 0;
    strcpy(rest, u + 1);
    *u = 0;
    t = strrchr(rest, '_');
    if (t != 0) {
        strcpy(tail, t);
        if (strcmp(tail, "_A") == 0 || strcmp(tail, "_B") == 0 || strcmp(tail, "_C") == 0 || strcmp(tail, "_D") == 0 ||
            strcmp(tail, "_E") == 0 || strcmp(tail, "_F") == 0 || strcmp(tail, "_G") == 0 || strcmp(tail, "_H") == 0)
            *t = 0;
    }
    if (_strcmpi(head, "ob") == 0 || _strcmpi(head + 1, "ob") == 0) {
        n = 0;
        for (i = 0; i < 0xb9dc; i += 0x41) {
            if (_strcmpi(rest, ai_Goods + i + 1) == 0) {
                obj->proto = n | INIT_PROTO_OBJECT;
                return 1;
            }
            n++;
        }
        return 0;
    }
    if (_strcmpi(head, "gb") != 0)
        return 0;
    n = 0;
    for (i = 0; i < 0xca78; i += 0x24d) {
        if (_strcmpi(rest, ai_BTypes + i + 1) == 0) {
            obj->proto = n | INIT_PROTO_BUILDING;
            return 1;
        }
        n++;
    }
    if (strstr(rest, "KONTOR") != 0) {
        obj->proto = INIT_PROTO_BUILDING | INIT_KONTOR_BUILDING;   /* the Kontor building, 0x2000000 | 0x47 */
        return 1;
    }
    return 0;
}
