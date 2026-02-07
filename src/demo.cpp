#include <iostream>
#include "sum.h"
#include "diff.h"
#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>     // 可选：用于保存/加载点云
#include <random>

// 自定义演示函数
static void myfunc()
{
    // 两数求和
    int x = 10, y = 20;
    int sum = add(x, y);
    std::cout << "The sum of " << x << " and " << y << " is " << sum << std::endl;

    // 两数求差
    int dif = diff(y, x);
    std::cout << "The diff of " << y << " and " << x << " is " << dif << std::endl;
}

// opncv演示函数
static void opencv_demo()
{
    std::string imagePath = "../data/17.jpg"; // 替换为你的图片路径
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error: Could not load image from path: " << imagePath << std::endl;
        system("pause");
        return;
    }
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0); // 按任意键关闭窗口
    cv::destroyAllWindows();
}

// pcl演示函数
static void pcl_demo()
{
    std::cout << "Creating a random colored point cloud..." << std::endl;

    // 定义带颜色的点类型
    using PointT = pcl::PointXYZRGB;
    pcl::PointCloud<PointT>::Ptr cloud(new pcl::PointCloud<PointT>);

    // 设置点云大小
    cloud->width    = 1000;
    cloud->height   = 1;
    cloud->is_dense = false;
    cloud->points.resize(cloud->width * cloud->height);

    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_x(-2.0f, 2.0f);
    std::uniform_real_distribution<float> dis_y(-2.0f, 2.0f);
    std::uniform_real_distribution<float> dis_z(0.0f, 4.0f);
    std::uniform_int_distribution<int> dis_color(0, 255);

    // 填充点云数据
    for (auto& pt : cloud->points) {
        pt.x = dis_x(gen);
        pt.y = dis_y(gen);
        pt.z = dis_z(gen);
        pt.r = static_cast<uint8_t>(dis_color(gen));
        pt.g = static_cast<uint8_t>(dis_color(gen));
        pt.b = static_cast<uint8_t>(dis_color(gen));
    }
    std::cout << "Point cloud created with " << cloud->size() << " points." << std::endl;

    // ================== 可视化 ==================
    pcl::visualization::PCLVisualizer viewer("PCL Point Cloud Viewer");
    viewer.setBackgroundColor(0, 0, 0); // 黑色背景
    viewer.addPointCloud<PointT>(cloud, "sample cloud");
    viewer.setPointCloudRenderingProperties(
        pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud"
    );
    viewer.addCoordinateSystem(1.0); // 添加 XYZ 坐标轴
    viewer.initCameraParameters();

    std::cout << "Press 'q' or close window to exit." << std::endl;

    // 主循环
    while (!viewer.wasStopped()) {
        viewer.spinOnce(100); // 刷新一次，延迟 100ms
    }
}

int main()
{
    // 打印 "Hello, World!"
    std::string greeting = "Hello, World!";
    std::cout << greeting << std::endl;

    // 调用演示函数
    myfunc();// 实现自定义函数的调用

    // 调用 OpenCV 演示函数
    opencv_demo();

    // 调用 PCL 演示函数
    pcl_demo();

#ifdef MY_TSET_SHOW_PRINTF
    std::cout << "MY_TSET_SHOW_PRINT is defined." << std::endl;
#endif

    return 0;
}
