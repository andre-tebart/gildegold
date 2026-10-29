/* 0x44e910, text-format line handler: reads the current material's b2 byte from the next token */

void agi_MatB2(void *unused, AGICTX *c)
{
    AGI_M(c).b2 = atoi(AGI_TOK(" "));
}
