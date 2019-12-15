#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QMap>
#include <QString>
#include <QColor>
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

struct CellStyle
{
    inline static CellStyle basic();
    bool bold;
    bool italic;
};

QDataStream &operator<<(QDataStream &out, const CellStyle &in);
QDataStream &operator>>(QDataStream &in, CellStyle &out);

typedef QPair<quint32, quint32> Coordinate;

class SpreadSheet
{
public:
    SpreadSheet();
    ~SpreadSheet();
    QString getAt(int, int);
    void setAt(int, int, QString);
    bool contains(int, int);
    CellStyle *getStyle(int row, int col);
    CellStyle *basicStyle;
    QString evalAt(int, int);
    QList<Coordinate> refStack;
    QMap<Coordinate, QString> cellData;
    QMap<Coordinate, CellStyle> cellStyles;
};

QDataStream &operator<<(QDataStream &out, const SpreadSheet &in);
QDataStream &operator>>(QDataStream &in, SpreadSheet &out);

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
