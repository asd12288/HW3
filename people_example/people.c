#include <stdlib.h>
#include <string.h>
#include "people.h"
#include "linked_list.h"

#define GROUP_DESCRIPTION "Group of people\n"

// The main structure for this module
struct people_s
{
    LinkedList list;
    char description[MAX_NAME_LEN];
    // more fields as needed
};

///////////////////////////////////////////////////////////////////////////////
/* person is the list element.
 * It could and should be a different module.
 *
 * Here it is in people module just for the example
 */
typedef struct person_s
{
    int id;
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
} person_t, *Person;

/* As the generic linked list requests, we need to implement copy, match, compare
 *  print and destroy functions in order use the list
 * The signature of these functions are supplied at linked_list.h file
 * In our case, ListElement is Person. Please note that this is a pointer
 * to the struct person_s
 */
static ListElement copyPerson(ListElement element)
{
    Person person = (Person)element;
    Person new_person = NULL;
    if (element == NULL)
        return NULL;
    new_person = (Person)malloc(sizeof(person_t));
    if (new_person == NULL)
        return NULL;
    new_person->id = person->id;
    /* Here we use MAX_NAME_LEN for simplicity. You should avoid that in your
       Assignment soulution.
       Actually, you need allocate the correct size for the person's first and last name
       and copy the strings to the new allocated memory 
    */
    strncpy(new_person->first_name, person->first_name, MAX_NAME_LEN);
    strncpy(new_person->last_name, person->last_name, MAX_NAME_LEN);

    return new_person;
}

static void freePerson(ListElement element)
{
    // Please note we did not allocate memory specific for first and last name
    // so we do not need to free them. Again, you should do so in your solution.
    /* Here, the strings were allocated in the person struct allocation */
    if (element)
        free(element);
}

static void printPerson(FILE *out, ListElement element)
{
    Person person = (Person)element;
    if (person == NULL)
        return;
    fprintf(out, "\t[%09d] %s %s\n", person->id, person->first_name,
            person->last_name);
}

/* Compare functions returns integer grater than, equal to or less than 0 according as element1 is grater, equal to or less than element2 */

// This wil be used to sort the list by persons first name
static int comparePersonsByFirstName(ListElement element1, ListElement element2)
{
    Person person1 = (Person)element1;
    Person person2 = (Person)element2;
    if (person1 == NULL || person2 == NULL)
        return 0;
    return strcmp(person1->first_name, person2->first_name);
}

// This wil be used to sort the list by persons last name
static int comparePersonsByLastName(ListElement element1, ListElement element2)
{
    Person person1 = (Person)element1;
    Person person2 = (Person)element2;
    if (person1 == NULL || person2 == NULL)
        return 0;
    return strcmp(person1->last_name, person2->last_name);
}

/* Match functions return integer grater than or equal to zero according as element 1 is gratter or equal to element 2 */

// This will be used to find person by id
static int matchPersonsById(ListElement element, KeyForListElement key)
{
    if (element == NULL || key == NULL)
        return 0;
    Person person = (Person)element;
    int id = *(int *)key; // Here key is int for id
    return person->id == id;
}

// This wil be used to filter person by last name
static int matchPersonsByLastName(ListElement element, KeyForListElement key)
{
    Person person = (Person)element;
    char *last_name = (char *)key; // Now the key is string
    if (person == NULL || last_name == NULL)
        return 0;
    return strcmp(person->last_name, last_name) == 0;
}

///////////////////////////////////////////////////////////////////////////////
/* API functions implementation */

PeopleResult
peopleCreate(People *people)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    People new_people = (People)malloc(sizeof(struct people_s));
    if (new_people == NULL)
        return PEOPLE_OUT_OF_MEMORY;

    sprintf(new_people->description, "%s", GROUP_DESCRIPTION);
    if (linkedListCreate(&(new_people->list), copyPerson, freePerson, printPerson) != LIST_SUCCESS)
    {
        free(new_people);
        return PEOPLE_OUT_OF_MEMORY;
    }

    *people = new_people;
    return PEOPLE_SUCCESS;
}

