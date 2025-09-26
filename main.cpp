#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    // 打开默认摄像头
    cv::VideoCapture cap(0);

    // 检查摄像头是否成功打开
    if (!cap.isOpened()) {
        std::cerr << "错误：无法打开摄像头" << std::endl;
        return -1;
    }

    // 创建窗口
    namedWindow("二维码扫描器", cv::WINDOW_AUTOSIZE);

    // 创建二维码检测器
    cv::QRCodeDetector qrDetector;

    // 存储二维码数据的变量
    std::vector<cv::Point> points;
    std::string decodedInfo;

    // 设置扫描二维码时间间隔
    int frameCounter = 0;
    const int DETECT_INTERVAL = 10;
    bool qrDetectedInPrevFrame = false;
    auto lastDetectionTime = std::chrono::steady_clock::now();
    const int MIN_DETECT_INTERVAL_MS = 1000;

    std::cout << "摄像头已启动，按 ESC 键退出..." << std::endl;

    while (true) {
        cv::Mat frame;

        // 捕获帧
        cap >> frame;

        // 检查帧是否为空
        if (frame.empty()) {
            std::cerr << "错误：无法获取帧" << std::endl;
            break;
        }

        imshow("frame", frame);

        // 计算时间间隔
        auto currentTime = std::chrono::steady_clock::now();
        auto timeSinceLastDetection = duration_cast<std::chrono::milliseconds>(currentTime - lastDetectionTime).count();

        // 检测条件：超过最小时间间隔，或上一帧检测到二维码
        bool shouldDetect = (timeSinceLastDetection >= MIN_DETECT_INTERVAL_MS) || qrDetectedInPrevFrame;

        if (shouldDetect)
        {
            // 尝试检测并解码二维码
            decodedInfo = qrDetector.detectAndDecode(frame, points);

            // 如果检测到二维码
            if (!points.empty()) {
                // 绘制二维码边界框
                for (int i = 0; i < points.size(); i++) {
                    line(frame, points[i], points[(i + 1) % points.size()], cv::Scalar(0, 255, 0), 4);
                }

                // 显示解码信息
                putText(frame, "二维码已识别", cv::Point(20, 40),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

                // 在控制台输出解码信息
                if (!decodedInfo.empty()) {
                    std::cout << "解码信息: " << decodedInfo << std::endl;
                }
            }
        }

        // 检查按键（ESC 键退出）
        int key = cv::waitKey(1);
        if (key == 27) { // ESC
            break;
        } else if (key == 's' || key == 'S') { // 按S键保存当前帧
            static int count = 1;
            std::string filename = "frame_" + std::to_string(count++) + ".png";
            imwrite(filename, frame);
            std::cout << "已保存帧到: " << filename << std::endl;
        }
    }

    // 释放资源
    cap.release();
    cv::destroyAllWindows();

    return 0;
}