#pragma once

#include <string>
#include <vector>

#include "ProjectDefines.h"

/**********************************************************************************************************************
    Classe d�crivant un objet JSON
**********************************************************************************************************************/
class CObjectJSON
{
public:
    CObjectJSON(E_TypeObjectJSON eType, std::string name, std::vector<std::string> values);
    CObjectJSON(E_TypeObjectJSON eType, std::string name, std::string value);
    virtual ~CObjectJSON() {}

public:
    virtual bool assertObject();
    //!< V�rifie que l'objet est coh�rent
    virtual void printObject();
    //!< Imprime l'objet dans la console

protected:
    E_TypeObjectJSON            m_eType;    // Type de l'objet JSON
    std::string                 m_name;     // Nom de l'�l�ment JSON
    std::vector<std::string>    m_values;   // Valeurs de l'�l�ment JSON
};

/* EOF */
