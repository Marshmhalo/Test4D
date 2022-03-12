#include "ObjectJSON.h"

/**********************************************************************************************************************
                Constructeur CObjectJSON

Description : Constructeur de la classe CObjectJSON
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->]   eType   : Type d'objet JSON
                name    : Nom de l'objet JSON
                values  : Valeurs de l'objet JSON
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [-->   Instance d'objet JSON initialisée
**********************************************************************************************************************/
template<class T>
inline CObjectJSON<T>::CObjectJSON(E_TypeObjectJSON eType, std::string name, std::vector<T> values)
    : m_eType(eType)
    , m_name(name)
    , m_values(values)
{
}

template<class T>
inline CObjectJSON<T>::CObjectJSON(E_TypeObjectJSON eType, std::string name, T value)
    : m_eType(eType)
    , m_name(name)
{
    m_values.push_back(value);
}

/**********************************************************************************************************************
                printObject

Description : Imprime l'objet dans la console
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] -
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> -
**********************************************************************************************************************/
template<class T>
void CObjectJSON<T>::printObject()
{
    printf("Object %s (type : %d) value(s): ", m_name, (int)m_eType);
    for (unsigned int u = 0; u < m_values.size(); ++u)
    {
        if (u > 0) printf(",");
        switch (m_eType)
        {
        case E_TOJ_null:    printf("null");             break;
        case E_TOJ_string:
        case E_TOJ_object:  printf("%s", m_values[u]);  break;
        case E_TOJ_number:  printf("%d", m_values[u]);  break;
        case E_TOJ_boolean: printf("%d", m_values[u]);  break;
        default:            printf("default");          break;
        }
    }
}

/* EOF */
