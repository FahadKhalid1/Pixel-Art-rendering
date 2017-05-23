#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString getImagePath();
    void DisplayImage(QImage *);
    QColor* Average (QImage*);
    QImage* LeastDifference(const QColor&);
    void  Artit ();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_btnpixelize_clicked();

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage *originalImage;
    QImage *LowResImage;
    QImage *Result;
    QImage *displayedImage;
    int SizeOfImage=50;//spnbox
    int SmallImageSize =5;
    std::vector<QImage*> lowImage;


};

#endif // MAINWINDOW_H
