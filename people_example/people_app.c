#include "people.h"

#define LAST_NAME_TEST "Smith"
#define ID_TEST 5
#define WRONG_ID_TEST 82
#define ERROR_STREAM stderr
#define OUT_STREAM stdout

#define HANDLE_RESULT(cmd)                                  \
    do                                                          \
    {                                                           \
        PeopleResult res = cmd;                                 \
        if ((res) != PEOPLE_SUCCESS)                            \
        {                                                       \
            fprintf(ERROR_STREAM, "*** %s Failed [%s] ***\n",        \
            #cmd, res2str(res));                                       \
        }                                                       \
    } while (0)

static const char *resultStrings[] = {
    "PEOPLE_SUCCESS",
    "PEOPLE_OUT_OF_MEMORY",
    "PEOPLE_BAD_ARGUMENT",
    "PEOPLE_NOT_FOUND",
    "PEOPLE_FAIL",
};

const char *res2str(PeopleResult res)
{
    if (res < 0 || res >= PEOPLE_FAIL)
    {
        return "Unknown result";
    }
    return resultStrings[res];
}

int main()
{
    char first[MAX_NAME_LEN] = {0};
    char last[MAX_NAME_LEN] = {0};
    People people = NULL;
    People tmp_people = NULL;

    HANDLE_RESULT(peopleCreate(&people));
    printf("\nAdding people...\n");
    HANDLE_RESULT(peopleAddPerson(people, 1, "John", "Smith"));
    HANDLE_RESULT(peopleAddPerson(people, 2, "Jane", "Smith"));
    HANDLE_RESULT(peopleAddPerson(people, 3, "John", "Doe"));
    HANDLE_RESULT(peopleAddPerson(people, 5, "Yossi", "Doe"));
    HANDLE_RESULT(peopleAddPerson(people, 4, "Mark", "Smith"));
    HANDLE_RESULT(peopleAddPerson(people, 6, "Avi", "Jacob"));
    HANDLE_RESULT(peopleAddPerson(people, 7, "Ahmed", "Assi"));
    HANDLE_RESULT(peopleAddPerson(people, 8, "David", "Avdat"));
    HANDLE_RESULT(peopleAddPerson(people, 9, "Maya", "Allon"));
    HANDLE_RESULT(peopleAddPerson(people, 10, "Rachel", "Smith"));
    HANDLE_RESULT(peopleReportAll(people, OUT_STREAM));

    printf("\nGet person name id: %d\n", ID_TEST);
    HANDLE_RESULT(peopleGetPersonsName(people, ID_TEST, first, last));
    printf("Person with id:%d First name: %s, Last name: %s\n", ID_TEST, first, last);

    printf("\nGet person name id: %d\n", WRONG_ID_TEST);
    HANDLE_RESULT(peopleGetPersonsName(people, WRONG_ID_TEST, first, last));

    printf("\nOnly pople with last name \"%s\"\n", LAST_NAME_TEST);
    HANDLE_RESULT(peopleFilterByLastName(people, LAST_NAME_TEST, &tmp_people));
    HANDLE_RESULT(peopleReportAll(tmp_people, OUT_STREAM));
    peopleDestroy(tmp_people);

    printf("\nSort by first name\n");
    HANDLE_RESULT(peopleSortByFirstName(people));
    HANDLE_RESULT(peopleReportAll(people, OUT_STREAM));

    peopleDestroy(people);
    return 0;
}
