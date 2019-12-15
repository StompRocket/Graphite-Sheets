#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QString>
#include <exception>

class ParsingError : public std::exception
{};

class DefinitionError : public std::exception
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
    double var();
    bool isNum();
    bool isAlpha();

protected:
    // Override this in a subclass to define your own variables
    virtual double getVar(QString named) = 0;
    virtual double getFunc(QString named, QList<double> args) = 0;
};

#endif // EXPRPARSER_H
