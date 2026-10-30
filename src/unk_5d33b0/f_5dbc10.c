/* 0x5dbc10, the default effect record (5 B): returns the address of ai_DefaultEffect (0x62de60: an
   empty name, the scale 1.0, the lifetime 25.0, then the entry's ids / callbacks). The twin
   0x5dbc20 returns its scale from the same record. */

int ai_GetDefaultEffect(void)
{
    return (int)&ai_DefaultEffect;
}
