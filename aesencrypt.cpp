#include "aesencrypt.h"
#include <openssl/aes.h>
#include <openssl/des.h>
#include <assert.h>
#include <iostream>
#include "common/hex.h"
#include "common/base64.h"

std::string AesEncrypt::AesEcbEncrypt(const std::string &in, const std::string key, AesEncrypt::AesPadding paddingtype, FixMode mode)
{
    std::string out;
    AES_KEY aes_key;
    if (AES_set_encrypt_key((const unsigned char*)key.c_str(), key.length()*8, &aes_key) < 0){
        return std::string("");
    }

    std::string src_paded = addPad(in, AES_BLOCK_SIZE, paddingtype);
    unsigned char *p = (unsigned char *)src_paded.c_str();
    assert((src_paded.length() % AES_BLOCK_SIZE) == 0);
    for (int i = 0; i < src_paded.length() / AES_BLOCK_SIZE; ++i){
        unsigned char dec[AES_BLOCK_SIZE+1] = {'\0'};
        AES_encrypt(p + i * AES_BLOCK_SIZE, dec, &aes_key);//获取到区块加密的
        switch (mode) {
        case Fix_Base64_Block:{//Base64区块加密
            std::string base64_str = base64_encode(dec, AES_BLOCK_SIZE);
            out.append(base64_str);
        }
            break;
        default:
            out.append((char *)dec, AES_BLOCK_SIZE);
            break;
        }
    }
    switch (mode) {
    case Fix_Base64:{
        out = base64_encode((unsigned char *)out.c_str(), src_paded.length());
    }break;
    default:
        break;
    }
    return out;
}

std::string AesEncrypt::AesEcbDecrypt(const std::string &dec, const std::string &key, AesEncrypt::AesPadding paddingtype, FixMode mode)
{
    std::string out;
    AES_KEY aes_key;
    if (AES_set_decrypt_key((const unsigned char*)key.c_str(), key.length()*8, &aes_key) < 0){
        std::cout<<__FUNCTION__<<"AES_set_encrypt_key error"<<std::endl;
        return "";
    }
    std::string dec_uncode = dec;
    char *p = (char *)dec_uncode.c_str();
    if(mode == Fix_Base64_Block){
        int block_length = BASE64_ENCODE_OUT_SIZE(AES_BLOCK_SIZE);//base64每个区块的长度
        bool benough = dec_uncode.length() % block_length == 0;
        int block_count = benough?(dec_uncode.length() / block_length) : (dec_uncode.length()/block_length +1);
        for (int i = 0; i < block_count; ++i) {
            int length = block_length;
            if(!benough && i== (block_count-1)){
                length = (dec_uncode.length() - (i)*block_length);
            }
            std::string src((p + i*block_length), length);
            unsigned int base64_out_length;
            std::string base64_out_str = base64_decode(src, &base64_out_length);//base64 解码后的
            char out_Decrypt[AES_BLOCK_SIZE+1] = {0};
            AES_decrypt((unsigned char *)base64_out_str.c_str(), (unsigned char *)out_Decrypt, &aes_key);
            out.append(out_Decrypt, AES_BLOCK_SIZE);
        }
    }else if(mode == Fix_Base64){
        unsigned int base64_out_length;
        std::string base64_out_str = base64_decode(dec_uncode, &base64_out_length);//base64 解码后的
        unsigned char *p = (unsigned char *)base64_out_str.c_str();
        for (int i = 0; i < base64_out_length/AES_BLOCK_SIZE; ++i) {
            char out_Decrypt[AES_BLOCK_SIZE+1] = {0};
            AES_decrypt((unsigned char *)(p + AES_BLOCK_SIZE*i), (unsigned char *)out_Decrypt, &aes_key);
            out.append(out_Decrypt, AES_BLOCK_SIZE);
        }
    }
    std::string out_str = removePad(out, AES_BLOCK_SIZE, paddingtype);
    return out_str;
}

