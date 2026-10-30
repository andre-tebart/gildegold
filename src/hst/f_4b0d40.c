/* 0x4b0d40, the GLAUBENSWECHSEL text command (callback 0x601b50[15], name 0x601070[15])
   "-KATHOLISCH_pp_m" / "-EVANGELISCH_pp_m": message 0x81 for pp percent (1..80) of the characters of the religion
   byte 0x0c that the sign word names (KATHOLISCH = 1 protestant, EVANGELISCH = 0 catholic, the two words of
   f3init.c's INIT_FIELD_RELIGION) in m groups (1..8, at most a third of them), today 8:00; the message's ch is the
   religion of the counted characters.
   Match-base session: the name array order {KATHOLISCH, EVANGELISCH} (the original copies KATHOLISCH from
   ds:0x601c20 into the lower slot [esp+0x14], so the KATHOLISCH word counts the 1s), the float product
   (float)cnt * p (the original's fild + fimul, an integer imul has no fimul and grows the frame by 8), the
   d1-before-d2 declaration order, the two zero stores as memset of a 4-byte buffer, and msg.time = ai_Time
   (byte-identical to am.h's TMX(msg) = NOWX, which is Q4-blocked) are the fixes; 165 -> 6 differing
   instructions. Q1 has evidence, Q2..Q7 pass.
   @deep only scheduling left: 3 order hunks of 226 instructions (6 differing) - the for-init i = 0 of both digit loops sits after fmt++ (4b0e0e) / after the *fmt == 0 test (4b0e52) in ours and before them in the original (4b0e05, 4b0e4d), and the TMX word store (msg.ids[0], [esp+0x7c]) is kept until after fromId/flags/car/n in the original (4b0fa9) but emitted right after the TMX dwords in ours (4b0f82). Meaning read equal against the disassembly: the name array is {KATHOLISCH, EVANGELISCH} (ds:0x601c20 into [esp+0x14]), the product is float (fild + fimul p), the d1/d2 slot pair and the msg fields match. Store-order / hoist work: vargen perm rounds or a source shape that keeps the word store late. */

int ai_CommandFaithChange(int a, int b, char *fmt)
{
    char name[2][16] = { "KATHOLISCH", "EVANGELISCH" };
    char d1[4];
    char d2[4];
    int rel, i, p, m, n, cnt;
    AIMSG msg;

    i = 0;
    if (*fmt != '-')
        return 0;
    fmt++;
    for (; i < 2; i++) {
        if (memcmp(fmt, name[i], strlen(name[i])) == 0)
            break;
    }
    if (i == 0)
        rel = 1;
    else if (i == 1)
        rel = 0;
    else
        return 0;
    fmt += strlen(name[i]);
    if (*fmt != '_')
        return 0;
    fmt++;
    memset(d1, 0, sizeof(d1));
    memset(d2, 0, sizeof(d2));
    for (i = 0; ; i++, fmt++) {
        if (*fmt == 0)
            return 0;
        if (*fmt == '_' || i >= 3)
            break;
        if (*fmt < '0' || *fmt > '9')
            return 0;
        d1[i] = *fmt;
    }
    if (*fmt == 0 || i == 0)
        return 0;
    fmt++;
    if (*fmt == 0)
        return 0;
    for (i = 0; *fmt != 0 && i < 3; i++, fmt++) {
        if (*fmt < '0' || *fmt > '9')
            return 0;
        d2[i] = *fmt;
    }
    if (i == 0)
        return 0;
    p = atoi(d1);
    m = atoi(d2);
    if (p < 1 || p > 80 || m < 1 || m > 8)
        return 0;
    cnt = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].kind <= 4 && G12_B(i, 0x0c) == rel)
            cnt++;
    }
    n = (int)((float)cnt * p * 0.01f);
    if (n == 0)
        return 1;
    m = max(min(m, n / 3), 1);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x81;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.ch = rel;
    msg.time = ai_TimeT;
    msg.fromId = -1;
    msg.flags = 2;
    msg.car = n;
    msg.n = m;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}
