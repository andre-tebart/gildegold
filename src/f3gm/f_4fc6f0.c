/* 0x4fc6f0, ages the health float (+0x1c) of the character c to the age (+10 = age): above the lifetime float (+0x20) by an iteration of v = (v - k) + k / v with k = +0x14 (as many times as the years over), otherwise towards the float +0x18; the change goes to 0x4c4ac0 (id, 0x1c, ..), the age to the character through 0x4c3c00 (2, 1, 10, &age). ROUGH, the number of iterations is a guess */

void ai_Unk4c5690(int a, const char *s);                     /* 0x4c5690 */
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d00 */
float gm_CharacterHealth(short *c);
float unk_Unk4fc890(short *c);
void unk_Unk4fd120(void);

void gm_AgeCharacter(char *c, unsigned age)
{
    float k, v, d;
    int n;

    if (c != 0) {
        if ((float)(age & 0xffff) > *(float *)(c + 0x20)) {
            k = *(float *)(c + 0x14);
            v = *(float *)(c + 0x1c);
            n = (int)((float)(age & 0xffff) - *(float *)(c + 0x20));
            if (n > 0) {
                do {
                    n--;
                    v = (v - k) + k / v;
                } while (n != 0);
            }
            d = v - *(float *)(c + 0x1c);
        } else {
            d = *(float *)(c + 0x18) - *(float *)(c + 0x1c);
        }
        ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, d);
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldDelta(2, 1, 10, &age);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
    }
}
