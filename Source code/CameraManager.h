#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <opencv2/opencv.hpp>

class CameraManager {
public:
    CameraManager();
    ~CameraManager();
    bool open(int deviceID = 0);
    void close();
    bool isOpened() const;
    bool read(cv::Mat &frame);
private:
    cv::VideoCapture cap;
};

#endif // CAMERAMANAGER_H 