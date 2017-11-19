#include <iostream>
#include <cstdlib>
#include "bintree.h"
using namespace std;

namespace binTree_modul
{


    binTree Create()
    {
        return NULL;
    }

    bool isNull(binTree b)
    {
        return (b == NULL);
    }

    base RootBT(binTree b)
    {
        if (b != NULL)
            return b->info;
    }
    bool RooBT(binTree b)
    {
        if (b == NULL) return false;
        else return true;
    }

    binTree Left(binTree b)
    {
        if (b != NULL)
            return b->lt;
    }
    binTree Right(binTree b)
    {
        if (b != NULL)
            return b->rt;
    }

    binTree ConsBT(const base &x, binTree &lst, binTree &rst)
    {
        binTree p;
        p = new node;
        if (p != NULL)
        {
            p->info = x;
            p->lt = lst;
            p->rt = rst;
            return p;
        }
    }

    void destroy(binTree &b)
    {
        if (b != NULL)
        {
            destroy(b->lt);
            destroy(b->rt);
            delete b;
            b = NULL;
        }
    }

}
