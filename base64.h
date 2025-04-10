#pragma once

#include <sys/types.h>
#include <stddef.h>

/** Base64 encoding options
  */
typedef enum
{
    /** use classical alphabet with padding
      */
    base64_mime = 0,

    /** omit padding characters
      */
    base64_sans_padding = 1,

    /** use url-safe alphabet (and omit padding characters)
      */
    base64_urlsafe      = 2 | base64_sans_padding
}
base64_options;

/** Base64 encode \a fill bytes read from \a input
  * \param output output buffer
  * \param capacity size of output buffer
  * \param input input buffer
  * \param fill number of bytes to read
  * \param options encoding options
  * \return number of bytes written on success (non-including the terminating null character),
  *         -n if output buffer size is insuffient (where n is the required size)
  */
ssize_t base64_encode(char *output, size_t capacity, const void *input, size_t fill, base64_options options);

/** Base64 decode \a fill characters read from \a input
  * \param output output buffer
  * \param capacity size of the output buffer
  * \param input input buffer
  * \param fill number of characters to read
  * \param options encoding options
  * \return number of characters written on success,
  *         -n if output buffer size is insuffient (where n is the required size)
  */
ssize_t base64_decode(void *output, size_t capacity, const char *input, size_t fill, base64_options options);
