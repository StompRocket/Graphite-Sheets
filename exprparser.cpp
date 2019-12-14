#include "exprparser.h"

ExprParser::ExprParser(QString string)
{
    str = string.toStdString();
    raw = str.c_str();
}