std::string AesEncrypt::AesCbcEncrypt(const std::string &in, const std::string &key, const std::string &ivec, AesEncrypt::AesPadding paddingtype, FixMode mode)
{
    std::string out;
    AES_KEY aes_key;
    if (AES_set_encrypt_key((const unsigned char*)key.c_str(), key.length()*8, &aes_key) < 0){
        //密码错误
        return std::string("");
    }
    if(ivec.length() < AES_BLOCK_SIZE){
        //向量长度错误
        std::cout<<__FUNCTION__<<"ivec length error, need more than "<<AES_BLOCK_SIZE<<ivec.length()<<std::endl;
    }
    std::string src_paded = addPad(in, AES_BLOCK_SIZE, paddingtype);
    //    std::cout<<"src_paded"<<QByteArray(src_paded.c_str())<<std::endl;
    if(src_paded.empty()){
        std::cout<<__FUNCTION__<<"pading error."<<std::endl;//添加补充字符失败
    }

    assert((src_paded.length() % AES_BLOCK_SIZE) == 0);

    unsigned char *dec = (unsigned char*)malloc(src_paded.length()+1);
    memset(dec, 0x0, src_paded.length()+1);

    unsigned char ivecArray[AES_BLOCK_SIZE*2];
    memset((unsigned char*)ivecArray, 0x00 ,AES_BLOCK_SIZE*2);
    strcpy( (char*)ivecArray, ivec.c_str());

    AES_cbc_encrypt((unsigned char*)src_paded.c_str(), dec, src_paded.length(), &aes_key, (unsigned char*)ivecArray, AES_ENCRYPT);//加密

    out.append((char *)dec, src_paded.length());//必须加上长度，否则出现0的时候，被折断字符串
    if(mode == Fix_Base64 || mode == Fix_Base64_Block){
        out = base64_encode((unsigned char *)out.c_str(), src_paded.length());
    }else if(mode == Fix_NULL){
#if 0//base64
        QByteArray data = QByteArray(out.c_str(), src_paded.length()).toBase64(QByteArray::Base64Encoding);
        out = data.toStdString();
#endif
    }
    free(dec);
    return out;

}

std::string AesEncrypt::AesCbcDecrypt(const std::string &dec, const std::string &key, const std::string &ivec, AesEncrypt::AesPadding paddingtype, FixMode mode)
{
    std::string out;
    AES_KEY aes_key;
    if (AES_set_decrypt_key((const unsigned char*)key.c_str(), key.length()*8, &aes_key) < 0){
        std::cout<<__FUNCTION__<<"AES_set_encrypt_key error"<<std::endl;
        return "";
    }
    if(ivec.length() < AES_BLOCK_SIZE){
        std::cout<<__FUNCTION__<<"ivec length error, need more than 16"<<ivec.length()<<std::endl;
    }
    std::string in_data;
    unsigned int in_data_length=0;
    if(mode == Fix_NULL){
#if 0//base64
        QByteArray data = QByteArray::fromBase64(dec.c_str(), QByteArray::Base64Encoding);
        std::string dec_temp = data.toStdString();
#else
        //不做处理
        in_data = dec;
        in_data_length = dec.length();
#endif
    }else if(mode == Fix_Base64 || mode == Fix_Base64_Block){
        in_data = base64_decode(dec, &in_data_length);//base64 解码后的
    }
    unsigned char *src = (unsigned char*)malloc(in_data_length + 1);
    memset(src, 0, in_data_length + 1);

    unsigned char ivecArray[AES_BLOCK_SIZE*2];
    memset((unsigned char*)ivecArray, 0x00 ,AES_BLOCK_SIZE*2);
    strcpy( (char*)ivecArray, ivec.c_str());
    AES_cbc_encrypt((unsigned char*)in_data.c_str(), src, in_data_length, &aes_key, (unsigned char*)ivecArray, AES_DECRYPT);//解密
    out.append((char*)src, in_data_length);
    out = removePad(out, AES_BLOCK_SIZE, paddingtype);
    free(src);

    return out;
}


