#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define max_users 100
#define max_name 15
#define max_password 15
#define max_motif 30
#define max_description 500
#define max_reclamation 100
#define max_time 30

typedef struct 
{
    char username[max_name];
    char password[max_password];
    int role; // 0: client, 1: admin, 2: agent
} account;

typedef struct 
{
    char username[max_name];
    char motif[max_motif];
    char description[max_description];
    char id[9];
    char status[10];
    time_t time;
    char date[30]; // Changed to fixed size
    char priority[10];
    char category[15]; // Increased size for clarity
    char note; // Consider changing this to an appropriate type
} reclamation;

        char categories[4][15] = {"financielle", "technique", "service", "autre"};
        account accounts[max_users];
        int existing_accounts = 0;

        reclamation reclamations[max_reclamation];
        int reclamation_count = 0;
        int logged_account;

void sign_up();
void log_in();

void admin();
void change_role();
void print_by_priority();

void agent();
void traiter_reclamation();
void modify_delete_reclamation();
void search_reclamation();

void client();
void reclamation_fct();

void save_users_to_file();
void load_users_from_file();

void save_reclamations_to_file();
void load_reclamations_from_file();

void print_by_priority();
int GetPriorityValue();

int main() 
{
    load_users_from_file();

    int option = 0;
    do 
    {
        printf("Menu principal\n");
        printf("1-S'inscrire:\n");
        printf("2-Se connecter:\n");
        printf("3-Sortie\n");
        printf("entrer une option: ");
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
                printf("Sortie...\n");
                break;
            default:
                printf("option non trouvee.\n");
                break;
        }
    } while (option != 3);
}

void sign_up() {
    account new_account;
    int pass_valid;
    if (max_users > existing_accounts) 
    {
        printf("entrer un nom d'utilisateur: ");
        scanf("%19s", new_account.username);
        for (int i = 0; i < existing_accounts; i++) {
            if (strcmp(accounts[i].username, new_account.username) == 0) {
                printf("Le nom d'utilisateur existe dejà!\n");
                return;
            }
        }
        do {
            printf("entrer un mot de passe: ");
            scanf("%19s", new_account.password);
            int has_uppercase = 0, has_lowercase = 0, has_number = 0, has_special_char = 0, has_username = 0;
            int len = strlen(new_account.password);
            if (len >= 8) {
                for (int i = 0; i < len; i++) {
                    if (isdigit(new_account.password[i])) {
                        has_number = 1;
                    } else if (islower(new_account.password[i])) {
                        has_lowercase = 1;
                    } else if (isupper(new_account.password[i])) {
                        has_uppercase = 1;
                    } else {
                        has_special_char = 1;
                    }
                }
                has_username = (strstr(new_account.password, new_account.username) == NULL) ? 1 : 0;
            }
            pass_valid = (has_uppercase && has_lowercase && has_number && has_special_char && has_username) ? 1 : 0;
            if (!pass_valid) {
                printf("Le mot de passe n'est pas valide. Il doit comporter au moins 8 caractères et contenir des majuscules, minuscules, chiffres et caractères speciaux.\n");
            }
        } while (!pass_valid);
        
        // Determine role
        new_account.role = (existing_accounts == 0) ? 1 : 0;
        accounts[existing_accounts++] = new_account;
        save_users_to_file();
        printf("Compte cree avec succès.\nVos informations de compte sont:\nUser: %s\tPassword: %s\n", new_account.username, new_account.password);
    } else {
        printf("Le nombre maximum d'utilisateurs a ete atteint\n");
    }
}

