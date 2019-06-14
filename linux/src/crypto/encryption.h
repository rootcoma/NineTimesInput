#ifndef ENCRYPTION_H
#define ENCRYPTION_H

int crypto_setup();

int crypto_xor_plaintext(char *plaintext, const unsigned char *cipher,
		unsigned long long len);

void randombytes(unsigned char *random_bytes, unsigned long long len);

#endif


