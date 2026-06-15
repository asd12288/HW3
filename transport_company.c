#include <stdlib.h>
#include <string.h>
#include "transport_company.h"

int typeCheck(TransportType type)
{
    switch (type)
    {
    case TRANSPORT_SHIPPING:
    case TRANSPORT_TRAIN:
    case TRANSPORT_LIGHT_TRAIN:
    case TRANSPORT_BUS:
    case TRANSPORT_FLIGHT:
    case TRANSPORT_TAXI:
    case TRANSPORT_ALL:
        return 1;
    default:
        return 0;
    }
}

int matchCompanyByType(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;
    TransportType type = *(TransportType *)key;

    return company->type == type || type == TRANSPORT_ALL;
}

int matchCompanyById(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;
    int id = *(int *)key;

    return company->id == id;
}

int matchCompanyByLost(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;

    if (company->expenses >= company->revenue)
    {
        return 1;
    }
    return 0;
}

int compareCompaniesById(SetElement elem1, SetElement elem2)
{
    TransportCompany *company1 = elem1;
    TransportCompany *company2 = elem2;

    if (company1->id < company2->id)
    {
        return -1;
    }

    if (company1->id > company2->id)
    {
        return 1;
    }
    return 0;
}

int compareCompaniesByProfit(SetElement elem1, SetElement elem2)
{
    TransportCompany *company1 = elem1;
    TransportCompany *company2 = elem2;

    double profit1 = company1->revenue - company1->expenses;
    double profit2 = company2->revenue - company2->expenses;

    if (profit1 < profit2)
    {
        return -1;
    }

    if (profit1 > profit2)
    {
        return 1;
    }
    return 0;
}

void printCompany(FILE *output, SetElement elem)
{
    TransportCompany *company = (TransportCompany *)elem;
    if (company == NULL)
    {
        return;
    }
    prog2_report_company(company->id, company->name, company->type,
                         company->revenue, company->expenses, output);
}

void freeCompany(SetElement elem)
{
    TransportCompany *company = (TransportCompany *)elem;
    if (company != NULL)
    {
        if (company->name != NULL)
        {
            free(company->name);
        }
        free(company);
    }
}

SetElement copyCompany(SetElement elem)
{

    TransportCompany *company = (TransportCompany *)elem;
    TransportCompany *company_copy = NULL;
    if (company == NULL)
    {
        return NULL;
    }

    company_copy = (TransportCompany *)malloc(sizeof(TransportCompany));
    if (company_copy == NULL)
    {
        return NULL;
    }

    company_copy->name = (char *)malloc(strlen(company->name) + 1);
    if (company_copy->name == NULL)
    {
        free(company_copy);
        return NULL;
    }

    strcpy(company_copy->name, company->name);

    company_copy->expenses = company->expenses;
    company_copy->revenue = company->revenue;
    company_copy->id = company->id;
    company_copy->type = company->type;

    return company_copy;
}
