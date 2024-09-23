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
    time_t time;
    char *date;
    char priority [10];
    char category[10];
    char note;

}reclamation;

    char category[4][15] = {"financielle","technique","service","autre"};


    account accounts[max_users];
    int existing_accounts = 0;

    reclamation reclamations[max_reclamation];
    int reclamation_count = 0;

    int logged_account;

void sign_up();
void log_in();


void admin();
void change_role();
void traiter_reclamation();
void search_reclamation();
void print_by_priority();

void agent();

void client();
void reclamation_fct();
void modify_delete_reclamation();

void save_users_to_file();
void load_users_from_file();

void save_reclamations_to_file();
void load_reclamations_from_file();

int main()
{
    int option = 0;

    do
    {
        load_reclamations_from_file();
        printf("Main Menu\n");
        printf("1-Sign Up:\n");
        printf("2-Log In:\n");
        printf("3-Exit\n");
        printf("enter an option: ");
        scanf("%d", &option);
        getchar();
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

            int has_uppercase = 0, has_lowercase = 0, has_number = 0, has_special_char = 0, has_username = 0;
            int len = strlen(new_account.password);

            if (len >= 8)
            {
                for (int i = 0; i < len; i++)
                {
                    if (new_account.password[i] >= '0' && new_account.password[i] <= '9')
                    {
                        has_number = 1;
                    }
                    else if (new_account.password[i] >= 'a' && new_account.password[i] <= 'z')
                    {
                        has_lowercase = 1;
                    }
                    else if (new_account.password[i] >= 'A' && new_account.password[i] <= 'Z')
                    {
                        has_uppercase = 1;
                    }
                    else
                    {
                        has_special_char = 1;
                    }
                    if(strstr(new_account.password,new_account.username) == 0)
                    {
                        has_username = 1;
                    }
                }

            }

                pass_valid = (has_uppercase && has_lowercase && has_number && has_special_char && has_username) ? 1 : 0;

                if (pass_valid == 1)
                {
                }
                else
                {
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
            save_users_to_file();
            printf("account created successfully.\n");
            printf("your acc info are:\nuser: %s\tpassword: %s\n", new_account.username, new_account.password);
        }
        else
        {
            printf("max number of users has reached\n");
        }
}

void log_in() {
    char user_name[max_name];
    char password[max_password];
    int login_successful = 0;
    int failed = 3;
    load_reclamations_from_file();

    if (existing_accounts > 0) {

        do
        {
            printf("Enter username: ");
            scanf("%19s", user_name);
            printf("Enter password: ");
            scanf("%19s", password);

            for (int i = 0; i < existing_accounts; i++)
            {
                if (strcmp(accounts[i].username, user_name) == 0 && strcmp(accounts[i].password, password) == 0) {
                    printf("Login successful!\n");
                    logged_account = i;
                    login_successful = 1;
                        switch (accounts[i].role)
                        {
                        case 1:
                            admin();
                            break;

                        case 2:
                            agent();

                        case 0:
                            client();
                            break;

                        default:
                            break;
                        }
                        return;
                    break;
                }
            }

            if (!login_successful)
            {
                printf("Invalid username or password.\nyou have (%d/3) uses left\n", failed - 1);
                failed--;
            }

            if (failed == 0)
            {
                printf("you failed 3 times, you can try again after 30:00");
                sleep(1800);
            }

            login_successful = 0;
        } while (failed > 0);
    }
    else
    {
        printf("No users exist.\n");
    }
}

void admin() {

    int option_admin;
    do
    {
    printf("--------------------------------\n");
    printf("Admin Menu:\n");
    printf("1-ajouter une reclamtion: \n");
    printf("2-afficher les utilisateurs:\n");
    printf("3-see all reclamations\n");
    printf("4-change role:\n");
    printf("5-trouver une reclamation:\n");
    printf("6-traiter une reclamation:\n");
    printf("7-logout:\n");

    printf("enter an option: ");
    scanf("%d",&option_admin);
        switch (option_admin)
        {
        case 1:
            reclamation_fct();
            break;
        case 2:
            for (int i = 0; i < existing_accounts; i++)
            {
                printf("%d- %s\n", i + 1, accounts[i].username);
            }
            break;
        case 3:
            if (reclamation_count > 0)
            {
                for (int i = 0; i < reclamation_count; i++)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
            else
            {
                printf("no reclamation available.");
            }
            break;

        case 4:
            change_role();
            break;

        case 5:
            search_reclamation();
            break;

        case 6:
            traiter_reclamation();

        case 7:
        logged_account = -1;
            return;
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
        if (strcmp (accounts[user_index - 1].username, accounts[logged_account].username) == 0)
        {
            printf("you cant change 'YOUR' role.\n");
        }
        else
        {
            printf("Enter new role (1 - admin, 2 - agent, 0 - client): ");
            int new_role;
            scanf("%d", &new_role);

            if (new_role >= 0 && new_role <= 2)
            {
                accounts[user_index - 1].role = new_role;
                printf("Role changed successfully.\n");
            }
            else
            {
                printf("Invalid role. Please try again.\n");
            }
        }


    }
    else
    {
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
    scanf("%d",&option_agent);
    switch (option_agent)
    {
    case 4:
        logged_account = -1;
        return;
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
    printf("2-modifier/suprimer une reclamation:\n");
    printf("3-afficher mes reclamations:\n");
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

    case 2:
        modify_delete_reclamation();
        break;

    case 3:

        break;

    case 4:
        logged_account = -1;
        return;
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

    int category_number;
    for (int i = 0; i < 4; i++)
    {
        printf("%d-%s\n", i+1,category[i]);
    }

    do
    {
        printf("choose a category: ");
        scanf("%d",&category_number);

        if (category_number < 1 || category_number > 4)
        {
            printf("invalid category\n");
        }
        else
        {
            strcpy(new_reclamation.category, category[category_number - 1]);
        }

    } while (category_number < 1 || category_number > 4);





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
            new_reclamation.time = time(NULL);
            new_reclamation.date = ctime(&new_reclamation.time);

            char haut [6][25]= {"blan","prioritaire","important","critique"};//urgent
            char moyen [6][25]= {"nadi","moyen","necessaire","crutial","Essentiel"};

            for (int i = 0; i < 6; i++)
            {
                if (strstr(new_reclamation.description, haut[i]) != NULL)
                {
                strcpy(new_reclamation.priority, "haute");
                }

                else if (strstr(new_reclamation.description, moyen[i]) != NULL)
                {
                strcpy(new_reclamation.priority, "moyenne");
                }
                else
                {
                strcpy(new_reclamation.priority, "base");
                }
            }


        printf("Your reclamation info are:\nMotif: %s\tID: %s\tStatus: %s\tDate: %s\tPriorite: %s\t",new_reclamation.Motif, new_reclamation.id, new_reclamation.status, new_reclamation.date, new_reclamation.priority);
        printf("Your username is: %s\tLA reclamation categorie est: %s\ndescription:\n%s\n", new_reclamation.username, new_reclamation.category, new_reclamation.description);

    reclamations[reclamation_count]  = new_reclamation; //dkhl lreclamation fl array
    reclamation_count++;
    save_reclamations_to_file();
}


void traiter_reclamation()
{
    if (reclamation_count == 0)
    {
        printf("No reclamation available.\n");
        return;
    }

    int traiter_option;
    do
    {
    printf("--------------------------------\n");
    printf("------traiter une reclamation------\n");
    printf("1-afficher toutes les reclamation:\n");
    printf("2-trouver une reclamation:\n");
    printf("3-suprimer une reclamation:\n");
    printf("4-traiter une reclamation:\n");
    printf("5-Quiter:\n");
    printf("entrez une option: ");
    scanf("%d",&traiter_option);

    switch (traiter_option)
    {
    case 1:
        if (reclamation_count > 0)
        {
            for (int i = 0; i < reclamation_count; i++)
            {
                printf("Motif: %s\tID: %s\tStatus: %s\tDate: %s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                printf("Username: %s\tCategory: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
            }
        }
        else
        {
            printf("No reclamation available.\n");
        }
        break;

    case 2:
        if (reclamation_count > 0)
        {
            search_reclamation();
        }
        break;

    case 3:
        if (reclamation_count > 0)
        {
            //delete_reclamation();
        }
        else
        {
            printf("No reclamation available.\n");
        }
        break;

    case 4:
        if (reclamation_count > 0)
        {
            for (int i = 0; i < reclamation_count; i++)
            {
                printf("Motif: %s\tID: %s\tStatus: %s\tDate: %s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                printf("choose a reclamation to traiter:\n");
                int num;
                scanf("%d",&num);
                if (num >= 1 && num <= reclamation_count)
                {
                    printf("choose a status:\n");
                    printf("1-En cours\n");
                    printf("2-traité\n");
                    printf("3-rejeter\n");
                    int status;
                    scanf("%d",&status);
                    if (status == 1)
                    {
                        strcpy(reclamations[num-1].status,"En cours");
                    }
                    else if (status == 2)
                    {
                        strcpy(reclamations[num-1].status,"traité");
                    }
                    else if (status == 3)
                    {
                        strcpy(reclamations[num-1].status,"rejeter");
                    }
                }
            }

        }
        else
        {
            printf("No reclamation available.\n");
        }

        break;

    case 5:
        logged_account = -1;
        return;
        break;

    default:
        printf("Invalid option. Please try again.\n");
        break;
    }
    } while (traiter_option != 5);

}

void search_reclamation()
{
    int search_option;
    do
    {
    printf("--------------------------------\n");
    printf("------rechercher une reclamation------\n");
    printf("1-trouver par motif:\n");
    printf("2-trouver par user:\n");
    printf("3-trouver par categorie:\n");
    printf("4-rechercher par ID:\n");
    printf("5-trouver par date:\n");
    printf("6-trouver par status:\n");
    printf("7-Quiter:\n");
    printf("entrez une option: ");
    scanf("%d",&search_option);

    switch (search_option)
    {
    case 1:
        {
            char search_motif[max_name];
            printf("entrez un motif: ");
            scanf("%s", &search_motif);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].Motif,       search_motif) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 2:
        {
            char search_user[max_name];
            printf("entrez un user: ");
            scanf("%s", &search_user);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].username, search_user) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 3:
        {
            char search_category[10];
            printf("entrez une categorie: ");
            scanf("%s", &search_category);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].category, search_category) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 4:
        {
            char search_id[9];
            printf("entrez un ID: ");
            scanf("%s", &search_id);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].id, search_id) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 5:
        {
            char search_date[30];
            printf("entrez une date: ");
            scanf("%s", &search_date);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].date, search_date) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 6:
        {
            char search_status[10];
            printf("entrez un status: ");
            scanf("%s", &search_status);

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].status, search_status) == 0)
                {
                    printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;

    case 7:
        logged_account = -1;
        return;
        break;
    }

    }while (search_option != 4);
}

void modify_delete_reclamation()
{
    if (accounts[logged_account].role == 0)
    {
        int reclamation_id;
        int i;
        printf("Enter the ID of the reclamation you want to modify or delete: ");
        scanf("%d", &reclamation_id);

        int found = 0;
        for (i = 0; i < reclamation_count; i++)
        {
            if (*reclamations[i].id == reclamation_id)
            {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Reclamation not found.\n");
            return;
        }

        time_t current_time = time(NULL);
        double time_diff = difftime(current_time, reclamations[i].time);
        if (strcmp(reclamations[i].status, "en cours") == 0 && time_diff < 24 * 60 * 60) {
            printf("1-Modify reclamation\n");
            printf("2-Delete reclamation\n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new motif: ");
                    getchar();
                    fgets(reclamations[i].Motif, max_motif - 1, stdin);
                        if (strlen(reclamations[i].Motif) == 0) {}
                        else
                        {
                        reclamations[i].Motif[strcspn(reclamations[i].Motif, "\n")] = '\0';
                        }

                    printf("Enter new description: ");
                    fgets(reclamations[i].description, max_description - 1, stdin);
                        if (strlen(reclamations[i].description) == 0) {}
                        else
                        {
                        reclamations[i].description[strcspn(reclamations[i].description, "\n")] = '\0'; // remove newline character
                        }
                    break;
                case 2:
                    for (int j = i; j < reclamation_count - 1; j++) {
                        reclamations[j] = reclamations[j + 1];
                    }
                    reclamation_count--;
                    printf("Reclamation deleted successfully.\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
        }
        else
        {
            printf("Reclamation is not eligible for modification or deletion.\n");
        }
    }

}

void print_by_priority()
{
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "haut") == 0)
        {
        printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
        }

    }
        for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "moyen") == 0)
        {
        printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
        }

    }
        for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "base") == 0)
        {
        printf("Motif: %s\tID: %s\t%s\t%s",reclamations[i].Motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
        }

    }
}

