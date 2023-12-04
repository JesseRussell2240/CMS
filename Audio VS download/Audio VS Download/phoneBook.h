#ifndef PHONEBOOK_H
#define PHONEBOOK_H

typedef struct phoneBook* PhoneBook;
typedef int Item;
typedef char* Msg;
typedef int Length;
typedef struct phoneBook* Link;
typedef int PayloadType;

struct phoneBook {
    Item user;
    Msg msg;
    Length length;
    Link vlinks;
    Link hlinks;
    PayloadType loadType;
};

void graph_init(PhoneBook* phonebook);
void graph_insert_edge(PhoneBook* phonebook, Item use, Msg mes, Length len, PayloadType load);
void graph_destroy(PhoneBook* phonebook);
void graph_print_adjacency(PhoneBook* phonebook);
void graph_insert_vertex(PhoneBook* phonebook, Item data);

#endif /* PHONEBOOK_H */