PeopleResult peopleDestroy(People people)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    if (linkedListDestroy(people->list) != LIST_SUCCESS)
        return PEOPLE_FAIL;

    free(people);
    return PEOPLE_SUCCESS;
}

PeopleResult peopleAddPerson(People people, int id, const char *firstName, const char *lastName)
{
    if (people == NULL || firstName == NULL || lastName == NULL)
        return PEOPLE_BAD_ARGUMENT;
    // Check id limitations if any

    person_t person;
    person.id = id;
    strcpy(person.first_name, firstName);
    strcpy(person.last_name, lastName);

    if (linkedListInsertLast(people->list, &person) != LIST_SUCCESS)
        return PEOPLE_OUT_OF_MEMORY;

    return PEOPLE_SUCCESS;
}

PeopleResult peopleRemovePerson(People people, int id)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    linkedListGoToHead(people->list);
    // Here we need to use the function matchPersonsById
    // in order to find the person with the given id
    if (linkedListFind(people->list, &id, matchPersonsById) == LIST_SUCCESS)
    {
        // if we successfully found the person, we know that
        // the list "current" is pointing at it
        // Note that linkedListRemoveCurrent() func will also use personFree that was
        //   supplied at linkedListCreate
        if (linkedListRemoveCurrent(people->list) != LIST_SUCCESS)
            return PEOPLE_FAIL;
    }
    else
    {
        return PEOPLE_NOT_FOUND;
    }
    return PEOPLE_SUCCESS;
}

PeopleResult peopleGetPersonsName(People people, int id, char *firstName, char *lastName)
{
    if (people == NULL || firstName == NULL || lastName == NULL)
        return PEOPLE_BAD_ARGUMENT;

    linkedListGoToHead(people->list);
    // Here we need to use the function matchPersonsById
    // in order to find the person with the given id
    if (linkedListFind(people->list, &id, matchPersonsById) == LIST_SUCCESS)
    {
        Person person;
        // if we successfully found the person, we know that
        // the list "current" is pointing at it
        if (linkedListGetCurrent(people->list, (ListElement *)&person) == LIST_SUCCESS)
        {
            strcpy(firstName, person->first_name);
            strcpy(lastName, person->last_name);
        }
        else
        {
            return PEOPLE_FAIL;
        }
    }
    else
    {
        return PEOPLE_NOT_FOUND;
    }

    return PEOPLE_SUCCESS;
}

PeopleResult peopleSortByFirstName(People people)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    if (linkedListSortElements(people->list, comparePersonsByFirstName) != LIST_SUCCESS)
        return PEOPLE_FAIL;

    return PEOPLE_SUCCESS;
}

PeopleResult peopleSortByLastName(People people)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    if (linkedListSortElements(people->list, comparePersonsByLastName) != LIST_SUCCESS)
        return PEOPLE_FAIL;

    return PEOPLE_SUCCESS;
}

PeopleResult peopleFilterByLastName(People people, char *name, People *new_people)
{
    if (people == NULL || name == NULL || new_people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    PeopleResult result = peopleCreate(new_people);
    if (result != PEOPLE_SUCCESS)
        return result;

    sprintf((*new_people)->description, "Group of people with last name \"%s\"\n", name);
    if (linkedListFilterElements(people->list, &((*new_people)->list), matchPersonsByLastName, name) != LIST_SUCCESS)
    {
        peopleDestroy(*new_people);
        return PEOPLE_FAIL;
    }

    return PEOPLE_SUCCESS;
}

PeopleResult peopleReportAll(People people, FILE *out)
{
    if (people == NULL)
        return PEOPLE_BAD_ARGUMENT;

    int num = linkedListGetNumElements(people->list);
    if (num == 0)
    {
        fprintf(out, "No people in the group\n");
        return PEOPLE_SUCCESS;
    }
    else
    {
        fprintf(out, "Group description: %s", people->description);
        fprintf(out, "%d people in the group\n", num);
        linkedListPrint(people->list, out, num);
    }

    return PEOPLE_SUCCESS;
}
