#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QMap>
#include <QString>
#include <QPair>
#include "exprparser.h"

class EvalError : public std::exception
{
public:
    EvalError(QString message) : message(message) {}
    QString message;
};

class RecursionError : public std::exception
{
public:
    RecursionError(QString message) : message(message) {}
    QString message;
};

class SpreadSheet
{
public:
    SpreadSheet();
    QString getAt(int, int);
    void setAt(int, int, QString);
    bool contains(int, int);
    QString evalAt(int, int);
    QList<QPair<int, int>> refStack;
    QMap<QPair<int, int>, QString> cellData;
private:
    QMap<QPair<int, int>, double> cellCache;
};

class SheetEval : public ExprParser
{
public:
    SheetEval(SpreadSheet *sheet) : sheet(sheet) {}

private:
    SpreadSheet *sheet;
    double getVar(QString) override;
    double getFunc(QString named, QList<double> args) override;
};

#endif // SPREADSHEET_H
