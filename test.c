#include <stdio.h>

struct person
{
    int age;
    char *name;
};

int main(void)
{
    int *x;
    int a = 10;
    x = &a;
    a = 12;
    printf("x: %d", *x);

    struct person person1;
//     scanf("%d", &person1.age);
// \
//     printf("age: %d and name: %s", person1.age, person1.name);

    return 0;
}