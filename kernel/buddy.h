//
// Created by vlad on 22.10.23.
//

#ifndef XV6_REP_BUDDY_H
#define XV6_REP_BUDDY_H

void bd_init(void *base, void *end);
void bd_free(void *p);
void *bd_malloc(uint64 nbytes);

#endif //XV6_REP_BUDDY_H
