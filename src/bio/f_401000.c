/* 0x401000, CRC-32 of buf (table bio_CrcTable, initial value and final xor 0xffffffff) */
unsigned int util_Hash(unsigned char *buf, unsigned int len)
{
    unsigned int crc = 0xffffffff;

    for (; len > 0; len--) {
        crc = (crc >> 8) ^ bio_CrcTable[(crc & 0xff) ^ *buf];
        buf++;
    }
    return ~crc;
}
