/****************************************************************
 * 
 * Description:
 * 
 * Resource:
 * 
 ****************************************************************/

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/highgui.hpp>

#include "steganography/LSB_Manip.h"
#include "steganography/Exception.h"

namespace Steganography {

/**
 * Constructor reads an input image into a cv::Mat buffer
 */ 
LSB_Manip::LSB_Manip(const std::string& img_file) : _buf(cv::imread(img_file)) {
  if(_buf.empty()) {
    throw Exception("ERROR: Can't read input image file.");
  }
}

void LSB_Manip::encode(const std::string& txt_file) {
  std::ifstream file(txt_file); // text file with message to encode
  if(!file.is_open()) {
    throw Exception("ERROR: Can't open message text file.");
  }

  char c(0); // temp char buffer to work on
  file.get(c); // read the first char from the file
  int bit_idx(7); // read head to track each bits in a char

  /**
   * Each character is represented by a byte or 8 bits. To hide the text into the
   * image we take one character and each of the 8 bits are stored in the least
   * significant bits of the pixel values (red, green, blue).
   */ 
  for(int row = 0; row < _buf.rows; ++row) {
    for(int col = 0; col < _buf.cols; ++col) {
      for(int color = 0; color < 3; ++color) {        
        if(file.eof() || !((c >> bit_idx) & 1)) {
          _buf.at<cv::Vec3b>(cv::Point(row, col)).val[color] &= ~1; // if bit is 0 : change LSB of present color value to 0.
        } else {
          _buf.at<cv::Vec3b>(cv::Point(row, col)).val[color] |= 1; // if bit is 1 : change LSB of present color value to 1.
        }

        if(file.eof() && bit_idx == 0) { // if eof and bit_idx is 0, then our message is successfully encoded.
          return;
        }

        if(bit_idx-- == 0) { // advance the read head to work on the next bit
          bit_idx = 7; // reset read head if all bits are checked, and get the next char to work on
          file.get(c); // fetch the next char from file
        }
      }
    }
  }
  // TODO: perhaps try best effort strategy and report number of bytes encoded.
  throw Exception("ERROR: Encode failed. Insufficient image size to fit text message.");
}

std::string LSB_Manip::decode() {
  std::vector<char>c_buf(_buf.rows * _buf.cols, 0); // buffer to store all the decoded chars
  int bit_idx(7); // read head to track each bits in a char
  int buf_idx(0); // write head for 

  for(int row = 0; row < _buf.rows; ++row) {
    for(int col = 0; col < _buf.cols; ++col) {
      for(int color = 0; color < 3; ++color) {
        if(_buf.at<cv::Vec3b>(cv::Point(row, col)).val[color] & 1) {
          c_buf[buf_idx] |= 1;
        }

        if(bit_idx-- == 0) {
          bit_idx = 7; // reset the bit read head
          if(c_buf[buf_idx++] == '\0') {
            return std::string(c_buf.data());
          }
        } else {
          c_buf[buf_idx] <<= 1;
        }
      }
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
  
  cv::imwrite(out_img, _buf);
}


} // namespace Steganography 