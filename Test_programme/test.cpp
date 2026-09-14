#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <exception>

#include "Transaction.h"
#include "Currentaccount.h"
#include "Savingsaccount.h"
#include "Client.h"
#include "Bank.h"

int main()
{
    std::cout << "==========================================" << std::endl;
    std::cout << "       TESTS DES CLASSES BANCAIRES        " << std::endl;
    std::cout << "==========================================" << std::endl;

    try
    {
        // ----------------------------------------------------
        // TEST 1 : Instanciation de la Banque et des Clients
        // ----------------------------------------------------
        std::cout << "\n[TEST 1] Creation de la banque et ajout de clients..." << std::endl;
        Bank myBank("BNP C++");

        myBank.addclient(Client(01, "Alice Dupont", 1234, std::vector<std::shared_ptr<BankAccount>>()));
        myBank.addclient(Client(02, "Bob Martin", 5678, std::vector<std::shared_ptr<BankAccount>>()));
        std::cout << "-> Clients ajoutes avec succes." << std::endl;

        // ----------------------------------------------------
        // TEST 2 : Ouverture de Comptes (Courant & Epargne)
        // ----------------------------------------------------
        std::cout << "\n[TEST 2] Ouverture des comptes..." << std::endl;

        // Alice : 1000€ solde, 200€ découvert autorisé
        std::string accAliceCC = myBank.createCurrentAccount(01, 1000.0, 200.0);
        // myBank.getClientByID(01)->addAccount(myBank.getAccountByNumber(accAliceCC));
        // std::vector<std::shared_ptr<BankAccount>> aliceAccounts = myBank.getClientByID(01)->getAccounts();

        // for (const auto &account : aliceAccounts)
        //{
        //     std::cout << "-> Compte d'Alice : " << account->getAccountNumber() << ", Solde : " << account->getBalance() << std::endl;
        // }
        //  Alice : 5000€ solde, 3% d'intérêts
        std::string accAliceCE = myBank.createSavingsAccount(01, 5000.0, 0.03);

        // Bob : 500€ solde, 100€ découvert
        std::string accBobCC = myBank.createCurrentAccount(02, 500.0, 100.0);

        std::cout << "-> Compte Courant Alice : " << accAliceCC << std::endl;
        std::cout << "-> Compte Epargne Alice : " << accAliceCE << std::endl;
        std::cout << "-> Compte Courant Bob   : " << accBobCC << std::endl;

        // ----------------------------------------------------
        // TEST 3 : Operations simples (Depot & Retrait)
        // ----------------------------------------------------
        myBank.displayBankInfo();
        std::cout << "\n[TEST 3] Operations sur les comptes d'Alice..." << std::endl;
        auto compteAlice = myBank.getAccountByNumber(accAliceCC);

        if (compteAlice)
        {
            compteAlice->deposit(250.0);
            std::cout << "-> Depot de 250 EUR effectue. Nouveau solde : "
                      << compteAlice->getBalance() << " EUR" << std::endl;

            // Retrait utilisant le découvert autorisé
            compteAlice->withdraw(1300.0);
            std::cout << "-> Retrait de 1300 EUR effectue (decouvert). Nouveau solde : "
                      << compteAlice->getBalance() << " EUR" << std::endl;
        }

        // ----------------------------------------------------
        // TEST 4 : Virement inter-comptes
        // ----------------------------------------------------
        std::cout << "\n[TEST 4] Virement entre Alice et Bob..." << std::endl;
        // Alice envoie 300€ à Bob depuis son compte épargne
        bool virementReussi = myBank.maketransfer(accAliceCE, accBobCC, 300.0);
        std::cout << "-> Statut du virement : " << (virementReussi ? "SUCCES" : "ECHEC") << std::endl;

        // ----------------------------------------------------
        // TEST 5 : Polymorphisme & Fin d'annee (Interets)
        // ----------------------------------------------------
        std::cout << "\n[TEST 5] Simulation de fin d'annee (calcul des interets)..." << std::endl;
        myBank.simulateendyear();

        // ----------------------------------------------------
        // TEST 6 : Test des cas d'erreur (Robustesse)
        // ----------------------------------------------------
        std::cout << "\n[TEST 6] Test des cas limites et erreurs..." << std::endl;

        // Retrait dépassant le découvert
        std::cout << "Tentative de retrait excessif chez Bob :" << std::endl;
        auto compteBob = myBank.getAccountByNumber(accBobCC);
        if (compteBob)
        {
            bool ok = compteBob->withdraw(10000.0); // Doit échouer
            std::cout << "-> Retrait de 10 000 EUR : " << (ok ? "Accepte" : "Refuse (Normal)") << std::endl;
        }
        myBank.saveToFile("bank_data.json");

        // Ajout d'un client doublon (Doit lever une exception si tu l'as géré)
        std::cout << "Tentative d'ajout d'un client existant :" << std::endl;
        myBank.addclient(Client(01, "Doublon Alice", 0000, std::vector<std::shared_ptr<BankAccount>>()));
    }
    catch (const std::exception &e)
    {
        // Attrape les exceptions (ex: runtime_error) et évite le crash brutal
        std::cerr << "\n[EXCEPTION ATTRAPEE] : " << e.what() << std::endl;
    }

    // ----------------------------------------------------
    // BILAN FINAL
    // ----------------------------------------------------
    std::cout << "\n==========================================" << std::endl;
    std::cout << "            BILAN FIN DE TEST             " << std::endl;
    std::cout << "==========================================" << std::endl;

    // Si tu as codé cette méthode, affiche l'état complet
    Bank myBank;
    myBank.loadFromFile("bank_data.json");
    myBank.displayBankInfo();

    return 0;
}