std::string AesEncrypt::addPad(const std::string &in, int blockSize, AesEncrypt::AesPadding type)
{
    std::string in_bak = in;
    int paddinglength = blockSize - in_bak.length() % blockSize;//填充长度
    if(paddinglength != blockSize){
        assert(paddinglength<blockSize);
        for(int i=0; i<paddinglength; i++){
            switch (type) {
            case ISO10126Padding:
                if(i == (paddinglength-1)){
                    in_bak += paddinglength;//最后一位为填充的长度
                    std::cout<<"ISO10126Padding pad end"<<paddinglength<<std::endl;
                }else{
                    char a = rand()%0xff;
                    in_bak += a;//其他位置填充0~0xff随机数
                    std::cout<<"ISO10126Padding pad at"<<i<<a<<std::endl;
                }
                break;
            case PKCS5Padding://虽然blcoksize只能为8，但是为了防止出错，仅仅警告
                if(blockSize != 8){
                    std::cout<<__FUNCTION__<<"warning: blocksize should be 8 while paddingtype is PKCS5Padding."<<std::endl;
                }
            case PKCS7Padding:
                in_bak += paddinglength;//所有位置都是填充的长度
                break;
            case ZEROPadding:
                in_bak += (char)0x00;//所有位置都是填充0
                break;
            case NoPadding:
                std::cout<<__FUNCTION__<<"error: data's length is invalid when NoPadding."<<in.length()<<paddinglength<<std::endl;
                return "";
            default:
                break;
            }
        }
    }else{
        switch (type) {
        case ISO10126Padding:
        case PKCS7Padding:
        case PKCS5Padding:
            paddinglength = blockSize;
            for(int i=0; i<blockSize; i++){
                in_bak += blockSize;
            }
            break;
        case ZEROPadding:
            paddinglength = blockSize;
            for(int i=0; i<blockSize; i++){
                in_bak += (char)0x00;
            }
            break;
            break;
        case NoPadding:
        default:
            paddinglength = 0;
            break;
        }
    }
    return in_bak;
}

std::string AesEncrypt::removePad(const std::string &in, int blockSize, AesEncrypt::AesPadding type)
{
    std::string in_bak = in;
    switch (type) {
    case ISO10126Padding:
    case PKCS5Padding://虽然blcoksize只能为8，但是为了防止出错，仅仅警告
    case PKCS7Padding:{
        std::cout<<__FUNCTION__<<in.c_str()<<std::endl;
        bool bpadOneBlock = true;//是否是刚好填充了一个区块，这种情况只会出现在密文刚好为blocksize整数倍下;
        for(int i=(in.length() - blockSize); i<in.length(); i++){
            int value = in.at(i);
            int padingvalue = blockSize;
            if(value != padingvalue){
                bpadOneBlock = false;
                break;
            }

        }
        int paddinglength = 0;
        if(bpadOneBlock){
            paddinglength = blockSize;//填充长度为一个区块
        }else{
            paddinglength = (int)in.at(in.length()-1);//填充长度为最后一位的值
            if(paddinglength>=blockSize){
                std::cout<<__FUNCTION__<<"error: data is invalid."<<paddinglength<< blockSize<<std::endl;
                return std::string();
            }
        }
        std::cout<<__FUNCTION__<<"paddinglength"<<paddinglength<<std::endl;
        if((in.length() % blockSize) != 0){
            std::cout<<__FUNCTION__<<"error: data's length is invalid."<<in.length()<< blockSize<<std::endl;
            break;
        }
        for(int i=0; i<paddinglength; i++){
            in_bak.pop_back();
        }
    }
        break;
    case ZEROPadding://填充0的不用管
    case NoPadding:
    default:
        break;
    }

    return in_bak;
}

