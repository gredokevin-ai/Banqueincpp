#include <iostream>
#include <string>
#include <limits>
#include <exception>

#include "Bank.h"
#include "Client.h"
#include "Bankaccount.h"
#include "Currentaccount.h"
#include "Savingsaccount.h"
#include "Transaction.h"

void nettoyerTampon()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Lit un entier de manière sécurisée
int lireChoix()
{
    int choix;
    while (!(std::cin >> choix))
    {
        nettoyerTampon();
        std::cout << "Saisie invalide. Entrez un nombre : ";
    }
    return choix;
}

// --- SOUS-MENUS ---

void afficherMenuAdmin(Bank &banque)
{
    bool retour = false;
    while (!retour)
    {
        std::cout << "\n--- ESPACE ADMINISTRATEUR ---\n"
                  << "1. Ajouter un client\n"
                  << "2. Ouvrir un compte courant\n"
                  << "3. Ouvrir un compte epargne\n"
                  << "4. Afficher le bilan de la banque\n"
                  << "5. Simuler les interets de fin d'annee\n"
                  << "6. Sauvegarder les donnees (JSON)\n"
                  << "0. Retour au menu principal\n"
                  << "Choix : ";

        int choix = lireChoix();
        try
        {
            switch (choix)
            {
            case 1:
            {
                int id, pin;
                std::string nom;
                // std::string id, nom, pin;
                std::cout << "ID Client : ";
                std::cin >> id;
                std::cout << "Nom : ";
                std::cin.ignore();
                std::getline(std::cin, nom);
                std::cout << "Code PIN (4 chiffres) : ";
                std::cin >> pin;
                banque.addclient(Client(id, nom, pin, std::vector<std::shared_ptr<BankAccount>>()));
                std::cout << "-> Client ajoute avec succes.\n";
                break;
            }
            case 2:
            {
                int id;
                double solde, decouvert;
                std::cout << "ID Client : ";
                std::cin >> id;
                std::cout << "Solde initial : ";
                std::cin >> solde;
                std::cout << "Decouvert autorise : ";
                std::cin >> decouvert;
                std::string num = banque.createCurrentAccount(id, solde, decouvert);
                std::cout << "-> Compte courant cree : " << num << "\n";
                break;
            }
            case 3:
            {
                int id;
                double solde, taux;
                std::cout << "ID Client : ";
                std::cin >> id;
                std::cout << "Solde initial : ";
                std::cin >> solde;
                std::cout << "Taux d'interet (ex: 0.03) : ";
                std::cin >> taux;
                std::string num = banque.createSavingsAccount(id, solde, taux);
                std::cout << "-> Compte epargne cree : " << num << "\n";
                break;
            }
            case 4:
                banque.displayBankInfo();
                break;
            case 5:
                banque.simulateendyear();
                break;
            case 6:
                if (banque.saveToFile("banque.json"))
                {
                    std::cout << "-> Fichier banque.json sauvegarde.\n";
                }
                else
                {
                    std::cerr << "-> Erreur lors de la sauvegarde.\n";
                }
                break;
            case 0:
                retour = true;
                break;
            default:
                std::cout << "Option invalide.\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "\n[ERREUR] " << e.what() << "\n";
        }
    }
}

void afficherEspaceClient(Bank &banque)
{
    int idClient, pin;
    std::cout << "\n--- CONNEXION CLIENT ---\n";
    std::cout << "ID Client : ";
    std::cin >> idClient;
    std::cout << "Code PIN : ";
    std::cin >> pin;

    Client *client = banque.getClientByID(idClient);
    if (!client || !client->authentification(pin))
    {
        std::cout << "-> Identifiants incorrects.\n";
        return;
    }

    bool deconnexion = false;
    while (!deconnexion)
    {
        std::cout << "\n--- BIENVENUE " << client->getName() << " ---\n"
                  << "1. Mes soldes & comptes\n"
                  << "2. Effectuer un depot\n"
                  << "3. Effectuer un retrait\n"
                  << "4. Effectuer un virement\n"
                  << "0. Se deconnecter\n"
                  << "Choix : ";

        int choix = lireChoix();
        try
        {
            switch (choix)
            {
            case 1:
                client->displayAccounts();
                break;
            case 2:
            {
                std::string numCompte;
                double montant;
                std::cout << "Numero de compte : ";
                std::cin >> numCompte;
                std::cout << "Montant à deposer : ";
                std::cin >> montant;
                auto compte = client->chercherCompte(numCompte);
                if (compte)
                {
                    compte->deposit(montant);
                    std::cout << "-> Depot effectue.\n";
                }
                else
                {
                    std::cout << "-> Compte non trouve.\n";
                }
                break;
            }
            case 3:
            {
                std::string numCompte;
                double montant;
                std::cout << "Numero de compte : ";
                std::cin >> numCompte;
                std::cout << "Montant à retirer : ";
                std::cin >> montant;
                auto compte = client->chercherCompte(numCompte);
                if (compte && compte->withdraw(montant))
                {
                    std::cout << "-> Retrait effectue.\n";
                }
                else
                {
                    std::cout << "-> Retrait refuse ou compte inexistant.\n";
                }
                break;
            }
            case 4:
            {
                std::string numSource, numCible;
                double montant;
                std::cout << "Depuis le compte : ";
                std::cin >> numSource;
                std::cout << "Vers le compte : ";
                std::cin >> numCible;
                std::cout << "Montant : ";
                std::cin >> montant;
                banque.maketransfer(numSource, numCible, montant);
                break;
            }
            case 0:
                deconnexion = true;
                std::cout << "Deconnexion reussie.\n";
                break;
            default:
                std::cout << "Option invalide.\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "\n[ERREUR] " << e.what() << "\n";
        }
    }
}

// --- MAIN ---

int main()
{
    Bank banque("BNP C++");

    // Tentative de chargement au demarrage
    if (!banque.loadFromFile("banque.json"))
    {
        std::cout << "Aucun fichier banque.json trouve. Initialisation d'une nouvelle banque.\n";
    }
    else
    {
        std::cout << "Donnees de la banque chargees depuis banque.json.\n";
    }

    bool quitter = false;
    while (!quitter)
    {
        std::cout << "\n========================================\n"
                  << "         GESTION BANCAIRE C++           \n"
                  << "========================================\n"
                  << "1. Espace Administrateur\n"
                  << "2. Espace Client (GAB)\n"
                  << "0. Quitter le programme\n"
                  << "Choix : ";

        int choix = lireChoix();

        switch (choix)
        {
        case 1:
            afficherMenuAdmin(banque);
            break;
        case 2:
            afficherEspaceClient(banque);
            break;
        case 0:
            quitter = true;
            std::cout << "Sauvegarde automatique avant de quitter...\n";
            banque.saveToFile("banque.json");
            std::cout << "Au revoir !\n";
            break;
        default:
            std::cout << "Option invalide. Veuillez reessayer.\n";
        }
    }

    return 0;
}
