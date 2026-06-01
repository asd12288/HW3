#ifndef __PEOPLE_H__
#define __PEOPLE_H__

/*
 * This is a simple example of a people ADT module
 * The module is implemented using a linked list module
 * 
 * 
 */

#include <stdio.h>

#define MAX_NAME_LEN 256

typedef struct people_s *People;

typedef enum
{
    PEOPLE_SUCCESS,
    PEOPLE_OUT_OF_MEMORY,
    PEOPLE_BAD_ARGUMENT,
    PEOPLE_NOT_FOUND,
    PEOPLE_FAIL,
} PeopleResult;

PeopleResult peopleCreate(People *people);
PeopleResult peopleDestroy(People people);
PeopleResult peopleAddPerson(People people, int id, const char *firstName, const char *lastName);
PeopleResult peopleRemovePerson(People people, int id);
PeopleResult peopleGetPersonsName(People people, int id, char *firstName, char *lastName);
PeopleResult peopleSortByFirstName(People people);
PeopleResult peopleSortByLastName(People people);
PeopleResult peopleFilterByLastName(People people, char *name, People *new_people); // This creates new filtered list
PeopleResult peopleReportAll(People people, FILE *out);

#endif /* __PEOPLE_H__ */
