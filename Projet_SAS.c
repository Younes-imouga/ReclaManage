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
#define max_time 840000
#define max_note 50

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
    char note[max_note]; // Consider changing this to an appropriate type
    time_t status_changed;
} reclamation;

        char categories[4][15] = {"financielle", "technique", "service", "autre"};
        account accounts[max_users];
        int existing_accounts = 0;

        reclamation reclamations[max_reclamation];
        reclamation by_priority[max_reclamation];

        int reclamation_count = 0;
        int logged_account;

        int temps_moyen;
        int temps_total;
        int Traite = 0;

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

void taux_de_resolution();
void delai_moyen();
void Rapport_journalier();


int main() 
{
    load_users_from_file();

    int option = 0;

    do 
    {
        printf("\n=================================\n");
        printf("         Menu Principal           \n");
        printf("=================================\n");
        printf("1-S'inscrire:\n");
        printf("2-Se connecter:\n");
        printf("3-Sortie\n");

        printf("Entrer une option: ");
        if (scanf("%d", &option) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }

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

    printf("\n==============================\n");
    printf("         Inscription           \n");
    printf("==============================\n");

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
                printf("Le mot de passe n'est pas valide. Il doit comporter au moins 8 caracteres et contenir des majuscules, minuscules, chiffres et caracteres speciaux.\n");
            }
        } while (!pass_valid);
        
        // Determine role
        new_account.role = (existing_accounts == 0) ? 1 : 0;
        accounts[existing_accounts++] = new_account;
        save_users_to_file();
        printf("Compte cree avec succes.\nVos informations de compte sont:\nUser: %s\tPassword: %s\n", new_account.username, new_account.password);
    } else {
        printf("Le nombre maximum d'utilisateurs a ete atteint\n");
    }
}

void log_in() {
    char user_name[max_name];
    char password[max_password];
    int login_successful = 0;
    int failed = 3;

    printf("\n==============================\n");
    printf("         Connexion            \n");
    printf("==============================\n");

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
                printf("Vous avez echoue 3 fois, vous pouvez essayer à nouveau apres 30 minutes.\n");
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
        printf("\n==============================\n");
        printf("         Menu Administrateur   \n");
        printf("==============================\n");

        printf("1-Ajouter une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Afficher les utilisateurs:\n");
        printf("4-Voir toutes les reclamations\n");
        printf("5-Changer le role:\n");
        printf("6-Trouver une reclamation:\n");
        printf("7-Traiter une reclamation:\n");
        printf("8-afficher les reclamations ordonees par priorite:\n");
        printf("9-afficher le nombre des reclamation:\n");
        printf("10-afficher le taux de resolution: \n");
        printf("11-afficher le delai moyen: \n");
        printf("12-executer le rapport journalier: \n");
        printf("13- quitter:\n");

        printf("Entrez une option: ");
        if (scanf("%d", &option_admin) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }

        
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
                printf("le nombre des reclamation est: %d",reclamation_count);
                break;
            case 10:
                taux_de_resolution();
                break;
            case 11:
                delai_moyen();
                break;
            case 12:
                Rapport_journalier();
                break;
            case 13:
                logged_account = -1; 
                return;
            default: 
                printf("Option invalide\n"); 
                break;
        }
    } while (option_admin != 13);
}

void change_role() {
    printf("\n==============================\n");
    printf("        Changer de Role       \n");
    printf("==============================\n");

    printf("Selectionnez un utilisateur pour changer de role:\n");

    printf("==============================\n");
    for (int i = 0; i < existing_accounts; i++) 
    {
        printf("%d - %s\n", i + 1, accounts[i].username);
        printf("==============================\n");
    }
    int user_index;
    printf("Entrez le numero de l'utilisateur: ");
        if (scanf("%d", &user_index) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }


    if (user_index > 0 && user_index <= existing_accounts) 
    {
        if (strcmp(accounts[user_index - 1].username, accounts[logged_account].username) == 0) {
            printf("Vous ne pouvez pas changer VOTRE role.\n");
        }
        else 
        {
            printf("Entrez le nouveau role (1 - admin, 2 - agent, 0 - client): ");
            int new_role;
            if (scanf("%d", &new_role) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }


            if (new_role >= 0 && new_role <= 2) 
            {
                accounts[user_index - 1].role = new_role;
                printf("Role change avec succes.\n");
                save_users_to_file();
            }
             else 
            {
                printf("Role invalide. Veuillez reessayer.\n");
            }
        }
    } 
    else 
    {
        printf("Selection d'utilisateur invalide. Veuillez reessayer.\n");
    }
}

