#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QString>
#include <exception>

class ParsingError : public std::exception
{};

class ExprParser
{
public:
    ExprParser();
    void setSourceString(QString source);
    double expr();

private:
    std::string str;
    const char *raw;
    char peek();
    char get();
    double number();
    double factor();
    double term();
    bool isNum();

protected:
    // Override this in a subclass to define your own variables
    double getVar(QString named);
};

#endif // EXPRPARSER_H
