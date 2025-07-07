#include "FaceDetector.h"

FaceDetector::FaceDetector(const std::string& shape_predictor_path) {
    detector = dlib::get_frontal_face_detector();
    dlib::deserialize(shape_predictor_path) >> sp;
}

std::vector<dlib::rectangle> FaceDetector::detect(const cv::Mat& frame) {
    dlib::cv_image<dlib::bgr_pixel> img(frame);
    return detector(img);
}

std::vector<dlib::full_object_detection> FaceDetector::detect_landmarks(const cv::Mat& frame) {
    std::vector<dlib::full_object_detection> shapes;
    dlib::cv_image<dlib::bgr_pixel> img(frame);
    auto faces = detector(img);
    for (auto& face : faces) {
        shapes.push_back(sp(img, face));
    }
    return shapes;
} 