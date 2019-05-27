# AesTool AES加密工具
> A aes tool of aes128、aes192、aes256 on ecb/cbc which used openllssl api.    
> 一个支持aes128、aes192、aes256 on ecb/cbc加密的工具；  
* 支持ISO10126Padding、PKCS5Padding/PKCS7Padding、ZEROPadding、NoPadding等填充模式；  
* 支持base64对加密内容进行转换。  


# 工作环境
* Qt5.7.1 msv2013 32bit  
* openssl  

# 接口参考
详见[aesencrypt.h ](https://github.com/alvin-xian/AesTool/blob/master/aesencrypt.h)

 

# 心得体会
 * 1、AES128/AES192/AES256的判断取决于密钥长度，密钥128位就是AES128
 * 2、AES的区块长度固定位128位，即16字节
 * 3、ECB是区块加密，每128位（16字节）的区块加密一次，然后再合并，CBC等是可以整体加密，因为AES算法里需要关联上一个区块
 * 4、填充是为了将要加密的明文填充为128位（16字节）的倍数，如果明文长度刚好为区块的倍数，除了nopadding/zeropadding之外，都需要填充多一个区块
 * 5、AES加密会出现“\0”,需要用hex/base64进行转化
 * 6、AES加密后密文的长度与明文（填充后）长度一致，主要原因是AES加密进行的是一系列的异或操作与循环移位，并没用进行扩展变换等
 * 7、CBC/OFB/CTR/CFB都需要向量IV，向量IV需要大于等于AES_BLOCK_SIZE
 
 # 工具界面预览
![img](https://github.com/alvin-xian/AesTool/blob/master/software.jpg)

