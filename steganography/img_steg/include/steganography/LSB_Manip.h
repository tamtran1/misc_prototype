/****************************************************************
 * 
 * Description:
 * 
 ****************************************************************/

#ifndef LSB_Manip_H
#define LSB_Manip_H

#include <string>
#include <fstream>

#include <opencv2/highgui.hpp>

namespace Steganography {

enum class Operation {noop, encode, decode, strip};

class LSB_Manip {
  public:
    LSB_Manip(const std::string& img_file);

    void encode(const std::string& txt_file);
    std::string decode();
    void strip();
    void write_img(const std::string& out_img);

  private:
    cv::Mat _buf;
};


} // namespace Steganography

#endif // LSB_Manip_H