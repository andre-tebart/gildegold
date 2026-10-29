/* 0x465c90, hands out the static fixed literal/length and distance trees (and their depths) that a fixed
 * huffman block decodes with; returns Z_OK */

  /* done */
  ZFREE(z, v);
  return Z_OK;
}


/* build fixed tables only once--keep them here */
#ifdef BUILDFIXED
local int fixed_built = 0;
#define FIXEDH 544      /* number of hufts used by fixed tables */
local inflate_huft fixed_mem[FIXEDH];
local uInt fixed_bl;  /* 0x5fd2f8 */
local uInt fixed_bd;  /* 0x5fd2fc */
local inflate_huft *fixed_tl;  /* 0x5fd300 */
local inflate_huft *fixed_td;  /* 0x5fe300 */
#else
#include "inffixed.h"
#endif


int inflate_trees_fixed(bl, bd, tl, td, z)
uIntf *bl;               /* literal desired/actual bit depth */
uIntf *bd;               /* distance desired/actual bit depth */
inflate_huft * FAR *tl;  /* literal/length tree result */
inflate_huft * FAR *td;  /* distance tree result */
z_streamp z;             /* for memory allocation */
{
#ifdef BUILDFIXED
  /* build fixed tables if not already */
  if (!fixed_built)
  {
    int k;              /* temporary variable */
    uInt f = 0;         /* number of hufts used in fixed_mem */
    uIntf *c;           /* length list for huft_build */
    uIntf *v;           /* work area for huft_build */

    /* allocate memory */
    if ((c = (uIntf*)ZALLOC(z, 288, sizeof(uInt))) == Z_NULL)
      return Z_MEM_ERROR;
    if ((v = (uIntf*)ZALLOC(z, 288, sizeof(uInt))) == Z_NULL)
    {
      ZFREE(z, c);
      return Z_MEM_ERROR;
    }

    /* literal table */
    for (k = 0; k < 144; k++)
      c[k] = 8;
    for (; k < 256; k++)
      c[k] = 9;
    for (; k < 280; k++)
      c[k] = 7;
    for (; k < 288; k++)
      c[k] = 8;
    fixed_bl = 9;
    huft_build(c, 288, 257, cplens, cplext, &fixed_tl, &fixed_bl,
               fixed_mem, &f, v);

    /* distance table */
    for (k = 0; k < 30; k++)
      c[k] = 5;
    fixed_bd = 5;
    huft_build(c, 30, 0, cpdist, cpdext, &fixed_td, &fixed_bd,
               fixed_mem, &f, v);

    /* done */
    ZFREE(z, v);
    ZFREE(z, c);
    fixed_built = 1;
  }
#endif
  *bl = fixed_bl;
  *bd = fixed_bd;
  *tl = fixed_tl;
  *td = fixed_td;
  return Z_OK;
}
