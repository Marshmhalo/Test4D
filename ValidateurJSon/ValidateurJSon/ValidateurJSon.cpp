// ValidateurJSon.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#define TEST "{ \
\"glossary\": { \
    \"title\": \"example glossary\", \
        \"GlossDiv\" : { \
        \"title\": \"S\", \
            \"GlossList\" : { \
            \"GlossEntry\": { \
                \"ID\": \"SGML\", \
                    \"SortAs\" : \"SGML\", \
                    \"GlossTerm\" : \"Standard Generalized Markup Language\", \
                    \"Acronym\" : \"SGML\", \
                    \"Abbrev\" : \"ISO 8879:1986\", \
                    \"GlossDef\" : { \
                    \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\", \
                        \"GlossSeeAlso\" : [\"GML\", \"XML\"] \
                }, \
                    \"GlossSee\" : \"markup\", \
                    \"testGpi\" : [{\"test1\" : \"5\"},{\"test2\" : 3}],\
                    \"gpi\" : 15 \
            } \
        } \
    } \
} \
}"


#include <stdio.h>
#include <iostream>

#include "ParserJSON.h"

int main()
{
    std::string test;
    std::cout << "Entrer la chaine a parser :" << std::endl;
    std::cin >> test;

    //CParserJSON* pParser = new CParserJSON(std::string(TEST));
    CParserJSON* pParser = new CParserJSON(test);
    if (pParser->parse())
        std::cout << std::endl << "parsing OK" << std::endl;
    else
        std::cout << std::endl << "parsing KO" << std::endl;
    delete pParser;
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
