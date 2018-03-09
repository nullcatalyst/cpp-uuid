#pragma once

#include <cstdint> // uint64_t

#include <chrono>
#include <random>
#include <string>
#include <b64.h>

namespace uuid {
    template<int _bitLength, typename _type>
    class Id_t {
    public:
        using Type = _type;
        static constexpr unsigned BitLength = _bitLength;
        static constexpr unsigned ByteLength = BitLength / 8;
        static constexpr unsigned ArrayLength = ByteLength / sizeof(Type);
        Type _value[ArrayLength];

    public:
        static Id_t random() {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::mt19937_64 generator(seed);
            std::uniform_int_distribution<uint64_t> distribution;

            Id_t id;
            for (int i = 0; i < ArrayLength; ++i) {
                id._value[i] = distribution(generator);
            }
            return id;
        }

        static constexpr Id_t fromString(const std::string & idString) {
            Id_t id;

            if (idString.length() == b64::encodedLength(ByteLength)) {
                if (!b64::decode(idString.c_str(), idString.length(), id._value)) {
                    id.clear();
                }
            }

            return id;
        }

        Id_t() {
            for (int i = 0; i < ArrayLength; ++i) {
                _value[i] = 0;
            }
        }

        Id_t(const void * raw) {
            const Type * value = (const Type *) raw;

            for (int i = 0; i < ArrayLength; ++i) {
                _value[i] = value[i];
            }
        }

        void clear() {
            for (int i = 0; i < ArrayLength; ++i) {
                _value[i] = 0;
            }
        }

        std::string toString() const {
            const unsigned int length = b64::encodedLength(ByteLength);
            char buffer[length + 1];

            b64::encode(_value, ByteLength, buffer);
            buffer[length] = '\0';

            return std::string(buffer);
        }

        constexpr bool operator ! () const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] == 0) {
                    return true;
                }
            }

            return false;
        }

        constexpr bool operator == (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr bool operator != (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return true;
                }
            }

            return false;
        }

        constexpr bool operator < (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return _value[i] < other._value[i];
                }
            }

            return false;
        }

        constexpr bool operator > (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return _value[i] > other._value[i];
                }
            }

            return false;
        }

        constexpr bool operator <= (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return _value[i] < other._value[i];
                }
            }

            return true;
        }

        constexpr bool operator >= (const Id_t & other) const {
            for (int i = 0; i < ArrayLength; ++i) {
                if (_value[i] != other._value[i]) {
                    return _value[i] > other._value[i];
                }
            }

            return true;
        }
    };

    using Id = Id_t<512, uint64_t>;
}
