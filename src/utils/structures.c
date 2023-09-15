#include "../headers/structures.h"

Package *pack(RequestType rqtype, ContentType cntype, void *arg)
{
    Package *pkg = (Package *) calloc(1, sizeof(Package));

    (*pkg).rqtype = CALLOC(RequestType, 1);
    (*(*pkg).rqtype) = rqtype;

    (*pkg).cntype = CALLOC(ContentType, 1);
    (*(*pkg).cntype) = cntype;

    if (cntype == GAMEPKG)
        (*pkg).arg.targ = arg;
    else
        (*pkg).arg.garg = arg;

    return pkg;
}
