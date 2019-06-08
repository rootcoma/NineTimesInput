#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>
#include "tweetnacl.h"
#include "encryption.h"
#include "config.h"
/**
 * TODO:
 *  - Come up with method of setting k and n that does not ruin security and
 *        also is easy enough for a user to configure
 */


#define CRYPTO_CONTEXT_ID "kxm"

// Note: These should be 32 and 24
#define SECRET_MIN_LEN 12
#define NONCE_MIN_LEN 5

// xsalsa20_xor key, 32 bytes
unsigned char kxm_crypto_k[crypto_stream_KEYBYTES] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

// nonce 24 bytes
unsigned char kxm_crypto_n[crypto_stream_NONCEBYTES] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
};


int crypto_setup()
{
    unsigned char hash_storage[crypto_hash_BYTES] = { 0 };
    const char *p = config_get_value("secret");
    if (p[0] == '\0') {
        fprintf(stderr, "[!] Configuration value for 'secret' is not set\n");
        return 0;
    }
    size_t str_len = strlen(p);
    if (str_len < SECRET_MIN_LEN) {
        fprintf(stderr, "[!] Configuration value 'secret' must be >= %d\n",
                SECRET_MIN_LEN);
        return 0;
    }
    crypto_hash(hash_storage, (unsigned char *)p, str_len);
    // 64 bytes truncated to 32
    memcpy(kxm_crypto_k, hash_storage, crypto_stream_KEYBYTES);
    p = config_get_value("nonce");
    if (p[0] == '\0') {
        fprintf(stderr, "[!] Configuration value for 'nonce' is not set\n");
        return 0;
    }
    str_len = strlen(p);
    if (strlen(p) < NONCE_MIN_LEN) {
        fprintf(stderr, "[!] Configuration value 'nonce' must be >= %d\n",
                NONCE_MIN_LEN);
        return 0;
    }
    crypto_hash(hash_storage, (unsigned char *)p, str_len);
     // 64 bytes truncated to 24
    memcpy(kxm_crypto_n, hash_storage, crypto_stream_NONCEBYTES);
    fprintf(stderr, "[+] Set up crypto\n");

    return 1;
}


// cipher should be the same size as plaintext
int crypto_xor_plaintext(unsigned char* cipher/*out*/, const char *plaintext,
        const unsigned long long len)
{
    const int err = crypto_stream_xor(cipher, (const unsigned char *)plaintext,
            len, kxm_crypto_n, kxm_crypto_k);
    if (err != 0) {
        fprintf(stderr, "[-] Could not encrypt string: %.32s\n", plaintext);
        return 0;
    }
    return 1;
}


// cipher should be the same size as plaintext
int crypto_xor_cipher(char* plaintext/*out*/, const unsigned char *cipher,
        const unsigned long long len)
{
    const int err = crypto_stream_xor((unsigned char *)plaintext, cipher,
            len, kxm_crypto_n, kxm_crypto_k);
    if (err != 0) {
        fprintf(stderr, "[-] Could not decrypt string: %.32s\n", cipher);
        return 0;
    }
    return 1;
}


void randombytes(unsigned char *random_bytes/*out*/, unsigned long long len)
{
#ifdef _WIN32
    HCRYPTPROV crypt_context_handle;
    // If this parameter is NULL, the user default provider is used.
    // For more information, see 
    // https://msdn.microsoft.com/ad1ff45c-7d02-431b-a287-e9db765476ce.
    // For a list of available cryptographic providers, see
    // https://msdn.microsoft.com/97e9a708-83b5-48b3-9d16-f7b54367dc4e
    // TODO: Change defaults if necessary
    //       I think NEWKEYSET as default would be better
#define GLOBAL_CONTEXT CryptAcquireContext(&crypt_context_handle,\
        CRYPTO_CONTEXT_ID, NULL, PROV_RSA_FULL, 0)

#define NEW_CONTEXT CryptAcquireContext(&crypt_context_handle,\
        CRYPTO_CONTEXT_ID, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)

    if (!NEW_CONTEXT && !GLOBAL_CONTEXT) {
        fprintf(stderr, "Failed to acquire crypt context\n");
        return;
    }
    if (!CryptGenRandom(crypt_context_handle, len, random_bytes)) {
        fprintf(stderr, "Failed to generate random numbers length: %llu\n",
                len);
        return;
    }
    CryptReleaseContext(crypt_context_handle, 0);
#else
    // TODO increase randomness
    srand((unsigned int)time(NULL));
    unsigned long long i;
    for(i=0; i<len; i++) {
        random_bytes[i] = rand()%256;
    }
#endif
}
