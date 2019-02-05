#include <iostream>
#include <fstream>

#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
  std::ifstream file("/vagrant/data/msg.txt");
  if(!file.is_open()) {
    std::cerr << "ERROR: Can't open message text file." << std::endl;
    std::exit(1);
  }

  cv::Mat img(cv::imread("/vagrant/data/640x480.png"));
	if(img.empty()) {
		std::cerr << "Image Error" << std::endl;
		std::exit(1);
	}

  if(img.isContinuous()) {
    std::cout << "image is continuous, matrix size: " << img.total() * img.elemSize() << std::endl;
  }


  for(int row = 0; row < img.rows; ++row) {
    auto ptr(img.ptr(row));
    for(int col = 0; col < img.cols; ++col) {
      // for(int color = 0; color < 3; ++color) {
        ptr[0] = std::stoi(argv[1]);
        ptr[1] = std::stoi(argv[2]);
        ptr[2] = std::stoi(argv[3]);
        // ptr += 3;
      // }
    }
  }

  cv::imwrite("/vagrant/data/img_out.png", img);
}