#include "bintree.h"
#include <iostream>

using namespace std;

namespace binTree_modul
{
    int getsize(node* p)
    {
        if (!p) return 0;
        return p->size;
    }

    void fixsize(node* p)
    {
        p->size = getsize(p->left) + getsize(p->right) + 1;
    }


    node* rotateright(node* p)
    {
        node* q = p->left;
        if (!q) return p;
        p->left = q->right;
        q->right = p;
        q->size = p->size;
        fixsize(p);
        return q;
    }

    node* rotateleft(node* q)
    {
        node* p = q->right;
        if (!p) return q;
        q->right = p->left;
        p->left = q;
        p->size = q->size;
        fixsize(q);
        return p;
    }

    node* insertroot(node* p, int k)
    {
        if (!p)
            return new node(k);
        if (k<p->value)
        {
            p->left = insertroot(p->left, k);
            return rotateright(p);
        }
        else
        {
            p->right = insertroot(p->right, k);
            return rotateleft(p);
        }
    }

    node* insertrandom(node* p, int k, int &t)
    {
        if (!p)
        {
            return new node(k);
            t = 0;
        }
        int random= rand()%10;
        if (!random)
        {
            t = 1;
            return insertroot(p, k);
        }

        if (p->value>k)
        {
            p->left = insertrandom(p->left, k, t);
            t = 0;
        }
        else
        {
            p->right = insertrandom(p->right, k, t);
            t = 0;
        }
        fixsize(p);
        return p;
    }


    bool isNull(binTree b)
    {
        return (b == NULL);
    }

    int RootBT(binTree b)
    {
        if (b == NULL) { exit(1); }
        else return b->value;
    }

    binTree Left(binTree b)
    {
        if (b == NULL) { exit(1); }
        else return b->left;
    }

    binTree Right(binTree b)
    {
        if (b == NULL) { exit(1); }
        else return b->right;
    }

    int CountTreeEl(binTree b)
    {
        int c =  1;
        if (b ==NULL)
            return 0;
        else
        {
            c += CountTreeEl(Left(b));
            c += CountTreeEl(Right(b));
            return c;
        }
    }
}







