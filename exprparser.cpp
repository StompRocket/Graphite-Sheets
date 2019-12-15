#include <QDebug>
#include "exprparser.h"

ExprParser::ExprParser(): str(), raw(nullptr) {}

void ExprParser::setSourceString(QString string)
{
    str = string.toStdString();
    raw = str.c_str();
}

char ExprParser::peek()
{
    return *raw;
}

char ExprParser::get()
{
    return *raw++;
}

bool ExprParser::isNum()
{
    return (peek() >= '0' && peek() <= '9') || peek() == '.';
}

bool ExprParser::isAlpha()
{
    return (peek() >= 'a' && peek() <= 'z')
        || (peek() >= 'A' && peek() <= 'Z')
        || peek() == '_';
}

double ExprParser::number()
{
    QString buf;
    while (isNum())
    {
        buf.append(get());
    }
    return buf.toDouble();
}

double ExprParser::var()
{
    QString buf;
    while (isAlpha())
    {
        buf.append(get());
    }
    while (isAlpha() || isNum())
    {
        buf.append(get());
    }

    // try for a function
    if (peek() == '(')
    {
        qDebug() << "Func" << buf;
        QList<double> args{};
        args.append(expr());
        while (peek() == ',')
        {
            get();
            args.append(expr());
        }
        if (get() != ')')
            throw ParsingError{};

        return getFunc(buf, args);
    }

    return getVar(buf);
}

double ExprParser::factor()
{
    if (isNum())
        return number();
    else if (isAlpha())
    {
        qDebug() << "Var!";
        return var();
    }
    else if (peek() == '(')
    {
        get(); // (
        double result = expr();
        if (get() != ')')
            throw ParsingError{};
        return result;
    }
    else if (peek() == '-')
    {
        get();
        return -expr();
    }
    return 0.0;
}

double ExprParser::term()
{
    double res = factor();

    while (peek() == '*' || peek() == '/')
        if (get() == '*')
            res *= factor();
        else
            res /= factor();
    return res;
}

double ExprParser::expr()
{
    double res = term();

    while (peek() == '+' || peek() == '-')
        if (get () == '+')
            res += term();
        else
            res -= term();

    return res;
}
