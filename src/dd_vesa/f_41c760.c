/* 0x41c760 */

char dd_StringToGuid(char *s, GUID *guid)
{
    int d1, d2, d3;
    int b0, b1, b2, b3, b4, b5, b6, b7;
    char ok = 0;

    if (s && *s && guid) {
        sscanf(s, "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", &d1, &d2, &d3,
               &b0, &b1, &b2, &b3, &b4, &b5, &b6, &b7);
        guid->Data1 = d1;
        guid->Data2 = d2;
        guid->Data3 = d3;
        guid->Data4[0] = b0;
        guid->Data4[1] = b1;
        guid->Data4[2] = b2;
        guid->Data4[3] = b3;
        guid->Data4[4] = b4;
        guid->Data4[5] = b5;
        guid->Data4[6] = b6;
        guid->Data4[7] = b7;
        ok = 1;
    }
    return ok;
}
