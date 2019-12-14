#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QMap>
#include <QString>
#include <QPair>

class SpreadSheet
{
public:
    SpreadSheet();
    QString getAt(int, int);
    void setAt(int, int, QString);
    bool contains(int, int);
    QString evalAt(int, int);

    QMap<QPair<int, int>, QString> cellData;
    QMap<QPair<int, int>, double> cellCache;
};

#endif // SPREADSHEET_H
