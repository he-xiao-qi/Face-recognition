# Qt+OpenCV+dlib 人脸识别系统

## 项目简介

本项目基于 C++、Qt6、OpenCV 和 dlib 实现了一个完整的人脸识别系统，支持摄像头实时采集、图片文件录入与识别、人脸检测、特征提取、特征录入、识别、数据库管理等功能。

## 功能特性

- 实时摄像头人脸检测与识别
- 支持本地图片文件的人脸录入与识别
- 人脸特征点检测与对齐
- 人脸特征向量提取与比对
- 人脸数据库管理（录入、保存、加载）
- 支持多用户、多样本录入
- 友好的Qt图形界面

## 环境依赖

- Windows 10/11
- Qt 6.9.1（建议MSVC 2022编译器）
- OpenCV 4.x
- dlib（通过vcpkg安装，需支持DNN模块）
- vcpkg（推荐包管理）
- CMake 3.16及以上

### 依赖安装建议

- Qt: [官网下载](https://download.qt.io/official_releases/qt/6.9/6.9.1/)
- OpenCV: [官网下载](https://opencv.org/releases/) 或 vcpkg安装
- dlib: `vcpkg install dlib[fftw3,opencv]`
- vcpkg: [vcpkg官网](https://github.com/microsoft/vcpkg)

## 编译与运行

1. **准备模型文件**

   - 下载 [shape_predictor_68_face_landmarks.dat](http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2)
   - 下载 [dlib_face_recognition_resnet_model_v1.dat](http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2)
   - 解压后放入 `models/` 目录

2. **配置依赖环境变量**

   - 确保 Qt、OpenCV、dlib 的 include/lib/bin 路径已配置到系统环境变量

3. **CMake编译**

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/vcpkg-master/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

4. **运行程序**

   ```bash
   cd Release
   FaceRecognition.exe
   ```

5. **注意事项**

   - 若运行目录下无 `models/` 文件夹，请将其复制到可执行文件同级目录。
   - 若遇到 DLL 缺失，请将 Qt、OpenCV、dlib 的 bin 目录加入 PATH。

## 主要目录结构

```
face/
├── CameraManager.cpp/h      # 摄像头管理模块
├── FaceDatabase.cpp/h       # 人脸数据库管理
├── FaceDetector.cpp/h       # 人脸检测与特征点
├── FaceRecognizer.cpp/h     # 人脸特征提取与比对
├── mainwindow.cpp/h         # Qt主界面
├── main.cpp                 # 应用入口
├── models/                  # 存放dlib模型文件
├── build/                   # 编译输出
├── CMakeLists.txt           # CMake配置
└── README.md                # 项目说明
```

## 常见问题与FAQ

- **Q: 编译报错"找不到dlib/fftw3/opencv"**
  - A: 请用vcpkg安装依赖，并用toolchain集成。
- **Q: 运行时报模型文件找不到**
  - A: 请将models目录复制到可执行文件同级目录。
- **Q: 识别效果不佳**
  - A: 建议每人多录入几张不同角度照片，调整比对阈值（如0.5~0.6）。
- **Q: 运行时报DLL缺失**
  - A: 检查Qt/OpenCV/dlib的bin目录是否加入PATH。

## 联系与支持

如有问题或建议，欢迎提issue或联系作者。 