void agent() 
{
    int option_agent;
    do
    {
        printf("\n==============================\n");
        printf("           Menu Agent         \n");
        printf("==============================\n");

        printf("1-Ajouter une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Trouver une reclamation:\n");
        printf("4-afficher tout les reclamation:\n");
        printf("5-Traiter une reclamation:\n");
        printf("6-Se deconnecter:\n");

        printf("Entrez une option: ");
        if (scanf("%d", &option_agent) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }


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
                search_reclamation(); 
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
                traiter_reclamation(); 
                break;

            case 6: 
                logged_account = -1; 
                return;
            default: 
                printf("Option invalide\n"); 
                break;

        }
    } while (option_agent != 6);
    
}

void client() 
{
    int option_client;
    do 
    {
        printf("\n==============================\n");
        printf("          Menu Client         \n");
        printf("==============================\n");

        printf("1-Entrer une reclamation:\n");
        printf("2-Modifier/Supprimer une reclamation:\n");
        printf("3-Afficher mes reclamations:\n");
        printf("4-Se deconnecter:\n");
        printf("Entrez une option: ");

        if (scanf("%d", &option_client) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }

        switch (option_client) 
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
                            break;
                        }
                        else if (i == reclamation_count - 1) 
                        {
                            printf("Aucune reclamation disponible.\n");
                        }   
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
            default: 
                printf("Option invalide \n"); 
                break;
        }
    } while (option_client != 4);
}

void reclamation_fct() 
{
    printf("\n================================\n");
    printf("         Reclamation Menu       \n");
    printf("================================\n");

    reclamation new_reclamation;
    account logged_rn = accounts[logged_account];

    printf("Entrez le motif de la reclamation: \n");
    getchar();
    fgets(new_reclamation.motif, max_motif - 1, stdin);
    new_reclamation.motif[strcspn(new_reclamation.motif, "\n")] = '\0'; // Remove newline character
    
    printf("Entrez la description de votre probleme:\n");
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
            strcpy(new_reclamation.priority, "temps_moyen");
        }
        else 
        {
            strcpy(new_reclamation.priority, "base");
        }

                printf("Vos informations de reclamation sont:\nMotif: %s\nID: %s\nStatut: %s\nDate: %s\nPriorite: %s\n", new_reclamation.motif, new_reclamation.id, new_reclamation.status, new_reclamation.date, new_reclamation.priority);
                printf("Votre nom d'utilisateur est: %s\nLa categorie de cette reclamation est: %s\nDescription:\n%s\n", new_reclamation.username, new_reclamation.category, new_reclamation.description);
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
    do 
    {
        printf("\n=====================================\n");
        printf("          Traiter Reclamation        \n");
        printf("=====================================\n");
        printf("1 - Afficher toutes les reclamations\n");
        printf("2 - Trouver une reclamation\n");
        printf("3 - Traiter une reclamation\n");
        printf("4 - Quitter\n");

        printf("Entrez une option: ");
        if (scanf("%d", &traiter_option) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }

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
                    int num ;
                    int choix = 0;
                    for (int i = 0; i < reclamation_count; i++) 
                    {
                        printf("%d - Motif: %s\nID: %s\nStatut: %s\nDate: %s",choix + 1, reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    choix++;
                    }
                    printf("Choisissez une reclamation à traiter:\n");
                    scanf("%d", &num);
                    if (num >= 1 && num <= reclamation_count) 
                    {
                        printf("Choisissez un statut:\n");
                        printf("1-En cours\n");
                        printf("2-Traite\n");
                        printf("3-Rejette\n");

                        int status;
                        scanf("%d", &status);

                        if (status == 1) 
                        {
                            strcpy(reclamations[num - 1].status, "En cours");
                        }
                         else if (status == 2) 
                        {
                            strcpy(reclamations[num - 1].status, "Traite");
                            Traite++;
                            reclamations[num - 1].status_changed = time(NULL);
                            temps_total += difftime(reclamations[num - 1].time, reclamations[num - 1].status_changed);
                        } 
                        else if (status == 3) 
                        {
                            strcpy(reclamations[num - 1].status, "Rejette");
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
        printf("\n=================================\n");
        printf("       Rechercher Reclamation     \n");
        printf("=================================\n");
        printf("1 - Trouver par motif\n");
        printf("2 - Trouver par utilisateur\n");
        printf("3 - Trouver par categorie\n");
        printf("4 - Rechercher par ID\n");
        printf("5 - Trouver par statut\n");
        printf("6 - Quitter\n");
        printf("Entrez une option: ");
    
        if (scanf("%d", &search_option) != 1) 
        {
            printf("Erreur: Veuillez entrer une option valide.\n");
            while (getchar() != '\n'); 
        }


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
                    printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
            return;
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
                    printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
            return;

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
            
            if (scanf("%d", &search_category) != 1) 
            {
                printf("Erreur: Veuillez entrer une option valide.\n");
                while (getchar() != '\n'); 
            }

            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].category, categories[search_category - 1]) == 0)
                {
                    printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
            return;
        }
        break;
    case 4:
        {
            char search_id[9];
            printf("entrez un ID: ");
            fgets(search_id, 9, stdin);
            for (int i = 0; i < reclamation_count; i++)
            {
                if (strcmp(reclamations[i].id, search_id) == 0)
                {
                    printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                    printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                }
            }
            return;
        }
        break;
 
    case 5:
        {
            int search_status;
            printf("\n1-En cours\n2-Resolu\n3-Annule\n");
            printf("entrez un status: ");
            if (scanf("%d", &search_status) != 1)
            {
                printf("Erreur: Veuillez entrer une option valide.\n");
                while (getchar() != '\n');
            }

            for (int i = 0; i < reclamation_count; i++)
            
            {
                switch (search_status)
                {
                    case 1: 
                    if (strcmp(reclamations[i].status, "en cours") == 0)
                    {
                        printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                    break;

                    case 2:
                    if (strcmp(reclamations[i].status, "resolu") == 0)
                    {
                        printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                    break;

                    case 3:
                    if (strcmp(reclamations[i].status, "rejeter") == 0)
                    {
                        printf("Motif: %s\nID: %s\nStatut: %s\nDate: %s", reclamations[i].motif, reclamations[i].id, reclamations[i].status, reclamations[i].date);
                        printf("\nNom d'utilisateur: %s\nCategorie: %s\nDescription: %s\n", reclamations[i].username, reclamations[i].category, reclamations[i].description);
                    }
                    break;
                }
            }
            return;
        }
        break;
    case 6:
        return;
        break;
    }
    }while (search_option != 6);
}

