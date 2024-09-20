#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define max_users 100
#define max_name 15
#define max_password 15
#define max_motif 30
#define max_description 500
#define max_reclamation 100


typedef struct //hada howa struct li aytsouwb bih laccounts
{
    char username[max_name];
    char password[max_password];
    int role;
} account;

typedef struct //hada struct li atsouwb bih reclamations
{
    char username[max_name];
    char Motif[max_motif];
    char description[max_description];
    char id[9];
    char status[10];
    time_t date;

}reclamation;

    account accounts[max_users];
    int existing_accounts = 0;

    reclamation reclamations[max_reclamation];
    int reclamation_count = 0;

    int logged_account;

void sign_up();
void log_in();

void admin();
void change_role();

void agent();

void client();
void reclamation_fct();


int main() 
{
    int option;
    
    do 
    {
    printf("Main Menu\n");
    printf("1-Sign Up:\n");
    printf("2-Log In:\n");
    printf("3-Exit\n");
    printf("enter an option: ");
        scanf("%d", &option);
        switch (option)
        {
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
                printf("option not found.\n");
                break;
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

            int has_uppercase = 0, has_lowercase = 0, has_number = 0, has_special_char = 0, has_username = 1;
            int len = strlen(new_account.password);

            if (len >= 8) 
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
                    }
                     else 
                    {
                        has_special_char = 1;
                    }
                    strstr(new_account.password,new_account.username);
                }

            }

                pass_valid = (has_uppercase && has_lowercase && has_number && has_special_char && has_username) ? 1 : 0;

                if (pass_valid == 1) {
                    printf("Password is valid.\n");
                } else {
                    printf("Password is invalid. It must be at least 8 characters long and must contain maj, min, num, and special char.\n");
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
            printf("account created successfully.\n");
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
    int failed = 3;

    if (existing_accounts > 0) {
        do {
            printf("Enter username: ");
            scanf("%19s", user_name);
            printf("Enter password: ");
            scanf("%19s", password);

            for (int i = 0; i < existing_accounts; i++) {
                if (strcmp(accounts[i].username, user_name) == 0 && strcmp(accounts[i].password, password) == 0) {
                    printf("Login successful!\n");
                        logged_account = i;
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
                    printf("Invalid username or password.\nyou have (%d/3) uses left\n",failed - 1);
                    failed--;
                    
                }

                if (failed == 0) 
                {
                    printf("you failed 3 times, you can try again after 30:00");
                    sleep(1800);
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
    printf("--------------------------------\n");

    int option_admin;
    printf("Admin Menu:\n");
    printf("1-change role:\n");
    printf("2-see all reclamations\n");
    printf("3-afficher les utilisateurs:\n");
    printf("4-trouver une reclamation:\n");
    printf("5-logout:\n");
    printf("enter an option: ");
    scanf("%d",&option_admin);
    do 
    {
        switch (option_admin)
        {
        case 1:
            change_role();
            break;
        case 2:
        if (reclamation_count > 0)
        {
            for (int i = 0; i < reclamation_count; i++)
            {
                printf("Motif: %s\tID: %s\n",reclamations[i].Motif, reclamations[i].id);
            }
        }
        else
        {
            printf("no reclamation available.");
        }
                
            
            break;
        case 3:
            for (int i = 0; i < existing_accounts; i++) 
            {
                printf("%d- %s\n", i + 1, accounts[i].username);
            }
            break;
        case 4:
            break;
        case 5:
        logged_account = -1;
            main();
            break;
        default:
            printf("invalid option\n");
            break;
        }
    } while (option_admin != 5);
    
}

void change_role() {
    printf("--------------------------------\n");

    printf("Select a user to change role:\n");
    for (int i = 0; i < existing_accounts; i++) {
        printf("%d - %s\n", i + 1, accounts[i].username);
    }
    
    int user_index;
    printf("Enter the number of the user: ");
    scanf("%d", &user_index);
    
    if (user_index > 0 && user_index <= existing_accounts) {
        printf("Enter new role (1 - admin, 2 - agent, 0 - client): ");
        int new_role;
        scanf("%d", &new_role);
        
        if (new_role >= 0 && new_role <= 2) {
            accounts[user_index - 1].role = new_role;
            printf("Role changed successfully.\n");
        } else {
            printf("Invalid role. Please try again.\n");
        }
    } else {
        printf("Invalid user selection. Please try again.\n");
    }
    
    return;
}

void agent() {
    printf("--------------------------------\n");

    int option_agent;
    printf("Agent Menu\n");
    printf("1-entrez une reclamation  :\n");
    printf("2-modifier une reclamation :\n");
    printf("3-suprimer une reclamation :\n");
    printf("4-logout: \n");
    printf("entrez une option: ");
    scanf("%d",option_agent);
    switch (option_agent)
    {
    case 4:
        logged_account = -1;
        main();
        break;
    
    default:
        printf("option invalide\n");
        break;
    }
}

void client() {
    int option_client;
    do
    {
    printf("--------------------------------\n");
    printf("Client Menu\n");
    printf("1-entrez une reclamation:\n");
    printf("2-modifier une reclamation:\n");
    printf("3-suprimer une reclamation:\n");
    printf("4-log out:\n");
    printf("entrez une option: ");
    scanf("%d",&option_client);

    switch (option_client)
    {
    case 1:
        if (reclamation_count != max_reclamation)
        {
           reclamation_fct();
        }
        else
        {
            printf("le num des reclamation a acceder le max.\n");
        }
        break;
    case 4:
        logged_account = -1;
        main();
        break;

    default:
        printf("option invalid \n");
        break;
        }
    } while (option_client != 4);
}

void reclamation_fct() 
{
    printf("--------------------------------\n");

    reclamation new_reclamation;
    account logged_rn = accounts[logged_account];

    printf("entrez le motif de la reclamation: \n");
    getchar();
    fgets(new_reclamation.Motif, max_motif - 1, stdin);
    new_reclamation.Motif[strcspn(new_reclamation.Motif, "\n")] = '\0'; // remove newline character

    printf("entrez la description de ton probleme:\n");
    fgets(new_reclamation.description, max_description - 1, stdin);
    new_reclamation.description[strcspn(new_reclamation.description, "\n")] = '\0'; // remove newline character

    strcpy (new_reclamation.username, logged_rn.username);

    strcpy(new_reclamation.status, "en cours");

            srand(time(NULL));
                int num;
                int id_existed ;

                do //loop ta ytsouwb id jdid
                {
                    id_existed = 0 ; //hna kadir rest kol mra f loop
                        for (int i = 0;  i < 8; i++)
                        {   
                                do //souwb hna iD
                                {
                                    num = rand() % 123;
                                } while (!( (num >= 48 && num <= 57) || (num >= 65 && num <= 90) || (num >= 97 && num <= 122) ));
                            new_reclamation.id[i] = (char)num;
                        }
                        new_reclamation.id[8] = '\0';
                        for (int i = 0; i < existing_accounts; i++) //chof l ID wach jdid
                        {
                            if (strcmp(new_reclamation.id, reclamations[i].id) == 0) 
                            {
                                id_existed = 1;
                                break;
                            }
                        }
                } while (id_existed == 1);
    reclamations[reclamation_count]  = new_reclamation; //dkhl lreclamation fl array
    reclamation_count++;    
}