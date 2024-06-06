#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "St.h"

typedef struct St {
    int value;
    struct St* next;
}St;

St* addStack(St* head, int value) {
    St *temp = (St*) malloc(sizeof(St));
    temp->value = value;
    temp->next = head;
    return temp;
}

St* removeStack(St* head, int value) {
    St* temp = head->next;
    free(head);
    return temp;
}

double calcStack(St* head, int place) {
    double res = 0;
    while (head != NULL) {
        res += pow(10, place) * head->value;
        place++;
        head = head->next;
    }
    return res;
}