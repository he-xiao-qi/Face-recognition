#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "FaceDetector.h"
#include "FaceRecognizer.h"
#include "FaceDatabase.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartCamera();
    void onStopCamera();
    void onCaptureFace();
    void onRecognizeFace();
    void updateFrame();
    void onAddImage();
    void onRecognizeImage();
    void onDeletePerson();

private:
    QLabel *videoLabel;
    QLabel *resultLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *captureButton;
    QPushButton *recognizeButton;
    QPushButton *addImageButton;
    QPushButton *recognizeImageButton;
    QPushButton *deletePersonButton;
    QVBoxLayout *mainLayout;
    QTimer *timer;
    cv::VideoCapture cap;
    cv::Mat currentFrame;
    FaceDetector *faceDetector;
    FaceRecognizer *faceRecognizer;
    FaceDatabase *faceDatabase;
    void displayImage(const cv::Mat &frame);
    std::string shape_predictor_path;
    std::string recognition_model_path;
    std::string database_path;
};

#endif // MAINWINDOW_H 