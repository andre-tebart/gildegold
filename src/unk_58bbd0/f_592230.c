/* 0x592230, the upgrade listener (89 B): the pump 0x5309a0 (this, 0x67fff) until 0; when the pending contact
   0x6f22e4 is "UPGRADE" the tech tree 0x591bf0 (building, 0x2c) opens */

void ai_UpgradeContact(int building)
{
    int r;

    r = sv_RunFrame(ai_UpgradeContact, 0x67fff);
    while (r != 0) {
        if (ai_ClickedObject != 0 && _strcmpi("UPGRADE", (char *)ai_ClickedObject) == 0)
            ai_TechTreeWindow(building, 0x2c);
        r = sv_RunFrame(ai_UpgradeContact, 0x67fff);
    }
}
