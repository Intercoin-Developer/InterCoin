// Copyright (c) 2015-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// C++ wrapper around ctaes, a constant-time AES implementation

#ifndef BITCOIN_CRYPTO_AES_H
#define BITCOIN_CRYPTO_AES_H

extern "C" {
#include "crypto/ctaes/ctaes.h"
}

static const int AES_BLOCKSIZE = 16;
static const int AES128_KEYSIZE = 16;
static const int AES256_KEYSIZE = 32;

/** An encryption class for AES-128. */
class AES128Encrypt
{
private:
    AES128_ctx ctx;

public:
    AES128Encrypt(const unsigned char key[16]);
    ~AES128Encrypt();
    void Encrypt(unsigned char ciphertext[16], const unsigned char plaintext[16]) const;
};

/** A decryption class for AES-128. */
class AES128Decrypt
{
private:
    AES128_ctx ctx;

public:
    AES128Decrypt(const unsigned char key[16]);
    ~AES128Decrypt();
    void Decrypt(unsigned char plaintext[16], const unsigned char ciphertext[16]) const;
};

/** An encryption class for AES-256. */
class AES256Encrypt
{
private:
    AES256_ctx ctx;

public:
    AES256Encrypt(const unsigned char key[32]);
    ~AES256Encrypt();
    void Encrypt(unsigned char ciphertext[16], const unsigned char plaintext[16]) const;
};

/** A decryption class for AES-256. */
class AES256Decrypt
{
private:
    AES256_ctx ctx;

public:
    AES256Decrypt(const unsigned char key[32]);
    ~AES256Decrypt();
    void Decrypt(unsigned char plaintext[16], const unsigned char ciphertext[16]) const;
};

class AES256ITCEncrypt
{
public:
    AES256ITCEncrypt(const unsigned char key[AES256_KEYSIZE], const unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES256ITCEncrypt();
    int Encrypt(const unsigned char* data, int size, unsigned char* out) const;

private:
    const AES256Encrypt enc;
    const bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES256ITCDecrypt
{
public:
    AES256ITCDecrypt(const unsigned char key[AES256_KEYSIZE], const unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES256ITCDecrypt();
    int Decrypt(const unsigned char* data, int size, unsigned char* out) const;

private:
    const AES256Decrypt dec;
    const bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES128ITCEncrypt
{
public:
    AES128ITCEncrypt(const unsigned char key[AES128_KEYSIZE], const unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES128ITCEncrypt();
    int Encrypt(const unsigned char* data, int size, unsigned char* out) const;

private:
    const AES128Encrypt enc;
    const bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES128ITCDecrypt
{
public:
    AES128ITCDecrypt(const unsigned char key[AES128_KEYSIZE], const unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES128ITCDecrypt();
    int Decrypt(const unsigned char* data, int size, unsigned char* out) const;

private:
    const AES128Decrypt dec;
    const bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

#endif // BITCOIN_CRYPTO_AES_H
