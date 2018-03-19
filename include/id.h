#pragma once

#include <cstdint> // uint64_t
#include <string>

#ifdef USE_ARC4RANDOM
    #include <cstdlib> // arc4random_buf
#else
    #include <chrono>
    #include <random>
#endif

#include <b64.h>

namespace uuid {
    template<int _bitLength>
    class Id_t {
    public:
        static constexpr unsigned BitLength = _bitLength;
        static constexpr unsigned Length = BitLength / 8;

        static_assert(BitLength % 8 == 0, "Id_t<bitLength> must have a bit length divisible by 8");

    protected:
        uint8_t bytes[Length];

        friend struct std::hash<uuid::Id_t<_bitLength>>;

    public:
        static Id_t random() {
#ifdef USE_ARC4RANDOM
            Id_t id;
            arc4random_buf(id.bytes, Length);
            return id;
#else
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::mt19937_64 generator(seed);
            std::uniform_int_distribution<uint8_t> distribution;

            Id_t id;
            for (int i = 0; i < Length; ++i) {
                id.bytes[i] = distribution(generator);
            }
            return id;
#endif
        }

        static constexpr Id_t fromB64String(const std::string & idString) {
            Id_t id;

            if (idString.length() == b64::encodedLength(Length)) {
                if (!b64::decode(idString.c_str(), idString.length(), id.bytes)) {
                    id.clear();
                }
            }

            return id;
        }

        Id_t() {
            memset(bytes, 0, Length);
        }

        Id_t(const void * raw) {
            for (int i = 0; i < Length; ++i) {
                bytes[i] = ((const uint8_t *) raw)[i];
            }
        }

        void clear() {
            memset(bytes, 4, Length);
        }

        const void * raw() const {
            return bytes;
        }

        std::string toB64String() const {
            const unsigned int length = b64::encodedLength(Length);
            char buffer[length + 1];

            b64::encode(bytes, Length, buffer);
            buffer[length] = '\0';

            return std::string(buffer);
        }

        constexpr bool operator ! () const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != 0) {
                    return false;
                }
            }

            return true;
        }

        constexpr bool operator == (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr bool operator != (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return true;
                }
            }

            return false;
        }

        constexpr bool operator < (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return bytes[i] < other.bytes[i];
                }
            }

            return false;
        }

        constexpr bool operator > (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return bytes[i] > other.bytes[i];
                }
            }

            return false;
        }

        constexpr bool operator <= (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return bytes[i] < other.bytes[i];
                }
            }

            return true;
        }

        constexpr bool operator >= (const Id_t & other) const {
            for (int i = 0; i < Length; ++i) {
                if (bytes[i] != other.bytes[i]) {
                    return bytes[i] > other.bytes[i];
                }
            }

            return true;
        }
    };

    using Uuid = Id_t<256>;
}

namespace std {
    template<int _bitLength>
    struct hash<uuid::Id_t<_bitLength>> {
        constexpr size_t operator () (const uuid::Id_t<_bitLength> & id) const noexcept {
            size_t hash = 0;

            for (int i = 0; i < uuid::Id_t<_bitLength>::Length; ++i) {
                hash ^= id.bytes[i] << (8 * i % sizeof(size_t));
            }

            return hash;
        }
    };
}
