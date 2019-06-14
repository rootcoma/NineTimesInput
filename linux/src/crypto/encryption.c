#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption.h"
#include "tweetnacl.h"
#include "config.h"

// Should be 32 and 24
#define CRYPTO_SECRET_MIN_LEN 12
#define CRYPTO_NONCE_MIN_LEN 5

// xsalsa20_xor key, 32 bytes
static unsigned char kxm_crypto_k[crypto_stream_KEYBYTES] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

// 24 bytes
static unsigned char kxm_crypto_n[crypto_stream_NONCEBYTES] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
};


// Note: This is intented as a test and should be changed in the future
int crypto_setup()
{
	unsigned char hash_storage[crypto_hash_BYTES] = { 0 };
	
	// Hash 'secret' from configuration and set as key
	char *p = config_get_value("secret");
	if (p[0] == '\0') {
		fprintf(stderr, "[!] Configuration value for 'secret' is not set\n");
		return 0;
	}
	size_t str_len = strlen(p);
	if (str_len < CRYPTO_SECRET_MIN_LEN) {
		fprintf(stderr, "[!] Configuration value for 'secret' must be >= %d bytes\n",
				CRYPTO_SECRET_MIN_LEN);
		return 0;
	}
	crypto_hash(hash_storage, (unsigned char *)p, str_len);
	// truncation from 64 to 32 bits
	memcpy(kxm_crypto_k, hash_storage, crypto_stream_KEYBYTES);

	// Hash 'nonce' from configuration and set as key
	p = config_get_value("nonce");
	if (p[0] == '\0') {
		fprintf(stderr, "[!] Configuration value for 'nonce' is not set\n");
		return 0;
	}
	str_len = strlen(p);
	if (str_len < CRYPTO_NONCE_MIN_LEN) {
		fprintf(stderr, "[!] Configuration value for 'nonce' must be >= %d bytes\n",
				CRYPTO_NONCE_MIN_LEN);
		return 0;
	}
	crypto_hash(hash_storage, (unsigned char *)p, str_len);
	// truncation from 64 to 24 bits
	memcpy(kxm_crypto_n, hash_storage, crypto_stream_NONCEBYTES);
	return 1;
}


int crypto_xor_plaintext(char *plaintext, const unsigned char *cipher,
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


