#include "base64.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int base64_encode_test(const char *input, const char *expected_output, base64_options options)
{
    const size_t capacity = 16;
    char output[capacity];
    ssize_t num_written = base64_encode(output, capacity, input, strlen(input), options);
    bool ok = (strcmp(output, expected_output) == 0 && num_written == strlen(expected_output));
    printf("base64_encode_test(\"%s\", \"%s\", \"%s\", %d): %s\n", input, expected_output, output, (int)options, ok ? "OK" : "FAILED");
    return ok ? 0 : 1;
}

int base64_decode_test(const char *input, const char *expected_output, base64_options options)
{
    const size_t capacity = 16;
    char output[capacity];
    ssize_t num_written = base64_decode(output, capacity, input, strlen(input), options);
    if (0 <= num_written && num_written < 16) output[num_written] = '\0';
    bool ok = (strcmp(output, expected_output) == 0 && num_written == strlen(expected_output));
    printf("base64_decode_test(\"%s\", \"%s\", \"%s\", %d): %s\n", input, expected_output, output, (int)options, ok ? "OK" : "FAILED");
    return ok ? 0 : 1;
}

int main(int argc, char *argv[])
{
    int errors = 0;

    {
        /** RFC 4648 test vectors
          */

        errors += base64_encode_test("", "", base64_mime);
        errors += base64_encode_test("f", "Zg==", base64_mime);
        errors += base64_encode_test("fo", "Zm8=", base64_mime);
        errors += base64_encode_test("foo", "Zm9v", base64_mime);
        errors += base64_encode_test("foob", "Zm9vYg==", base64_mime);
        errors += base64_encode_test("fooba", "Zm9vYmE=", base64_mime);
        errors += base64_encode_test("foobar", "Zm9vYmFy", base64_mime);

        errors += base64_encode_test("", "", base64_sans_padding);
        errors += base64_encode_test("f", "Zg", base64_sans_padding);
        errors += base64_encode_test("fo", "Zm8", base64_sans_padding);
        errors += base64_encode_test("foo", "Zm9v", base64_sans_padding);
        errors += base64_encode_test("foob", "Zm9vYg", base64_sans_padding);
        errors += base64_encode_test("fooba", "Zm9vYmE", base64_sans_padding);
        errors += base64_encode_test("foobar", "Zm9vYmFy", base64_sans_padding);

        errors += base64_decode_test("", "", base64_mime);
        errors += base64_decode_test("Zg==", "f", base64_mime);
        errors += base64_decode_test("Zm8=", "fo", base64_mime);
        errors += base64_decode_test("Zm9v", "foo", base64_mime);
        errors += base64_decode_test("Zm9vYg==", "foob", base64_mime);
        errors += base64_decode_test("Zm9vYmE=", "fooba", base64_mime);
        errors += base64_decode_test("Zm9vYmFy", "foobar", base64_mime);

        errors += base64_decode_test("", "", base64_sans_padding);
        errors += base64_decode_test("Zg", "f", base64_sans_padding);
        errors += base64_decode_test("Zm8", "fo", base64_sans_padding);
        errors += base64_decode_test("Zm9v", "foo", base64_sans_padding);
        errors += base64_decode_test("Zm9vYg", "foob", base64_sans_padding);
        errors += base64_decode_test("Zm9vYmE", "fooba", base64_sans_padding);
        errors += base64_decode_test("Zm9vYmFy", "foobar", base64_sans_padding);
    }

    return errors;
}
