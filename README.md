#创建 PCL + OpenCV + VTK Demo Project

这是一个基于 C++ 和 VS Code 的点云处理与可视化项目，使用 **PCL (Point Cloud Library)** 进行点云操作，**OpenCV** 处理图像，**VTK** 实现 3D 可视化。

> 💡 本项目为学习和演示目的设计，支持 Windows 平台下的编译与调试。

---

## 🧩 功能简介

- ✅ 使用 PCL 加载并显示点云数据（如 `.pcd` 文件）
- ✅ 调用 OpenCV 进行基础图像处理
- ✅ 利用 VTK 创建交互式 3D 可视化窗口
- ✅ 支持 Debug / Release 模式构建
- ✅ 提供调试宏控制输出信息

---

## 🛠️ 技术栈

| 组件 | 版本 |
|------|------|
| C++ | C++17 |
| CMake | v3.16+ |
| PCL | 1.15.1 |
| OpenCV | 4.9.0 |
| VTK | 9.4 |
| 编译器 | MSVC (Visual Studio) |
| IDE | VS Code + CMake Tools |

---
## 📁 工程目录结构
- `PROJECT/`
  - `bin/` —— 生成的可执行文件 (.exe)
  - `build/` —— CMake 构建中间文件
  - `data/` —— 示例数据（如 `point_cloud.pcd`）
  - `src/` —— 源代码文件
    - `main.cpp`
    - `...`
  - `CMakeLists.txt` —— CMake 配置文件
  - `README.md` —— 本说明文档

  ---
