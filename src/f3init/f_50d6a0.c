/* 0x50d6a0, the scene object callback (a function start Ghidra missed): an object without an owner record (SCENEOBJ.owner) takes the one of its closest ancestor (SCENEOBJ.parent) that has one */


void init_InheritOwner(char *o)
{
    SCENEOBJ *obj = (SCENEOBJ *)o;
    SCENEOBJ *par, *own;

    own = obj->owner;
    par = obj->parent;
    while (own == 0 && par != 0) {
        own = par->owner;
        obj->owner = own;
        par = par->parent;
    }
}