void log_in() {
    char user_name[max_name];
    char password[max_password];
    int login_successful = 0;
    int failed = 3;
    if (existing_accounts > 0) {
        do {
            printf("Entrer le nom d'utilisateur: ");
            scanf("%19s", user_name);
            printf("Entrer le mot de passe: ");
            scanf("%19s", password);
            for (int i = 0; i < existing_accounts; i++) {
                if (strcmp(accounts[i].username, user_name) == 0 && strcmp(accounts[i].password, password) == 0) {
                    printf("Connexion reussie!\n");
                    logged_account = i;
                    login_successful = 1;
                    switch (accounts[i].role) {
                        case 1: admin(); break;
                        case 2: agent(); break;
                        case 0: client(); break;
                        default: break;
                    }
                    return;
                }
            }
            if (!login_successful) {
                printf("Nom d'utilisateur ou mot de passe invalide.\nIl vous reste (%d/3) tentatives.\n", failed - 1);
                failed--;
            }
            if (failed == 0) {
                printf("Vous avez echoue 3 fois, vous pouvez essayer à nouveau après 30 minutes.\n");
                sleep(1800);
            }
            login_successful = 0;
        } while (failed > 0);
    } else {
        printf("Aucun utilisateur n'existe.\n");
    }
}

void admin() 
{
    int option_admin;
    do {
        printf("--------------------------------\n");
        printf("Menu Admin:\n");
        printf("1-Ajouter une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Afficher les utilisateurs:\n");
        printf("4-Voir toutes les reclamations\n");
        printf("5-Changer le role:\n");
        printf("6-Trouver une reclamation:\n");
        printf("7-Traiter une reclamation:\n");
        printf("8-afficher les reclamations ordonees par priorite:\n");
        printf("9-Se deconnecter:\n");
        printf("Entrez une option: ");
        scanf("%d", &option_admin);
        switch (option_admin) 
        {
            case 1: 
                if (reclamation_count != max_reclamation)
                {
                    reclamation_fct();
                }
                else
                {
                    printf("Le nombre de reclamations a atteint le maximum.\n");
                }
                break;
            case 2:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        modify_delete_reclamation();
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 3:
                for (int i = 0; i < existing_accounts; i++) 
                {
                    printf("%d- %s\n", i + 1, accounts[i].username);
                }
                break;
            case 4:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        printf("\nMotif: %s\nID: %s\nStatus: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nUser: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 5: 
                change_role(); 
                break;
            case 6: 
                search_reclamation(); 
                break;
            case 7: 
                traiter_reclamation(); 
                break;
            case 8: 
                if (reclamation_count > 0) 
                {
                    print_by_priority();
                }
                else
                {
                    printf("Aucune reclamation disponible.\n");
                }
                
                break;
            case 9: 
                logged_account = -1; 
                return;
            default: 
                printf("Option invalide\n"); 
                break;
        }
    } while (option_admin != 9);
}

void change_role() {
    printf("--------------------------------\n");
    printf("Selectionnez un utilisateur pour changer de role:\n");
    for (int i = 0; i < existing_accounts; i++) {
        printf("%d - %s\n", i + 1, accounts[i].username);
    }
    int user_index;
    printf("Entrez le numero de l'utilisateur: ");
    scanf("%d", &user_index);
    if (user_index > 0 && user_index <= existing_accounts) {
        if (strcmp(accounts[user_index - 1].username, accounts[logged_account].username) == 0) {
            printf("Vous ne pouvez pas changer VOTRE role.\n");
        } else {
            printf("Entrez le nouveau role (1 - admin, 2 - agent, 0 - client): ");
            int new_role;
            scanf("%d", &new_role);
            if (new_role >= 0 && new_role <= 2) {
                accounts[user_index - 1].role = new_role;
                printf("Role change avec succès.\n");
                save_users_to_file();
            } else {
                printf("Role invalide. Veuillez reessayer.\n");
            }
        }
    } else {
        printf("Selection d'utilisateur invalide. Veuillez reessayer.\n");
    }
}


void agent() 
{
    int option_agent;
    do
    {
        printf("--------------------------------\n");
        printf("Menu Agent\n");
        printf("1-Ajouter une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Trouver une reclamation:\n");
        printf("4-Traiter une reclamation:\n");
        printf("5-Se deconnecter:\n");
        printf("Entrez une option: ");
        scanf("%d", &option_agent);
        switch (option_agent) 
        {
            case 1: 
                if (reclamation_count != max_reclamation)
                {
                    reclamation_fct();
                }
                else
                {
                    printf("Le nombre de reclamations a atteint le maximum.\n");
                }
                break;
            case 2:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        modify_delete_reclamation();
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 3:
                for (int i = 0; i < existing_accounts; i++) {
                    printf("%d- %s\n", i + 1, accounts[i].username);
                }
                break;
            case 4:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        printf("\nMotif: %s\nID: %s\nStatus: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nUser: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 5: 
                change_role(); 
                break;
            case 6: 
                search_reclamation(); 
                break;
            case 7: 
                traiter_reclamation(); 
                break;
            case 8: 
                if (reclamation_count > 0) 
                {
                    print_by_priority();
                }
                else
                {
                    printf("Aucune reclamation disponible.\n");
                }
                
                break;
            case 9: 
                logged_account = -1; 
                return;
            default: 
                printf("Option invalide\n"); 
                break;
        }
    } while (option_agent != 5);
    
}

void client() {
    int option_client;
    do 
    {
        printf("--------------------------------\n");
        printf("Menu Client\n");
        printf("1-Entrer une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Afficher mes reclamations:\n");
        printf("4-Se deconnecter:\n");
        printf("Entrez une option: ");

        scanf("%d", &option_client);
        switch (option_client) {
            case 1:
                if (reclamation_count != max_reclamation) 
                {
                    reclamation_fct();
                } 
                else 
                {
                    printf("Le nombre de reclamations a atteint le maximum.\n");
                }
                break;
            case 2: 
                modify_delete_reclamation(); 
                break;
            case 3: 
                if (reclamation_count > 0)
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        if (strcmp(reclamations[i].username, accounts[logged_account].username) == 0) 
                        {
                            printf("\nMotif: %s\nID: %s\nStatus: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                            printf("\nUser: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                        }
                    }
                }
                else
                {
                    printf("Aucune reclamation disponible.\n");
                }
                
                break;
            case 4: 
                logged_account = -1; return;
            default: printf("Option invalide \n"); break;
        }
    } while (option_client != 4);
}

void reclamation_fct() 
{
    printf("--------------------------------\n");
    reclamation new_reclamation;
    account logged_rn = accounts[logged_account];
    printf("Entrez le motif de la reclamation: \n");
    getchar();
    fgets(new_reclamation.motif, max_motif - 1, stdin);
    new_reclamation.motif[strcspn(new_reclamation.motif, "\n")] = '\0'; // Remove newline character
    printf("Entrez la description de votre problème:\n");
    fgets(new_reclamation.description, max_description - 1, stdin);
    new_reclamation.description[strcspn(new_reclamation.description, "\n")] = '\0'; // Remove newline character
    
    int category_number;
    for (int i = 0; i < 4; i++) 
    {
        printf("%d-%s\n", i + 1, categories[i]);
    }
    do 
    {
        printf("Choisissez une categorie: ");
        scanf("%d", &category_number);
        if (category_number < 1 || category_number > 4) 
        {
            printf("Categorie invalide\n");
        }
        else 
        {
            strcpy(new_reclamation.category, categories[category_number - 1]);
        }
    } while (category_number < 1 || category_number > 4);
    
        strcpy(new_reclamation.username, logged_rn.username);
        strcpy(new_reclamation.status, "en cours");
        
        srand(time(NULL));
        int num;
        int id_existed;
        do {
            id_existed = 0; // Reset for each loop iteration
            for (int i = 0; i < 8; i++) 
            {
                do 
                {
                    num = rand() % 123;
                } while (!((num >= 48 && num <= 57) || (num >= 65 && num <= 90) || (num >= 97 && num <= 122)));
                new_reclamation.id[i] = (char)num;
            }
            new_reclamation.id[8] = '\0'; // Null-terminate the string
            for (int i = 0; i < reclamation_count; i++) 
            { // Check if ID already exists
                if (strcmp(new_reclamation.id, reclamations[i].id) == 0) 
                {
                    id_existed = 1;
                    break;
                }
            }
        } while (id_existed == 1);
    
                new_reclamation.time = time(NULL);
                strcpy(new_reclamation.date, ctime(&new_reclamation.time));
                new_reclamation.date[strcspn(new_reclamation.date, "\n")] = 0; // Remove newline from ctime
                
        if (strstr(new_reclamation.description, "urgent") != NULL || strstr(new_reclamation.description, "critique") != NULL) 
        {
            strcpy(new_reclamation.priority, "haute");
        } 
        else if (strstr(new_reclamation.description, "important") != NULL) 
        {
            strcpy(new_reclamation.priority, "moyenne");
        }
        else 
        {
            strcpy(new_reclamation.priority, "bas");
        }

                printf("Vos informations de reclamation sont:\nMotif: %s\nID: %s\nStatut: %s\nDate: %s\nPriorite: %s\n",
                            new_reclamation.motif, new_reclamation.id, new_reclamation.status, new_reclamation.date, new_reclamation.priority);
                printf("Votre nom d'utilisateur est: %s\nLa categorie de cette reclamation est: %s\nDescription:\n%s\n",
                            new_reclamation.username, new_reclamation.category, new_reclamation.description);
                            reclamations[reclamation_count++] = new_reclamation; // Store the new reclamation in the array
}

void traiter_reclamation() 
{
    if (reclamation_count == 0) 
    {
        printf("Aucune reclamation disponible.\n");
        return;
    }
    int traiter_option;
    do {
        printf("--------------------------------\n");
        printf("------Traiter une reclamation------\n");
        printf("1-Afficher toutes les reclamations:\n");
        printf("2-Trouver une reclamation:\n");
        printf("3-Traiter une reclamation:\n");
        printf("4-Quitter:\n");

        printf("Entrez une option: ");
        scanf("%d", &traiter_option);

        switch (traiter_option) 
        {
            case 1:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 2:
                if (reclamation_count > 0) 
                {
                    search_reclamation();
                }
                else
                {
                    printf("Aucune reclamation disponible.\n");
                }
                
                break;
            case 3:
                if (reclamation_count > 0) 
                {
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                    printf("Choisissez une reclamation à traiter:\n");
                    int num;
                    scanf("%d", &num);
                    if (num >= 1 && num <= reclamation_count) 
                    {
                        printf("Choisissez un statut:\n");
                        printf("1-En cours\n");
                        printf("2-Traite\n");
                        printf("3-Rejete\n");

                        int status;
                        scanf("%d", &status);

                        if (status == 1) 
                        {
                            strcpy(reclamations[num - 1].status, "En cours");
                        }
                         else if (status == 2) 
                        {
                            strcpy(reclamations[num - 1].status, "Traite");
                        } 
                        else if (status == 3) 
                        {
                            strcpy(reclamations[num - 1].status, "Rejete");
                        }
                        printf("entrez une note: ");
                        fgets(reclamations[num - 1].note, 100, stdin);
                    }
                } 
                else 
                {
                    printf("Aucune reclamation disponible.\n");
                }
                break;
            case 4:
                logged_account = -1;
                return;
                break;
            default:
                printf("Option invalide. Veuillez reessayer.\n");
                break;
        }
    } while (traiter_option != 4);
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
    printf("5-trouver par status:\n");
    printf("6-Quiter:\n");
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
                if (strcmp(reclamations[i].motif, search_motif) == 0)
                {
                    printf("motif: %s\tID: %s\t%s\t%s",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
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
                    printf("motif: %s\tID: %s\t%s\t%s",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;
    case 3:
        {
            int search_category;
            printf("entrez une categorie: ");
            for (int i = 0; i < 4; i++)
            {
                printf("%d - %s\n", i + 1, categories[i]);
            }
            
            scanf("%d", &search_category);
            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].category, categories[search_category - 1]) == 0)
                {
                    printf("motif: %s\tID: %s\t%s\t%s",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;
    case 4:
        {
            char search_id[9];
            printf("entrez un ID: ");
            fgets(search_id, 9, stdin);
            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].id, search_id) == 1)
                {
                    printf("motif: %s\tID: %s\t%s\t%s",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;
 
    case 5:
        {
            char search_status[10];
            printf("entrez un status: ");
            fgets(search_status, 10, stdin);
            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].status, search_status) == 0)
                {
                    printf("motif: %s\tID: %s\t%s\t%s",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("%s\t%s\n%s", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
        }
        break;
    case 6:
        return;
        break;
    }
    }while (search_option != 4);
}

void modify_delete_reclamation() 
{
    char id[9];
    printf("Entrez l'ID de la reclamation à modifier ou supprimer: ");
    scanf("%8s", id);
    int found = -1;

    for (int i = 0; i < reclamation_count; i++) 
    {
        if (strcmp(reclamations[i].id, id) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found != -1) 
    {
        int option;
        printf("Reclamation trouvee. Que voulez-vous faire?\n");
        printf("1 - Modifier\n2 - Supprimer\n");
        scanf("%d", &option);

        if (option == 1) 
        {
            printf("Entrez le nouveau motif: ");
            getchar();
            fgets(reclamations[found].motif, max_motif - 1, stdin);
            reclamations[found].motif[strcspn(reclamations[found].motif, "\n")] = '\0'; // Remove newline
            
            printf("Entrez la nouvelle description: ");
            fgets(reclamations[found].description, max_description - 1, stdin);
            reclamations[found].description[strcspn(reclamations[found].description, "\n")] = '\0'; // Remove newline
            
            printf("Reclamation modifiee avec succès.\n");
        } 
        else if (option == 2) 
        {
            // Shift elements to delete the reclamation
            for (int i = found; i < reclamation_count - 1; i++) 
            {
                reclamations[i] = reclamations[i + 1];
            }
            reclamation_count--;
            printf("Reclamation supprimee avec succès.\n");
        }
         else 
        {
            printf("Option invalide.\n");
        }
    } 
    else 
    {
        printf("Aucune reclamation trouvee avec cet ID.\n");
    }
}

void save_users_to_file() 
{
    FILE *file = fopen("users.txt", "wb");
    if (file) 
    {
        for (int i = 0; i < existing_accounts; i++)
        {
            fprintf(file, "%s %s %d\n", accounts[i].username, accounts[i].password, accounts[i].role);
        }
    } 
    else 
    {
        printf("Erreur lors de l'enregistrement des utilisateurs.\n");
    }
        fclose(file);
}

void load_users_from_file() 
{
    FILE *file = fopen("users.txt", "rb");
    if (file) 
    {
        int i = 0;
        while (i < max_users && fscanf(file, "%s %s %d\n", &accounts[i].username, &accounts[i].password, &accounts[i].role) != EOF)
        {
        i++ ;
        }
        existing_accounts = i;
        fclose(file);
    }
     else 
    {
        printf("Erreur lors du chargement des utilisateurs.\n");
    }
}

void print_by_priority()
{
        for (int i = 0; i < reclamation_count ; i++)
    {
        for (int j = i+1; j < reclamation_count  ; j++)
        {
            if (GetPriorityValue(reclamations[j].priority) > GetPriorityValue(reclamations[i].priority))
            {
                reclamation temp = reclamations[i];
                reclamations[i] = reclamations[j];
                reclamations[j] = temp;
            }
        }
    }


    printf("Prioriter haute: \n");
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "haut") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", reclamations[i].username, reclamations[i].category, reclamations[i].description);        
        }
    }
    printf("Prioriter moyenne: \n");

    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "moyen") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", reclamations[i].username, reclamations[i].category, reclamations[i].description);        
        }

    }
    
    printf("Prioriter base: \n");
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "base") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", reclamations[i].username, reclamations[i].category, reclamations[i].description);        
        }

    }
}

int GetPriorityValue() 
{
    for (int i = 0; i < reclamation_count; i++)
    {
    if (strcmp(reclamations[i].priority, "haute"))
        return 3;

    if (strcmp(reclamations[i].priority, "moyenne"))
        return 2;

    if (strcmp(reclamations[i].priority, "bas"))
        return 1;
    }
}
