#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsScene> //(this class provides a surface for managing
//2D graphical items. used with graphic view for viewing.)
#include <QBuffer> //Provides a data store for raw data to later be used as vertices or uniforms.
#include "iostream"//Header that defines the standard input/output stream objects
#include<QDir>
using namespace std;//This includes stuff like cout, cin, string, vector, map, etc.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->pushButton_2->setEnabled(false); // at first all buttons are disabled except load.
    ui->pushButton_3->setEnabled(false);// at first all buttons are disabled except load.
    ui->btnpixelize->setEnabled(false);// at first all buttons are disabled except load.


    originalImage = new QImage; //constructors
    displayedImage = new QImage;
    LowResImage = new QImage;
    Result = new QImage;

}

MainWindow::~MainWindow()
{
    delete originalImage;// distructors
    delete displayedImage;
    delete LowResImage;
    delete Result;

    delete ui;
}

void MainWindow::on_pushButton_clicked()// load button
{


    QString imgpath=getImagePath(); // here we provide the path of the
    if (!imgpath.isNull()){
        originalImage->load(imgpath); // varible img set is equal to the QImage
        ui->pushButton_3->setEnabled(true); // enabeling our display button
    }
    //ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);
    //image.save(saveImage(), "PNG");
}


QString MainWindow::getImagePath(){
    return QFileDialog::getOpenFileName(this,
                                        "Open a file",
                                        QString(),
                                        "Images (*.png *.jpg *.jpeg)");
}


void MainWindow::DisplayImage(QImage *img){ // display button
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*img)); // this
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(img->rect(),Qt::KeepAspectRatio);// resize image to fit graphics view
    ui->pushButton_2->setEnabled(true);//enabeling save button
    ui->btnpixelize->setEnabled(true);// enable pixelize button
    displayedImage = img;// to keep trak of the latest displayed image.
}

void MainWindow::on_pushButton_3_clicked()// display button
{
    DisplayImage(originalImage);//calling display function
}

void MainWindow::on_pushButton_2_clicked()// save button
{
    QString savename = QFileDialog::getSaveFileName(this,// open dialogue to save image
                                                    tr("Save the image"),
                                                    "",
                                                    tr("Images (*.png)"));
    displayedImage->save(savename, "png");//save any displayed img
}

void MainWindow::on_btnpixelize_clicked()// pixelize
{
    //lowering he resolution
    LowResImage = new QImage(originalImage->scaled(SizeOfImage,SizeOfImage,Qt::KeepAspectRatio));
    DisplayImage(LowResImage);// display
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)// to set pixel size.
{
    SizeOfImage = arg1;// to set pixel size.
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    SmallImageSize=arg1;// to set pixel size for data base images.
}

void MainWindow::on_pushButton_4_clicked()
{
    // source :http://stackoverflow.com/questions/36005814/load-images-from-folder-with-qt
    // StackOverflow, User : Apin, Date : Mar 15 2016
    QString datapath = QFileDialog::getExistingDirectory(this, tr("Select DB folder"));
    QDir dir(datapath);
    QStringList filter;
    filter << QLatin1String("*.png");//filter to select .png image
    filter << QLatin1String("*.jpeg");//filter to select .jpeg image
    filter << QLatin1String("*.jpg");//filter to select .pjg image
    dir.setNameFilters(filter);
    QFileInfoList filelistinfo = dir.entryInfoList();//list of the filtered images


    // END OF SOURCE



    foreach (const QFileInfo& fileinfo, filelistinfo) {

        // Get file name
        QString itemroot = fileinfo.absoluteFilePath();//filepath

        QImage tmp(itemroot);

        //  pixelized version
        QImage *img = new QImage(tmp.scaled(SmallImageSize,SmallImageSize,Qt::KeepAspectRatio));

        // add data to vector
        lowImage.push_back(img);
    }
}

QColor* MainWindow::Average(QImage *img){
    // Computes the mean value of an Image{
    // determine image size
    int height = img->height();
    int width = img->width();

    // init counters in RGB
    int AllR = 0;
    int AllG = 0;
    int AllB = 0;
    // Goign through the image
    for ( int a = 0; a < width; a++){
        for ( int b = 0; b < height; b++){

            // get the current pixel value
            QColor presentpixel( img->pixel( a, b));

            // add the corresponding channels
                           AllR += presentpixel.red();
                           AllG += presentpixel.green();
                           AllB += presentpixel.blue();
                       }
                   }
                   // '/' to get the mean.
                   AllR /= height * width;
                   AllG /= height * width;
                   AllB /= height * width;

                   // creating the resulting color as a qRGB
                   return new QColor( AllR, AllG, AllB);

}

QImage* MainWindow::LeastDifference(const QColor& pixcolor){

    // Try to find the best match in the DataBase given a color
   // Base on the mean absolute difference otpimisation fucntion
 // Counters for RGB and Total
       int divergentR, divergentG, divergentB, divergentT;

       // Base score, 3 channels, max 255/channel
       int Goal = 255 * 3;

       // latest result
       QImage* latestbest;

       // Parse the vector
       for (std::vector<QImage*>::iterator iter = lowImage.begin(); iter != lowImage.end(); iter++) {

           // Get current item
           QImage* readyImg = *iter;//current img
           QColor *readyMean = Average(readyImg);

           // Compute the absolute mean difference
           divergentR = abs(pixcolor.red() -  readyMean->red());
           divergentG = abs(pixcolor.green() - readyMean->green());
           divergentB = abs(pixcolor.blue() -  readyMean->blue());

           // Total mean absolute difference
           divergentT = divergentR + divergentG + divergentB;

           // Check if new latest result
           if (divergentT < Goal) {
               latestbest = readyImg;

               // Update latest goal
               Goal = divergentT;
           }
       }
       return latestbest;
}

void MainWindow:: Artit(){

    {
        // Replace evry pixel in baseImg->lowImage by an image in DB which is the
        // Retreive the image
        // get Width and Height
        int h = LowResImage->height();
        int w = LowResImage->width();

        // best result for a given pixel initialization
        QImage* bestResult ;

        // Prepare the result image with good size
        Result = new QImage( w * SmallImageSize, h * SmallImageSize, QImage::Format_RGB32);

        // for every pixel in the lowImage
        for ( int i = 0; i < w; i++){
            for ( int j = 0; j < h; j++){

                // Get current pixel
                QColor currentpixel(LowResImage->pixel( i,j));

                // Get the best result in the DataBase
                bestResult = LeastDifference(currentpixel);

                // Get pixelized version

                // For every pixel in the ebst match low resolution
                for (int k = 0; k< bestResult->width();k++){
                    for (int l = 0; l < bestResult->height(); l++){

                        // Get current pixel
                        QColor currentMatch =bestResult->pixel(k,l);

                        // Place on res at the good position
                        Result->setPixel(i * SmallImageSize + k, j * SmallImageSize + l, currentMatch.rgb());
                    }
                }
            }
        }
    }


}



void MainWindow::on_pushButton_5_clicked()
{
     Artit();
     DisplayImage(Result);
}
