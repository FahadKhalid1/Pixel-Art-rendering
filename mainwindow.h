#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString getImagePath();
    QString saveImage();
    void DisplayImage(QPixmap);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_btnpixelize_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPixmap image;
    QImage img;
};

#endif // MAINWINDOW_H
