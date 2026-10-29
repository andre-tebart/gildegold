/* 0x5a6b50, the text of a city name (24 B): 0x5176d0 formats the value with "%1G" 0x61168c into buf */

void ai_FormatCityName(int value, char *buf)
{
    txt_Format(buf, "%1G", value);
}
