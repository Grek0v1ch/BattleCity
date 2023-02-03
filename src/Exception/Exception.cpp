#include "Exception.h"

namespace Exception {
    Exception::Exception(const std::string& msg) : m_msg(msg) {}

    const char *Exception::what() const noexcept {
        return m_msg.c_str();
    }
}