#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsScene>
#include <QBuffer>
#include "iostream"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->btnpixelize->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{


   QString imgpath=getImagePath();


   img = QImage(imgpath);

   cout << img.width();
   if (!imgpath.isNull()){
            ui->pushButton_3->setEnabled(true);
            image.load(imgpath);}
    //ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);
   //image.save(saveImage(), "PNG");
}


QString MainWindow::getImagePath(){
    return QFileDialog::getOpenFileName(this,
                                    "Open a file",
                                    QString(),
                                    "Images (*.png *.jpg *.jpeg)");
}

QString MainWindow::saveImage(){
    return QFileDialog::getSaveFileName(this,
                                 tr("Save the image"),
                                 "",
                                 tr("Images (*.png*.jpg *.jpeg);;All Files (*)"));
}

void MainWindow::DisplayImage(QPixmap img){

    scene->addPixmap(img);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(img.rect().height(),img.rect().width());

    ui->pushButton_2->setEnabled(true);
    ui->btnpixelize->setEnabled(true);


}

void MainWindow::on_pushButton_3_clicked()
{
    DisplayImage(image);
}

void MainWindow::on_pushButton_2_clicked()
{
    image.save(saveImage(), "PNG");
}

void MainWindow::on_btnpixelize_clicked()
{
    for(int i = 0; i < img.width(); i += 20)
        for(int j = 0; j < img.height(); j += 20){

            int count = 0, r=0,g=0,b=0,a=0;

            for(int k = 0; k < 20; ++k)
                for(int l = 0; l < 20; ++l){

                    if (i+k < img.width() && j+l <img.height()) {

                        QColor color(img.pixel(i+k,j+l));

                        r += color.red();
                        g += color.green();
                        b += color.blue();
                        a += color.alpha();

                        // count the iteration to get the total pixel of the cube
                        count++;
                    }
                }

            r /= count; g /= count; b /= count; a /= count;

            QRgb meanColor = qRgba(r,g,b,a);

            for(int k = 0; k < 20; ++k)
                for(int l = 0; l < 20; ++l)

                    if (i+k < img.width() && j+l <img.height())
                        img.setPixel(i+k, j+l, meanColor);


        }

    image = QPixmap::fromImage(img);
    DisplayImage(image);
}
