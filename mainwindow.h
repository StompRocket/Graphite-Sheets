#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "spreadsheet.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mainTable_cellActivated(int row, int column);

    void on_mainTable_cellEntered(int row, int column);

    void on_mainTable_itemSelectionChanged();

    void on_mainTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_formulaEdit_textChanged(const QString &arg1);

    void on_formulaEdit_textEdited(const QString &arg1);

    void on_mainTable_cellClicked(int row, int column);

    void on_mainTable_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    int rowCount;
    int colCount;
    int currentRow;
    int currentCol;
    SpreadSheet sheet;
    void setupTable();
    void renderCells();
};
#endif // MAINWINDOW_H
