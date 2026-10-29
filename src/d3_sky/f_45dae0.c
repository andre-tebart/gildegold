/* 0x45dae0, stores the six sky shades in d3sky_Colors (edge, inner, centre, inner, edge; slot 0 is not one of them) */

void d3sky_InitColors(void)
{
    d3sky_Colors[0] = D3SKY_COLOR0;
    d3sky_Colors[1] = D3SKY_COLOR_EDGE;
    d3sky_Colors[2] = D3SKY_COLOR_INNER;
    d3sky_Colors[3] = D3SKY_COLOR_CENTER;
    d3sky_Colors[4] = D3SKY_COLOR_INNER;
    d3sky_Colors[5] = D3SKY_COLOR_EDGE;
}
