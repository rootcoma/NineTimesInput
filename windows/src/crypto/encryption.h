#ifndef ENCRYPTION_H
#define ENCRYPTION_H

int crypto_xor_plaintext(unsigned char* cipher/*out*/, const char *plaintext,
        const unsigned long long len);

int crypto_setup();

// Needed by tweetnacl
void randombytes(unsigned char *random_bytes/*out*/, unsigned long long length);

#endif

