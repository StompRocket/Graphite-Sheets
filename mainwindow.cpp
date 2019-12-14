#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentRow(1)
    , currentCol(1)
    , sheet()
{
    ui->setupUi(this);

    auto *central = new QWidget();
    ui->verticalLayout->setContentsMargins(10, 10, 10, 10);
    central->setLayout(ui->verticalLayout);
    this->setCentralWidget(central);

    rowCount = 20;
    colCount = 6;

    setupTable();

    ui->formulaEdit->setEnabled(false);
    renderCells();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    ui->mainTable->setRowCount(rowCount);
    ui->mainTable->setColumnCount(colCount);

    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < colCount; j++)
        {
            auto *item = new QTableWidgetItem{};
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->mainTable->setItem(i, j, item);
        }
    }
}

void MainWindow::on_mainTable_cellActivated(int row, int column)
{
    qDebug() << row << column;
}

void MainWindow::on_mainTable_cellEntered(int, int)
{
    // bruh
}

void MainWindow::on_mainTable_itemSelectionChanged()
{
    if (ui->mainTable->selectedItems().length() == 1)
    {
        currentRow = ui->mainTable->currentRow();
        currentCol = ui->mainTable->currentColumn();
        ui->formulaEdit->setEnabled(true);
    }
    else
    {
        ui->formulaEdit->setEnabled(false);
        ui->formulaEdit->setText("");
    }
}

void MainWindow::on_mainTable_currentCellChanged(int newRow, int newCol, int, int)
{
    currentRow = newRow;
    currentCol = newCol;
    qDebug() << newRow << newCol;
    ui->formulaEdit->setText(sheet.getAt(currentRow, currentCol));
}

void MainWindow::on_formulaEdit_textChanged(const QString &)
{
    // Don't use this slot, it executes even when the data is not edited by user
}

void MainWindow::on_formulaEdit_textEdited(const QString &text)
{
    sheet.setAt(currentRow, currentCol, text);
    renderCells();
}

void MainWindow::renderCells()
{
    qDebug() << currentRow << "," << currentCol;
    for (auto i = sheet.cellData.constBegin(); i != sheet.cellData.constEnd(); i++)
    {
        const auto [row, col] = i.key();
        qDebug() << row << "," << col << sheet.evalAt(row, col);
        QTableWidgetItem *item = new QTableWidgetItem{};
        item->setText(sheet.evalAt(row, col));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->mainTable->setItem(row, col, item);
    }
}

void MainWindow::on_mainTable_cellClicked(int row, int column)
{
    currentCol = column;
    currentRow = row;
}

void MainWindow::on_mainTable_cellDoubleClicked(int, int)
{
    ui->formulaEdit->focusWidget();
}
