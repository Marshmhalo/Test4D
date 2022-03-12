#pragma once

#include <string>
#include <vector>

#include "ProjectDefines.h"

/**********************************************************************************************************************
    Classe décrivant un objet JSON
**********************************************************************************************************************/
class CObjectJSON
{
public:
    CObjectJSON(E_TypeObjectJSON eType, std::string name, std::vector<std::string> values);
    CObjectJSON(E_TypeObjectJSON eType, std::string name, std::string value);
    virtual ~CObjectJSON() {}

public:
    virtual bool assertObject();
    //!< Vérifie que l'objet est cohérent
    virtual void printObject();
    //!< Imprime l'objet dans la console

protected:
    E_TypeObjectJSON            m_eType;    // Type de l'objet JSON
    std::string                 m_name;     // Nom de l'élément JSON
    std::vector<std::string>    m_values;   // Valeurs de l'élément JSON
};

/* EOF */
