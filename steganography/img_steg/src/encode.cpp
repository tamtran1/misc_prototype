#include <iostream>
#include <fstream>

#include <opencv2/highgui.hpp>

/* Checks whether the bit is set or not at a particular position.
 * Returns true if set
 * Returns false if not set
 * 7 6 5 4 3 2 1 0
 */
bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if(ch & 1) {
    return true;
  }
	return false;
}

/* ./encode image.png textfile.txt output_image.png
 * argv[0] = ./encode
 * argv[1] = image.png
 * argv[2] = textfile.txt
 * argv[3] = output_image.png
 */
int main(int argc, char** argv) {
	if(argc != 4) { // Checks if proper number of arguments are passed
		std::cerr << "Arguments Error" << std::endl;
		std::exit(1);
	}

	cv::Mat image(cv::imread(argv[1])); // Stores original image
	if(image.empty()) {
		std::cerr << "Image Error" << std::endl;
		std::exit(1);
	}

	std::ifstream file(argv[2]); // Open file for text information
	if(!file.is_open()) {
		std::cerr << "File Error" << std::endl;
		std::exit(1);
	}

	char ch; // char to work on
	file.get(ch); // reads the first char from the file
	int bit_count = 0; // contains information about which bit of char to work on
	bool last_null_char = false; // to check whether file has ended
	bool encoded = false; // to check if the whole message is encoded or not

	/* To hide text into images. We are taking one char (8 bits) and each of the 8 bits are stored
	 * in the Least Significant Bits (LSB) of the pixel values (Red,Green,Blue).
	 * We are manipulating bits in such way that changing LSB of the pixel values will not make a huge difference.
	 * The image will still look similiar to the naked eye.
	 */
	for(int row = 0; row < image.rows; ++row) {
		for(int col = 0; col < image.cols; ++col) {
			for(int color = 0; color < 3; ++color) {
				cv::Vec3b pixel(image.at<cv::Vec3b>(cv::Point(row,col))); // stores the pixel details

				// if bit is 1 : change LSB of present color value to 1.
				// if bit is 0 : change LSB of present color value to 0.
				if(isBitSet(ch, 7 - bit_count)) {
					pixel.val[color] |= 1;
        } else {
					pixel.val[color] &= ~1;
        }
        
				image.at<cv::Vec3b>(cv::Point(row,col)) = pixel; // update the image with the changed pixel values
				++bit_count; // increment bit_count to work on next bit

				// if last_null_char is true and bit_count is 8, then our message is successfully encode.
				if(last_null_char && bit_count == 8) {
					encoded = true;
					goto OUT;
				}

				// if bit_count is 8 we pick the next char from the file and work on it
				if(bit_count == 8) {
					bit_count = 0;
					file.get(ch);

					// if EndOfFile(EOF) is encountered insert NULL char to the image
					if(file.eof()) {
						last_null_char = true;
						ch = '\0';
					}
				}
			}
		}
	}
	OUT:; // label for goto

	if(!encoded) { // whole message was not encoded
		std::cout << "Message too big. Try with larger image." << std::endl;
		std::exit(1);
	}

  try { // attempt to writes the stegnographic image
    cv::imwrite(argv[3], image);
  } catch(const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }
}