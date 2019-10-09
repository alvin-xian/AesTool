#ifndef HEADER_AES_H
#define HEADER_AES_H

# include <stddef.h>

# define AES_ENCRYPT     1
# define AES_DECRYPT     0

# define AES_MAXNR 14
# define AES_BLOCK_SIZE 16

struct aes_key_st {
# ifdef AES_LONG
    unsigned long rd_key[4 * (AES_MAXNR + 1)];
# else
    unsigned int rd_key[4 * (AES_MAXNR + 1)];
# endif
    int rounds;
};

typedef struct aes_key_st AES_KEY;
typedef unsigned char BYTE;            // 8-bit byte
typedef unsigned int WORD;             // 32-bit word, change to "long" for 16-bit machines

#ifdef __cplusplus
extern "C"{//编译的时候一直报找不到以下函数，报lnk2019，估计就是编译器将函数名按照C++的方式编译了，所以找不到符号链接。加上extern "C"就可以了
#endif

extern int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);
extern int AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

extern void AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);
extern void AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

extern void AES_ecb_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key,
					const int enc);

extern void AES_cbc_encrypt(const unsigned char *in, unsigned char *out,
                     size_t length, const AES_KEY *key,
                     unsigned char *ivec, const int enc);
#ifdef __cplusplus
};
#endif
#endif


