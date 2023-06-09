// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

char *strcpy(char *destination, const char *source)
{
	/* TODO: Implement strcpy(). */
	char *ptr = destination;
	while (*source != '\0') {
    	*ptr++ = *source++;
	}
 
	*ptr = '\0';

	return destination;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncpy(). */
	char* ptr = destination;
    while (*source != '\0' && len) {
        *ptr++ = *source++;
		len--;
    }

    while (len) {
        *ptr++ = '\0';
        len--;
    }

	return destination;
}

char *strcat(char *destination, const char *source)
{
	/* TODO: Implement strcat(). */
	char *ptr = destination + strlen(destination);
 	while (*source != '\0') {
        *ptr++ = *source++;
    }
 
    *ptr = '\0';
	
	return destination;
}

char *strncat(char *destination, const char *source, size_t len)
{
	/* TODO: Implement strncat(). */
	char *ptr = destination + strlen(destination);
 	while (*source != '\0' && len) {
        *ptr++ = *source++;
		len--;
    }
 
    *ptr = '\0';

	return destination;
}

int strcmp(const char *str1, const char *str2)
{
	/* TODO: Implement strcmp(). */
	while (*str1 == *str2) {
        if (*str1 == '\0') {
            return 0;
        }  
		str1++;
        str2++;
    }
 
    return *(const unsigned char*)str1 < *(const unsigned char*)str2 ? -1 : 1;
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	/* TODO: Implement strncmp(). */
	while (*str1 == *str2 && len) {
		if (*str1 == '\0') {
			return 0;
		} else if (*str1 != *str2) {
			return *(const unsigned char*)str1 < *(const unsigned char*)str2 ? -1 : 1;
		}
		str1++;
		str2++;
		len--;
	}

	return 0;
}

size_t strlen(const char *str)
{
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	/* TODO: Implement strchr(). */
	while (*str != '\0') {
        if (*str == c) {
            return str;
		}
        str++;
    }

	return NULL;
}

char *strrchr(const char *str, int c)
{
	/* TODO: Implement strrchr(). */
	char *ptr_result = NULL;
	while (*str != '\0') {
        if (*str == c) {
            ptr_result = str;
		}
        str++;
    }

	return ptr_result;
}

char *strstr(const char *str1, const char *str2)
{
	/* TODO: Implement strstr(). */
	if (str2 == '\0') {
		return str1;
	}

	while (*str1 != '\0') {
        const char *ptr1 = str1;
        const char *ptr2 = str2;
        while (*ptr2 != '\0' && *ptr1 == *ptr2) {
            ptr1++;
            ptr2++;
        }
        if (*ptr2 == '\0') {
            return str1;
        }
        str1++;
    }

	return NULL;
}

char *strrstr(const char *str1, const char *str2)
{
	/* TODO: Implement strrstr(). */
	char *ptr_result = NULL;
	while (*str1 != '\0') {
        const char *ptr1 = str1;
        const char *ptr2 = str2;
        while (*ptr2 != '\0' && *ptr1 == *ptr2) {
            ptr1++;
            ptr2++;
        }
        if (*ptr2 == '\0') {
            ptr_result = str1;
        }
        str1++;
    }

	return ptr_result;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	/* TODO: Implement memcpy(). */
	void *ptr = destination;
	while (num--) {
		*(char*)ptr++ = *(char*)source++;
	}

	return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
	void *ptr = destination;  
  	if (source < ptr) {
		ptr += num - 1;
		source += num;
		while (num--) {
			*(char*)ptr-- = *(char*)source--;
		} 
  	} else {
    	while (num--) {
      		*(char*)ptr++ = *(char*)source++;
    	}
  	}

	return destination;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	/* TODO: Implement memcmp(). */
	while (num--) {
		if (*(const unsigned char*)ptr1++ != *(const unsigned char*)ptr2++) {
			return *(const unsigned char*)ptr1 < *(const unsigned char*)ptr2 ? -1 : 1;
		}
	}
	
    return 0;
}

void *memset(void *source, int value, size_t num)
{
	/* TODO: Implement memset(). */
	while (num--) {
	    *(char*)source++ = (char)value;
	}

	return source;
}
