#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_RECORDS 100

typedef struct telephone
{
    char phone_number[255];
    char surname[255];
    float balance;
    int deleted;
}contact;

int deleted = 0;
int num_records = 0;
contact records[MAX_RECORDS];

char filename[100];

char* Concatination(char *string_1, char *string_2);
void Open_and_write(char *file_directory, char* filename);
void Show_information(char *file_directory, char* filename);
void Search_by_key(char *file_directory, char* filename);
void Append_contact(char *file_directory, char* filename);
void Delete_by_key(char *file_directory, char* filename);


int main()
{

    int persons;
    int choice_1, choice_2 = -1;
    printf("Input name of your future file:");
    scanf("%s", &filename);
    char file_directory[255];
    do
    {
    
    printf("----------  What do you want to choose?  ----------\n");
    printf("\t1. Make new file and read it \n\t2. Show file \n\t3. Search information by keyword \n\t4. Append new information to file \n\t5. Delete information \n\t6. Exit to program");
    printf("\n\n\nYour choice:");
    scanf("%d", &choice_1);

    switch (choice_1)
    {
    case 1:
    
    printf("----- Do uou want to choose your own directory or make new file here? -----\n");
    printf("\n\n \t1. Choose your own directory \n\t2. Create file here\n\n Your choice: ");
    scanf("%d", &choice_2);

    while(choice_2 != 1 || choice_2 != 2){
        if(choice_2 == 1)
    {
        printf("Input your directory way: ");
        scanf("%s", &file_directory);
        Open_and_write(file_directory,filename);
        break;
    }
    else if (choice_2 == 2)
    {
        strcpy(file_directory, filename);
        Open_and_write(file_directory, filename);
        break;
    }
    else
    {
        printf("\n!!!! You inputed wrong value. Try again !!!!\n");
        printf("\n \t1. Choose your own directory \n\t2. Create file here");
        scanf("%d", &choice_2);
    }
    }
        break;
    
    case 2:

        Show_information(file_directory, filename);
        break;

    case 3:

        Search_by_key(file_directory, filename);
        break;
    
    case 4:
        Append_contact(file_directory, filename);
        break;
    
    case 5:
        
        Delete_by_key(file_directory, filename);
        break;
    
    case 6:
        return 0;
        break;
    
    default:
        printf("----------  You choosed unavalible menu item   ----------\n");
        printf("Choose another one: ");
        scanf("%d", &choice_1);
        break;
    }   
    } while (true);
}

char* Concatination(char *string_1, char *string_2)
{
    char *result = malloc(strlen(string_1)+strlen(string_2)+1);

    strcpy(result, string_1);
    strcat(result, string_2);

    return result;
}

void Open_and_write(char *file_directory, char* filename)
{
    Concatination(file_directory, filename);
    FILE *fp = fopen(Concatination(file_directory, ".txt"), "wb");
    if (!fp) {
        printf("Failed to create file\n");
        return;
    }

    printf("How many users you want to add?\n");
    printf("Enter the number of records: ");
    scanf("%d", &num_records);

    for (int i = 0; i < num_records; i++) {
        printf("\nUser #%d\n\n", i+1);

        printf("Enter the phone number: ");
        scanf("%s", &records[i].phone_number);

        printf("Enter the owner's surname: ");
        scanf("%s", &records[i].surname);

        printf("Enter the balance: ");
        scanf("%f", &records[i].balance);
    }

    fwrite(records, sizeof(contact), num_records, fp);
    fclose(fp);

    printf("File created successfully\n");
}

void Show_information(char *file_directory, char* filename)
{
    Concatination(file_directory, filename);
    FILE *fp = fopen(Concatination(file_directory, ".txt"), "rb");
    if (!fp) {
        printf("File not found\n");
        return;
    }

    fread(records, sizeof(contact), num_records, fp);

    printf("Phone Number\tOwner's surname\tBalance\n");
    for (int i = 0; i < num_records; i++) {
        printf("%s\t\t%s\t\t%.2f\n", records[i].phone_number, records[i].surname, records[i].balance);
    }

    fclose(fp);
}


void Search_by_key(char *file_directory, char* filename)
{
    Concatination(file_directory, filename);
    FILE* fp;
    contact record;
    char search_number[15];
    int found = 0;

    fp = fopen(Concatination(file_directory, ".txt"), "rb");

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter phone number to search: ");
    scanf("%s", search_number);

    while (fread(&record.phone_number, sizeof(record.phone_number), 1, fp)) {
        if(fread(&record.deleted,sizeof(record.deleted), 1, fp) == 1) continue;
        if(strcmp(record.phone_number, search_number) == 0) 
        {
            found = 1;
            fread(&record.surname, sizeof(record.surname), 1, fp);
            fread(&record.balance, sizeof(record.balance), 1, fp);
            printf("\nPhone number\tSurname\t\tBalance\n");
            printf("%s\t%s\t\t%.2f\n", record.phone_number, record.surname, record.balance);
            break;
        } else 
        {
            fseek(fp, sizeof(record) - sizeof(record.phone_number), SEEK_CUR);
        }
    }

    if(!found) {
        printf("\nRecord not found!\n");
    }

    fclose(fp);
}


void Append_contact(char *file_directory, char* filename)
{
    Concatination(file_directory, filename);
    FILE* fp;
    contact record;

    fp = fopen(Concatination(file_directory, ".txt"), "ab");

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter details for new record:\n");
    printf("Phone number: ");
    scanf("%s", &records[num_records].phone_number);
    printf("Surname: ");
    scanf("%s", &records[num_records].surname);
    printf("Balance: ");
    scanf("%f", &records[num_records].balance);

    

    fwrite(&records[num_records], sizeof(contact), 1, fp);
    fclose(fp);
    num_records++;
    printf("\nRecord added successfully!\n");
}

void Delete_by_key(char *file_directory, char* filename)
{
    Concatination(file_directory, filename);
    FILE* fp;
    contact record;
    char search_number[15];
    int found = 0;

    fp = fopen(Concatination(file_directory, ".txt"), "r+b");

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter phone number to delete: ");
    scanf("%s", search_number);

    while(fread(&record, sizeof(contact), 1, fp) == 1) 
    {
        if(strcmp(record.phone_number, search_number) == 0) {
            found = 1;
            record.deleted = 1;
            fseek(fp, -sizeof(contact), SEEK_CUR);
            fwrite(&record, sizeof(contact), 1, fp);
            break;
        }
    }

    fclose(fp);

    if (found == 0) {
        printf("\nContact not found\n");
        return;
    }

    printf("\nContact with phone number %s marked as deleted\n", search_number);
}


    