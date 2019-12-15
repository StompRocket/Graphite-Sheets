#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

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
    bool enableControls = false;
    if (ui->mainTable->selectedItems().length() == 1)
    {
        currentRow = ui->mainTable->currentRow();
        currentCol = ui->mainTable->currentColumn();

        enableControls = true;
    }
    else
    {
        ui->formulaEdit->setText("");
    }
    ui->formulaEdit->setEnabled(enableControls);
    ui->actionBold->setEnabled(enableControls);
    ui->actionItalics->setEnabled(enableControls);
}

void MainWindow::on_mainTable_currentCellChanged(int newRow, int newCol, int, int)
{
    currentRow = newRow;
    currentCol = newCol;
    qDebug() << newRow << newCol;
    ui->formulaEdit->setText(sheet.getAt(currentRow, currentCol));
    auto *style = sheet.getStyle(currentRow, currentCol);
    ui->actionBold->setChecked(style->bold);
    ui->actionItalics->setChecked(style->italic);
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
        QTableWidgetItem *item = new QTableWidgetItem{};
        CellStyle *style = sheet.getStyle(row, col);
        QFont font;
        font.setBold(style->bold);
        font.setItalic(style->italic);
        item->setFont(font);
        try
        {
            item->setText(sheet.evalAt(row, col));
            ui->statusbar->showMessage("");
        }
        catch (EvalError &err)
        {
            item->setText("#ERR");
            item->setTextColor(QColor{255, 0, 0, 255});
            ui->statusbar->showMessage(err.message);
        }

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
    ui->formulaEdit->setFocus(Qt::FocusReason::ShortcutFocusReason);
}

void MainWindow::on_actionBold_triggered(bool checked)
{
    sheet.getStyle(currentRow, currentCol)->bold = checked;
    renderCells();
}

void MainWindow::on_actionItalics_triggered(bool checked)
{
    sheet.getStyle(currentRow, currentCol)->italic = checked;
    renderCells();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dlg{this};
    dlg.setNameFilter("Graphite Sheets (*.gs)");
    dlg.setFileMode(QFileDialog::ExistingFile);
    if (dlg.exec())
    {
        filePath = dlg.selectedFiles().first();
    }

    openFile();
}

void MainWindow::openFile()
{
    QFile f(filePath);
    f.open(QIODevice::ReadOnly);
    QDataStream s(&f);
    s >> sheet;
    renderCells();
}

void MainWindow::saveFile()
{
    QFile out(filePath);
    out.open(QIODevice::WriteOnly);
    QDataStream s(&out);
    s << sheet;
}

void MainWindow::on_actionSave_triggered()
{
    if (filePath != "")
    {
        saveFile();
    }
    else
    {
        filePath = QFileDialog::getSaveFileName(this, "Save Sheet", ".", "Graphite Sheets (*.gs)");
        saveFile();
    }
}
