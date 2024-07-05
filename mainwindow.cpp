#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


// Устанавливает режим рисования на "Прямоугольник" и обновляет область рисования
void MainWindow::on_rectangleButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Rectangle;
    ui->drawingArea->update();
}


// Устанавливает режим рисования на "Эллипс" и обновляет область рисования
void MainWindow::on_ellipseButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Ellipse;
    ui->drawingArea->update();
}


// Устанавливает режим рисования на "Треугольник" и обновляет область рисования
void MainWindow::on_triangleButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Triangle;
    ui->drawingArea->update();
}


// Устанавливает режим рисования на "Соединение" и обновляет область рисования
void MainWindow::on_connectButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Connection;
    ui->drawingArea->update();
}


// Устанавливает режим рисования на "Перемещение"
void MainWindow::on_moveButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Move;
}

// Устанавливает режим рисования на "Удаление"
void MainWindow::on_deleteButton_clicked()
{
    ui->drawingArea->currentMode = DrawingArea::Mode::Delete;
}


// Открывает диалоговое окно для выбора файла и сохраняет текущий рисунок в выбранный файл
void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Drawing"), "", tr("Data Files (*.dat)"));
    if (!fileName.isEmpty()) {
        ui->drawingArea->saveToFile(fileName);
    }
}


// Открывает диалоговое окно для выбора файла и загружает рисунок из выбранного файла
void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Drawing"), "", tr("Data Files (*.dat)"));
    if (!fileName.isEmpty()) {
        ui->drawingArea->loadFromFile(fileName);
    }
}
