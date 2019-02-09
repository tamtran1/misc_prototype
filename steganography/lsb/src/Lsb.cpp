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

#include "lsb/Lsb.h"
#include "lsb/Exception.h"

namespace Lsb {

/**
 * Description: Constructor reads an input image into a cv::Mat buffer
 */ 
Lsb::Lsb(const std::string& img_file): _img(cv::imread(img_file)), _img_bits(_img.total() * _img.elemSize()) {
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
int Lsb::encode(std::istream& is) {
  is.seekg(0, is.end);
  auto msg_bits(is.tellg() * 8 + 8);
  is.seekg(0, is.beg);
  char c(0);

  if(msg_bits + 8 > _img_bits) {
    throw Exception("ERROR: Message size too big for image. Use a bigger image.");
  }

  return _mtrx_wkr([&, this](Mtrx_Wkr_Data& dat, int color)->bool {
    if(dat.bits % 8 == 0) {
      is.get(c);
    }

    dat.row[color] &= 254;
    if(dat.bits < msg_bits - 7) {
      dat.row[color] |= (c & 1 << dat.bits % 8) >> dat.bits % 8;
    }
    
    return dat.bits++ == msg_bits;
  });
}

/**
 * Description: decodes hidden message from image using least significant bit.
 * To extract the hidden message from the image, we will itterate through each pixels and extract the
 * lsb of each pixel channels (rgb) and this way we can get our message.
 * 
 * @return: number of bytes decoded from image
 */
int Lsb::decode() {
  _str.resize(_img_bits / 8, 0); // size up _str vector to _img max chars capacity

  return _mtrx_wkr([this](Mtrx_Wkr_Data& dat, int color)->bool {
    this->_str[dat.bits / 8] |= (dat.row[color] & 1) << dat.bits++ % 8; // manipulate the char bit according to the lsb of the pixel values
    if(dat.bits % 8 == 7 && this->_str[dat.bits / 8] == '\0') {
      _str.resize(dat.bits / 8);
      return true;
    }
    return (dat.bits % 8 == 7 && this->_str[dat.bits / 8] == '\0');
  });
}

/**
 * Description: strip the image of lsb hidden messages by setting every pixels and sub
 * channels lsb to zero.
 * 
 * @return: bytes processed
 */
int Lsb::strip() {
  return _mtrx_wkr([this](Mtrx_Wkr_Data& dat, int color)->bool {
    dat.row[color] &= 254; // set every pixels and its color channels lsb to zero
    return ++dat.bits == _img_bits;
  });
}

/**
 * Description: write a processed image out to disk
 */
void Lsb::write_img(const std::string& out_img) {
  if(out_img.empty()) {
    throw Exception("ERROR: Output image file not specified");
  }
  
  cv::imwrite(out_img, _img);
}

std::ostream& operator<<(std::ostream& os, const Lsb& lm) {
  return os << lm._str.data();
}

/**
 * Description: helper for encode and decode methods to reduce redundant codes
 * 
 * @return: number of bytes encoded ot decoded
 */
int Lsb::_mtrx_wkr(std::function<bool(Mtrx_Wkr_Data&, int)> fn) {
  Mtrx_Wkr_Data dat{};

  for(int row = 0; row < _img.rows; ++row) {
    dat.row = _img.ptr(row);
    for(int col = 0; col < _img.cols; ++col) {
      for(int color = 0; color < 3; ++color) {
        if(fn(dat, color)) {
          return dat.bits / 8;
        }
      }
      dat.row += 3;
    }
  }


  throw Exception("ERROR: Matrix operation failed. Cannot parse last byte. Image might be bad.");
}


} // namespace Lsb 