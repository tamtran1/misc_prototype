/****************************************************************
 * 
 * Description:
 * 
 ****************************************************************/

#ifndef LSB_Manip_H
#define LSB_Manip_H

#include <string>
#include <fstream>
#include <vector>

#include <opencv2/highgui.hpp>

namespace Steganography {


/**
 * Description: use to pass temp data between _mtrx_wkr and lambda function
 */ 
struct Mtrx_Wkr_Data {
  unsigned char* row; // pointer to the current row of pixels being work on
  int bit_idx; // read head to track each bits in a char
  int nbytes; // number of bytes processed so far
};

class LSB_Manip {
public:
  LSB_Manip(const std::string& img_file);
  int encode(std::istream& is);
  int decode();
  int strip();
  void write_img(const std::string& out_img);
  friend std::ostream& operator<<(std::ostream& os, const LSB_Manip& lm);

private:
  cv::Mat _img;
  int _cap; // capacity of chars an image can hold
  std::vector<char> _str{};
  
  int _mtrx_wkr(std::function<bool(Mtrx_Wkr_Data&, int)> fn);
};


} // namespace Steganography

#endif // LSB_Manip_H