void modify_delete_reclamation()
{
    if (accounts[logged_account].role == 0)
    {
        char reclamation_id[9];
        int i;
        printf("Entrez l'ID de la reclamation que vous souhaitez modifier ou supprimer: ");
        scanf("%s", &reclamation_id);
        int found = 0;
        for (i = 0; i < reclamation_count; i++)
        {
            if (strcmp(reclamations[i].id, reclamation_id) == 0 && strcmp(reclamations[i].username, accounts[logged_account].username) == 0)
            {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            printf("Reclamation non trouvee.\n");
            return;
        }
        if (strcmp(reclamations[i].status, "en cours") == 0 && difftime(time(NULL), reclamations[i].time) < max_time) 
        {
            printf("1-Modifier la reclamation\n");
            printf("2-Supprimer la reclamation\n");
            printf("Entrez votre choix: ");
            int choice;
            char motif_new[max_motif];
            char description_new[max_description];
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Entrez un nouveau motif (vide pour ne pas modifier): ");
                    getchar();
                    fgets(motif_new, max_motif - 1, stdin);
                        if (strlen(motif_new) == 0) 
                        {
                            printf("Motif non modifie.\n");
                        }
                        else
                        {
                        strcpy(reclamations[i].motif, motif_new);
                        reclamations[i].motif[strcspn(reclamations[i].motif, "\n")] = '\0';
                        }
                    printf("Entrez une nouvelle description (vide pour ne pas modifier): ");
                    fgets(description_new, max_description - 1, stdin);
                        if (strlen(description_new) == 0) 
                        {
                            printf("Description non modifiee.\n");
                        }
                        else
                        {
                        strcpy(reclamations[i].description, description_new);
                        reclamations[i].description[strcspn(reclamations[i].description, "\n")] = '\0'; // remove newline character
                        }
                    break;
                case 2:
                    for (int j = i; j < reclamation_count - 1; j++) 
                    {
                        reclamations[j] = reclamations[j + 1];
                    }
                    reclamation_count--;
                    printf("La reclamation a ete supprimee avec succes.\n");
                    break;
                default:
                    printf("Choix invalide.\n");
                    break;
            }
        }
        else
        {
            printf("La reclamation n'est pas eligible a la modification ou a la suppression.\n");
        }
    }
    
    else
    {
        char id[9];
        printf("=================================\n");
        printf("   Modifier/Supprimer Reclamation\n");
        printf("=================================\n");
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
                
                printf("Reclamation modifiee avec succes.\n");
            } 
            else if (option == 2) 
            {
                // Shift elements to delete the reclamation
                for (int i = found; i < reclamation_count - 1; i++) 
                {
                    reclamations[i] = reclamations[i + 1];
                }
                reclamation_count--;
                printf("Reclamation supprimee avec succes.\n");
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

        for (int i = 0; i < reclamation_count; i++)
        {
            by_priority[i] = reclamations[i];
        }
        

    for (int i = 0; i < reclamation_count ; i++)
    {
        for (int j = i+1; j < reclamation_count  ; j++)
        {
            if (GetPriorityValue(by_priority[j].priority) > GetPriorityValue(by_priority[i].priority))
            {
                reclamation temp = by_priority[i];
                by_priority[i] = by_priority[j];
                by_priority[j] = temp;
            }
        }
    }


    printf("Prioriter haute: \n");
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(by_priority[i].priority, "haute") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",by_priority[i].motif, by_priority[i].id, by_priority[i].status, by_priority[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", by_priority[i].username, by_priority[i].category, by_priority[i].description);        
        }
    }
    printf("Prioriter temps_moyen: \n");

    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(by_priority[i].priority, "moyen") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",by_priority[i].motif, by_priority[i].id, by_priority[i].status, by_priority[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", by_priority[i].username, by_priority[i].category, by_priority[i].description);        
        }

    }
    
    printf("Prioriter basee: \n");
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].priority, "basee") == 0)
        {
        printf("motif: %s\nID: %s\nStatus: %s\nDate: %s\n",by_priority[i].motif, by_priority[i].id, by_priority[i].status, by_priority[i].date);
        printf("User: %s\nCategorie: %s\nDescription: %s", by_priority[i].username, by_priority[i].category, by_priority[i].description);        
        }

    }
}

