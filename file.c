#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

// Function to pad the data if the input is not a multiple of AES block size (16 bytes)
void pad_buffer(unsigned char* buffer, int* len) {
    int padding = AES_BLOCK_SIZE - (*len % AES_BLOCK_SIZE);
    for (int i = *len; i < *len + padding; i++) {
        buffer[i] = padding;  // Pad with the value of the number of padding bytes
    }
    *len += padding;
}

// Function to encrypt a file using AES
void encrypt_aes(const char* input_file, const char* output_file, const unsigned char* key) {
    FILE* fin = fopen(input_file, "rb");
    FILE* fout = fopen(output_file, "wb");

    if (fin == NULL || fout == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    AES_KEY encrypt_key;
    AES_set_encrypt_key(key, 128, &encrypt_key);  // Set AES encryption key (128-bit)

    unsigned char inbuffer[AES_BLOCK_SIZE], outbuffer[AES_BLOCK_SIZE];
    int len;

    // Read the file, encrypt each block, and write to the output file
    while ((len = fread(inbuffer, 1, AES_BLOCK_SIZE, fin)) > 0) {
        if (len < AES_BLOCK_SIZE) {
            pad_buffer(inbuffer, &len);  // Pad the last block if necessary
        }
        AES_encrypt(inbuffer, outbuffer, &encrypt_key);
        fwrite(outbuffer, 1, AES_BLOCK_SIZE, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("File successfully encrypted.\n");
}

// Function to decrypt a file using AES
void decrypt_aes(const char* input_file, const char* output_file, const unsigned char* key) {
    FILE* fin = fopen(input_file, "rb");
    FILE* fout = fopen(output_file, "wb");

    if (fin == NULL || fout == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    AES_KEY decrypt_key;
    AES_set_decrypt_key(key, 128, &decrypt_key);  // Set AES decryption key (128-bit)

    unsigned char inbuffer[AES_BLOCK_SIZE], outbuffer[AES_BLOCK_SIZE];
    int len;

    // Read the file, decrypt each block, and write to the output file
    while ((len = fread(inbuffer, 1, AES_BLOCK_SIZE, fin)) > 0) {
        AES_decrypt(inbuffer, outbuffer, &decrypt_key);
        fwrite(outbuffer, 1, len, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Password is correct. File successfully decrypted.\n");
}

// Function to check if the entered password is correct
bool check_password(const char* correct_password, const char* input_password) {
    return strcmp(correct_password, input_password) == 0;
}

int main() {
    char input_file[100], output_file[100], password[5];
    unsigned char aes_key[16];  // AES key (128 bits = 16 bytes)
    int choice;
    bool running = true;

    while (running) {
        // Step 1: Select operation (Encrypt, Decrypt, or Exit)
        printf("\nChoose an option:\n1. Encrypt file\n2. Decrypt file\n3. Exit program\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            // Step 2: Enter file paths
            printf("Enter the path of the input file: ");
            scanf("%s", input_file);
            printf("Enter the path of the output file: ");
            scanf("%s", output_file);

            // Step 3: Add password (must be 4 characters)
            printf("Enter a 4-character password: ");
            scanf("%4s", password);

            // Step 4: Create a 128-bit AES key (pad the password with null bytes)
            memset(aes_key, 0, sizeof(aes_key));  // Initialize AES key buffer with zeroes
            strncpy((char*)aes_key, password, 4);  // Copy the password into the AES key

            // Encrypt the file
            encrypt_aes(input_file, output_file, aes_key);

        }
        else if (choice == 2) {
            // Step 2: Enter file paths
            printf("Enter the path of the input file: ");
            scanf("%s", input_file);
            printf("Enter the path of the output file: ");
            scanf("%s", output_file);

            // Step 3: Ask for password to decrypt
            printf("Enter the password to decrypt the file: ");
            char entered_password[5];
            scanf("%4s", entered_password);

            // Step 4: Verify if the password is correct
            if (!check_password(password, entered_password)) {
                printf("Wrong password. Decryption failed.\n");
                continue;
            }

            // Step 5: Decrypt the file if the password is correct
            decrypt_aes(input_file, output_file, aes_key);

        }
        else if (choice == 3) {
            printf("Exiting the program.\n");
            running = false;  // Exit the loop

        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
