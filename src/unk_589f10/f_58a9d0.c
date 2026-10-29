/* 0x58a9d0, the lobby's control 5 / 6 hook (96 B): control 5 of 0x62aa88, the server query 0x52e860 (0, 0, the
   screen 0x6ed938, U_I (0x6ed938), 0x35, three out ints) with control 6
   @deep byte but not readable: Q1 (unit unnamed, @named_by -), Q2 (d2_SetForm, sv_CreateFormSlider), Q3, Q4 U_I; same lobby/control API naming debt as 0x58a940 */

void ai_Unk58a9d0(void)
{
    int a;
    int b;
    int c;
    int s;

    if (U_I((unsigned)&full_data_62aa88) != -1) {
        d2_SetForm(U_I((unsigned)&full_data_62aa88), 5);
        s = U_I((unsigned)&d2_LastWindow);
        d2_SetForm(U_I((unsigned)&full_data_62aa88), 6);
        sv_CreateFormSlider(0, 0, U_I((unsigned)&d2_LastWindow), s, 0x35, &a, &b, &c);
    }
}
