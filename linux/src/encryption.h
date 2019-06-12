#ifndef ENCRYPTION_H
#define ENCRYPTION_H

int output_decrypt(char *plaintext, const unsigned char *cipher,
		unsigned long long len);

void randombytes(unsigned char *random_bytes, unsigned long long len);

#endif


