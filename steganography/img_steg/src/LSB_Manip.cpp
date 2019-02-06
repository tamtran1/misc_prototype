/****************************************************************
 * 
 * Description: Proof of concept for image steganography 
 * 
 * Resource:
 *  lambda functions - https://medium.com/@DakshHub/lambdas-the-companion-of-modern-c-b7dfd43b5abb
 ****************************************************************/

#include <iostream>
#include <string>
#include <functional>

#include <opencv2/highgui.hpp>

#include "steganography/LSB_Manip.h"
#include "steganography/Exception.h"

namespace Steganography {

/**
 * Description: Constructor reads an input image into a cv::Mat buffer
 */ 
LSB_Manip::LSB_Manip(const std::string& img_file): _img(cv::imread(img_file)), _cap((_img.total() * _img.elemSize()) / 8) {
  if(_img.empty()) {
    throw Exception("ERROR: Can't read input image file.");
  }
}

/**
 * Description: encode text messages into png image using best effort strategy.
 * Each character is represented by a byte or 8 bits. To hide the text into the
 * image we take one character and each of the 8 bits are stored in the least
 * significant bits of the pixel values (blue, green, red).
 * 
 * @return number of bytes encoded into image
 */
int LSB_Manip::encode(std::istream& is) {
  char c(is.get()); // temp char buffer to work on

  return _mtrx_wkr([&, this](Mtrx_Wkr_Data& dat, int color)->bool {
    if(is.eof() || dat.nbytes == this->_cap - 1 || !((c >> dat.bit_idx) & 1)) {
      dat.row[color] &= ~1; // if bit is 0 : change LSB of present color value to 0.
    } else {
      dat.row[color] |= 1; // if bit is 1 : change LSB of present color value to 1.
    }

    // if eof and bit_idx is 0, then our message is successfully encoded.
    if((dat.nbytes == this->_cap - 1 || is.eof()) && dat.bit_idx == 0) {
      return true; // we use this to break out of all three loops
    }

    if(dat.bit_idx-- == 0) { // advance the read head to work on the next bit
      dat.bit_idx = 7; // reset read head if all bits are checked, and get the next char to work on
      ++dat.nbytes; // increment byte count
      is.get(c); // fetch the next char from file
    }

    return false; // either end of matrix or end of text file has not reach yet
  });
}

/**
 * Description: decodes hidden message from image using least significant bit.
 * To extract the hidden message from the image, we will itterate through each pixels and extract the
 * lsb of each pixel channels (rgb) and this way we can get our message.
 * 
 * @return: number of bytes decoded from image
 */
int LSB_Manip::decode() {
  _str.resize(_cap, 0); // resize _str vector to _img max chars capacity and fill it with zeros

  return _mtrx_wkr([this](Mtrx_Wkr_Data& dat, int color)->bool {
    if(dat.row[color] & 1) { // manipulate the char bit according to the lsb of the pixel values
      this->_str[dat.nbytes] |= 1;
    }

    if(dat.bit_idx-- == 0) {
      dat.bit_idx = 7; // reset the bits read head
      if(this->_str[dat.nbytes++] == '\0') {
        this->_str.resize(dat.nbytes); // resize _str vector to length of message decoded from _img
        return true;
      }
    } else {
      this->_str[dat.nbytes] <<= 1; // move completed bit over to make room for next bit
    }

    return false;
  });
}

/**
 * Description: strip the image of lsb hidden messages by setting every pixels and sub
 * channels lsb to zero.
 * 
 * @return: bytes processed
 */
int LSB_Manip::strip() {
  std::cout << "Stripping least significant bit from image file." << std::endl;
  return _mtrx_wkr([this](Mtrx_Wkr_Data& dat, int color)->bool {
    dat.row[color] &= ~1; // set every pixels and its color channels lsb to zero

    if(dat.nbytes == this->_cap - 1 && dat.bit_idx == 0) {
      return true; // we use this to break out of all three loops
    }

    if(dat.bit_idx-- == 0) {
      dat.bit_idx = 7; // track bits to count bytes processed
      ++dat.nbytes; // increment byte count
    }

    return false;
  });
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

/**
 * Description: helper for encode and decode methods to reduce redundant codes
 * 
 * @return: number of bytes encoded ot decoded
 */
int LSB_Manip::_mtrx_wkr(std::function<bool(Mtrx_Wkr_Data&, int)> fn) {
  Mtrx_Wkr_Data dat{nullptr, 7, 0};

  for(int row = 0; row < _img.rows; ++row) {
    dat.row = _img.ptr(row);
    for(int col = 0; col < _img.cols; ++col) {
      for(int color = 0; color < 3; ++color) {
        if(fn(dat, color)) {
          return dat.nbytes;
        }
      }
      dat.row += 3;
    }
  }

  throw Exception("ERROR: Matrix operation failed. Cannot parse last byte. Image might be bad.");
}


} // namespace Steganography 