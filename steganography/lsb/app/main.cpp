/****************************************************************
 * 
 * Description: main driver to test LSB steganography implimentation
 * 
 ****************************************************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <getopt.h>
#include <bitset>
#include <sstream>

#include "lsb/Lsb.h"
#include "lsb/Exception.h"
#include "common/ColorText.h"

using namespace ColorText;

enum class Op {encode, decode, strip, noop};

void usage(const std::string& arg) {
  std::cout << "Usage:\t" << arg
            << "[-edsh] [-i <input.png>] [-b <data>] [-o <output.png>]" << std::endl;
  std::cout << "\n"
            << "Options:\n"
            << "  -i, --image <input_image.png>\n"
            << "                          The input image used to hide the data.\n"
            << "  -b, --data <data>\n"
            << "                          The data to hide. Can be a simple text file.\n"
            << "  -o, --out <output.png>\n"
            << "                          Specify a file name for the encoded output image.\n"
            << "                          Defaults to image_out.png in current directory\n"
            << "                          if option is not specified.\n"
            << "  -e, --encode            Set encoding operation.\n"
            << "  -d, --decode            Set decoding operation.\n"
            << "  -s, --strip             Set lsb stripping operation.\n"
            << "  -h, --help              Show this help dialogue." << std::endl;
}

int main(int argc, char** argv) {
  std::string image;
  std::ifstream data;
  std::string ofile("image_out.png");
  Op op(Op::noop);
  int c(0); // getopt character value

  option opt[] = {
    {"data", required_argument, nullptr, 'b'},
    {"image", required_argument, nullptr, 'i'},
    {"out", required_argument, nullptr, 'o'},
    {"encode", no_argument, nullptr, 'e'},
    {"decode", no_argument, nullptr, 'd'},
    {"strip", no_argument, nullptr, 's'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
  };

  // no ':' at start of argstring so getopt will produce error for us
  while((c = getopt_long(argc, argv, "edshb:i:o:", opt, nullptr)) != -1 ) {
    switch(c) {
      case 0: {  // getopt sets a flag, just keep going
        break;
      } case 'b': {
        data.open(optarg, std::ifstream::in);
        if(!data.is_open()) {
          std::cerr << "ERROR: Can't open text file." << std::endl;
          std::exit(1);
        }
        break;
      } case 'i': {
        image = optarg;
        break;
      } case 'o': {
        ofile = optarg;
        break;
      } case 'e': {
        op = Op::encode;
        break;
      } case 'd': {
        op = Op::decode;
        break;
      } case 's': {
        op = Op::strip;
        break;
      } case 'h': {
        usage(argv[0]);
        return 0;
        break;
      } case '?': {
        // getopt will produce error message for us
        std::exit(1);
      }
    }
  }

  try {
    Lsb::Lsb lm(image);
    switch(op) {
      case Op::encode: {
        data.seekg(0, data.end);
        int msg_len(data.tellg());
        std::cout << Color::green << "message characers: " << Color::blue << msg_len  << "\n"
                  << Color::green << "characters encoded: " << Color::blue << lm.encode(data.seekg(0, data.beg))
                  << Color::reset << std::endl;
        lm.write_img(ofile);
        break;
      } case Op::decode: {
        std::cout << Color::green << "characters decoded: " << Color::blue << lm.decode() << "\n"
                  << Color::green << "message: " << Color::blue << lm
                  << Color::reset << std::endl;
        break;
      } case Op::strip: {
        int bytes(lm.strip());
        lm.write_img(ofile);
        std::cout << Color::green << "Bytes processed: " << Color::blue << bytes << "\n"
                  << Color::reset << std::endl;
        break;
      } case Op::noop: {
        std::cout << Bg::red << Color::yellow  << "Operation not specified. Nothing to do." << Color::reset << std::endl;
        break;
      }
    }
  } catch(const Lsb::Exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  } catch(const cv::Exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }
}