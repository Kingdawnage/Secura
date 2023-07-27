#include "Cryptograph.h"
#include <fstream>
#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/pem.h>

//Function to generate a secure encryption key using PBKDF2
std::vector<unsigned char> Keygen(const std::string& password, const std::vector<unsigned char>& salt)
{
	const int iterations = 10000;
	const int keyLength = 32;

	std::vector<unsigned char> key(keyLength);
	PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.length()), salt.data(),
		static_cast<int>(salt.size()), iterations, EVP_sha256(), keyLength, key.data());

	return key;
}

void FileEncrypt(const std::string& inputPath, const std::string& outputPath, const std::string& password)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	// Generate IV
	std::vector<unsigned char> IV(EVP_MAX_IV_LENGTH);
	if (RAND_bytes(IV.data(), EVP_MAX_IV_LENGTH) != 1) {
		return;
	}

	//Generate salt for key derivation
	std::vector<unsigned char> salt(EVP_MAX_KEY_LENGTH);
	if (RAND_bytes(IV.data(), EVP_MAX_IV_LENGTH) != 1) {
		return;
	}

	//Generate encryption key using PBKDF2
	std::vector<unsigned char> key = Keygen(password, salt);

	//Initialize encryption
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), IV.data()) != 1) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	//Open Input and Output Files
	std::ifstream inputFile(inputPath, std::ios::binary);
	std::ofstream outputFile(outputPath, std::ios::binary);
	if (!inputFile || !outputFile) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	//Write salt and IV to output file
	outputFile.write(reinterpret_cast<const char*>(salt.data()), EVP_MAX_KEY_LENGTH);
	outputFile.write(reinterpret_cast<const char*>(IV.data()), EVP_MAX_IV_LENGTH);

	std::vector<unsigned char> buffer(EVP_CIPHER_CTX_block_size(ctx));
	int bytesProcessed = 0;
	while (inputFile.good()) {
		inputFile.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
		int bytesRead = static_cast<int>(inputFile.gcount());

		if (EVP_EncryptUpdate(ctx, buffer.data(), &bytesProcessed, buffer.data(), bytesRead) != 1) {
			EVP_CIPHER_CTX_free(ctx);
			return;
		}

		outputFile.write(reinterpret_cast<const char*>(buffer.data()), bytesProcessed);
	}

	//Finalize encryption
	if (EVP_EncryptFinal_ex(ctx, buffer.data(), &bytesProcessed) != 1) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	outputFile.write(reinterpret_cast<const char*>(buffer.data()), bytesProcessed);

	//Clear context
	EVP_CIPHER_CTX_free(ctx);
}

void FileDecrypt(const std::string& inputPath, const std::string& outputPath, const std::string& password)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	//Read salt and IV from input file
	std::ifstream inputFile(inputPath, std::ios::binary);
	if (!inputFile) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	std::vector<unsigned char> salt(EVP_MAX_KEY_LENGTH);
	inputFile.read(reinterpret_cast<char*>(salt.data()), EVP_MAX_KEY_LENGTH);

	std::vector<unsigned char> IV(EVP_MAX_IV_LENGTH);
	inputFile.read(reinterpret_cast<char*>(IV.data()), EVP_MAX_IV_LENGTH);

	//Derive the decryption key using the provided password and salt
	std::vector<unsigned char> key = Keygen(password, salt);

	//Initialize decryption
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), IV.data()) != 1) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	//Set padding to false
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	//Open output file
	std::ofstream outputFile(outputPath, std::ios::binary);
	if (!outputFile) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	//Decrypt and write data
	std::vector<unsigned char> buffer(EVP_CIPHER_CTX_block_size(ctx));
	int bytesProcessed = 0;
	while (inputFile.good()) {
		inputFile.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
		int bytesRead = static_cast<int>(inputFile.gcount());

		if (EVP_DecryptUpdate(ctx, buffer.data(), &bytesProcessed, buffer.data(), bytesRead) != 1) {
			EVP_CIPHER_CTX_free(ctx);
			return;
		}

		outputFile.write(reinterpret_cast<const char*>(buffer.data()), bytesProcessed);
	}

	//Finalize decryption
	if (EVP_DecryptFinal_ex(ctx, buffer.data(), &bytesProcessed) != 1) {
		EVP_CIPHER_CTX_free(ctx);
		return;
	}

	outputFile.write(reinterpret_cast<const char*>(buffer.data()), bytesProcessed);

	//Clean up
	EVP_CIPHER_CTX_free(ctx);
}
