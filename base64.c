#include "base64.h"
#include <stdint.h>
#include <stdlib.h>

static const char *mime_alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static const char *url_alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789-_";

ssize_t base64_encode(char *output, size_t capacity, const void *input, size_t fill, base64_options options)
{
    const uint8_t *bytes = (const void *)input;
    const size_t n = fill;
    size_t i = 0;

    const size_t m = 4 * (n / 3 + (n % 3 != 0));

    size_t l = 0;

    if (capacity < m + 1) return -(ssize_t)(m + 1);

    const char *alphabet = (options == base64_urlsafe) ? url_alphabet : mime_alphabet;

    while (i < n) {
        uint32_t bits = 0;
        for (int j = 0; j < 3; ++j) {
            bits |= (i < n) ? (uint32_t)bytes[i] : UINT32_C(0);
            bits <<= 8;
            ++i;
        }
        bits >>= 8;

        for (int k = 0; k < 4; ++k) {
            output[l++] = alphabet[(bits & UINT32_C(0xFC0000)) >> 18];
            bits <<= 6;
        }
    }
    if (i > 0) {
        while (i > n) {
            output[--l] = (options & base64_sans_padding) ? '\0' : '=';
            --i;
        }
        if (!(options & base64_sans_padding)) {
            l = m;
        }
    }

    output[l] = '\0';
    return l;
}

ssize_t base64_decode(void *output, size_t capacity, const char *input, size_t fill, base64_options options)
{
    uint8_t *bytes = (uint8_t *)output;
    const size_t m = fill;
    size_t n = 3 * m / 4;

    if (!(options & base64_sans_padding))
    {
        size_t p = 0;
        while (m - p > 0) {
            ++p;
            if (input[m - p] != '=') {
                --p;
                break;
            }
        }

        if (p > 2) return -1;

        n -= p;
    }

    if (capacity < n) return -(ssize_t)n;

    const char *alphabet = (options == base64_urlsafe) ? url_alphabet : mime_alphabet;

    size_t i = 0;

    for (size_t l = 0; l < m;) {
        uint32_t bits = 0;
        for (int k = 0; k < 4; ++k) {
            uint32_t ch = '=';
            if (l < fill) ch = input[l++];
            if ('A' <= ch && ch <= 'Z') ch -= 'A';
            else if ('a' <= ch && ch <= 'z') ch = (ch - 'a') + 26;
            else if ('0' <= ch && ch <= '9') ch = (ch - '0') + 52;
            else if (ch == alphabet[62]) ch = 62;
            else if (ch == alphabet[63]) ch = 63;
            else if (ch == '=') ch = 0;
            else return -1;
            bits |= ch;
            bits <<= 6;
        }
        bits >>= 6;
        for (int j = 0; j < 3; ++j) {
            if (i == n) break;
            uint8_t ch = (bits & UINT32_C(0xFF0000)) >> 16;
            bits <<= 8;
            bytes[i++] = ch;
        }
        if (i == n) break;
    }

    return n;
}
