#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 设置窗口标题和大小
    setWindowTitle("人脸识别系统");
    resize(700, 450);
    
    // 设置窗口背景渐变色
    this->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f0f4ff, stop:1 #e0eaff);");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 顶部大号彩色标题
    QLabel *titleLabel = new QLabel("人脸识别系统", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2d5be3; margin-top: 20px; margin-bottom: 10px;");

    videoLabel = new QLabel("摄像头画面", this);
    videoLabel->setFixedSize(400, 300);
    videoLabel->setStyleSheet("background: #fff; border: 2px solid #2d5be3; border-radius: 10px; font-size: 16px; color: #888;");

    resultLabel = new QLabel("识别结果", this);
    resultLabel->setStyleSheet("font-size: 20px; color: #e36c2d; font-weight: bold; margin: 10px 0;");

    startButton = new QPushButton("打开摄像头", this);
    stopButton = new QPushButton("关闭摄像头", this);
    captureButton = new QPushButton("录入人脸", this);
    recognizeButton = new QPushButton("识别人脸", this);
    addImageButton = new QPushButton("图片录入", this);
    recognizeImageButton = new QPushButton("图片识别", this);
    deletePersonButton = new QPushButton("删除人像", this);

    // 按钮美化样式
    QString btnStyle = "QPushButton {"
                      "background-color: #2d5be3;"
                      "color: white;"
                      "font-size: 18px;"
                      "border-radius: 8px;"
                      "padding: 8px 0;"
                      "margin: 6px 0;"
                      "}"
                      "QPushButton:hover {"
                      "background-color: #4a77e3;"
                      "}";
    startButton->setStyleSheet(btnStyle);
    stopButton->setStyleSheet(btnStyle);
    captureButton->setStyleSheet(btnStyle);
    recognizeButton->setStyleSheet(btnStyle);
    addImageButton->setStyleSheet(btnStyle);
    recognizeImageButton->setStyleSheet(btnStyle);
    deletePersonButton->setStyleSheet(btnStyle);

    // 左侧布局：摄像头画面+识别结果
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(videoLabel);
    leftLayout->addWidget(resultLabel);
    leftLayout->addStretch();

    // 右侧布局：所有按钮
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(startButton);
    rightLayout->addWidget(stopButton);
    rightLayout->addWidget(captureButton);
    rightLayout->addWidget(recognizeButton);
    rightLayout->addWidget(addImageButton);
    rightLayout->addWidget(recognizeImageButton);
    rightLayout->addWidget(deletePersonButton);
    rightLayout->addStretch();

    // 主体左右布局
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    mainHLayout->addLayout(leftLayout, 2); // 左侧占2份
    mainHLayout->addLayout(rightLayout, 1); // 右侧占1份

    // 总体竖直布局，顶部为标题
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(titleLabel);
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->setSpacing(12);
    mainVLayout->setContentsMargins(30, 20, 30, 20);

    centralWidget->setLayout(mainVLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartCamera);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStopCamera);
    connect(captureButton, &QPushButton::clicked, this, &MainWindow::onCaptureFace);
    connect(recognizeButton, &QPushButton::clicked, this, &MainWindow::onRecognizeFace);
    connect(addImageButton, &QPushButton::clicked, this, &MainWindow::onAddImage);
    connect(recognizeImageButton, &QPushButton::clicked, this, &MainWindow::onRecognizeImage);
    connect(deletePersonButton, &QPushButton::clicked, this, &MainWindow::onDeletePerson);

    // 路径初始化
    shape_predictor_path = "models/shape_predictor_68_face_landmarks.dat";
    recognition_model_path = "models/dlib_face_recognition_resnet_model_v1.dat";
    database_path = "models/face_db.txt";
    try {
        faceDetector = new FaceDetector(shape_predictor_path);
        faceRecognizer = new FaceRecognizer(recognition_model_path);
        faceDatabase = new FaceDatabase();
        faceDatabase->load(database_path);
        QMessageBox::information(this, "初始化", "程序初始化成功！");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "初始化错误", QString("初始化失败: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "初始化错误", "初始化失败: 未知错误");
    }
}

