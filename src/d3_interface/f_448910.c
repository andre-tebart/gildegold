/* 0x448910, creates the cameras of a new scene: "MegaCam" at (0, 0, -256) and, when d3_Unk633438 is set, the
   three orthographic view cameras "Oben" (above), "Vorne" (front) and "Seite" (side)
   @deep Q2 d3_Unk633438: read all four image references (only store is 0 in d3_Init 0x43bc31; reads d3_SpawnObject 0x43c5f8, here 0x44894d, d3_LightInvalidate 0x4538c0), .data initial 0, no pointer to 0x633438 in the image, no export; GildeGold_TL.exe has the same flag (0x661324, set 0 only, LightInvalidate test absent); the three roles (ortho view cameras Oben/Vorne/Seite, 'r' lights become type 5, on==0 invalidation) give no evidenced name */

void d3if_CreateCameras(void)
{
    d3if_MegaCam = (int)d3_SpawnObject(3, "MegaCam");
    d3_SetObjectPosXYZ((D3OBJECT *)d3if_MegaCam, 0, 0, -256.0f);
    d3_SetObjectRotXYZ((D3OBJECT *)d3if_MegaCam, 0, 0, 0);
    d3_LinkTop((D3OBJECT *)d3if_MegaCam);
    if (d3_Unk633438) {
        d3if_CamOben = (int)d3_SpawnObject(3, "Oben");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamOben, 0, 256.0f, 0);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamOben, -1.57079637f, 0, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamOben);
        ((D3OBJECT *)d3if_CamOben)->objectFlags &= 0xfd;
        d3if_CamVorne = (int)d3_SpawnObject(3, "Vorne");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamVorne, 0, 0, -256.0f);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamVorne, 0, 0, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamVorne);
        ((D3OBJECT *)d3if_CamVorne)->objectFlags &= 0xfd;
        d3if_CamSeite = (int)d3_SpawnObject(3, "Seite");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamSeite, 256.0f, 0, 0);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamSeite, 0, 1.57079637f, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamSeite);
        ((D3OBJECT *)d3if_CamSeite)->objectFlags &= 0xfd;
    }
}
