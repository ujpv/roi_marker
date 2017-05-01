#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace std;

cv::Mat preview;
cv::Mat image;

std::vector<cv::Point2d> points(4);
std::string out_filename;
double scale = 1;

void mouse_event(int event, int x, int y, int flags, void* userdata) {
    if (event != cv::EVENT_LBUTTONUP)
        return;

    static size_t current = 0;
    if (current > 3) {
        image.copyTo(preview);
        current = 0;
    }
    points[current] = cv::Point(x, y);

    if (current > 0) {
        cv::line(preview, points[current - 1], points[current], cv::Scalar(0, 0, 255));
    }

    if (current == 3) {
        cv::line(preview, points[3], points[current], cv::Scalar(0, 0, 255));
        std::ofstream out_file;
        out_file.open(out_filename, std::ofstream::out | std::ofstream::trunc);
        for (size_t i = 0; i < 4; ++i)
            out_file << int(points[i].x / scale) << ' ' << int(points[i].y / scale) << '\n';
    }

    ++current;
    cv::imshow("Image", preview);

    std::cout << "mouse pressed:" << x << ' ' << y << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "usage: roi_marker image_file_name [scale]\n";
        return 0;
    }

    std::string filename(argv[1]);
    image = cv::imread(filename);
    if (!image.data) {
        std::cout <<"No image data \n";
        return 0;
    }

    out_filename = filename + ".roi";

    cv::Size size = image.size();

    if (argc > 2) {
        scale = std::atof(argv[2]);
        cv::resize(image, image, cv::Size(size.width * scale, size.height * scale));
    }

    image.copyTo(preview);

    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", preview);
    cv::setMouseCallback("Image", mouse_event);

    std::cout << "Image loaded.\n";

    cv::waitKey(0);

    return 0;
}
