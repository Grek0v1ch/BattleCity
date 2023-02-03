#pragma once

#include <exception>
#include <string>

namespace Exception {
    class Exception : public std::exception {
    private:
        std::string m_msg;

    public:
        Exception(const std::string& msg);
        ~Exception() override = default;

        virtual const char* what() const noexcept override;
    };
}