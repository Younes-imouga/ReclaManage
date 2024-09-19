#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define max_users 100
#define max_name 15
#define max_password 15
#define max_motif 30
#define max_description 500

typedef struct //hada howa struct li aytsouwb bih laccounts
{
    char username[max_name];
    char password[max_password];
    int role;
} account;

typedef struct 
{
    char Motif[max_motif];
    char id[9];
    char description[max_description];
    char status[10];
    time_t date;

}reclamation;


    account accounts[max_users];
    int existing_accounts = 0;

void sign_up();
void log_in();

void admin();
void agent();
void client();


int main() 
{
    int option;
    
    do 
    {
        printf("--------------Menu:--------------\n");
        printf("1-sign up:\n2-log in:\n3.exit:\n");
        printf("enter an option:  ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                sign_up();
                break;
            case 2:
                log_in();
                break;
            case 3:
                printf("Exiting...");
                break;
            default:
                printf("option not found.");
        }
    } while (option != 3);
}

void sign_up() 
{
    account new_account;
    int pass_valid;

        if (max_users > existing_accounts) 
        {
            printf("enter a username: ");
            scanf("%19s", new_account.username);

                    for (int i = 0; i < existing_accounts; i++) 
                    {
                        if (strcmp(accounts[i].username, new_account.username) == 0) 
                        {
                                printf("Username already exists!\n");
                                return;
                        }
                    }

            do 
            {
            printf("enter a password: ");
            scanf("%19s", new_account.password);

            int has_uppercase = 0, has_lowercase = 0, has_number = 0, has_special_char = 0;
            int len = strlen(new_account.password);

            if (len >= 6) 
            {
                for (int i = 0; i < len; i++) 
                {
                    if (new_account.password[i] >= '0' && new_account.password[i] <= '9') 
                    {
                        has_number = 1;
                    }
                    if (new_account.password[i] >= 'a' && new_account.password[i] <= 'z') 
                    {
                        has_lowercase = 1;
                    }
                    if (new_account.password[i] >= 'A' && new_account.password[i] <= 'Z') 
                    {
                        has_uppercase = 1;
                    } else {
                        has_special_char = 1;
                    }
                }
            }

                pass_valid = (has_uppercase && has_lowercase && has_number && has_special_char) ? 1 : 0;

                if (pass_valid == 1) {
                    printf("Password is valid.\n");
                } else {
                    printf("Password is invalid. It must be at least 6 characters long and must contain maj, min, num, and special char.\n");
                }
            } while (pass_valid == 0);

                    //make the role      1->admin    2->agent   0->client
            if (existing_accounts == 0) 
            {
                new_account.role = 1;
            } 
            else 
            {
                new_account.role = 0;
            }

            accounts[existing_accounts++] = new_account; //dkhl laccount jdid fl global array
            printf("account created successfully.");
            printf("your acc info are:\nuser: %s\tpassword: %s\n", new_account.username, new_account.password, new_account.role);
        } 
        else 
        {
            printf("max number of users has reached\n");
        }
}

void log_in() {
    char user_name[max_name];
    char password[max_password];
    int failed = 5;

    if (existing_accounts > 0) {
        do {
            printf("Enter username: ");
            scanf("%19s", user_name);
            printf("Enter password: ");
            scanf("%19s", password);

            for (int i = 0; i < existing_accounts; i++) {
                if (strcmp(accounts[i].username, user_name) == 0 && strcmp(accounts[i].password, password) == 0) {
                    printf("Login successful!\n");
                    switch (accounts[i].role) 
                    {
                        case 1:
                            admin();
                            break;
                        case 2:
                            agent();
                            break;
                        case 0:
                            client();
                            break;
                    }
                    break;
                } 
                else 
                {
                    printf("Invalid username or password.\n");
                    failed--;
                }

                if (failed == 0) 
                {
                    printf("you failed 5 times, you can try again after 30:00");
                    return;
                }
            }
        } 
        while (failed > 0);
    } 
    else 
    {
        printf("No users exist.");
    }
}

void admin() {
    int option_admin;
    printf("----------------------Menu----------------------\n");
}

void agent() {

}

void client() {

}