#include <QVariant>
#include <QtMath>
#include "spreadsheet.h"

SpreadSheet::SpreadSheet()
{
    cellData[QPair<int,int>(2,3)] = "Hello";
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

double SpreadSheet::getVar(QString named)
{
    if (named == "pi")
        return 3.141592;
    else throw DefinitionError{};
}

double SpreadSheet::getFunc(QString named, QList<double> args)
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
    else throw DefinitionError{};
}

QString SpreadSheet::evalAt(int x, int y)
{
    QString value = getAt(x, y);
    if (value.startsWith("="))
    {
        value.remove(0, 1);
        setSourceString(value);
        try
        {
            return QString::number(expr());
        }
        catch (ParsingError &)
        {
            throw EvalError{"Error parsing expression"};
        }
        catch (DefinitionError &)
        {
            throw EvalError{"Error resolving variable"};
        }
    }
    else return value;
}
