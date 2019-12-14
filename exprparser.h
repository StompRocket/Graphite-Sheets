#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QString>

class ExprParser
{
public:
    ExprParser(QString string);

private:
    std::string str;
    const char *raw;
};

#endif // EXPRPARSER_H
