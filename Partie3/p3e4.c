/*
Attention, ce code est bugué !
Trouvez les erreurs et résolvez les,
gardez en commentaire les erreurs et
expliquez d'où vient l'erreur et
comment l'empêcher
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int age;
    // department petit (8)
    char department[8];
} Employee;

typedef struct {
    Employee *array;
    size_t length;
    size_t capacity;
} Registry;

int reg_add(Registry *r, const char *name, int age, const char *department) {
    if (r->length == r->capacity) {
        size_t ncap = r->capacity ? r->capacity * 2 : 2;
        // BUG: perte de r->array si échec
        r->array = realloc(r->array, ncap * sizeof *r->array);
        if (!r->array)
            return -1;
        r->capacity = ncap;
    }
    Employee *e = &r->array[r->length++];
    // BUG: manque +1 (pour '\0')
    e->name = malloc(strlen(name));
    if (!e->name)
        return -1;
    strcpy(e->name, name);
    e->age = age;
    // BUG: possible overflow (department[8])
    strcpy(e->department, department);
    return 0;
}

void reg_destroy(Registry *r) {
    // BUG: fuite sur e->name
    free(r->array);
    r->array = NULL;
    r->length = 0;
    r->capacity = 0;
}

int main(void) {
    Registry r = {0};
    // trop long pour department[8]
    reg_add(&r, "Alice", 30, "Engineering");
    reg_add(&r, "Bob", 40, "HR");
    for (size_t i = 0; i < r.length; ++i)
        printf("%s (%d) - %s\n",
               r.array[i].name,
               r.array[i].age,
               r.array[i].department);
    reg_destroy(&r);

    for (size_t i = 0; i < 2; ++i)
        printf("%s (%d) - %s\n",
               r.array[i].name,
               r.array[i].age,
               r.array[i].department);
    return 0;
}
