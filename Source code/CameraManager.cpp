#include "CameraManager.h"

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {
    close();
}

bool CameraManager::open(int deviceID) {
    if (!cap.isOpened()) {
        return cap.open(deviceID);
    }
    return true;
}

void CameraManager::close() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraManager::isOpened() const {
    return cap.isOpened();
}

bool CameraManager::read(cv::Mat &frame) {
    if (cap.isOpened()) {
        return cap.read(frame);
    }
    return false;
} 