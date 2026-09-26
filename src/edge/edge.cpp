#include "edge.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <iostream>

namespace {

// 图像路径：可执行文件输出在 bin/ 下运行，所以相对项目根目录
const std::string IMG_PATH = "../data/17.jpg";

cv::Mat g_gray;    // 灰度图（预处理后）
cv::Mat g_edges;   // 边缘检测结果
int g_high_thr = 100; // Canny 高阈值（滑动条调节，0~500）

// 滑动条回调：阈值变化时重新计算边缘
void on_threshold(int, void*)
{
    // Canny 惯例：低阈值 = 0.5 * 高阈值
    int low  = std::max(1, g_high_thr / 2);
    int high = std::max(g_high_thr, low + 1);
    cv::Canny(g_gray, g_edges, low, high);
    cv::imshow("Canny Edges", g_edges);
}

} // namespace

void edge_demo()
{
    // 1. 读取彩色原图
    cv::Mat image = cv::imread(IMG_PATH, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: Could not load image from path: " << IMG_PATH << std::endl;
        return;
    }

    // 2. 转为灰度图（Canny 只处理单通道）
    cv::cvtColor(image, g_gray, cv::COLOR_BGR2GRAY);

    // 3. 高斯模糊降噪（抑制噪声产生的虚假边缘）
    cv::GaussianBlur(g_gray, g_gray, cv::Size(3, 3), 0);

    // 4. 创建显示窗口 + 阈值滑动条
    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original", image);

    cv::namedWindow("Canny Edges", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("High Threshold", "Canny Edges", &g_high_thr, 500, on_threshold);

    // 5. 初始计算并显示一次
    on_threshold(0, nullptr);

    // 6. 等待按键退出
    cv::waitKey(0);
    cv::destroyAllWindows();
}
