#include "ParserJSON.h"
#include "ObjectJSON.h"


/**********************************************************************************************************************
                Constructeur CParserJSON

Description : Constructeur de la classe CParserJSON
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   stringToParse : Chanine de caractères à parser
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Instance de parser JSON initialisée
**********************************************************************************************************************/
CParserJSON::CParserJSON(std::string stringToParse)
    : m_string(stringToParse)
{

}

/**********************************************************************************************************************
                parse

Description : Fonction de parsing de la chaine de caractères
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] -
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parse()
{
    return parse(m_string);
}

/**********************************************************************************************************************
                parse

Description : Fonction de parsing de la chaine de caractères (récursive)
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] stringToParse : Chaine de caractères à parser
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parse(std::string stringToParse)
{
    char* buffer = const_cast<char*>(stringToParse.c_str());

    char* debElement = NULL, * finElement = NULL;
    char* debNom = NULL, * finNom = NULL;
    char* debValeurs = NULL, * finValeurs = NULL;
    char* debArray = NULL, * finArray = NULL;
    char* debValeur = NULL, * finValeur = NULL;
    char* debObjValeur = NULL, * finObjValeur = NULL;
    char* bufferValues = NULL;
    char* bufferValue = NULL;
    char* bufferArray = NULL;
    bool isObjectArray = false;
    bool hasNext = false, hasNextArray = false;


    while (NULL != (debElement = findChar(buffer, '{')))    // On recherche des éléments tant qu'il y en a
    {
        ++debElement;

        // Vérification d'élément "complet" ///////////////////////////////////
        if (NULL == (finElement = findChar(debElement, '}')) || finElement <= debElement+1)
        {
            printf("Parse impossible : Elément vide ou incomplet\n");
            return false;
        }
        buffer = finElement+1;
        ///////////////////////////////////////////////////////////////////////

        do
        {
            // Recherche du nom de l'élément //////////////////////////////////////
            if (NULL == (debNom = findChar(hasNext ? finValeurs+1 : debElement, '\"')) || NULL == (finNom = findChar(++debNom, '\"')) || finNom == debNom)
            {
                printf("Parse impossible : Nom de l'élément vide ou incomplet\n");
                return false;
            }
            std::string name(debNom, finNom - debNom);
            printf("name:%s:\n", name.c_str());
            ///////////////////////////////////////////////////////////////////////

            // Recherche de la valeur de l'élément ////////////////////////////////
            if (NULL == (debValeurs = findChar(finNom + 1, ':')))
            {
                printf("Parse impossible : Mauvais format (:)\n");
                return false;
            }

            finValeurs = findChar(++debValeurs, ',');
            hasNext = (NULL != finValeurs);
            if (!hasNext) finValeurs = finElement;

            if (finValeurs <= debValeurs)
            {
                printf("Parse impossible : Pas de valeur\n");
                return false;
            }
            std::string values(debValeurs, finValeurs - debValeurs);
            bufferValues = const_cast<char*>(values.c_str());

            char* end = NULL;
            const char* testInt = values.c_str();
            long lval = strtol(testInt, &end, 10);
            if (end != testInt && (*end == '\0' || *end == '\t' || *end == ' ' || *end == '\r' || *end == '\n') && errno != ERANGE)
            {   // Cas du nombre
                printf("value:%ld\n", lval);
            }
            else
            {

                debArray = findChar(bufferValues, '[');
                debObjValeur = findChar(bufferValues, '{');
                debValeur = findChar(bufferValues, '"');

                if (NULL != debArray &&
                    (NULL == debObjValeur || debArray < debObjValeur) &&
                    (NULL == debValeur || debArray < debValeur))
                { // Cas d'un tableau
                    ++debArray;

                    // Vérification de tableau complet //////////////////////
                    if (NULL == (finArray = findChar(debArray, ']')) || finArray <= debArray + 1)
                    {
                        printf("Parse impossible : Tableau vide ou incomplet\n");
                        return false;
                    }

                    std::string arrayJSON(debArray, finArray - debArray);
                    bufferArray = const_cast<char*>(arrayJSON.c_str());

                    do
                    {
                        debObjValeur = findChar(bufferArray, '{');
                        debValeur = findChar(bufferArray, '"');

                        hasNextArray = (NULL != (debArray = findChar(bufferArray, ',')));

                        if (NULL != debObjValeur &&
                            (NULL == debValeur || debObjValeur < debValeur))
                        {   // Cas de la valeur objet
                            hasNextArray = false; // Déjà géré dabns la fonction de base
                            if (false == parse(arrayJSON))
                                return false;
                        }
                        else if (NULL != debValeur)
                        {
                            if (NULL == (debValeur = findChar(bufferArray, '"')) || NULL == (finValeur = findChar(++debValeur, '"')) || finValeur <= debValeur)
                            {   // Cas de la valeur standard

                                printf("Parse impossible : Valeur vide ou incomplete\n");
                                return false;
                            }
                            std::string value(debValeur, finValeur - debValeur);
                            printf("value:%s\n", value.c_str());
                        }

                        if (hasNextArray)
                            bufferArray = debArray + 1;
                    } while (hasNextArray);
                }
                else
                {
                    if (NULL != debObjValeur &&
                        (NULL == debValeur || debObjValeur < debValeur))
                    {   // Cas de la valeur objet
                        if (false == parse(values))
                            return false;
                    }
                    else if (NULL != debValeur)
                    {
                        if (NULL == (debValeur = findChar(bufferValues, '"')) || NULL == (finValeur = findChar(++debValeur, '"')) || finValeur <= debValeur)
                        {   // Cas de la valeur standard

                            printf("Parse impossible : Valeur vide ou incomplete\n");
                            return false;
                        }
                        std::string value(debValeur, finValeur - debValeur);
                        printf("value:%s\n", value.c_str());
                    }
                }
            }
            ///////////////////////////////////////////////////////////////////////
        } while (hasNext);
    }

    return true;
}

/**********************************************************************************************************************
                trimSpace

Description : Fonction de trime des espaces
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   buffer  : Buffer à trimer
                sens    : Sens dans lequel on doit enlever les espaces
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Buffer trimé
**********************************************************************************************************************/
char* CParserJSON::trimSpace(char* buffer, E_SensRecherche sens)
{
    while (*buffer == ' ' || *buffer == '\t' || *buffer == '\r' || *buffer == '\n' || *buffer == '\0')
    {
        switch (sens) {
        case E_SR_Avant:    ++buffer; break;
        case E_SR_Arriere:  --buffer; break;
        default: return buffer;
        }
    }

    return buffer;
}

/**********************************************************************************************************************
                findChar

Description : Fonction de recherche d'un caractère dans une chaine
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   buffer  : Buffer dans lequel rechercher le caractère - NON MODIFIE
                value   : Valeur à rechercher
                size    : Taille maximum du buffer à prendre en compte
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
char* CParserJSON::findChar(char* buffer, char value, unsigned int size/*=0*/)
{
    unsigned int idx = 0;
    bool inString = false;
    int level1 = 0, level2 = 0;

    if (0 == size)
        size = strlen(buffer);

    while (NULL != buffer[idx] && idx < size)
    {
        if (!inString && level1 == 0 && level2 == 0 && buffer[idx] == value)
            return &buffer[idx];
        if (buffer[idx] == '\"')
            inString = !inString;

        if (buffer[idx] == '}')  --level1;
        if (buffer[idx] == '{')  ++level1;
        if (buffer[idx] == ']')  --level2;
        if (buffer[idx] == '[')  ++level2;

        ++idx;
    }

    return NULL;
}

/* EOF */