MainWindow::~MainWindow() {
    if (cap.isOpened()) cap.release();
    faceDatabase->save(database_path);
    delete faceDetector;
    delete faceRecognizer;
    delete faceDatabase;
}

void MainWindow::onStartCamera() {
    if (!cap.isOpened()) {
        cap.open(0);
        if (!cap.isOpened()) {
            QMessageBox::warning(this, "错误", "无法打开摄像头");
            return;
        }
    }
    timer->start(30);
}

void MainWindow::onStopCamera() {
    timer->stop();
    if (cap.isOpened()) cap.release();
    videoLabel->setText("摄像头画面");
}

void MainWindow::onCaptureFace() {
    if (currentFrame.empty()) return;
    auto shapes = faceDetector->detect_landmarks(currentFrame);
    if (shapes.empty()) {
        QMessageBox::information(this, "提示", "未检测到人脸");
        return;
    }
    bool ok;
    QString name = QInputDialog::getText(this, "录入人脸", "请输入姓名：", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    auto desc = faceRecognizer->get_face_descriptor(currentFrame, shapes[0]);
    faceDatabase->add(desc, name.toStdString());
    QMessageBox::information(this, "提示", "人脸录入成功");
}

void MainWindow::onRecognizeFace() {
    if (currentFrame.empty()) return;
    auto shapes = faceDetector->detect_landmarks(currentFrame);
    if (shapes.empty()) {
        QMessageBox::information(this, "提示", "未检测到人脸");
        return;
    }
    auto desc = faceRecognizer->get_face_descriptor(currentFrame, shapes[0]);
    auto db_descs = faceDatabase->get_descriptors();
    auto db_labels = faceDatabase->get_labels();

    // 数据库为空直接返回未知
    if (db_descs.empty() || db_labels.empty()) {
        resultLabel->setText("识别结果：未知");
        return;
    }

    double min_dist = 1e9;
    int min_idx = -1;
    for (size_t i = 0; i < db_descs.size(); ++i) {
        double dist = faceRecognizer->compare(desc, db_descs[i]);
        qDebug() << "与" << QString::fromStdString(db_labels[i]) << "距离:" << dist;
        if (dist < min_dist) {
            min_dist = dist;
            min_idx = i;
        }
    }
    // 阈值可根据实际情况调整
    double threshold = 0.4;
    if (min_idx != -1 && min_dist < threshold) {
        resultLabel->setText(QString("识别结果：%1 (距离：%2)").arg(QString::fromStdString(db_labels[min_idx])).arg(QString::number(min_dist, 'f', 4)));
    } else {
        resultLabel->setText("识别结果：未知");
    }
}

// 检查字符串是否包含中文字符
static bool hasChinese(const QString& str) {
    for (const QChar& ch : str) {
        if (ch.unicode() >= 0x4E00 && ch.unicode() <= 0x9FFF) {
            return true;
        }
    }
    return false;
}

void MainWindow::onAddImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (fileName.isEmpty()) return;
    // 检查路径是否包含中文
    if (hasChinese(fileName)) {
        QMessageBox::warning(this, "路径错误", "图片路径包含中文，请将图片移动到全英文路径后再试！");
        return;
    }
    qDebug() << "选择的图片路径:" << fileName;
    cv::Mat img = cv::imread(fileName.toStdString());
    if (img.empty()) {
        qDebug() << "cv::imread 加载失败，尝试用 QImage 加载";
        QImage qimg(fileName);
        if (qimg.isNull()) {
            QMessageBox::warning(this, "错误", QString("无法加载图片: %1").arg(fileName));
            return;
        }
        img = cv::Mat(qimg.height(), qimg.width(), CV_8UC4, (void*)qimg.bits(), qimg.bytesPerLine()).clone();
        cv::cvtColor(img, img, cv::COLOR_BGRA2BGR);
        if (img.empty()) {
            QMessageBox::warning(this, "错误", QString("QImage转换cv::Mat失败: %1").arg(fileName));
            return;
        }
    }
    auto shapes = faceDetector->detect_landmarks(img);
    if (shapes.empty()) {
        QMessageBox::information(this, "提示", "未检测到人脸");
        return;
    }
    bool ok;
    QString name = QInputDialog::getText(this, "录入人脸", "请输入姓名：", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    auto desc = faceRecognizer->get_face_descriptor(img, shapes[0]);
    faceDatabase->add(desc, name.toStdString());
    QMessageBox::information(this, "提示", "图片人脸录入成功");
}

void MainWindow::onRecognizeImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (fileName.isEmpty()) return;
    // 检查路径是否包含中文
    if (hasChinese(fileName)) {
        QMessageBox::warning(this, "路径错误", "图片路径包含中文，请将图片移动到全英文路径后再试！");
        return;
    }
    qDebug() << "选择的图片路径:" << fileName;
    cv::Mat img = cv::imread(fileName.toStdString());
    if (img.empty()) {
        qDebug() << "cv::imread 加载失败，尝试用 QImage 加载";
        QImage qimg(fileName);
        if (qimg.isNull()) {
            QMessageBox::warning(this, "错误", QString("无法加载图片: %1").arg(fileName));
            return;
        }
        img = cv::Mat(qimg.height(), qimg.width(), CV_8UC4, (void*)qimg.bits(), qimg.bytesPerLine()).clone();
        cv::cvtColor(img, img, cv::COLOR_BGRA2BGR);
        if (img.empty()) {
            QMessageBox::warning(this, "错误", QString("QImage转换cv::Mat失败: %1").arg(fileName));
            return;
        }
    }
    auto shapes = faceDetector->detect_landmarks(img);
    if (shapes.empty()) {
        QMessageBox::information(this, "提示", "未检测到人脸");
        return;
    }
    auto desc = faceRecognizer->get_face_descriptor(img, shapes[0]);
    auto db_descs = faceDatabase->get_descriptors();
    auto db_labels = faceDatabase->get_labels();

    // 数据库为空直接返回未知
    if (db_descs.empty() || db_labels.empty()) {
        QMessageBox::information(this, "识别结果", "识别结果：未知");
        return;
    }

    double min_dist = 1e9;
    int min_idx = -1;
    for (size_t i = 0; i < db_descs.size(); ++i) {
        double dist = faceRecognizer->compare(desc, db_descs[i]);
        qDebug() << "与" << QString::fromStdString(db_labels[i]) << "距离:" << dist;
        if (dist < min_dist) {
            min_dist = dist;
            min_idx = i;
        }
    }
    double threshold = 0.3;
    if (min_idx != -1 && min_dist < threshold) {
        QMessageBox::information(this, "识别结果", QString("识别结果：%1 (距离：%2)").arg(QString::fromStdString(db_labels[min_idx])).arg(QString::number(min_dist, 'f', 4)));
    } else {
        QMessageBox::information(this, "识别结果", "识别结果：未知");
    }
}

void MainWindow::onDeletePerson() {
    auto db_labels = faceDatabase->get_labels();
    if (db_labels.empty()) {
        QMessageBox::information(this, "提示", "数据库为空，无可删除人像！");
        return;
    }
    QStringList nameList;
    for (const auto& name : db_labels) {
        nameList << QString::fromStdString(name);
    }
    bool ok = false;
    QString selected = QInputDialog::getItem(this, "删除人像", "请选择要删除的人名：", nameList, 0, false, &ok);
    if (!ok || selected.isEmpty()) return;
    if (faceDatabase->remove(selected.toStdString())) {
        faceDatabase->save(database_path);
        QMessageBox::information(this, "提示", QString("已删除：%1").arg(selected));
    } else {
        QMessageBox::warning(this, "错误", "删除失败！");
    }
}

void MainWindow::updateFrame() {
    if (cap.isOpened()) {
        cap >> currentFrame;
        if (!currentFrame.empty()) {
            displayImage(currentFrame);
        }
    }
}

void MainWindow::displayImage(const cv::Mat &frame) {
    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
    QImage img((const uchar*)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img).scaled(videoLabel->size(), Qt::KeepAspectRatio));
} 