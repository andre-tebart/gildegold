/* 0x53f500, for every inn (kind 5 / 0x16) whose record 0x12c is older than a day sends the script message "rclc" ("clear dunkle ecke") */

void he_Unk53f500(void)
{
    int f, r;
    int arg[4];

    f = gm_BuildQuery(1, 5, 0x16);
    while (f != 0) {
        r = gm_Query(*(int *)(f + 0x5d), 1, 0, 0x120);
        if (r != 0 && (r = gm_Query(*(int *)(r + 0x14), 1, 0, 300)) != 0 && U_I((unsigned)&ai_Time) - *(int *)(r + 0x28) > 1) {
            arg[2] = *(int *)(f + 1);
            arg[0] = 0x636c6572;
            arg[1] = -1;
            arg[3] = -1;
            cm_BeginGroup("clear dunkle ecke");
            ai_Unk4c5d40(arg);
            cm_EndGroup("clear dunkle ecke");
        }
        f = gm_BuildNext();
    }
}