int GetPriorityValue(char *priority) 
{
    for (int i = 0; i < reclamation_count; i++)
    {
    if (strcmp(reclamations[i].priority, "haute"))
        return 3;

    if (strcmp(reclamations[i].priority, "temps_moyen"))
        return 2;

    if (strcmp(reclamations[i].priority, "base"))
        return 1;
    }
}

void taux_de_resolution()
{
    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].status, "Traite") == 0)
        {
            Traite++;
        }
    }
    printf("traiter: %d\n", Traite);
    printf("num total: %d\n", reclamation_count);
    printf("taux de resolution: %d\n", Traite * 100 / reclamation_count);
}

void delai_moyen()
{
    temps_moyen = temps_total / Traite;
    int temps_moyen_heur = temps_moyen / 3600;
    int temps_moyen_minute = (temps_moyen % 3600) / 60;
    int temps_moyen_seconde = temps_moyen % 60;

    printf("delai moyen: %d : %d : %d\n", temps_moyen_heur, temps_moyen_minute, temps_moyen_seconde);
}

void Rapport_journalier()
{
    int en_cours = 0;
    int rejeter = 0;

    for (int i = 0; i < reclamation_count; i++)
    {
        if (strcmp(reclamations[i].status, "en cours"))
        {
            en_cours++;
        }
        if (strcmp(reclamations[i].status, "rejeter"))
        {
            rejeter++;
        }
        
    }
    
    FILE *file = fopen("rapport_quotidien.txt", "w");
    if (!file) 
    {
        printf("Erreur lors de la création du fichier.\n");
        return;
    }
    else
    {
    fprintf(file, "Rapport Quotidien des Réclamations\n");
    fprintf(file, "=================================\n\n");

    fprintf(file, "Nombre total de réclamations : %d\n", reclamation_count);
    
    fprintf(file, "Nombre total de réclamations en cours: %d\n", en_cours);
    fprintf(file, "Nombre total de réclamations Traiter: %d\n", Traite);
    fprintf(file, "Nombre total de réclamations rejeter: %d\n", rejeter);


    fprintf(file, "Taux de resolution des réclamations : %.2f%%\n", Traite * 100 / reclamation_count);
    fprintf(file, "Delai moyen de traitement des reclamations resolues : %.2f secondes\n", temps_moyen);

    fprintf(file, "=================================\n\n");

    fclose(file);
    printf("Rapport quotidien genere avec succes.\n");
    return;
    }
}

// same name with upper case works it shouldn't damn it

// client modification : when there's no claim it still tell you to choose an id

// TRAITER LES RECLAMATION afficher les reclamation just add \n

// fix search by status