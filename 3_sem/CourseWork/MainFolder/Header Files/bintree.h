#include <iostream>

namespace binTree_modul
{
    struct node
    {
        int value;
        int size;
        node* left;
        node* right;
        node(int k) { value = k; left = right = 0; size = 1; }
    };

    typedef node *binTree;

    int getsize(node* p);
    void fixsize(node* p);
    node* rotateright(node* p);
    node* rotateleft(node* q);
    node* insertroot(node* p, int k);
    node* insertrandom(node* p, int k, int &t);

    bool isNull(binTree);
    int RootBT(binTree);
    binTree Left(binTree);
    binTree Right(binTree);
    int CountTreeEl(binTree b);
}
