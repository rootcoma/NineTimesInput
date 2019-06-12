#include <Windows.h>
#include <stdio.h>
#include "tweetnacl.h"
#include "encryption.h"

#define CRYPTO_CONTEXT_ID "kxm"

// XXX: Temporary keys, need to find a way to transfer keys safely, or allow user to
//      select new keys.
// xsalsa20_xor key, 32 bytes
const unsigned char kxm_crypto_k[crypto_stream_KEYBYTES] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

// nonce 24 bytes
const unsigned char kxm_crypto_n[crypto_stream_NONCEBYTES] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
};


// cipher should be the same size as plaintext
int output_encrypt(unsigned char* cipher/*out*/, const char *plaintext,
        const unsigned long long len)
{
    const int err = crypto_stream_xor(cipher, (const unsigned char*)plaintext,
            len, kxm_crypto_n, kxm_crypto_k);
    if (err != 0) {
        fprintf(stderr, "[-] Could not encrypt string: %.32s\n", plaintext);
        return 0;
    }
    return 1;
}


void randombytes(unsigned char *random_bytes/*out*/, unsigned long long len)
{
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
}

