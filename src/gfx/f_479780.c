/* 0x479780, makes a copy of a surface: the same description and make flags, the whole source image blitted over;
   returns the new surface */

D2SURF *d2_CopySurface(D2SURF *surf)
{
    D2SURF desc;
    D2SURF *ns;

    desc = *surf;
    desc.DDSurface = 0;
    desc.bits = 0;
    ns = d2_MakeSurface(&desc, surf->makeFlags);
    d2_BlitPart(0, 0, ns->w, ns->h, surf, 0, 0, ns);
    return ns;
}