void load_users_from_file()
{
    FILE *file = fopen("users.txt", "r");

    int i = 0;
    while (i < max_users && fscanf(file, "%s %s %d\n", &accounts[i].username, &accounts[i].password, &accounts[i].role) != EOF)
    {
    i++ ;
    }
    existing_accounts = i;
    fclose(file);
}


void save_users_to_file()
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error saving users to file.\n");
        return;
    }
    for (int i = 0; i < existing_accounts; i++)
    {
        fprintf(file, "%s %s %d\n", accounts[i].username, accounts[i].password, accounts[i].role);
    }
    fclose(file);
}

void load_reclamations_from_file()
{
    FILE *file = fopen("reclamations.txt", "r");
    int i = 0;
    while (i < max_users && fscanf(file, "%s %s %s %s %s %s\n", reclamations[i].username, reclamations[i].Motif, reclamations[i].date, reclamations[i].status, reclamations[i].priority, reclamations[i].description) != EOF)
    {
    i++ ;
    }
    existing_accounts = i;
    fclose(file);

}

void save_reclamations_to_file()
{
    FILE *file = fopen("reclamations.txt", "w");
    if (file == NULL)
    {
        printf("Error saving reclamations to file.\n");
        return;
    }
    for (int i = 0; i < reclamation_count; i++)
    {
        fprintf(file, "%s %s %s %s %s %s\n", reclamations[i].username, reclamations[i].Motif, reclamations[i].date, reclamations[i].status, reclamations[i].priority, reclamations[i].description);
    }
    fclose(file);
}
