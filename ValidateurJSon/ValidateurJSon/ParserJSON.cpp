#include "ParserJSON.h"

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
    char* next = NULL, *nextArray = NULL;


    while (NULL != (debElement = findChar(buffer, '{')))    // On recherche des éléments tant qu'il y en a
    {
        ++debElement;

        // Vérification d'élément "complet" ///////////////////////////////////
        if (NULL == (finElement = findChar(debElement, '}')) || finElement <= debElement+1)
        {
            printf("Parse impossible : Element vide ou incomplet\n");
            return false;
        }
        buffer = finElement+1;
        ///////////////////////////////////////////////////////////////////////

        do
        {
            if (!parseName(next ? next : debElement, finNom)) return false;

            // Recherche de la valeur de l'élément ////////////////////////////////
            if (NULL == (debValeurs = findChar(finNom + 1, ':')))
            {
                printf("Parse impossible : Mauvais format (:)\n");
                return false;
            }
            if (!parseValues(debValeurs, finElement, next)) return false;
            ///////////////////////////////////////////////////////////////////////
        } while (next);
    }

    return true;
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

/**********************************************************************************************************************
                parseName

Description : Fonction de parse du nom de l'objet
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   debElement : Debut de l'élément dans lequel on cherche le nom
                finNom : Fin du nom retourné par la fonction
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parseName(char* debElement, char* &finNom)
{
    char *debNom = NULL;
    if (NULL == (debNom = findChar(debElement, '\"')) || NULL == (finNom = findChar(++debNom, '\"')) || finNom == debNom)
    {
        printf("Parse impossible : Nom de l'element vide ou incomplet\n");
        return false;
    }
    std::string name(debNom, finNom - debNom);
    printf("name:%s:\n", name.c_str());

    return true;
}

/**********************************************************************************************************************
                parseValues

Description : Fonction de parse de la chaine des valeurs de l'objet
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   debValeurs : Début de la valeur
                finElement : Fin de l'élément
                next : Element suivant rempli par la fonction
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parseValues(char* debValeurs, char* finElement, char*& next)
{
    // Vérifications de base //////////////////////////////////////////////////
    char* finValeurs = NULL;

    finValeurs = findChar(++debValeurs, ',');
    next = (NULL != finValeurs) ? finValeurs + 1 : NULL;
    if (!next) finValeurs = finElement;

    if (finValeurs <= debValeurs)
    {
        printf("Parse impossible : Pas de valeur\n");
        return false;
    }
    ///////////////////////////////////////////////////////////////////////////


    std::string values(debValeurs, finValeurs - debValeurs);
    char * bufferValues = const_cast<char*>(values.c_str());

    if (isInteger(bufferValues))    // Cas des nombres
        return true;

    char* debArray = findChar(bufferValues, '[');
    char* debObjValeur = findChar(bufferValues, '{');
    char* debValeur = findChar(bufferValues, '"');
    char* finArray = NULL, * finValeur = NULL;

    if (NULL != debArray && (NULL == debObjValeur || debArray < debObjValeur) && (NULL == debValeur || debArray < debValeur))
    {   // Cas d'un tableau ('[' trouvé avant '{' et '"')
        if (!parseArray(++debArray)) return false;
    }
    else
    {
        if (NULL != debObjValeur && (NULL == debValeur || debObjValeur < debValeur))
        {   // Cas de la valeur objet
            if (!parse(values)) return false;
        }
        else if (NULL != debValeur)
        {   // Cas de la valeur standard
            if (!parseValue(bufferValues)) return false;
        }
    }

    return true;
}

/**********************************************************************************************************************
                parseValue

Description : Fonction de parse de la valeur de l'objet
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   debValeur : Debut de la valeur
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parseValue(char* debValeur)
{
    char* finValeur = NULL;
    if (NULL == (debValeur = findChar(debValeur, '"')) || NULL == (finValeur = findChar(++debValeur, '"')) || finValeur <= debValeur)
    {   // Cas de la valeur standard

        printf("Parse impossible : Valeur vide ou incomplete\n");
        return false;
    }
    std::string value(debValeur, finValeur - debValeur);
    printf("value:%s\n", value.c_str());
    return true;
}

/**********************************************************************************************************************
                parseArray

Description : Fonction de parse d'un tableau
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   debArray : Debut du tableau
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Statut du parsing
**********************************************************************************************************************/
bool CParserJSON::parseArray(char* debArray)
{
    char* finArray = findChar(debArray, ']');

    // Vérification de tableau complet //////////////////////
    if (NULL == finArray || finArray <= debArray + 1)
    {
        printf("Parse impossible : Tableau vide ou incomplet\n");
        return false;
    }

    std::string arrayJSON(debArray, finArray - debArray);
    char* bufferArray = const_cast<char*>(arrayJSON.c_str());

    char* next = NULL;
    do
    {
        char * debObjValeur = findChar(bufferArray, '{');
        char* debValeur = findChar(bufferArray, '"');
        char* finValeur = NULL;

        if (NULL != debObjValeur && (NULL == debValeur || debObjValeur < debValeur))
        {   // Cas de la valeur objet

            if (!parse(arrayJSON)) return false;

            next = NULL; // Déjà géré dans la fonction de parse de base
        }
        else
        {   // Cas de la valeur standard

            if (!parseValue(bufferArray)) return false;

            next = (NULL != (debArray = findChar(bufferArray, ','))) ? debArray + 1 : NULL;
            bufferArray = next;
        }

    } while (next);

    return true;
}

/**********************************************************************************************************************
                isInteger

Description : Renvoie True s'il s'agit d'un Integer
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] value : valeur a parser
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> True s'il s'agit d'un Integer
**********************************************************************************************************************/
bool CParserJSON::isInteger(char* value)
{
    char* end = NULL;
    long lval = strtol(value, &end, 10);
    if (end != value && (*end == '\0' || *end == '\t' || *end == ' ' || *end == '\r' || *end == '\n') && errno != ERANGE)
    {   // Cas du nombre
        printf("value:%ld\n", lval);
        return true;
    }
    return false;
}

/* EOF */
