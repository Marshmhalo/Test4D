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
CObjectJSON::CObjectJSON(E_TypeObjectJSON eType, std::string name, std::vector<std::string> values)
    : m_eType(eType)
    , m_name(name)
    , m_values(values)
{
}

CObjectJSON::CObjectJSON(E_TypeObjectJSON eType, std::string name, std::string value)
    : m_eType(eType)
    , m_name(name)
{
    m_values.push_back(value);
}

/**********************************************************************************************************************
                assertObject

Description : Vérifie que l'objet est cohérent
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] -
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> True si l'objet est correct, false sinon
**********************************************************************************************************************/
bool CObjectJSON::assertObject()
{
    bool isOK = m_values.size()>0;
    for (unsigned int u = 0; u < m_values.size(); ++u)
    {
        const char* value = m_values[u].c_str();
        switch (m_eType)
        {
        
        
        case E_TOJ_number:
        {
            char* end;
            long number = strtol(value, &end, 16);                  // On part sur des nombres de 16 chiffres maximum
            if (end == value || *end != 0 || errno == ERANGE)  // Vérification qu'on ait bien un numéro, qu'il ne reste rien dans la chaine et qu'on atteigne pas les 16 caractères
                isOK = false;
        }
        break;

        case E_TOJ_null:    if (0 != _stricmp(value, "null"))                                   isOK = false; break;
        case E_TOJ_boolean: if (0 != _stricmp(value, "true") && 0 != _stricmp(value, "false"))  isOK = false; break;

        case E_TOJ_string:
        case E_TOJ_object:
        default: break;  // Toujours cohérent
        }
    }
    return isOK;
}

/**********************************************************************************************************************
                printObject

Description : Imprime l'objet dans la console
-----------------------------------------------------------------------------------------------------------------------
ENTREES: -->] -
-----------------------------------------------------------------------------------------------------------------------
SORTIES: [--> -
**********************************************************************************************************************/
void CObjectJSON::printObject()
{
    printf("Object %s (type : %d) value(s): ", m_name.c_str(), (int)m_eType);
    for (unsigned int u = 0; u < m_values.size(); ++u)
    {
        if (u > 0) printf(",");
        switch (m_eType)
        {
        case E_TOJ_null:    printf("null");                     break;
        case E_TOJ_string:
        case E_TOJ_object:
        case E_TOJ_number:
        case E_TOJ_boolean: printf("%s", m_values[u].c_str());  break;
        default:            printf("default");                  break;
        }
    }
    printf("\t%s\n", assertObject() ? "OK" : "KO");
    printf("\n");
}

/* EOF */
