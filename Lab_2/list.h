#include <QMainWindow>
#include <QTextStream>
#include <Qstring>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
typedef QString base;	// базовый тип элементов (атомов)
struct  s_expr;
struct  two_ptr  {
    s_expr  *hd;
    s_expr  *tl;
    two_ptr();
} ;	//end two_ptr;


struct  s_expr {
public:
    bool tag; // true: atom, false: pair
    struct  {
        base  atom;
        two_ptr pair;
    } node;
    s_expr();

};			//end s_expr

typedef  s_expr  *lisp;

class data_container
{
public:
    lisp main_list;
    QString inp_str;
    QString out_str;
    QTextStream str_in;
    QTextStream str_out;
    int i;

    data_container();

    void read_lisp(lisp&);
    void write_lisp (const lisp);
    bool brack_analizator();
    void destroy (lisp);
    void add_spaces_and_simplifie();
    void change_lisp(lisp&, base, base);

private:

    void read_s_expr (base, lisp&);
    void read_seq (lisp&);
    lisp head (const lisp);
    lisp tail (const lisp);
    lisp cons (const lisp, const lisp);
    lisp make_atom (const base);
    bool isAtom (const lisp);
    bool isNull (const lisp);
    void write_seq (const lisp);
    void change_seq(lisp&, base, base);
};
