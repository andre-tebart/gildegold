/* 0x42c3a0, the sound timer's highest delay (AIL_get_timer_highest_delay) for msx: a tail call through the
   import slot. It is sfx's own code (its nop fill before it is the compiler's alignment inside one object, not
   LINK's int3 padding between objects), so it is no import thunk */

int sfx_TimerHighestDelay(void)
{
    return AIL_get_timer_highest_delay();
}
