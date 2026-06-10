#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
#include <stdlib.h>
#include <string.h>

typedef struct TransportCompany_s
{
    int id;
    char *name;
    TransportType type;
    double revenue;
    double expenses;

} TransportCompany;

struct TransportManager_s
{
    Set companies;
};

static int compareCompaniesById(SetElement elem1, SetElement elem2)
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

void printCompany(FILE *output, SetElement elem)
{

    TransportCompany *company = (TransportCompany *)elem;
    if (company == NULL)
    {
        return;
    }
    fprintf(output, "Company id: %d", company->id);
}

static void freeCompany(SetElement elem)
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

static SetElement copyComapny(SetElement elem)
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

TransportManager TransportManagerCreate(void)
{

    TransportManager new_tm = (TransportManager)malloc(sizeof(struct TransportManager_s));
    if (new_tm == NULL)
    {
        free(new_tm);
        return NULL;
    }

    SetResult result = setCreate(&new_tm->companies, compareCompaniesById, copyComapny, freeCompany, printCompany);

    if (result != SET_SUCCESS)
    {
        free(new_tm);
        return NULL;
    }

    return new_tm;
}

// TransportResult TransportManagerAdd(TransportManager tm,
//                                     const char *name, int id, TransportType type, double revenue,
//                                     double expenses)
// {

//     return 0;
// }
