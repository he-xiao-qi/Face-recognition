#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <vector>
#include <opencv2/opencv.hpp>

class FaceDetector {
public:
    FaceDetector(const std::string& shape_predictor_path);
    std::vector<dlib::rectangle> detect(const cv::Mat& frame);
    std::vector<dlib::full_object_detection> detect_landmarks(const cv::Mat& frame);
private:
    dlib::frontal_face_detector detector;
    dlib::shape_predictor sp;
};

#endif // FACEDETECTOR_H 