/****************************************************************
 * 
 * Description: common class for displaying color text in the console
 * 
 * Resource: https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 *  https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 * 
 ****************************************************************/

#ifndef COLOR_TEXT_H
#define COLOR_TEXT_H

#include <ostream>

namespace ColorText {


namespace Color {
  enum Color {
    reset = 0,
    black = 90,
    red = 91,
    green = 92,
    yellow = 93,
    blue = 94,
    magenta = 95,
    cyan = 96,
    white = 97,
  };

  std::ostream& operator<<(std::ostream& os, Color c) {
    return os << "\033[" << static_cast<int>(c) << "m";
  }
} // namespace Color

namespace Bg {
  enum BackgroundColor {
    red = 41,
    green = 42,
    blue = 44,
  };

  std::ostream& operator<<(std::ostream& os, BackgroundColor c) {
    return os << "\033[" << static_cast<int>(c) << "m";
  }
} // namespace Bg


} // namespace ColorText

#endif // COLOR_TEXT_H