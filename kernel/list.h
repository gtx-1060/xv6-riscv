//
// Created by vlad on 22.10.23.
//

#ifndef XV6_REP_LIST_H
#define XV6_REP_LIST_H

struct list  {
    struct list* prev;
    struct list* next;
};

void lst_init(struct list *lst);
int lst_empty(struct list *lst);
void lst_remove(struct list *e);
void* lst_pop(struct list *lst);
void lst_push(struct list *lst, void *p);
void lst_print(struct list *lst);

#endif //XV6_REP_LIST_H
