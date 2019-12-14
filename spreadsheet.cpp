#include <QVariant>
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
            return "#ERR";
        }
    }
    else return value;
}
