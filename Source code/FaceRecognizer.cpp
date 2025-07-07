#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer(const std::string& model_path) {
    dlib::deserialize(model_path) >> net;
}

FaceRecognizer::~FaceRecognizer() {}

bool FaceRecognizer::loadCascade(const std::string &cascadePath) {
    return faceCascade.load(cascadePath);
}

std::vector<cv::Rect> FaceRecognizer::detectFaces(const cv::Mat &frame) {
    std::vector<cv::Rect> faces;
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(80, 80));
    return faces;
}

//void FaceRecognizer::train(const std::vector<cv::Mat> &images, const std::vector<int> &labels) {
//    recognizer->train(images, labels);
//}

//int FaceRecognizer::predict(const cv::Mat &face) {
//    int label = -1;
//    double confidence = 0.0;
//    recognizer->predict(face, label, confidence);
//    return label;
//}

//void FaceRecognizer::saveModel(const std::string &modelPath) {
//    recognizer->save(modelPath);
//}

//void FaceRecognizer::loadModel(const std::string &modelPath) {
//    recognizer->read(modelPath);
//}

std::vector<float> FaceRecognizer::get_face_descriptor(const cv::Mat& face, const dlib::full_object_detection& shape) {
    dlib::matrix<dlib::rgb_pixel> face_chip;
    dlib::cv_image<dlib::bgr_pixel> img(face);
    dlib::extract_image_chip(img, dlib::get_face_chip_details(shape, 150, 0.25), face_chip);
    std::vector<dlib::matrix<dlib::rgb_pixel>> faces = {face_chip};
    std::vector<dlib::matrix<float,0,1>> descriptors = net(faces);
    std::vector<float> desc(descriptors[0].begin(), descriptors[0].end());
    return desc;
}

double FaceRecognizer::compare(const std::vector<float>& desc1, const std::vector<float>& desc2) {
    double sum = 0.0;
    for (size_t i = 0; i < desc1.size(); ++i) {
        double diff = desc1[i] - desc2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
} 