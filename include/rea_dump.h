/* rea_dump.h -- debug frame dumps (REA_FRAME_DUMP=1 in tools/build.py); not part of the original. Included
   by dd_vesa.c (dd_Flip) and d3_d3d.c (d3_EndScene) under the flag only. */
#ifndef REA_DUMP_H
#define REA_DUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>


/* debug frame dumps (REA_FRAME_DUMP=1 in tools/build.py; read with tools/framedump.py). While a file
   dump.trigger (holding a frame count) exists in the game directory, every d3_EndScene writes the back buffer
   as dump/t<tick>_scene<k>.raw and dd_Flip writes dump/t<tick>_final<k>.raw and counts the file down (a count of
   0 disarms; the exe's CRT has no remove). A dump file is a 32 byte header of 8 little endian ints (width, height, bits per pixel, pitch in
   bytes, red / green / blue mask, tick) followed by height rows of pitch bytes. All state lives in the trigger
   file, so the dump code adds no functions or data; the lock is the surface's own
   (dd_LockSurface would change dd_Pitch / dd_PitchPixels); a failed lock is noted in dump/lockfail.txt. */
/* a trace line appended to dump/trace.txt: REA_DUMP_TRACE((rd_f, "fmt", args...)) */
#define REA_DUMP_TRACE(args)                                                                              \
    {                                                                                                     \
        FILE *rd_f;                                                                                       \
        CreateDirectory("dump", NULL);                                                                    \
        rd_f = fopen("dump/trace.txt", "a");                                                              \
        if (rd_f) {                                                                                       \
            fprintf args;                                                                                 \
            fclose(rd_f);                                                                                 \
        }                                                                                                 \
    }
/* n bytes at ptr written to the file path (under dump/) */
#define REA_DUMP_BYTES(path, ptr, n)                                                                      \
    {                                                                                                     \
        FILE *rd_f;                                                                                       \
        CreateDirectory("dump", NULL);                                                                    \
        rd_f = fopen(path, "wb");                                                                         \
        if (rd_f) {                                                                                       \
            fwrite(ptr, 1, n, rd_f);                                                                      \
            fclose(rd_f);                                                                                 \
        }                                                                                                 \
    }

/* the pixels of a GFX.C surface (D2SURF, d2.h) whose bits pointer is valid (system memory, or locked) written as
   dump/t<tick>_<stage><k>.raw in the frame dump format (masks 0) */
#define REA_DUMP_SURF(surf, stage)                                                                        \
    {                                                                                                     \
        char rd_path[64];                                                                                 \
        int rd_hdr[8], rd_y, rd_k;                                                                        \
        FILE *rd_f;                                                                                       \
        unsigned int rd_tick = GetTickCount();                                                            \
        CreateDirectory("dump", NULL);                                                                    \
        if ((surf)->bits) {                                                                               \
            for (rd_k = 0;; rd_k++) {                                                                     \
                sprintf(rd_path, "dump/t%08u_%s%d.raw", rd_tick, stage, rd_k);                            \
                rd_f = fopen(rd_path, "rb");                                                              \
                if (!rd_f)                                                                                \
                    break;                                                                                \
                fclose(rd_f);                                                                             \
            }                                                                                             \
            rd_f = fopen(rd_path, "wb");                                                                  \
            if (rd_f) {                                                                                   \
                rd_hdr[0] = (surf)->w;                                                                    \
                rd_hdr[1] = (surf)->h;                                                                    \
                rd_hdr[2] = (surf)->bpp;                                                                  \
                rd_hdr[3] = (surf)->rowbytes;                                                             \
                rd_hdr[4] = rd_hdr[5] = rd_hdr[6] = 0;                                                    \
                rd_hdr[7] = rd_tick;                                                                      \
                fwrite(rd_hdr, sizeof rd_hdr, 1, rd_f);                                                   \
                for (rd_y = 0; rd_y < (surf)->h; rd_y++)                                                  \
                    fwrite((char *)(surf)->bits + rd_y * (surf)->rowbytes, (surf)->rowbytes, 1, rd_f);    \
                fclose(rd_f);                                                                             \
            }                                                                                             \
        }                                                                                                 \
    }

