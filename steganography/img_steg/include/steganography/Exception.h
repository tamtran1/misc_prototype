#ifndef STEGANOGRAPHY_EXCEPTION_H
#define STEGANOGRAPHY_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace Steganography {

struct Exception : std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace Steganography

#endif // STEGANOGRAPHY_EXCEPTION_H