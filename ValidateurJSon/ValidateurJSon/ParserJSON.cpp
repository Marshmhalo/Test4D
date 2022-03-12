#include "ParserJSON.h"
#include "ObjectJSON.h"


/**********************************************************************************************************************
                Constructeur CParserJSON

Description : Constructeur de la classe CParserJSON
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   stringToParse : Chanine de caract�res � parser
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Instance de parser JSON initialis�e
**********************************************************************************************************************/
CParserJSON::CParserJSON(std::string stringToParse)
    : m_string(stringToParse)
{

}

/**********************************************************************************************************************
                parse

Description : Fonction de parsing de la chaine de caract�res
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

Description : Fonction de parsing de la chaine de caract�res (r�cursive)
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] stringToParse : Chaine de caract�res � parser
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
    bool isObjectArray = false;


    while (NULL != (debElement = findChar(buffer, '{')))    // On recherche des �l�ments tant qu'il y en a
    {
        ++debElement;

        // V�rification d'�l�ment "complet" ///////////////////////////////////
        if (NULL == (finElement = findChar(debElement, '}')) || finElement <= debElement+1)
        {
            printf("Parse impossible : El�ment vide ou incomplet\n");
            return false;
        }
        buffer = finElement;
        --finElement;
        ///////////////////////////////////////////////////////////////////////

        // Recherche du nom de l'�l�ment //////////////////////////////////////
        if (NULL == (debNom = findChar(debElement, '"')) || NULL == (finNom = findChar(debNom+1, '"')) || finNom == debNom + 1 )
        {
            printf("Parse impossible : Nom de l'�l�ment vide ou incomplet\n");
            return false;
        }
        std::string name(debNom, finNom - debNom);
        ///////////////////////////////////////////////////////////////////////

        // Recherche de la valeur de l'�l�ment ////////////////////////////////
        if (NULL == (debValeurs = findChar(finNom, ':')))
        {
            printf("Parse impossible : Mauvais format (:)\n");
            return false;
        }
        ++debValeurs;

        finValeurs = finElement;
        if (finValeurs <= debValeurs)
        {
            printf("Parse impossible : Pas de valeur\n");
            return false;
        }
        std::string values(debValeurs, finValeurs - debValeurs);
        bufferValues = const_cast<char*>(values.c_str());

        if (NULL != (debArray = findChar(bufferValues, '[')))
        { // Cas d'un tableau
            ++debArray;

            // V�rification de tableau complet //////////////////////
            if (NULL == (finArray = findChar(debArray, ']')) || finArray <= debArray+1 )
            {
                printf("Parse impossible : Tableau vide ou incomplet\n");
                return false;
            }
            --finArray;

            isObjectArray = (NULL != (debValeur = findChar(debArray, '{')));

            if (isObjectArray)
            {   //TODO Tableau d'objets

            }
            else
            {   //TODO Tableau standard

            }
        }
        else
        {
            if (NULL != (debObjValeur = findChar(debValeurs, '{')) &&
               (NULL == (debValeur = findChar(debValeurs, '"')) || debObjValeur < debValeur))
            {   // Cas de la valeur objet

                if (NULL == (debValeur = findChar(debValeurs, '{')) || NULL == (finValeur = findChar(debValeur + 1, '}')) || finValeur <= debValeur + 1)
                {
                    printf("Parse impossible : Objet valeur vide ou incomplete\n");
                    return false;
                }
                std::string value(debValeur, finValeur - debValeur);
                if (false == parse(value))
                    return false;
            }
            else
            {
                if (NULL == (debValeur = findChar(debValeurs, '"')) || NULL == (finValeur = findChar(debValeur + 1, '"')) || finValeur <= debValeur + 1)
                {   // Cas de la valeur standard

                    printf("Parse impossible : Valeur vide ou incomplete\n");
                    return false;
                }
                std::string value(debValeur, finValeur - debValeur);
                CObjectJSON object(E_TOJ_string, name, value);
                object.printObject();
            }
        }
        ///////////////////////////////////////////////////////////////////////
    }

    return true;
}

/**********************************************************************************************************************
                trimSpace

Description : Fonction de trime des espaces
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   buffer  : Buffer � trimer
                sens    : Sens dans lequel on doit enlever les espaces
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Buffer trim�
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

Description : Fonction de recherche d'un caract�re dans une chaine
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   buffer  : Buffer dans lequel rechercher le caract�re - NON MODIFIE
                value   : Valeur � rechercher
                size    : Taille maximum du buffer � prendre en compte
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> Statut du parsing
**********************************************************************************************************************/
char* CParserJSON::findChar(char* buffer, char value, unsigned int size/*=0*/)
{
    unsigned int idx = 0;
    bool inString = false;

    if (0 == size)
        size = strlen(buffer);

    while (NULL != buffer[idx] && idx < size)
    {
        if (!inString && buffer[idx] == value)
            return &buffer[idx];
        if (buffer[idx] == '\"')
            inString = !inString;
        ++idx;
    }

    return NULL;
}

/* EOF */
