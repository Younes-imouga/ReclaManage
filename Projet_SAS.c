#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_users 100
#define max_name 20
#define max_password 20

typedef  struct //hada howa struct li aytsouwb bih laccounts
{
    char id;
    char username[max_name];
    char password[max_password];
    int role;
}account;

    //array bach yb9a fih laccounts
    account accounts[max_users];
    int existing_accounts = 1; //num d'acc


void sign_up();
void log_in();
void client();
void agent();
void admin();


int main()
{    
    int option;
    char username[20], password[20];

    
    do{
    printf("--------------Menu:--------------\n");
    printf("1-sign up:\n2-log in:\n3.exit:");
    printf("enter an option:  ");
    scanf("%d",&option);

    switch (option)
    {
    case 1:
        sign_up();
        existing_accounts++;
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
    }while(option!=3);

}

void sign_up()
{
    account new_account;
    int pass_valid;

        printf("enter a username: ");
        scanf("%.19s", new_account.username);
                for (int i = 0; i < existing_accounts; i++) 
                {
                    if (strcmp(accounts[i].username, new_account.username) == 0) 
                    {
                        printf("Username already exists!\n");
                        return;
                    }
                }
        
        do{
        printf("entrer a password: ");
        scanf("%.19s",new_account.password);

        //hna khasni ndir password check

        if (pass_valid == 1)
        {
            printf("password is valid.\n");
        }
        else
        {
            printf("password is invalid.");
        }
        }while(pass_valid == 1);
        
        accounts[existing_accounts++] = new_account; //dkhl laccout jdid fl global array
        printf("account created successfully.");
}
