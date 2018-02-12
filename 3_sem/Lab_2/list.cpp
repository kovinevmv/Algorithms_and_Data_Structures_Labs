#include "mainwindow.h"

two_ptr::two_ptr()
{
    hd = NULL;
    tl = NULL;
}

s_expr::s_expr()
{
    tag = false;
    node.atom = "";
}//end union node

data_container::data_container()
{
    main_list = NULL;
    inp_str = "";
    out_str = "";
    i=0;
}

void data_container::add_spaces_and_simplifie()
{
    int a = -2;
    while((a = inp_str.indexOf('(',a+2))!=-1)
    {
        inp_str.insert(a, ' ');
        inp_str.insert(a+2, ' ');
    }
    a = -2;
    while((a = inp_str.indexOf(')',a+2))!=-1)
    {
        inp_str.insert(a, ' ');
        inp_str.insert(a+2, ' ');
    }
    inp_str = inp_str.simplified();
}

void data_container:: read_lisp(lisp &y)
{
    base x;
    do str_in >> x;
    while (x==" ");
    read_s_expr(x,y);
}

void data_container::read_s_expr(base prev, lisp &y)
{
    if(prev==")")
        return;
    else if (prev!="(")
        y = make_atom(prev);
    else
        read_seq(y);
};

void data_container::read_seq(lisp &y)
{
    base x;
    lisp p1, p2;
    str_in >> x;
    if((str_in.status() == QTextStream::WriteFailed))
    {
        return;
    }
    else
    {
        while(x==" ")
            str_in>> x;
        if(x==")")
            y = NULL;
        else
        {
            read_s_expr(x,p1);
            read_seq(p2);
            y = cons(p1,p2);
        }

    }
}

lisp data_container::cons(const lisp h, const lisp t)
{
    lisp p;
    if (isAtom(t))
    {
        exit(1);
    }
    else
    {
        p = new s_expr;
        if(p==NULL)
        {

            exit(1);
        }
        else
        {
            p->tag = false;
            p->node.pair.hd = h;
            p->node.pair.tl = t;
            return p;
        }
    }
}



lisp data_container::make_atom(const base x)
{
    lisp s;
    s = new s_expr;
    s->tag = true;
    s->node.atom = x;
    return s;
}

bool data_container::isAtom(const lisp s)
{
    if (s==NULL)
        return false;
    else
        return (s->tag);
}

void data_container::write_lisp(const lisp x)
{
    if(isNull(x))
        out_str +=" ( )";
    else if(isAtom(x))
        out_str += ' ' +x->node.atom;
    else
    {
        out_str+=" (";
        write_seq(x);
        out_str+= " )";
    }
}

void data_container::write_seq(const lisp x)
{
    if (!isNull(x))
    {
        write_lisp(head(x));
        write_seq(tail(x));
    }
}

lisp data_container::head (const lisp s)
{
    if (s != NULL)
        if (!isAtom(s))
            return s->node.pair.hd;
        else
        {
            exit(1);
        }
    else
    {
        exit(1);
    }
}

lisp data_container::tail(const lisp s)
{
    if (s != NULL)
        if (!isAtom(s))
            return s->node.pair.tl;
        else
        {
            exit(1);
        }
    else
    {
        exit(1);
    }
}

bool data_container::isNull(const lisp s)
{
    if (s == NULL)
        return true;
    return false;
}

void data_container::destroy(lisp s)
{
    if(s!=NULL)
    {
        if(!isAtom(s))
        {
            destroy(head(s));
            destroy(tail(s));
        }
    delete s;
    };
}


bool data_container::brack_analizator()
{
    int count = 0;
    if(inp_str.isEmpty())
        return false;
    if( inp_str.indexOf(' ') == -1
            && inp_str.indexOf('(') == -1
            && inp_str.indexOf(')') == -1)
        return true;
    if(inp_str[0] != '(')
        return false;
    for (int i = 0; i < inp_str.size(); i++)
    {
        if(inp_str[i] == '(')
            count++;
        if(inp_str[i] == ')')
            count--;
        if(count == 0 && i!= inp_str.size()-1)
            return false;
    }
    if (count == 0)
        return true;
    return false;
}

void data_container::change_lisp(lisp& x, base e, base c)
{
    if(isAtom(x))
    {
        if(x->node.atom == e)
            x->node.atom = c;
        return;
    }
    else if(!isAtom(x))
    {
        change_seq(x, e, c);
    }
}

void data_container::change_seq(lisp& x, base e, base c)
{
    if (!isNull(x))
    {
        change_lisp(x->node.pair.hd, e,c);
        change_seq(x->node.pair.tl, e,c);
    }
}

