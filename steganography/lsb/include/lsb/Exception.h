#ifndef STEGANOGRAPHY_EXCEPTION_H
#define STEGANOGRAPHY_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace Lsb {

struct Exception : std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace Lsb

#endif // STEGANOGRAPHY_EXCEPTION_H