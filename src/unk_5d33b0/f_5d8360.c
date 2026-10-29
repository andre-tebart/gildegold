/* 0x5d8360, the tailor contact prompt (198 B), reached from the production-building contact
   listener 0x5d8420 (its contact_PLAUSCH branch): when the character's flags1cc (+0x1cc of
   ai_Chars[ai_PlayerId]) has bit 0x20000, the message box 0x521150 shows ai_TailorBlockedText
   (flags 4) and the function returns. Otherwise sv_CheckMoney (the character, 1) asks whether its
   money covers the chat; when it does, a D3_ZEROed request record (0x50 bytes: +0 = 1, +4 = 0x400,
   +0x10 = 6) goes through the amount menu ai_AmountMenu (the caption ai_TailorAmountText,
   ch_EmptyName as the second text, the callback ai_TailorChatWindow). Ends with the camera follow
   sv_CameraFollow. Void; the two arguments the contact dispatch passes are not read. */

/* The 0x50-byte request record 0x5c1130 sends: the kind at +0, the flags / id at +4 and the event
   kind at +0x10. */
typedef struct CONTACTREQ {
    int type;                           /* +0x00 */
    int a;                              /* +0x04 */
    char pad08[8];
    int b;                              /* +0x10 */
    char pad14[0x3c];
} CONTACTREQ;

void ai_TailorContact(int building, int arg)
{
    CONTACTREQ req;

    if ((((AICHARVIEW *)(ai_Chars + ai_PlayerId * 0x21c))->flags1cc & 0x20000) != 0) {
        sv_MessageBox((int)ai_TailorBlockedText, 4);
        return;
    }
    if (sv_CheckMoney((char *)(ai_Chars + ai_PlayerId * 0x21c), 1) != 0) {
        D3_ZERO(&req, sizeof(req));
        req.type = 1;
        req.a = 0x400;
        req.b = 6;
        ai_AmountMenu((int)&req, (int)ai_TailorAmountText, (int)ch_EmptyName, (int)ai_TailorChatWindow);
    }
    sv_CameraFollow();
}
