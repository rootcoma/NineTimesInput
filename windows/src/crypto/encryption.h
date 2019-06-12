#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// TODO: rename, this is xor, so it encrypts and decrypts
int output_encrypt(unsigned char* cipher/*out*/, const char *plaintext,
        const unsigned long long len);


// Needed by tweetnacl
void randombytes(unsigned char *random_bytes/*out*/, unsigned long long length);

#endif

