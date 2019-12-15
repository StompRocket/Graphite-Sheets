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

class SpreadSheet : private ExprParser
{
public:
    SpreadSheet();
    QString getAt(int, int);
    void setAt(int, int, QString);
    bool contains(int, int);
    QString evalAt(int, int);

    QMap<QPair<int, int>, QString> cellData;
private:
    QMap<QPair<int, int>, double> cellCache;
    double getVar(QString) override;
    double getFunc(QString named, QList<double> args) override;
};

#endif // SPREADSHEET_H
