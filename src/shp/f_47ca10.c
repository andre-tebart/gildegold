/* 0x47ca10, sets the colour of mode 5 shapes */

void d2_SetMode5Colour(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int c = dd_RGB(r, g, b);

    d2_Mode5Colour = c << 16 | c;
}
