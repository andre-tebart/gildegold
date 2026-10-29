/* 0x4e7550, the character who holds the office the kind k is the deputy of (0x24 -> kind 0x27, the person's office; 0x23 / 0x25 -> kind 0x26, the historical
   event's office); 0 when there is none. The election hmt_Election uses it as the reference object (hmt_Score) of the year's office type */

char *amt_GetSuperiorOfficeHolder(int k)
{
    AMTREC o;

    if (k == 0x24) {
        if (amt_GetFirstOffice(0x27, &o) != 0)
            return gm_FindChar(o.owner);
    } else if (k == 0x25 || k == 0x23) {
        if (amt_GetFirstOffice(0x26, &o) != 0)
            return gm_FindChar(o.owner);
    }
    return 0;
}
