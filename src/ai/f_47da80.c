/* 0x47da80, updates the needs of every character in use */

void ai_UpdateNeeds(void)
{
    AICHAR *c;

    c = ai_Chars;
    do {
        if (c->id != 0xffff)
            ai_GrowNeeds(c);
        c++;
    } while ((int)c < (int)(ai_Chars + 768));
}
