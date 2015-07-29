#include <stdio.h>
#include <sstream>
#include <string>
#include "fj2262encrypt.h"
#include <stdint.h>
using namespace std;

int len;
HexEncoder hexEnc;
const char *buffa;
string encoded_iv, encoded_cipher, recovered, cipher, decodedCipher;

int ReturnHiddenKey(const char* nameOfFile, string secretKey)
{
	/*
		Use HexEncoder to read my secret key and then
		encode it and return it. Uses Put and Get to Put
		the string contents into a byte array and output it
		using Get to return this
	*/

	ifstream fileName;
	fileName.open(nameOfFile);
	// string secretKey;
	if(fileName.is_open())
	{
		//nameOfFile << fileOutput;
		getline(fileName, secretKey);
	}
	return 0;
}

int EncryptFile(int fd)
{
	//Get the Secret Key from the ReturnHiddenKey function
	string myKey;
	ReturnHiddenKey("/home/jonny/Documents/mySecretKey.txt", myKey);

	//Declare Crypto Variables for RandomNum/Encryption
	AutoSeededRandomPool prng;
	CBC_Mode< AES >::Encryption e;

	//Get the length of the file, used for sizing variables
	int fileLength = lseek(fd, 0, SEEK_END);
	char fileContents[fileLength];

	//Read the contents of the file the the char array 
	pread(fd, fileContents, fileLength, 0);

	//Convert char array to a string
	string fileContentsToString(reinterpret_cast<char*>(fileContents), fileLength);

	//Decrypt the key
	string unencryptedKey;
	StringSource(myKey, true, 
		new HexDecoder(
			new StringSink(unencryptedKey)));

	//Store the encrypted Key in bytes form 
	const byte* encryptedKeyInBytes = (const byte*) unencryptedKey.data();
	
	//Initialzie and create the IV
	byte IV[AES::BLOCKSIZE];
	prng.GenerateBlock(IV, sizeof(IV));

	//Encode the IV and output it to a string
	string encryptedIV;
	StringSource(IV, sizeof(IV), true, 
		new HexEncoder(
			new StringSink(encryptedIV)));

	//Set the Encryption Key with the SecretKey and IV, both in bytes form	
	e.SetKeyWithIV(encryptedKeyInBytes, 16, IV);

	//Encrypt the File
	string cipherContents;
	StringSource file(fileContentsToString, true, 
		new StreamTransformationFilter(e,
			new StringSink(cipherContents)));

	//Encode the Cipher
	string encryptedCipherContents;
	StringSource(cipherContents, true, 
		new HexEncoder(
			new StringSink(encryptedCipherContents)));

	string finalOutput;
	finalOutput = encryptedIV + encryptedCipherContents;
	const char *bufContents = finalOutput.c_str();
	pwrite(fd, bufContents, finalOutput.length(), 0);

return 0;
}

int DecryptFile(int fd)
{
	//Get length of file contents
	int fileLength = lseek(fd, 0, SEEK_END);
	//Initialize file contents variable
	char fileContents[fileLength];

	//Read contents of file to char	
	pread(fd, fileContents, fileLength, 0);
	string fileContentsToString(reinterpret_cast<char*>(fileContents), fileLength);

	//Get the secret key from the location
	string myKey;
	ReturnHiddenKey("/home/jonny/Documents/mySecretKey.txt", myKey);

	//Load the secret key and hex decode it
	string decodedKey;
	StringSource(myKey, true, new HexDecoder(new StringSink(decodedKey)));
	const byte* decodedKeyInBytes = (const byte*) decodedKey.data();

	//Store the encrypted IV
	string encryptedIv;
	encryptedIv = fileContentsToString.substr(0,32);
	
	//Store the enrypted file contents
	string encryptedContents;
	encryptedContents = fileContentsToString.substr(encryptedIv.length());

	//Decode the encoded IV into a string then into a byte variable
	string decodedIv;
	StringSource(encryptedIv, true, new HexDecoder(new StringSink(decodedIv)));
	const byte* decryptedByteIV = (const byte*) decodedIv.data();

	//Decode the encoded contents into a string
	string decryptedContents;
	StringSource(encryptedContents, true, new HexDecoder(new StringSink(decryptedContents)));

	//Declare decryption and set decryption key with key and IV
	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV(decodedKeyInBytes, sizeof(decodedKeyInBytes)*2, decryptedByteIV);

	//Get the final output using the decrypted contents and cipher block
	string finalOutput;
	StringSource ss(decryptedContents, true, 
		new StreamTransformationFilter(d, 
			new StringSink (finalOutput)));

	//Output the file contents to a file, truncate by the length
	//of the correct character count.
	const char *fconts = finalOutput.c_str();
	pwrite(fd, fconts, finalOutput.length(), 0);
	ftruncate(fd, finalOutput.length());
	return 0;
}



