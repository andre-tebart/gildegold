/* 0x40d270, registers the commands above (the last function of this file) */

void chc_Register(void)
{
    evt_ImportCommand("CreateCharacter", chc_CreateCharacter, 1, 4, 6, 7, 7, 7);
    evt_ImportCommand("CreateCharacterAtDummy", chc_CreateCharacterAtDummy, 1, 3, 6, 1, 6);
    evt_ImportCommand("KillCharacter", chc_KillCharacter, 5, 1, 1);
    evt_ImportCommand("WalkToDummy", chc_WalkToDummy, 5, 2, 1, 1);
    evt_ImportCommand("WalkToDummyRotate", chc_WalkToDummyRotate, 5, 2, 1, 1);
    evt_ImportCommand("WalkToDummyVerified", chc_WalkToDummyVerified, 5, 3, 1, 1, 1);
    evt_ImportCommand("WalkToDummyRotateVerified", chc_WalkToDummyRotateVerified, 5, 3, 1, 1, 1);
    evt_ImportCommand("PlayAnimation", chc_PlayAnimation, 5, 3, 1, 6, 1);
    evt_ImportCommand("PlayAnimationSound", chc_PlayAnimationSound, 5, 5, 1, 6, 1, 1, 6);
    evt_ImportCommand("PlayAnimationScript", chc_PlayAnimationScript, 5, 5, 1, 6, 1, 1, 6);
    evt_ImportCommand("PlayAnimationScriptInt", chc_PlayAnimationScriptInt, 5, 6, 1, 6, 1, 1, 6, 1);
    evt_ImportCommand("GetCharacterHandle", chc_GetCharacterHandle, 1, 1, 6);
    evt_ImportCommand("TakeObject", chc_TakeObject, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("TakeObjectLeft", chc_TakeObjectLeft, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("TakeObjectScript", chc_TakeObjectScript, 1, 6, 1, 6, 1, 1, 1, 6);
    evt_ImportCommand("GiveObject", chc_GiveObject, 1, 2, 1, 6);
    evt_ImportCommand("DropObject", chc_DropObject, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("DropObjectLeft", chc_DropObjectLeft, 1, 4, 1, 6, 1, 1);
    evt_ImportCommand("SitDown", chc_SitDown, 1, 2, 1, 6);
    evt_ImportCommand("SitDownAtOnce", chc_SitDownAtOnce, 1, 2, 1, 6);
    evt_ImportCommand("GetUp", chc_GetUp, 1, 2, 1, 6);
    evt_ImportCommand("SetCharacterCamera", chc_SetCharacterCamera, 5, 2, 1, 6);
    evt_ImportCommand("MoveCharacterCamera", chc_MoveCharacterCamera, 5, 3, 1, 6, 1);
    evt_ImportCommand("PreloadAnimation", chc_PreloadAnimation, 5, 5, 1, 6, 6, 6, 6);
    evt_ImportCommand("PreloadSitMesh", (int (*)())sv_ReturnZero, 5, 1, 1);
    evt_ImportCommand("CharacterIdle", chc_CharacterIdle, 1, 1, 1);
    evt_ImportCommand("StopCharacter", chc_StopCharacter, 5, 1, 1);
    evt_ImportCommand("KillCharacterAnimations", chc_KillCharacterAnimations, 5, 1, 1);
    evt_ImportCommand("DummyBlocked", chc_DummyBlocked, 1, 2, 1, 1);
    evt_ImportCommand("CharacterCount", chc_CharacterCount, 1, 0);
    evt_ImportCommand("LookAtCharacter", chc_LookAtCharacter, 5, 2, 1, 1);
    evt_ImportCommand("LookAtObject", chc_LookAtObject, 5, 2, 1, 1);
    evt_ImportCommand("SetCharacterToDummy", chc_SetCharacterToDummy, 5, 2, 1, 1);
    evt_ImportCommand("CharacterSitting", chc_CharacterSitting, 5, 1, 1);
    evt_ImportCommand("GetCharacterSubObjectHandle", chc_GetCharacterSubObjectHandle, 5, 2, 1, 6);
    evt_ImportCommand("SetCharacterStepSample", chc_SetCharacterStepSample, 5, 1, 6);
    evt_ImportCommand("SetLowPoly", chc_SetLowPoly, 5, 2, 1, 1);
    evt_ImportCommand("AttachObject", chc_AttachObject, 5, 3, 1, 6, 6);
    evt_ImportCommand("SetCharacterTransperancy", chc_SetCharacterTransperancy, 5, 2, 1, 1);
    evt_ImportCommand("CharacterSitsDown", chc_CharacterSitsDown, 5, 1, 1);
    evt_ImportCommand("FindInCharacterName", chc_FindInCharacterName, 1, 2, 1, 6);
}
