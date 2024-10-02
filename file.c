// Usage press 1 to encrypt and 2 to decrypt. It will ask path to store the file as well as password.
// During decryption if the password is wrong. It will not output the file. File path is need to enter eg. D:\a.txt to store in txt form 
// During encyption any file extension can use use eg. D:\a.bin or D:\a.dat
// Password length is 4. 
// Recommended software to run from are Visual Studio and Dev C++
// This use Xor encryption 
// No third party extension is need to run this program. 
// Feel free to edit this for your use case :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void encrypt(const char *input_file, const char *output_file, const char *password) {
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");

    if (fin == NULL || fout == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char buffer;
    int i = 0;
    size_t password_len = strlen(password);

    while (fread(&buffer, 1, 1, fin)) {
        buffer ^= password[i % password_len];  
        fwrite(&buffer, 1, 1, fout);
        i++;
    }

    fclose(fin);
    fclose(fout);

    printf("File successfully encrypted.\n");
}

bool check_password(const char *correct_password, const char *input_password) {
    return strcmp(correct_password, input_password) == 0;
}

void decrypt(const char *input_file, const char *output_file, const char *password) {
    char entered_password[5];

    
    printf("Enter the password to decrypt the file: ");
    scanf("%4s", entered_password);

   
    if (!check_password(password, entered_password)) {
        printf("Wrong password. Decryption failed.\n");
        return;  
    }

    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");

    if (fin == NULL || fout == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char buffer;
    int i = 0;
    size_t password_len = strlen(password);

    while (fread(&buffer, 1, 1, fin)) {
        buffer ^= password[i % password_len];  
        fwrite(&buffer, 1, 1, fout);
        i++;
    }

    fclose(fin);
    fclose(fout);

    printf("Password is correct. File successfully decrypted.\n");
}

int main() {
    char input_file[100], output_file[100], password[5];  
    int choice;
    bool running = true;

    while (running) {
       
        printf("\nChoose an option:\n1. Encrypt file\n2. Decrypt file\n3. Exit program\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            
            printf("Enter the path of the input file: ");
            scanf("%s", input_file);
            printf("Enter the path of the output file: ");
            scanf("%s", output_file);

           
            printf("Enter a 4-character password: ");
            scanf("%4s", password);  

            
            encrypt(input_file, output_file, password);

        } else if (choice == 2) {
           
            printf("Enter the path of the input file: ");
            scanf("%s", input_file);
            printf("Enter the path of the output file: ");
            scanf("%s", output_file);

          
            decrypt(input_file, output_file, password);

        } else if (choice == 3) {
            printf("Exiting the program.\n");
            running = false;  

        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
