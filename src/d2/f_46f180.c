/* 0x46f180, sets the text colour (a, b, c) of the 5-bit modes */

void d2_SetTextColour(unsigned char a, unsigned char b, unsigned char c)
{
    d2_SetMode5Colour(a, b, c);
}