#define REA_DUMP_TRIGGER(n)                                                                                   {                                                                                                             FILE *rd_t = fopen("dump.trigger", "r");                                                                  char rd_line[32];                                                                                         (n) = 0;                                                                                                  if (rd_t) {                                                                                                   if (fgets(rd_line, sizeof rd_line, rd_t))                                                                     (n) = atoi(rd_line);                                                                                  fclose(rd_t);                                                                                         }                                                                                                     }
#define REA_DUMP_BACKBUFFER(stage)                                                                        \
    {                                                                                                     \
        DDSURFACEDESC2 rd_ddsd;                                                                           \
        char rd_path[64];                                                                                 \
        int rd_hdr[8], rd_y, rd_k;                                                                        \
        FILE *rd_f;                                                                                       \
        HRESULT rd_hr;                                                                                    \
        unsigned int rd_tick = GetTickCount();                                                            \
        memset(&rd_ddsd, 0, sizeof rd_ddsd);                                                              \
        rd_ddsd.dwSize = sizeof rd_ddsd;                                                                  \
        CreateDirectory("dump", NULL);                                                                    \
        rd_hr = dd_BackBuffer ? dd_BackBuffer->lpVtbl->Lock(dd_BackBuffer, NULL, &rd_ddsd,                \
                                                            DDLOCK_WAIT | DDLOCK_READONLY, NULL)          \
                              : DDERR_INVALIDPARAMS;                                                      \
        if (rd_hr == DD_OK) {                                                                             \
            for (rd_k = 0;; rd_k++) {                                                                     \
                sprintf(rd_path, "dump/t%08u_%s%d.raw", rd_tick, stage, rd_k);                            \
                rd_f = fopen(rd_path, "rb");                                                              \
                if (!rd_f)                                                                                \
                    break;                                                                                \
                fclose(rd_f);                                                                             \
            }                                                                                             \
            rd_f = fopen(rd_path, "wb");                                                                  \
            if (rd_f) {                                                                                   \
                rd_hdr[0] = rd_ddsd.dwWidth;                                                              \
                rd_hdr[1] = rd_ddsd.dwHeight;                                                             \
                rd_hdr[2] = rd_ddsd.ddpfPixelFormat.dwRGBBitCount;                                        \
                rd_hdr[3] = rd_ddsd.lPitch;                                                               \
                rd_hdr[4] = rd_ddsd.ddpfPixelFormat.dwRBitMask;                                           \
                rd_hdr[5] = rd_ddsd.ddpfPixelFormat.dwGBitMask;                                           \
                rd_hdr[6] = rd_ddsd.ddpfPixelFormat.dwBBitMask;                                           \
                rd_hdr[7] = rd_tick;                                                                      \
                fwrite(rd_hdr, sizeof rd_hdr, 1, rd_f);                                                   \
                for (rd_y = 0; rd_y < (int)rd_ddsd.dwHeight; rd_y++)                                      \
                    fwrite((char *)rd_ddsd.lpSurface + rd_y * rd_ddsd.lPitch, rd_ddsd.lPitch, 1, rd_f);   \
                fclose(rd_f);                                                                             \
            }                                                                                             \
            dd_BackBuffer->lpVtbl->Unlock(dd_BackBuffer, NULL);                                           \
        } else {                                                                                          \
            rd_f = fopen("dump/lockfail.txt", "a");                                                       \
            if (rd_f) {                                                                                   \
                fprintf(rd_f, "%s: Lock failed, hr=%08lx\n", stage, (unsigned long)rd_hr);                \
                fclose(rd_f);                                                                             \
            }                                                                                             \
        }                                                                                                 \
    }

#endif
