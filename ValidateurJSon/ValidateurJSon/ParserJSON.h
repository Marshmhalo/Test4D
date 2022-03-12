#pragma once

#include <string>

#include "ProjectDefines.h"

/**********************************************************************************************************************
    Classe de parsing du JSON

Description : Cette classe sert � parser une cha�ne de caract�res en JSON et retourne des CObjectJSON.
**********************************************************************************************************************/

class CParserJSON
{
public:
    CParserJSON(std::string stringToParse);
    virtual ~CParserJSON() {}

    virtual bool parse();
    //!< Fonction de parsing de la chaine de caract�res

protected:
    virtual bool parse(std::string stringToParse);
    //!< Fonction de parsing de la chaine de caract�res (r�cursive)
    virtual char* trimSpace(char* buffer, E_SensRecherche sens);
    //!< Fonction de trime des espaces
    virtual char* findChar(char* buffer, char value, unsigned int size=0);
    //!< Fonction de recherche d'un caract�re dans une chaine (pour �viter de prendre en compte les chaines dans les chaines)

protected:
    std::string m_string;
};

/* EOF */
