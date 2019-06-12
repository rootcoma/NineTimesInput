#ifndef ENCRYPTION_H
#define ENCRYPTION_H

int output_encrypt(unsigned char* cipher/*out*/, const char *plaintext,
        const unsigned long long len);

void randombytes(unsigned char *random_bytes/*out*/, unsigned long long length);

#endif

