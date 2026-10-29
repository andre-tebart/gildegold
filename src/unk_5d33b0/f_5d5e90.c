/* 0x5d5e90, applies the option values to the run time (117 B): the game speed factor cm_SpeedFactor
   from the "speed" option (ai_OptionSpeed * 0.00625 + 0.25; in a network game (ai_GameFlags & 4) from
   the shared speed level cm_SpeedLevel: (level * 0.25 + 0.5) * 0.8), the input base kb_SetBase
   (ai_OptionMouseSpeed - 0x40), the camera step ai_CameraStep = 100 - the low byte of the
   "camera_speed" option and di_SwapLR = 0. Called on every option apply, at the game start and by the
   speed keys (0x589cf0). */


void ai_ApplyOptions(void)
{
    double speed;

    di_SwapLR = 0;
    if ((ai_GameFlags & 4) != 0)
        speed = ((double)(unsigned int)cm_SpeedLevel * 0.25 + 0.5) * 0.8;
    else
        speed = (float)ai_OptionSpeed * 0.00625f + 0.25;
    cm_SpeedFactor = (float)speed;
    kb_SetBase(ai_OptionMouseSpeed - 0x40);
    ai_CameraStep = 100 - (ai_OptionCameraSpeed & 0xff);
}
