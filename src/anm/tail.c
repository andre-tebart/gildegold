
char ai_Time[14] = {0}; /* 0x770ae0, AITIME (ai.h): 14 bytes (int, short, int, int); a 3-byte placeholder
                           here put anm_NAnimals / anm_Animals right behind it in .bss, so cm.c's message 3
                           (ai_Time[6] / [10]) overwrote the animal list pointer */
char anm_KindNames[8][0x20] = { "DOG", "CAT", "BIRD", "COW", "SHEEP", "CHICK", "PIG", "HORSE" };  /* 0x602688, "DOG", "CAT", "BIRD", "COW", "SHEEP", "CHICK", "PIG", "HORSE" */
int anm_NAnimals = 0;  /* 0x6f0734, the number of animals */
ANMREC * anm_Animals = 0;  /* 0x6f0738, the animal list (32 records, "anm:animals") */
int anm_NextAnimal = 0;  /* 0x6f073c, the animal updated next */
unsigned int anm_LastSpawn = 0;  /* 0x6f0740, d3_Time of the last spawn try */
