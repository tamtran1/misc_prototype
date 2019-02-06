/****************************************************************
 * 
 * Description:
 * 
 ****************************************************************/
#include <iostream>
#include <vector>
#include <fstream>

#include "steganography/LSB_Manip.h"
#include "steganography/Exception.h"

void usage() {
  std::cout << "Usage:" << std::endl;
  // TODO: prints usage here
  std::exit(0);
}

std::string get_arg(const std::vector<std::string> vec, int idx) {
  if(idx == vec.size()) {
    std::cerr << "ERROR: Missing argument." << std::endl;
    std::exit(1);
  }
  return vec[idx];
}

int main(int argc, char** argv) {
  std::string in_img;
  std::ifstream in_txt;
  std::string out_img;
  Steganography::Operation op(Steganography::Operation::noop);

  std::vector<std::string> argv_str(argv, argv + argc); // copy all cli arguments into string vector
	for(int i = 1; i < argv_str.size(); ++i) { // parsing command line options. Start at 1 to skip program name 
		if(argv_str[i] == "-i" || argv_str[i] == "--in-image") {
      in_img = get_arg(argv_str, ++i);
    } else if(argv_str[i] == "-t" || argv_str[i] == "--in-text") {
      in_txt.open(get_arg(argv_str, ++i), std::ifstream::in);
      if(!in_txt.is_open()) {
        std::cerr << "ERROR: Can't open text file." << std::endl;
        std::exit(1);
      }
    } else if(argv_str[i] == "-o" || argv_str[i] == "--out-image") {
      out_img = get_arg(argv_str, ++i);
    } else if(argv_str[i] == "-e" || argv_str[i] == "--encode") {
      op = Steganography::Operation::encode;
    } else if(argv_str[i] == "-d" || argv_str[i] == "--decode") {
      op = Steganography::Operation::decode;
    } else if(argv_str[i] == "-s" || argv_str[i] == "--strip") {
      op = Steganography::Operation::strip;
    } else if(argv_str[i] == "-h" || argv_str[i] == "--help") {
      usage();
    } else {
      std::cerr << "ERROR: Unknown option: " << argv_str[i] << std::endl;
      std::exit(1);
    }
	}

  try {
    Steganography::LSB_Manip lm(in_img);
    switch(op) {
      case Steganography::Operation::encode: {
        in_txt.seekg(0, in_txt.end);
        int msg_len(in_txt.tellg());
        std::cout << "message characers: " << msg_len << std::endl;
        std::cout << "characters encoded: " << lm.encode(in_txt.seekg(0, in_txt.beg)) << std::endl;
        lm.write_img(out_img);
        break;
      } case Steganography::Operation::decode: {
        std::cout << "characters decoded: " << lm.decode() << std::endl;
        std::cout << lm;
        break;
      } case Steganography::Operation::strip: {
        int bytes(lm.strip());
        lm.write_img(out_img);
        std::cout << "Bytes processed: " << bytes << std::endl;
        break;
      } case Steganography::Operation::noop: {
        std::cout << "No operation specified. Nothing to do." << std::endl;
        break;
      }
    }
  } catch(const Steganography::Exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  } catch(const cv::Exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }
}