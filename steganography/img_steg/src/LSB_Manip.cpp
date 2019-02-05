/****************************************************************
 * 
 * Description:
 * 
 * Resource:
 * 
 ****************************************************************/

#include <iostream>
#include <string>

#include <opencv2/highgui.hpp>

#include "steganography/LSB_Manip.h"
#include "steganography/Exception.h"

namespace Steganography {

/**
 * Constructor reads an input image into a cv::Mat buffer
 */ 
LSB_Manip::LSB_Manip(const std::string& img_file): _img(cv::imread(img_file)), _cap((_img.total() * _img.elemSize()) / 8) {
  if(_img.empty()) {
    throw Exception("ERROR: Can't read input image file.");
  }
}

/**
 * Description: encode text messages into png image using best effort strategy.
 * 
 * @return number of bytes encoded into image
 */
int LSB_Manip::encode(std::istream& is) {
  char c(is.get()); // temp char buffer to work on
  int bit_idx(7); // read head to track each bits in a char
  int cnt(0); // number of chars encoded

  /**
   * Each character is represented by a byte or 8 bits. To hide the text into the
   * image we take one character and each of the 8 bits are stored in the least
   * significant bits of the pixel values (blue, green, red).
   */ 
  for(int row = 0; row < _img.rows; ++row) {
    auto row_ptr(_img.ptr(row));
    for(int col = 0; col < _img.cols; ++col) {
      for(int color = 0; color < 3; ++color) {
        if(is.eof() || cnt == _cap - 1 || !((c >> bit_idx) & 1)) {
          row_ptr[color] &= ~1; // if bit is 0 : change LSB of present color value to 0.
        } else {
          row_ptr[color] |= 1; // if bit is 1 : change LSB of present color value to 1.
        }

        if((cnt == _cap - 1 || is.eof()) && bit_idx == 0) { // if eof and bit_idx is 0, then our message is successfully encoded.
          return cnt;
        }

        if(bit_idx-- == 0) { // advance the read head to work on the next bit
          bit_idx = 7; // reset read head if all bits are checked, and get the next char to work on
          ++cnt; // increment chars count
          is.get(c); // fetch the next char from file
        }
      }
      row_ptr += 3; // advance row read head position
    }
  }

  throw Exception("ERROR: Encode failed.");
}

/**
 * Description:
 * 
 * @return number of bytes decoded from image
 */
int LSB_Manip::decode() {
  _str.resize(_cap, 0); // resize _str vector to _img max chars capacity and fill it with zeros
  int bit_idx(7); // read head to track bits in a char
  int buf_idx(0); // write head for char vector
  int cnt(0); // track number of chars written to _str to resize it later

  for(int row = 0; row < _img.rows; ++row) {
    auto row_ptr(_img.ptr(row));
    for(int col = 0; col < _img.cols; ++col) {
      for(int color = 0; color < 3; ++color) {
        if(row_ptr[color] & 1) {
          _str[buf_idx] |= 1;
        }

        if(bit_idx-- == 0) {
          bit_idx = 7; // reset the bits read head
          ++cnt;
          if(_str[buf_idx++] == '\0') {
            _str.resize(cnt); // resize _str vector to length of message decoded from _img
            return cnt;
          }
        } else {
          _str[buf_idx] <<= 1;
        }
      }
      row_ptr += 3; // advance row read head position
    }
  }
}

void LSB_Manip::strip() {
  std::cout << "Stripping least significant bit from image file." << std::endl;
}

void LSB_Manip::write_img(const std::string& out_img) {
  if(out_img.empty()) {
    throw Exception("ERROR: Output image file not specified");
  }
  
  cv::imwrite(out_img, _img);
}

std::ostream& operator<<(std::ostream& os, const LSB_Manip& lm) {
  return os << lm._str.data();
}


} // namespace Steganography 