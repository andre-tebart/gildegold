/* 0x44e840, text-format line handler: sets the current material's blend mode from the next token,
   "MIX" = 1, "ADD" = 2, anything else = 0 */

void agi_Blend(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");

    AGI_M(c).mode = 0;
    if (!strcmp(s, "MIX")) {
        AGI_M(c).mode = 1;
        return;
    }
    if (!strcmp(s, "ADD"))
        AGI_M(c).mode = 2;
}
