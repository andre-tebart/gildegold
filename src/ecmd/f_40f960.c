/* 0x40f960 */

/* ---- 0x40f960.. (worker sndreg) ---- */
/* handlers registered by ocmd_Register; defined above by workers ocmd / ocmd2 (prototypes here are
   address placeholders, the orchestrator reconciles them with the real definitions) */
int ocmd_CreateObject();  /* 0x40d8d0 */
int ocmd_CreateObjectAtDummy();  /* 0x40d940 */
int ocmd_CreateObjectGroupAtDummy();  /* 0x40d9a0 */
int ocmd_KillObject();  /* 0x40da10 */
int ocmd_SetPos();  /* 0x40da50 */
int ocmd_SetAngle();  /* 0x40dab0 */
int ocmd_MoveObject();  /* 0x40db30 */
int ocmd_RotateObject();  /* 0x40dbb0 */
int ocmd_MoveObjectRelative();  /* 0x40dc30 */
int ocmd_RotateObjectRelative();  /* 0x40dcb0 */
int ocmd_SetAmbiente();  /* 0x40dd50 */
int ocmd_LightCalc();  /* 0x40dd90 */
int ocmd_ReplaceObject();  /* 0x40ddb0 */
int ocmd_LoadScene();  /* 0x40ddf0 */
int ocmd_AttachAnim();  /* 0x40de30 */
int ocmd_PreloadAnim();  /* 0x40dfc0 */
int ocmd_AttachAnimLooped();  /* 0x40e0e0 */
int ocmd_AttachAnimLoopedToAll();  /* 0x40e430 */
int ocmd_DetachAnimFromAll();  /* 0x40e4f0 */
int ocmd_GetObjectHandle();  /* 0x40d680 */
int ocmd_GetRndObjectHandle();  /* 0x40d730 */
int ocmd_GetRndSubObjectHandle();  /* 0x40d7b0 */
int ocmd_GetSubObjectHandle();  /* 0x40d860 */
int ocmd_SetNoTextures();  /* 0x40e620 */
int ocmd_SetLightSet();  /* 0x40e640 */
int ocmd_SetFogSet();  /* 0x40e670 */
int ocmd_CameraFlight();  /* 0x40e6f0 */
int ocmd_ZoomOnObject();  /* 0x40ea00 */
int ocmd_CameraFlightEnhanced();  /* 0x40e750 */
int ocmd_NewCameraFlight();  /* 0x40e810 */
int ocmd_ObjectFlight();  /* 0x40e8b0 */
int ocmd_CreateRain();  /* 0x40ea90 */
int ocmd_DeleteRain();  /* 0x40eaa0 */
int ocmd_CreateParticle();  /* 0x40ec70 */
int ocmd_KillParticle();  /* 0x40edc0 */
int ocmd_SetCameraToDummy();  /* 0x40ee10 */
int ocmd_StopCameraFlight();  /* 0x40eea0 */
int ocmd_RenameObject();  /* 0x40eec0 */
int ocmd_SetBlocked();  /* 0x40eef0 */
int ocmd_SetTransient();  /* 0x40ef40 */
int ocmd_CreateEmitter();  /* 0x40ef90 */
int ocmd_KillEmitter();  /* 0x40f0c0 */
int ocmd_SetEmitterAmplitude();  /* 0x40f110 */
int ocmd_SetEmitterPhasespeed();  /* 0x40f1b0 */
int ocmd_SetEmitterDirection();  /* 0x40f250 */
int ocmd_SetEmitterSize();  /* 0x40f2c0 */
int ocmd_SetEmitterVelocity();  /* 0x40f320 */
int ocmd_SetEmitterAcceleration();  /* 0x40f390 */
int ocmd_SetEmitterRndVelocity();  /* 0x40f400 */
int ocmd_SetEmitterPlane();  /* 0x40f470 */
int ocmd_SetEmitterTimeAndAlpha();  /* 0x40f4f0 */
int ocmd_SetEmitterColor();  /* 0x40f560 */
int ocmd_SetEmitterFlags();  /* 0x40f5d0 */
int ocmd_SetEmitterInitFill();  /* 0x40f690 */
int ocmd_SetEmitterRebirthFill();  /* 0x40f6f0 */
int ocmd_SetEmitterTextureMode();  /* 0x40f750 */
int ocmd_SetEmitterIsTrigger();  /* 0x40f7a0 */
int ocmd_SetEmitterTriggerOnce();  /* 0x40f800 */
int ocmd_SetEmitterMaxTrigger();  /* 0x40f850 */
int ocmd_TriggerEmitter();  /* 0x40f8a0 */
int ocmd_SetParticlePos();  /* 0x40f8f0 */
int ocmd_SelectAllTextureSets();  /* 0x40e5a0 */

