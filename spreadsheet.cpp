#include <QVariant>
#include <QtMath>
#include <QDebug>
#include <QDataStream>
#include "spreadsheet.h"

SpreadSheet::SpreadSheet()
{
    cellData[QPair<int,int>(2,3)] = "Hello";
    cellStyles[QPair<int,int>(2,3)] = CellStyle{true, false};
}

QString SpreadSheet::getAt(int x, int y)
{
    QPair<int,int> at(x, y);
    if (cellData.contains(at))
    {
        return cellData.value(at);
    } else return "";
}

bool SpreadSheet::contains(int x, int y)
{
    return cellData.contains(QPair<int,int>(x, y));
}

void SpreadSheet::setAt(int x, int y, QString val)
{
    cellData[QPair<int,int>(x,y)] = val;
}

void SpreadSheet::clearReferences()
{
    refStack.clear();
}

double SheetEval::getVar(QString named)
{
    if (named == "pi")
        return 3.141592;
    else if (named.length() >= 2 && named[0].isUpper() && named[1].isNumber())
    {
        qDebug() << sheet->refStack;
        bool ok = true;
        int col = named[0].unicode() - 'A';
        named.remove(0, 1);
        // Starts at 1
        int row = named.toInt(&ok) - 1;

        qDebug() << "Reference to" << row << col;

        if (!ok)
        {
            qDebug() << "Is not number";
            throw DefinitionError{named};
        }

        if (sheet->refStack.contains(QPair<int,int>(row, col)))
        {
            qDebug() << "Recursion :(";
            for (auto coord : sheet->refStack)
            {
                qDebug() << coord;
            }
            throw RecursionError{named};
        }

        auto res = sheet->evalAt(row, col).toDouble(&ok);

        if (!ok)
            throw DefinitionError{named};

        qDebug() << "=" << res;

        return res;
    }
    else throw DefinitionError{named};
}

double SheetEval::getFunc(QString named, QList<double> args)
{
    if (named == "sqrt")
        return qSqrt(args[0]);
    else if (named == "sin")
        return qSin(args[0]);
    else if (named == "tan")
        return qTan(args[0]);
    else if (named == "cos")
        return qCos(args[0]);
    else if (named == "ceil")
        return qCeil(args[0]);
    else if (named == "floor")
        return qFloor(args[0]);
    else throw DefinitionError{named};
}

CellStyle CellStyle::basic()
{
    return CellStyle
    {
        false,
        false,
    };
}

CellStyle *SpreadSheet::getStyle(int row, int col)
{
    QPair<int,int> p(row, col);
    return &cellStyles[p];
}

QString SpreadSheet::evalAt(int x, int y)
{
    QString value = getAt(x, y);
    if (value.startsWith("="))
    {
        refStack.append(QPair<int,int>(x, y));
        value.remove(0, 1);
        try
        {
            qDebug() << "Evaling" << value << refStack;
            SheetEval eval{this};
            eval.setSourceString(value);
            QString res = QString::number(eval.expr());
            refStack.removeLast();
            return res;
        }
        // No point cleaning up the ref stack if an error occurs.
        catch (ParsingError &)
        {
            throw EvalError{"Error parsing expression"};
        }
        catch (DefinitionError &)
        {
            throw EvalError{"Error resolving variable"};
        }
        catch (RecursionError &)
        {
            throw EvalError{"Recursive cell references are not allowed"};
        }
    }
    else return value;
}

QDataStream &operator<<(QDataStream &out, const SpreadSheet &in)
{
    out << in.cellData << in.cellStyles;

    return out;
}

QDataStream &operator>>(QDataStream &in, SpreadSheet &out)
{
    in >> out.cellData >> out.cellStyles;

    return in;
}

QDataStream &operator<<(QDataStream &out, const CellStyle &in)
{
    out << in.bold << in.italic;
}
QDataStream &operator>>(QDataStream &in, CellStyle &out)
{
    in >> out.bold >> out.italic;
}
