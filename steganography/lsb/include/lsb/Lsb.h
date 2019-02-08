/****************************************************************
 * 
 * Description:
 * 
 ****************************************************************/

#ifndef LSB_H
#define LSB_H

#include <string>
#include <fstream>
#include <vector>

#include <opencv2/highgui.hpp>

namespace Lsb {


/**
 * Description: use to pass temp data between _mtrx_wkr and lambda function
 */ 
struct Mtrx_Wkr_Data {
  unsigned char* row; // pointer to the current row of pixels being work on
  int bit_idx; // read head to track each bits in a char
  int nbytes; // number of bytes processed so far
};

class Lsb {
public:
  Lsb(const std::string& img_file);
  int encode(std::istream& is);
  int decode();
  int strip();
  void write_img(const std::string& out_img);
  friend std::ostream& operator<<(std::ostream& os, const Lsb& lm);

private:
  cv::Mat _img;
  int _cap; // capacity of chars an image can hold
  std::vector<char> _str{};
  
  int _mtrx_wkr(std::function<bool(Mtrx_Wkr_Data&, int)> fn);
};


} // namespace Lsb

#endif // LSB_H