void ocmd_Register(void)
{
    evt_ImportCommand("CreateObject", ocmd_CreateObject, 1, 4, 7, 7, 7, 6);
    evt_ImportCommand("CreateObjectAtDummy", ocmd_CreateObjectAtDummy, 1, 2, 6, 1);
    evt_ImportCommand("CreateObjectGroupAtDummy", ocmd_CreateObjectGroupAtDummy, 1, 2, 6, 1);
    evt_ImportCommand("KillObject", ocmd_KillObject, 5, 1, 1);
    evt_ImportCommand("SetPos", ocmd_SetPos, 5, 4, 1, 7, 7, 7);
    evt_ImportCommand("SetAngle", ocmd_SetAngle, 5, 4, 1, 7, 7, 7);
    evt_ImportCommand("MoveObject", ocmd_MoveObject, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("RotateObject", ocmd_RotateObject, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("MoveObjectRelative", ocmd_MoveObjectRelative, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("RotateObjectRelative", ocmd_RotateObjectRelative, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetAmbiente", ocmd_SetAmbiente, 5, 3, 1, 1, 1);
    evt_ImportCommand("LightCalc", ocmd_LightCalc, 5, 1, 1);
    evt_ImportCommand("ReplaceObject", ocmd_ReplaceObject, 1, 2, 1, 6);
    evt_ImportCommand("LoadScene", ocmd_LoadScene, 5, 1, 6);
    evt_ImportCommand("AttachAnim", ocmd_AttachAnim, 1, 3, 1, 6, 1);
    evt_ImportCommand("PreloadAnim", ocmd_PreloadAnim, 1, 2, 1, 6);
    evt_ImportCommand("AttachAnimLooped", ocmd_AttachAnimLooped, 1, 3, 1, 6, 1);
    evt_ImportCommand("AttachAnimLoopedToAll", ocmd_AttachAnimLoopedToAll, 1, 4, 6, 6, 1, 1);
    evt_ImportCommand("DetachAnimFromAll", ocmd_DetachAnimFromAll, 5, 1, 6);
    evt_ImportCommand("GetObjectHandle", ocmd_GetObjectHandle, 1, 1, 6);
    evt_ImportCommand("GetRndObjectHandle", ocmd_GetRndObjectHandle, 1, 1, 6);
    evt_ImportCommand("GetRndSubObjectHandle", ocmd_GetRndSubObjectHandle, 1, 2, 1, 6);
    evt_ImportCommand("GetSubObjectHandle", ocmd_GetSubObjectHandle, 1, 2, 1, 6);
    evt_ImportCommand("SetNoTextures", ocmd_SetNoTextures, 5, 1, 1);
    evt_ImportCommand("SetLightSet", ocmd_SetLightSet, 5, 1, 1);
    evt_ImportCommand("SetFogSet", ocmd_SetFogSet, 5, 3, 1, 1, 1);
    evt_ImportCommand("CameraFlight", ocmd_CameraFlight, 5, 3, 1, 6, 6);
    evt_ImportCommand("ZoomOnObject", ocmd_ZoomOnObject, 5, 2, 1, 6);
    evt_ImportCommand("CameraFlightEnhanced", ocmd_CameraFlightEnhanced, 5, 7, 1, 6, 6, 6, 6, 6, 6);
    evt_ImportCommand("NewCameraFlight", ocmd_NewCameraFlight, 5, 7, 1, 6, 6, 6, 6, 6, 6);
    evt_ImportCommand("ObjectFlight", ocmd_ObjectFlight, 5, 7, 1, 1, 6, 6, 6, 6, 6);
    evt_ImportCommand("CreateRain", ocmd_CreateRain, 1, 1, 1);
    evt_ImportCommand("DeleteRain", ocmd_DeleteRain, 1, 1, 1);
    evt_ImportCommand("CreateParticle", ocmd_CreateParticle, 5, 1, 1);
    evt_ImportCommand("KillParticle", ocmd_KillParticle, 5, 1, 1);
    evt_ImportCommand("SetCameraToDummy", ocmd_SetCameraToDummy, 5, 1, 6);
    evt_ImportCommand("StopCameraFlight", ocmd_StopCameraFlight, 5, 0);
    evt_ImportCommand("RenameObject", ocmd_RenameObject, 1, 2, 1, 6);
    evt_ImportCommand("SetBlocked", ocmd_SetBlocked, 1, 2, 1, 1);
    evt_ImportCommand("SetTransient", ocmd_SetTransient, 1, 2, 1, 1);
    evt_ImportCommand("CreateEmitter", ocmd_CreateEmitter, 1, 7, 1, 1, 1, 1, 6, 1, 1);
    evt_ImportCommand("KillEmitter", ocmd_KillEmitter, 1, 1, 1);
    evt_ImportCommand("SetEmitterAmplitude", ocmd_SetEmitterAmplitude, 1, 6, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterPhasespeed", ocmd_SetEmitterPhasespeed, 1, 6, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterDirection", ocmd_SetEmitterDirection, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterSize", ocmd_SetEmitterSize, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterVelocity", ocmd_SetEmitterVelocity, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterAcceleration", ocmd_SetEmitterAcceleration, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterRndVelocity", ocmd_SetEmitterRndVelocity, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterPlane", ocmd_SetEmitterPlane, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterTimeAndAlpha", ocmd_SetEmitterTimeAndAlpha, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterColor", ocmd_SetEmitterColor, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterFlags", ocmd_SetEmitterFlags, 1, 7, 1, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterInitFill", ocmd_SetEmitterInitFill, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterRebirthFill", ocmd_SetEmitterRebirthFill, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterTextureMode", ocmd_SetEmitterTextureMode, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterIsTrigger", ocmd_SetEmitterIsTrigger, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterTriggerOnce", ocmd_SetEmitterTriggerOnce, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterMaxTrigger", ocmd_SetEmitterMaxTrigger, 1, 2, 1, 1);
    evt_ImportCommand("TriggerEmitter", ocmd_TriggerEmitter, 1, 1, 1);
    evt_ImportCommand("SetParticlePos", ocmd_SetParticlePos, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SelectAllTextureSets", ocmd_SelectAllTextureSets, 1, 2, 6, 1);
}
