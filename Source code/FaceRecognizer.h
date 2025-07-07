#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H

#include <dlib/dnn.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/face.hpp>
#include <vector>
#include <string>

// dlib官方ResNet网络结构定义（兼容官方模型和MSVC）
template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N,BN,1,dlib::tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2<dlib::avg_pool<2,2,2,2,dlib::skip1<dlib::tag2<block<N,BN,2,dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET> 
using block  = BN<dlib::con<N,3,3,1,1,dlib::relu<BN<dlib::con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = dlib::relu<residual<block,N,dlib::affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = dlib::relu<residual_down<block,N,dlib::affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = dlib::loss_metric<dlib::fc_no_bias<128,dlib::avg_pool_everything<
                            alevel0<
                            alevel1<
                            alevel2<
                            alevel3<
                            alevel4<
                            dlib::max_pool<3,3,2,2,dlib::relu<dlib::affine<dlib::con<32,7,7,2,2,
                            dlib::input_rgb_image_sized<150>
                            >>>>>>>>>>>>;

class FaceRecognizer {
public:
    FaceRecognizer(const std::string& model_path);
    ~FaceRecognizer();
    bool loadCascade(const std::string &cascadePath);
    std::vector<cv::Rect> detectFaces(const cv::Mat &frame);
    //void train(const std::vector<cv::Mat> &images, const std::vector<int> &labels);
    //int predict(const cv::Mat &face);
    //void saveModel(const std::string &modelPath);
    //void loadModel(const std::string &modelPath);
    std::vector<float> get_face_descriptor(const cv::Mat& face, const dlib::full_object_detection& shape);
    double compare(const std::vector<float>& desc1, const std::vector<float>& desc2);
private:
    //cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer;
    cv::CascadeClassifier faceCascade;
    anet_type net;
};

#endif // FACERECOGNIZER_H 