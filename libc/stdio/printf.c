#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

size_t print_d(size_t *maxrem, int dec);
size_t print_p(size_t *maxrem, unsigned int ptrVal);
size_t itoa(char *dst, int num);
size_t ltoa(char *dst, long int num);

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
    } else if (*format == 'p') {
      format++;
      size_t newLen;
      newLen = print_p(&maxrem, va_arg(parameters, unsigned int));
      if (newLen == 0)
        return -1;

      written += newLen;
    } else if (*format == 'x') {
      format++;
      int hexVal = va_arg(parameters, int);
      char hexChar[3];

      for (int i = 1; i != -1; i--) {
        hexChar[i] = hexVal & 0x0f;
        hexVal = hexVal >> 4;
        if (hexChar[i] <= 9)
          hexChar[i] += 0x30;
        else
          hexChar[i] += 55;
      }
      hexChar[2] = 0;
      if (!print(hexChar, 2))
        return -1;
      written += 2;
    } else if (*format == 'd') {
      written += print_d(&maxrem, va_arg(parameters, int));
      format++;
    } else if (*format == 'l') {
      format++;
      long int iVal = va_arg(parameters, long int);
      char str[20];
      size_t len;
      len = 0;
      if (iVal < 0) {
        iVal *= -1;
        if (!print("-", 1))
          return -1;
        written++;
      }

      len = ltoa(str, iVal);

      if (maxrem < len) {
        return -1;
      }
      if (!print(str, len))
        return -1;

      written += len;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

size_t print_p(size_t *maxrem, unsigned int ptrVal) {
  if (*maxrem < 9)
    return -1;

  char ptrChar[9];
  for (int i = 7; i != -1; i--) {
    ptrChar[i] = ptrVal & 0x0f;
    ptrVal = ptrVal >> 4;
    if (ptrChar[i] <= 9)
      ptrChar[i] += 0x30;
    else
      ptrChar[i] += 55;
  }
  ptrChar[8] = 0;
  if (!print("0x", 2))
    return -1;
  if (!print(ptrChar, 8))
    return -1;

  return 10;
}

size_t print_d(size_t *maxrem, int dec) {
  char str[10];
  size_t written, len;

  written = 0;
  len = 0;
  if (dec < 0) {
    dec *= -1;
    if (!print("-", 1))
      return 0;
    written++;
  }

  len = itoa(str, dec);

  if (*maxrem < len) {
    return -1;
  }
  if (!print(str, len))
    return -1;

  written += len;
  return written;
}

size_t itoa(char *dst, int num) {
  size_t len = 0;
  if (num == 0) {
    dst[0] = '0';
    dst[1] = '\0';
    return 1;
  }

  for (int tmp = num; tmp != 0; tmp = tmp / 10)
    len++;

  for (int i = len-1; i >= -1; i--) {
    dst[i] = (num % 10) + 0x30;
    num = num / 10;
  }

  return len;
}

size_t ltoa(char *dst, long int num) {
  size_t len = 0;
  if (num == 0) {
    dst[0] = '0';
    dst[1] = '\0';
    return 1;
  }

  for (long int tmp = num; tmp != 0; tmp = tmp / 10)
    len++;

  for (int i = len-1; i >= -1; i--) {
    dst[i] = (num % 10) + 0x30;
    num = num / 10;
  }

  return len;
}
