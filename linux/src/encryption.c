#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"
#include "tweetnacl.h"

// XXX: Don't use these values
// xsalsa20_xor key, 32 bytes
const unsigned char kxm_crypto_k[crypto_stream_KEYBYTES] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

// 24 bytes
const unsigned char kxm_crypto_n[crypto_stream_NONCEBYTES] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
};


int output_decrypt(char *plaintext, const unsigned char *cipher,
		unsigned long long len)
{
	const int err = crypto_stream_xor((unsigned char *)plaintext, cipher, len,
		kxm_crypto_n, kxm_crypto_k);
	if (err != 0) {
		fprintf(stderr, "[-] Error decrypting cipher text\n");
		return 0;
	}

	return 1;
}

void randombytes(unsigned char *random_bytes, unsigned long long len)
{
	srand((unsigned int)time(NULL));
	// TODO: Fill random_bytes with len bytes of better randomness
	// This isn't needed for any part of tweetnacl used yet
	unsigned long long i;
	for (i=0; i<len; i++) {
		random_bytes[i] = rand();
	}
}



