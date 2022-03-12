// ValidateurJSon.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>

#include "ProjectDefines.h"
#include "ObjectJSON.h"

int main()
{
    std::vector <std::string> vect;
    vect.push_back(std::string("test1"));
    vect.push_back(std::string("test2"));
    CObjectJSON<int>*           pObjectNull = new CObjectJSON<int>(E_TOJ_null, std::string("GpiNull"), 0);
    CObjectJSON<std::string>*   pObjectStr  = new CObjectJSON<std::string>(E_TOJ_string, std::string("GpiStr"), vect);
    CObjectJSON<int>*           pObjectNb   = new CObjectJSON<int>(E_TOJ_number, std::string("GpiNb"), 7);
    CObjectJSON<std::string>*   pObjectObj  = new CObjectJSON<std::string>(E_TOJ_object, std::string("GpiObj"), std::string("obj"));
    CObjectJSON<int>*           pObjectBool = new CObjectJSON<int>(E_TOJ_boolean, std::string("GpiBool"), 1);

    pObjectNull->printObject();
    pObjectStr->printObject();
    pObjectNb->printObject();
    pObjectObj->printObject();
    pObjectBool->printObject();

    delete pObjectNull;
    delete pObjectStr;
    delete pObjectNb;
    delete pObjectObj;
    delete pObjectBool;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
