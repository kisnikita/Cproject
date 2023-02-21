/*
 * IZP projekt 2
 * Author: Nikita Kislov (xkislo00)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char data[30];
    int length;
}element;

typedef struct {
    int size;
    int cap;
    element *items;
} Universe;


typedef struct {
    int size;
    int cap;
    int *index;
} Set;

typedef struct {
    int size;
    int cap;
    Set *items;
}Array_of_Sets;


typedef struct {
    int first;
    int second;
}Pair;

typedef struct{
    int size;
    int cap;
    Pair *data;
}Relation;

typedef struct {
    int size;
    int cap;
    Relation *items;
}Array_of_Relation;


//contains information about line(U ||S || R)
typedef struct {
    int size;
    int cap;
    bool *first_letter; // 0 - U || S, 1 - R

} First_letters_array;

//used to read file to memory
typedef struct {
    int universe_flag; //0 - not reading 1 - reading 2 - was read
    int element_flag; //0 - not reading 1 - reading 2 - was read
    bool set_flag; //0 - not reading 1 - reading
    bool relation_flag; //0 - not reading 1 - reading
    bool pair_flag; //0 - not reading 1 - reading
    bool command_flag; //0 - not reading 1 - reading
} Flags;

// contains data about operation(prikaz)
typedef struct {
    int command_index; // index in array of commands
    int first_arg;
    int second_arg;
    bool two_int_mode; // 0 - 1 argument, 1 - 2 arguments
    bool set_or_relation; // 0 - set, 1 - relation
}Command;


void Universe_ctor(Universe *a) {
    a->size = 0;
    a->cap = 0;
    a->items = NULL;
}

void Universe_dtor(Universe *a) {
    if (a->cap)
        free(a->items);
    a->items = NULL;
    a->cap = a->size = 0;
}

void Universe_resize(Universe *a, int new_cap) {
    element *resized;
    resized = (element *)realloc(a->items, new_cap * sizeof(element));
    if (resized != NULL)
    {
        a->items = resized;
        a->cap = new_cap;
    }
}

void Universe_append(Universe *a, char *item, int length) {
    if (a->size == a->cap)
        Universe_resize(a, a->cap ? a->cap + a->cap : 1);

    if (a->size < a->cap)
    {
        for (int i = 0; i < length + 1; ++i) {
            a->items[a->size].data[i] = item[i];
        }
        a->items[a->size].length = length + 1;
        a->size++;
    }
}

void Universe_print(Universe a)
{
    printf("U");
    for (int i = 0; i < a.size; i++)
    {
        printf(" %s", a.items[i].data);
    }
}

bool contains_banned_words(char *string){
    char *str;
    char *banned[21];
    banned[0] = "empty";
    banned[1] = "card";
    banned[2] = "complement";
    banned[3] = "union";
    banned[4] = "intersect";
    banned[5] = "minus";
    banned[6] = "subseteq";
    banned[7] = "subset";
    banned[8] = "equals";
    banned[9] = "reflexive";
    banned[10] = "symmetric";
    banned[11] = "antisymmetric";
    banned[12] = "transitive";
    banned[13] = "function";
    banned[14] = "domain";
    banned[15] = "codomain";
    banned[16] = "injective";
    banned[17] = "surjective";
    banned[18] = "bijective";
    banned[19] = "true";
    banned[20] = "false";

    for (int i = 0; i < 21; ++i) {
        str = strstr(string, banned[i]);
        if (str){
            return 1;
        }
    }
    return 0;
}



void Set_ctor(Set *a) {
    a->size = 0;
    a->cap = 0;
    a->index = NULL;
}

void Set_dtor(Set *a) {
    if (a->cap)
        free(a->index);
    a->index = NULL;
    a->cap = a->size = 0;
}

void Set_resize(Set *a, int new_cap){
    int *resized;
    resized = realloc(a->index, new_cap * sizeof(int));
    if (resized != NULL)
    {
        a->index = resized;
        a->cap = new_cap;
    }
}

void Set_append(Set *a, int b) {
    if (a->size == a->cap)
        Set_resize(a, a->cap ? a->cap + a->cap : 1);

    if (a->size < a->cap)
    {
        a->index[a->size] = b;
        a->size++;
    }
}

//returns index of element in universe
int index_in_universe(char *item, Universe b){
    for (int i = 0; i < b.size; i++) {
        if (strncmp(item, b.items[i].data, b.items[i].length) == 0)
            return i;
    }
    return -1;
}

void print_Set(Set set, Universe universe){
    printf ("S");
    for (int i = 0; i < set.size; ++i) {
        printf(" %s", universe.items[set.index[i]].data);
    }
}



void Array_of_Sets_ctor(Array_of_Sets *a) {
    a->size = 0;
    a->cap = 0;
    a->items = NULL;
}

void Array_of_Sets_dtor(Array_of_Sets *a) {
    if (a->cap){
        for (int i = 0; i < a->size; ++i) {
            Set_dtor(&a->items[i]);
        }
        free(a->items);
    }
    a->items = NULL;
    a->cap = a->size = 0;
}

void Array_of_Sets_resize(Array_of_Sets *a, int new_cap) {
    Set *resized;
    resized = realloc(a->items, new_cap * sizeof(Set));
    if (resized != NULL)
    {
        a->items = resized;
        a->cap = new_cap;
    }
}



void Relation_ctor(Relation *a) {
    a->size = 0;
    a->cap = 0;
    a->data = NULL;
}

void Relation_dtor(Relation *a) {
    if (a->cap)
        free(a->data);
    a->data = NULL;
    a->cap = a->size = 0;
}

void Relation_resize(Relation *a, int new_cap) {
    Pair *resized;
    resized = (Pair *)realloc(a->data, new_cap * sizeof(Pair));
    if (resized != NULL)
    {
        a->data = resized;
        a->cap = new_cap;
    }
}

void Relation_append(Relation *a, Pair pair ) {
    if (a->size == a->cap)
        Relation_resize(a, a->cap ? a->cap + a->cap : 1);

    if (a->size < a->cap)
    {
        a->data[a->size] = pair;
        a->size++;
    }
}

//parses string in pair
Pair get_arguments_from_string(char string[], Universe universe){
    Pair pair;
    char buffer1[30];
    char buffer2[30];
    int buffer_counter = 0;
    int string_counter = 0;
    while (string[string_counter] != ' '){
        buffer1[buffer_counter++] = string[string_counter++];
    }
    buffer1[buffer_counter] = '\0';
    buffer_counter = 0;
    string_counter++;
    while (string[string_counter] != '\0'){
        buffer2[buffer_counter++] = string[string_counter++];
    }
    buffer2[buffer_counter] = '\0';
    pair.first = index_in_universe(buffer1, universe);
    pair.second = index_in_universe(buffer2, universe);

    return pair;
}

void print_Relation(Relation relation, Universe universe){
    printf ("\nR");
    for (int i = 0; i < relation.size; ++i) {
        printf(" (%s ", universe.items[relation.data[i].first].data);
        printf("%s)", universe.items[relation.data[i].second].data);
    }
}

//returns amount of pairs without recurrence on first position
int Relation_without_recurrence_first(Relation relation){
    int different_el = relation.size;
    for (int i = 0; i < relation.size - 1; ++i) {
        for (int j = i+1; j <relation.size ; ++j) {
            if (relation.data[i].first == relation.data[j].first) {
                different_el -= 1;
                break;
            }
        }
    }
    return(different_el);
}

//returns amount of pairs without recurrence on second position
int Relation_without_recurrence_second(Relation relation){
    int different_el = relation.size;
    for (int i = 0; i < relation.size - 1; ++i) {
        for (int j = i+1; j <relation.size ; ++j) {
            if (relation.data[i].second == relation.data[j].second) {
                different_el -= 1;
                break;
            }
        }
    }
    return(different_el);
}



void Array_of_Relation_ctor(Array_of_Relation *a) {
    a->size = 0;
    a->cap = 0;
    a->items = NULL;
}

void Array_of_Relation_dtor(Array_of_Relation *a) {
    if (a->cap){
        for (int i = 0; i < a->size; ++i) {
            Relation_dtor(&a->items[i]);
        }
        free(a->items);
    }
    a->items = NULL;
    a->cap = a->size = 0;
}

void Array_of_Relation_resize(Array_of_Relation *a, int new_cap) {
    Relation *resized;
    resized = realloc(a->items, new_cap * sizeof(Relation));
    if (resized != NULL)
    {
        a->items = resized;
        a->cap = new_cap;
    }
}



void First_letters_array_ctor(First_letters_array *a) {
    a->size = 0;
    a->cap = 0;
    a->first_letter = NULL;
}

void First_letters_array_dtor(First_letters_array *a) {
    if (a->cap)
        free(a->first_letter);
    a->first_letter = NULL;
    a->cap = a->size = 0;
}

void First_letters_array_resize(First_letters_array *a, int new_cap) {
    bool *resized;
    resized = realloc(a->first_letter, new_cap * sizeof(bool));
    if (resized != NULL)
    {
        a->first_letter = resized;
        a->cap = new_cap;
    }
}

void First_letters_array_append(First_letters_array *a, bool b) {
    if (a->size == a->cap)
        First_letters_array_resize(a, a->cap ? a->cap + a->cap : 1);

    if (a->size < a->cap)
    {
        a->first_letter[a->size] = b;
        a->size++;
    }
}



Command get_command_from_string(char string[]){
    Command cmd;
    cmd.command_index = -1;
    cmd.first_arg = -1;
    cmd.second_arg = -1;
    cmd.two_int_mode = 1;
    cmd.set_or_relation = 0;

    char *keywords[19];
    keywords[0] = "empty";
    keywords[1] = "card";
    keywords[2] = "complement";
    keywords[3] = "union";
    keywords[4] = "intersect";
    keywords[5] = "minus";
    keywords[6] = "subseteq";
    keywords[7] = "subset";
    keywords[8] = "equals";
    keywords[9] = "reflexive";
    keywords[10] = "symmetric";
    keywords[11] = "antisymmetric";
    keywords[12] = "transitive";
    keywords[13] = "function";
    keywords[14] = "domain";
    keywords[15] = "codomain";
    keywords[16] = "injective";
    keywords[17] = "surjective";
    keywords[18] = "bijective";


    int string_counter = 0;
    int buffer_counter = 0;
    char buffer[16];
    while (string[string_counter] != ' '){
        buffer[buffer_counter++] = string[string_counter++];
    }
    buffer[buffer_counter] = '\0';
    for (int i = 0; i < 19; ++i) {
        if (strcmp(keywords[i], buffer) == 0){
            cmd.command_index = i;
            break;
        }
    }
    buffer_counter = 0;
    string_counter++;
    while (string[string_counter] != ' '){
        if (string[string_counter] == '\0'){
            cmd.two_int_mode = 0;
            break;
        }
        buffer[buffer_counter++] = string[string_counter++];
    }
    buffer[buffer_counter] = '\0';
    cmd.first_arg = atoi(buffer);

    buffer_counter = 0;
    string_counter++;
    while (string[string_counter] && cmd.two_int_mode == 1){
        buffer[buffer_counter++] = string[string_counter++];
    }
    buffer[buffer_counter] = '\0';
    cmd.second_arg = atoi(buffer);
    if (cmd.command_index > 8) {
        cmd.set_or_relation = 1;
    }
    return cmd;
}


//returns set/relation index in set/relation array
int count_right_index(First_letters_array a, int line_number, bool mode){
    int right_index = -1;
    for (int i = 0; i < line_number; ++i) {
        // 0 counting right index of set
        if (mode == 0){
            if (a.first_letter[i] == 0){
                right_index++;
            }
        }else{
            // 1 counting right index of relation
            if (a.first_letter[i] == 1){
                right_index++;
            }
        }
    }
    return right_index;
}



bool check_is_empty(Set set) {
    if (!set.size) {
        return 1;
    }else {
        return 0;
    }
}

void card_set(Set set){
    printf("%d", set.size);
}

//returns difference of two sets
Set minus_set(Set set1, Set set2) {
    int counter_rep = 0;
    Set set3;
    Set_ctor(&set3) ;
    for (int i = 0; i < set1.size; i++) {
        counter_rep = 0;
        for (int j = 0; j < set2.size; j++) {
            if (set1.index[i] == set2.index[j]){
                break;
            } else counter_rep++;
        }
        if (counter_rep == set2.size){
            Set_append(&set3, set1.index[i]);
        }
    }
    return set3;
}

void complement_set(Set uni, Set set, Universe universe){
    print_Set(minus_set(uni, set), universe);
}

void union_set(Set set1, Set set2, Universe universe){
    printf ("S");
    for (int i = 0; i < set1.size; ++i) {
        printf(" %s", universe.items[set1.index[i]].data);
    }
    for (int i = 0; i < minus_set(set2, set1).size; ++i) {
        printf(" %s", universe.items[minus_set(set2, set1).index[i]].data);
    }
}

void intersect_set(Set set1, Set set2, Universe universe) {
    print_Set(minus_set(set1, minus_set(set1, set2)), universe);
}

void subseteq_set(Set set1, Set set2){
    if (check_is_empty(minus_set(set1, set2))){
        printf("true");
    }else{
        printf("false");
    }
}

bool equals_set(Set set1, Set set2){
    if (set1.size == set2.size && check_is_empty(minus_set(set1, set2)) && check_is_empty(minus_set(set2, set1))){
        return 1;
    }else{
        return 0;
    }
}

void subset_set(Set set1, Set set2){
    if (set1.size && !equals_set(set1, set2)){
        subseteq_set(set1, set2);
    }else printf("false");
}



void reflexive_relation(Relation relation){
    int different_el_first = Relation_without_recurrence_first(relation);
    int different_el_second = Relation_without_recurrence_second(relation);
    int ref_count = 0;
    for (int i = 0; i < relation.size; ++i) {
        if (relation.data[i].first == relation.data[i].second)
            ref_count++;
    }
    if (ref_count == different_el_first && ref_count == different_el_second){
        printf("true");
    }else{
        printf("false");
    }
}

void symmetric_relation(Relation relation){
    bool symm = 1;
    for (int i = 0; i < relation.size; ++i) {
        if (relation.data[i].first == relation.data[i].second){
            continue;
        }
        for (int j = 0; j < relation.size; ++j) {
            symm = 0;
            if (relation.data[i].first == relation.data[j].second &&
                relation.data[i].second == relation.data[j].first) {
                symm = 1;
                break;
            }
        }
        if (!symm){
            printf("false");
            break;
        }
    }
    if (symm) {
        printf("true");
    }
}

void antisymmetric_relation (Relation relation){
    bool antisymm = 1;
    for (int i = 0; i < relation.size; ++i) {
        if (relation.data[i].first == relation.data[i].second){
            continue;
        }
        for (int j = 0; j < relation.size; ++j) {
            if (relation.data[i].first == relation.data[j].second &&
                relation.data[i].second == relation.data[j].first) {
                antisymm = 0;
                break;
            }
        }
        if (!antisymm){
            printf("false");
            break;
        }
    }
    if (antisymm) {
        printf("true");
    }
}

void transitive_relation(Relation relation){
    bool is_transitive = true;
    for (int i = 0; i < relation.size; ++i) {
        for (int j = 0; j < relation.size; ++j) {
            if (relation.data[i].second == relation.data[j].first) {
                is_transitive = false;
                for (int k = 0; k < relation.size; ++k) {
                    if (relation.data[k].first == relation.data[i].first &&
                        relation.data[k].second == relation.data[j].second) {
                        is_transitive = true;
                        break;
                    }
                }
            }
            if (!is_transitive){
                printf("false");
                return;
            }
        }
    }
    printf("true");
}

void function_relation(Relation relation){
    bool incongruity = 0;
    for (int i = 1; i < relation.size; ++i) {
        for (int j = 0; j < i; ++j) {
            if (relation.data[i].first == relation.data[j].first && relation.data[i].second != relation.data[j].second){
                incongruity = 1;
                break;
            }
        }
        if (incongruity) {
            break;
        }
    }
    if (incongruity){
        printf("false");
    }else{
        printf("true");
    }
}

void domain_relation(Relation relation, Universe universe){
    printf ("S");
    bool recurrence = 0;
    printf(" %s", universe.items[relation.data[0].first].data);
    for (int i = 1; i < relation.size; ++i) {
        for (int j = 0; j < i; ++j) {
            if (relation.data[i].first == relation.data[j].first){
                recurrence = 1;
                break;
            }
        }
        if (!recurrence){
            printf(" %s", universe.items[relation.data[i].first].data);
        }
        recurrence = 0;
    }
}

void codomain_relation(Relation relation, Universe universe){
    printf ("S");
    bool recurrence = 0;
    printf(" %s", universe.items[relation.data[0].second].data);
    for (int i = 1; i < relation.size; ++i) {
        for (int j = 0; j < i; ++j) {
            if (relation.data[i].second == relation.data[j].second){
                recurrence = 1;
                break;
            }
        }
        if (!recurrence){
            printf(" %s", universe.items[relation.data[i].second].data);
        }
        recurrence = 0;
    }
}


// processes operation, if input is wrong returns error code
int do_command(Command cmd, First_letters_array fl_a, Array_of_Sets a_sets, Array_of_Relation a_rel, Universe universe) {
    printf("\n");
    if (cmd.command_index == - 1){
        fprintf(stderr, "Command doesnt exist");
        return 23;
    }
    if(cmd.first_arg > fl_a.size || cmd.first_arg <= 0){
        fprintf(stderr, "Wrong input");
        return 24;
    }
    if (cmd.set_or_relation == 0){
        //commands for sets
        if (fl_a.first_letter[cmd.first_arg - 1] != 0){
            fprintf(stderr, "No set on this line");
            return 25;
        }
        if (cmd.command_index > 8){
            fprintf(stderr, "Wrong command");
            return 26;
        }
        if (cmd.two_int_mode == 0){
            //one argument
            if (cmd.command_index > 2){
                fprintf(stderr, "Wrong command");
                return 27;
            }

            if (cmd.command_index == 0) {
                if (check_is_empty(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)])){
                    printf("true");
                }else{
                    printf("false");
                }
            }
            if (cmd.command_index == 1) {
                card_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
            }
            if (cmd.command_index == 2) {
                complement_set(a_sets.items[0],
                               a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                               universe);
            }
        }else{
            //two arguments
            if (fl_a.first_letter[cmd.second_arg - 1] != 0 || cmd.second_arg > fl_a.size){
                fprintf(stderr, "No set on this line");
                return 28;
            }
            if (cmd.command_index <= 2){
                fprintf(stderr, "Wrong command");
                return 29;
            }
            if (cmd.command_index == 3){
                union_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                          a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)],
                          universe);
            }
            if (cmd.command_index == 4){
                intersect_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                              a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)],
                              universe);
            }
            if (cmd.command_index == 5){
                print_Set(minus_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                                    a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)]),
                          universe);
            }
            if (cmd.command_index == 6){
                subseteq_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                             a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)]);
            }
            if (cmd.command_index == 7){
                subset_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                           a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)]);
            }
            if (cmd.command_index == 8){
                if (equals_set(a_sets.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                               a_sets.items[count_right_index(fl_a, cmd.second_arg, cmd.set_or_relation)])){
                    printf("true");
                }else{
                    printf("false");
                }
            }
        }
    }else{
        //commands for relations
        if (cmd.set_or_relation){
            if (fl_a.first_letter[cmd.first_arg - 1] != 1){
                fprintf(stderr, "No relation on this line");
                return 30;
            }
            if (cmd.command_index < 9) {
                fprintf(stderr, "Wrong command");
                return 31;
            }
            if (cmd.two_int_mode == 0){
                //one argument
                if (cmd.command_index > 15){
                    fprintf(stderr, "Wrong command");
                    return 32;
                }
                if (cmd.command_index == 9){
                    reflexive_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
                }
                if (cmd.command_index == 10){
                    symmetric_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
                }
                if (cmd.command_index == 11){
                    antisymmetric_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
                }
                if (cmd.command_index == 12){
                    transitive_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
                }
                if (cmd.command_index == 13){
                    function_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)]);
                }
                if (cmd.command_index == 14){
                    domain_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                                    universe);
                }
                if (cmd.command_index == 15){
                    codomain_relation(a_rel.items[count_right_index(fl_a, cmd.first_arg, cmd.set_or_relation)],
                                      universe);
                }
            }else{
                //two argument
                if (fl_a.first_letter[cmd.second_arg - 1] != 1 || cmd.second_arg > fl_a.size){
                    fprintf(stderr, "No relation on this line");
                    return 33;
                }
                if (cmd.command_index <= 15){
                    fprintf(stderr, "Wrong command");
                    return 34;
                }
                if (cmd.command_index == 16){
                    printf("%s", "injective");
                }
                if (cmd.command_index == 17){
                    printf("%s", "surjective");
                }
                if (cmd.command_index == 18){
                    printf("%s", "bijective");
                }
            }
        }
    }
    return 0;
}





int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Program accepts only 1 argument or too many arguments");
        return 31;
    }

    char ch;
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Error: The document was not opened.");
        return 213;
    }

    Universe universe;
    Universe_ctor(&universe);

    Array_of_Sets a_sets;
    Array_of_Sets_ctor(&a_sets);
    int set_counter = 0;

    Array_of_Relation a_rel;
    Array_of_Relation_ctor(&a_rel);
    int rel_counter = 0;

    Pair pair_tmp;
    pair_tmp.first = -1;
    pair_tmp.second = -1;

    First_letters_array fl_a;
    First_letters_array_ctor(&fl_a);


    Flags flags;
    flags.universe_flag = 0;
    flags.set_flag = 0;
    flags.relation_flag = 0;
    flags.pair_flag = 0;
    flags.element_flag = 0;
    flags.command_flag = 0;

    Command cmd;
    cmd.first_arg = -1;
    cmd.command_index = -1;
    cmd.second_arg = -1;

    int line_counter = 1;

    //buffer for reading elements of universe\sets
    char buffer[30];
    int buffer_counter = 0;

    //buffer for reading pairs
    char relation_buffer[70];
    int relation_buffer_counter= 0;

    //buffer for reading operations
    char command_buffer[32];
    int command_buffer_counter = 0;

    int error_code = 0;


    while((ch = fgetc(fp)) != EOF) {
        if (line_counter == 1001){
            fprintf(stderr, "Error: Quantity of lines > 1000 ");
            error_code = 21;
            break;
        }
        if (ch == 'U' && flags.universe_flag == 0) {
            First_letters_array_append(&fl_a, 0);
            flags.universe_flag = 1;
            a_sets.size++;
            Array_of_Sets_resize(&a_sets, set_counter + 1);
            Set_ctor(&a_sets.items[set_counter++]);
            flags.set_flag = 1;
            continue;
        }

        if (ch == 'S' && flags.set_flag == 0)
        {
            First_letters_array_append(&fl_a, 0);
            Array_of_Sets_resize(&a_sets, set_counter + 1);
            a_sets.size++;
            Set_ctor(&a_sets.items[set_counter++]);
            flags.set_flag = 1;
            continue;
        }
        if (ch == 'R' && flags.relation_flag == 0) {
            First_letters_array_append(&fl_a, 1);
            Array_of_Relation_resize(&a_rel, rel_counter +1);
            a_rel.size++;
            Relation_ctor(&a_rel.items[rel_counter++]);
            flags.relation_flag = 1;
            continue;
        }
        if (ch == 'C' && flags.command_flag == 0) {
            flags.command_flag = 1;
            continue;
        }

        if (ch == ' ' && flags.element_flag != 1) {
            flags.element_flag = 1;
            continue;
        }

        if (flags.element_flag == 1) {
            if (ch != ' ' && ch != '\n') {
                buffer[buffer_counter++] = ch;
            } else {
                buffer[buffer_counter] = '\0';
            }
        }

        if (flags.element_flag == 1 && (ch == ' ' || ch == '\n')) {
            if (flags.universe_flag == 1) {
                if (contains_banned_words(buffer)){
                    fprintf(stderr, "Buffer contains banned word");
                    error_code = 78;
                    break;
                }
                Universe_append(&universe, buffer, buffer_counter);
            }
            if (flags.set_flag == 1) {
                int element_as_index_set = index_in_universe(buffer, universe);
                if (element_as_index_set == -1) {
                    fprintf(stderr, "Error: element is not in universe");
                    error_code = 1;
                    break;
                } else {
                    Set_append(&a_sets.items[set_counter - 1], element_as_index_set);
                }
            }
            flags.element_flag = 2;
            buffer_counter = 0;
        }

        if (flags.element_flag == 2 && ch == ' ') {
            flags.element_flag = 1;
        }

        if (flags.universe_flag == 1 && ch == '\n') {
            Universe_print(universe);
            flags.universe_flag = 2;
            flags.set_flag = 0;
        }

        if (flags.set_flag == 1 && ch == '\n') {
            printf("\n");
            print_Set(a_sets.items[set_counter - 1], universe);
            flags.set_flag = 0;
        }

        if (flags.pair_flag == 0 && ch == '('){
            flags.pair_flag = 1;
        }

        if (flags.pair_flag == 1 && ch!= '(' && ch != ')'){
            relation_buffer[relation_buffer_counter++] = ch;
        }

        if (flags.pair_flag == 1 && ch == ')'){
            relation_buffer[relation_buffer_counter] = '\0';
            pair_tmp = get_arguments_from_string(relation_buffer, universe);
            flags.pair_flag = 0;
            relation_buffer_counter = 0;
            if (pair_tmp.first == -1 || pair_tmp.second == -1) {
                fprintf(stderr, "Error: element is not in universe");
                error_code = 2;
                break;
            } else {
                Relation_append(&a_rel.items[rel_counter - 1],pair_tmp);
            }
        }


        if (flags.relation_flag == 1 && ch == '\n'){
            print_Relation(a_rel.items[rel_counter - 1], universe);
            flags.relation_flag = 0;
        }

        if (flags.command_flag == 1) {
            command_buffer[command_buffer_counter++] = ch;
        }

        if (flags.command_flag == 1 && ch == '\n') {
            command_buffer[command_buffer_counter - 1] = '\0';
            cmd = get_command_from_string(command_buffer);
            error_code = do_command(cmd, fl_a, a_sets, a_rel, universe);
            if (error_code){
                break;
            }
            flags.command_flag = 0;
            command_buffer_counter = 0;
        }

        if (ch == '\n') {
            line_counter++;
        }
    }
    fclose(fp);
    Universe_dtor(&universe);
    Array_of_Sets_dtor(&a_sets);
    Array_of_Relation_dtor(&a_rel);
    First_letters_array_dtor(&fl_a);
    return error_code;
}