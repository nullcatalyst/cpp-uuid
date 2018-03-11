#pragma once

#include "./id.h"

namespace uuid {
    class Guid : public Id_t<128> {
    public:
        static Guid random() {
            return Guid{Id_t<128>::random()};
        }

        static Guid fromB64String(const std::string & b64String) {
            return Guid{Id_t<128>::fromB64String(b64String)};
        }

        static Guid fromGuidString(const std::string & guidString) {
            Guid guid;

            if (guidString.length() != 36) {
                // Incorrect string length. It can't be formatted correctly
                return guid;
            }

            const char * cString = guidString.c_str();

            int i = 0;
            int j = 0;
            uint8_t a;
            uint8_t b;

#define fromHexChar(x) (                                            \
        (x) >= '0' && (x) <= '9' ? (x) - '0' : (                    \
        (x) >= 'A' && (x) <= 'F' ? (x) - 'A' + 10 : (               \
        (x) >= 'a' && (x) <= 'f' ? (x) - 'a' + 10 : (               \
        (0xff))))                                                   \
        )
#define byteLoop(end, termChar)                                     \
            do {                                                    \
                for ( ; i < (end); ++i) {                           \
                    a = cString[j++]; a = fromHexChar(a);           \
                    b = cString[j++]; b = fromHexChar(b);           \
                    if (a > 0xf || b > 0xf) {                       \
                        goto fromGuidString_error;                  \
                    }                                               \
                    guid.bytes[i] = ((a & 0xf) << 4) | (b & 0xf);   \
                }                                                   \
                if (cString[j++] != (termChar)) {                   \
                    goto fromGuidString_error;                      \
                }                                                   \
            } while (false)

            byteLoop(4, '-');
            byteLoop(6, '-');
            byteLoop(8, '-');
            byteLoop(10, '-');
            byteLoop(16, '\0');

#undef byteLoop
#undef toHexChar

            return guid;

        fromGuidString_error:
            // guid.clear();
            return guid;
        }

        std::string toGuidString() const {
            char buffer[37];

            int i = 0;
            int j = 0;

#define toHexChar(x) ((x) < 10 ? '0' + (x) : ((x) - 10) + 'a')
#define byteLoop(end, termChar)                                     \
            do {                                                    \
                for ( ; j < (end); ++j) {                           \
                    buffer[i++] = toHexChar((bytes[j] >> 4) & 0xf); \
                    buffer[i++] = toHexChar(bytes[j] & 0xf);        \
                }                                                   \
                buffer[i++] = (termChar);                           \
            } while (false)

            byteLoop(4, '-');
            byteLoop(6, '-');
            byteLoop(8, '-');
            byteLoop(10, '-');
            byteLoop(16, '\0');

#undef byteLoop
#undef toHexChar

            return std::string(buffer);
        }
    };
}
