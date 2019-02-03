#include <iostream>

#include <opencv2/highgui.hpp>

/* Checks whether the bit is set or not at a particular position.
 * Returns true if set
 * Returns false if not set
 * 7 6 5 4 3 2 1 0
 */
bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

/* ./decode output_image.png
  * argv[0] = ./decode
  * argv[1] = output_image.png
  */
int main(int argc, char** argv) {
	if(argc != 2) { // Checks if proper number of arguments are passed
		std::cerr << "Arguments Error" << std::endl;
		std::exit(1);
	}

	cv::Mat image = cv::imread(argv[1]); // Stores original image
	if(image.empty()) {
		std::cerr << "Image Error" << std::endl;
		std::exit(1);
	}

	char ch(0); // char to work on
	int bit_count(0); // contains information about which bit of char to work on

	/*To extract the message from the image, we will iterate through the pixels and extract the LSB of
	 * the pixel values (RGB) and this way we can get our message.
	 */
	for(int row = 0; row < image.rows; ++row) {
		for(int col = 0; col < image.cols; ++col) {
			for(int color = 0; color < 3; ++color) {
				cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(row,col)); // stores the pixel details
				
				if(isBitSet(pixel.val[color],0)) { // manipulate char bits according to the LSB of pixel values
					ch |= 1;
        }

				++bit_count; // increment bit_count to work on next bit

				if(bit_count == 8) { // bit_count is 8, that means we got our char from the encoded image
					if(ch == '\0') {// NULL char is encountered
            std::cout << std::endl; // print a new line after displaying the message
						goto OUT;
          }

					bit_count = 0;
					std::cout << ch;
					ch = 0;
				} else {
					ch = ch << 1;
				}
			}
		}
	}

	OUT:; // label for goto
}