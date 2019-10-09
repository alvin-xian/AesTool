#ifndef AesEncrypt_H
#define AesEncrypt_H
#include "stdio.h"
#include "string"

/**
 * @brief The AesEncrypt class
 * aes加密总结
 * 1、AES128/AES192/AES256的判断取决于密钥长度，密钥128位就是AES128
 * 2、AES的区块长度固定位128位，即16字节
 * 3、ECB是区块加密，每128位（16字节）的区块加密一次，然后再合并，CBC等是可以整体加密，因为AES算法里需要关联上一个区块
 * 4、填充是为了将要加密的明文填充为128位（16字节）的倍数，如果明文长度刚好为区块的倍数，除了nopadding/zeropadding之外，都需要填充多一个区块
 * 5、AES加密会出现“\0”,需要用hex/base64进行转化
 * 6、AES加密后密文的长度与明文（填充后）长度一致，主要原因是AES加密进行的是一系列的异或操作与循环移位，并没用进行扩展变换等
 * 7、CBC/OFB/CTR/CFB都需要向量IV，向量IV需要大于等于AES_BLOCK_SIZE
 * *************************************************
 * AES,高级加密标准（英语：Advanced Encryption Standard，缩写：AES），
 * 在密码学中又称Rijndael加密法，是美国联邦政府采用的一种区块加密标准。
 * 这个标准用来替代原先的DES，已经被多方分析且广为全世界所使用。
 * 严格地说，AES和Rijndael加密法并不完全一样（虽然在实际应用中二者可以互换），
 * 因为Rijndael加密法可以支持更大范围的区块和密钥长度：AES的区块长度固定为128 bite比特，密钥长度则可以是128，192或256比特；
 * 而Rijndael使用的密钥和区块长度可以是32位的整数倍，以128位为下限，256比特为上限。
 * 包括AES-ECB,AES-CBC,AES-CTR,AES-OFB,AES-CFB
 * *************************************************
 * @author alvin
 * @date 2019-5-20
 */
class AesEncrypt
{
private:
    AesEncrypt();
public:
    /**
        【ISO10126Padding】	ISO10126 填充字符串由一个字节序列组成，此字节序列的最后一个字节填充字节序列的长度，其余字节填充随机数据。
        下面的示例演示此模式的工作原理。
        BLOCK_SIZE：8Byte（范围1~255）
        1、数据： FF FF FF FF FF FF FF FF FF （9Byte）
        ISO10126 填充： FF FF FF FF FF FF FF FF FF 7D 2A 75 EF F8 EF 07（16Byte）
        2、数据： FF FF FF FF FF FF FF FF FF （8Byte）
        ISO10126 填充： FF FF FF FF FF FF FF FF 08 08 08 08 08 08 08 08 08（16Byte）

        【PKCS7Padding】 PKCS #7 填充字符串由一个字节序列组成，每个字节填充该字节序列的长度。
        下面的示例演示这些模式的工作原理。
        BLOCK_SIZE：8Byte（范围1~255）
        1、数据： FF FF FF FF FF FF FF FF FF （9Byte）
        PKCS7 填充： FF FF FF FF FF FF FF FF FF 07 07 07 07 07 07 07（16Byte）
        2、数据： FF FF FF FF FF FF FF FF FF （8Byte）
        PKCS7 填充： FF FF FF FF FF FF FF FF 08 08 08 08 08 08 08 08 08（16Byte）

        【PKCS5Padding】 PKCS #5 相当于BLOCK_SIZE固定为8字节的PKCS7，是PKCS7的子集

        【ZEROPadding】	填充字符串由设置为零的字节组成。
        下面的示例演示这些模式的工作原理。
        BLOCK_SIZE：8Byte（范围1~255）
        1、数据： FF FF FF FF FF FF FF FF FF（9Byte）
        ZEROPadding 填充： FF FF FF FF FF FF FF FF FF 00 00 00 00 00 00 00（16Byte）
        2、数据： FF FF FF FF FF FF FF FF FF （8Byte）
        ZEROPadding 填充： FF FF FF FF FF FF FF FF 00 00 00 00 00 00 00 00（16Byte）

        【NoPadding】 不需要填充，但是明文必须是BLOCK_SIZE的倍数

        注：BLOCK_SIZE代表加密区块是多少个字节， AES_BLOCK_SIZE=16Byte
    **/
    enum AesPadding{
        ISO10126Padding,
        PKCS5Padding,
        PKCS7Padding,
        ZEROPadding,
        NoPadding
    };

    enum FixMode{
        Fix_NULL,
        Fix_Base64,
        Fix_Base64_Block,
//        Fix_Hex,
//        Fix_Hex_Block
    };

    /**
         * @brief AesEcbEncrypt
         * AES加密，ECB模式。
         * @param in 需要加密的数据
         * @param key 长度为16/24/32字节，对应aes128/aes192/aes256
         * @param paddingtype
         * @return 加密后的数据
         */
    static std::string AesEcbEncrypt(const std::string &in,
                                     const std::string key,
                                     AesPadding paddingtype = PKCS7Padding, FixMode mode = Fix_Base64);
    static std::string AesEcbDecrypt(const std::string& dec,
                                     const std::string& key,
                                     AesPadding paddingtype = PKCS7Padding, FixMode mode = Fix_Base64);

    /**
         * @brief AesCbcEncrypt
         * AES加密，CBC模式，需要密钥+向量
         * @param in 需要加密的数据
         * @param key 密钥 长度为16/24/32字节，对应aes128/aes192/aes256
         * @param ivec 向量，需要大于等于AES_BLOCK_SIZE
         * @param paddingtype
         * @return
         */
    static std::string AesCbcEncrypt(const std::string &in,
                                     const std::string &key,
                                     const std::string &ivec,
                                     AesPadding paddingtype = ZEROPadding,
                                     FixMode mode = Fix_Base64);
    static std::string AesCbcDecrypt(const std::string& dec,
                                     const std::string& key,
                                     const std::string & ivec,
                                     AesPadding paddingtype = ZEROPadding,
                                     FixMode mode = Fix_Base64);


private:
    //补全原数据，需要在加密前处理
    static std::string addPad(const std::string &in, int blockSize, AesPadding type);
    //在加密后，删除补全的数据
    static std::string removePad(const std::string &in, int blockSize, AesPadding type);
};


#endif // AesEncrypt_H
