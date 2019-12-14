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

double ExprParser::number()
{
    QString buf;
    while (isNum())
    {
        buf.append(get());
    }
    return buf.toDouble();
}

double ExprParser::factor()
{
    if (isNum())
        return number();
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
