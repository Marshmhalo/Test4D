#pragma once

#include <string>

/**********************************************************************************************************************
    Classe de parsing du JSON

Description : Cette classe sert à parser une chaîne de caractères en JSON et retourne des CObjectJSON.
**********************************************************************************************************************/

class CParserJSON
{
public:
    CParserJSON(std::string stringToParse);
    virtual ~CParserJSON() {}

    virtual bool parse();
    //!< Fonction de parsing de la chaine de caractères

protected:
    virtual bool parse(std::string stringToParse);
    //!< Fonction de parsing de la chaine de caractères (récursive)
    virtual char* findChar(char* buffer, char value, unsigned int size=0);
    //!< Fonction de recherche d'un caractère dans une chaine (pour éviter de prendre en compte les chaines dans les chaines)
    virtual bool parseName(char* debElement, char* &finNom);
    //!< Fonction de parse du nom de l'objet
    virtual bool parseValues(char* debValeurs, char* finElement, char*& next);
    //!< Fonction de parse de la chaine des valeurs de l'objet
    virtual bool parseValue(char* debValeur);
    //!< Fonction de parse de la valeur de l'objet
    virtual bool parseArray(char* debArray);
    //!< Fonction de parse d'un tableau
    virtual bool isInteger(char* value);
    //!< Renvoie True s'il s'agit d'un Integer

protected:
    std::string m_string;
};

/* EOF */
