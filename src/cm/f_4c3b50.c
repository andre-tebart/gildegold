/* 0x4c3b50, the state of the group: 1 done, 0 pending, -1 one failed */

int cm_SendGroupState(void)
{
    int ret;
    unsigned int s;

    ret = 1;
    if (ai_Unkf8121c != ai_Unka68c04 && ai_Unkf8121c < ai_Unka68c04) {
        for (s = ai_Unkf8121c; s < ai_Unka68c04; s++) {
            if (ai_Unkf31160[s & 0x7fff].state == 0)
                return 0;
            if (ai_Unkf31160[s & 0x7fff].state == 2)
                ret = -1;
        }
    }
    return ret;
}
