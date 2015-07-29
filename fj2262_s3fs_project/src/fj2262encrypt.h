#ifndef S3FS_FJ2262ENCRYPT_H_
#define S3FS_FJ2262ENCRYPT_H_
//File Encryption && Decryption
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include "cryptopp/modes.h"
#include "cryptopp/files.h"
using CryptoPP::FileSource;
using CryptoPP::FileSink;
#include "cryptopp/aes.h"
using CryptoPP::AES;
#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
#include "cryptopp/default.h"
using CryptoPP::DefaultEncryptorWithMAC;
#include "cryptopp/ccm.h"
using CryptoPP::CBC_Mode;

int EncryptFile(int fd);
int DecryptFile(int inputValue);

#endif
