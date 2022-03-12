#pragma once

#include <string>
#include <vector>

#include "ProjectDefines.h"

/**********************************************************************************************************************
    Classe d�crivant un objet JSON
**********************************************************************************************************************/
template <class T>
class CObjectJSON
{
public:
    CObjectJSON<T>(E_TypeObjectJSON eType, std::string name, std::vector<T> values);
    CObjectJSON<T>(E_TypeObjectJSON eType, std::string name, T value);
    virtual ~CObjectJSON() {}

public:
    virtual void printObject();

protected:
    E_TypeObjectJSON    m_eType;    // Type de l'objet JSON
    std::string         m_name;     // Nom de l'�l�ment JSON
    std::vector<T>      m_values;   // Valeurs de l'�l�ment JSON
};

/* EOF */
