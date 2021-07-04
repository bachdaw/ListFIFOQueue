#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


struct Node
{
    int data;
    Node* pxor;
};

Node* XOR(Node* a, Node* b)
{
    return (Node*)((int)a ^ (int)b);
}

void PrintForward(Node* head)
{
    Node* curr = head;
    Node* prev = NULL;
    Node* next;

    if (curr == NULL) 
        printf("NULL");
    while (curr != NULL)
    {
        printf("%d ", curr->data);

        next = XOR(prev, curr->pxor);
        prev = curr;
        curr = next;
    }
    printf("\n");
}

void PrintBackward(Node* tail)
{
    Node* curr = tail;
    Node* prev;
    Node* next = NULL;

    if (curr == NULL) 
        printf("NULL");
    while (curr != NULL)
    {
        printf("%d ", curr->data);

        prev = XOR(next, curr->pxor);
        next = curr;
        curr = prev;
    }
    printf("\n");
}

int Count(Node* qfront, Node* qback, Node* head, Node* tail, bool* isEmpty)
{
    int count = 0;
    Node* curr = head;
    Node* prev = NULL;
    Node* next = NULL;

    if (*isEmpty == true)
        return count;
    else
    {
        count++;
        while (curr != qfront)
        {
            next = XOR(prev, curr->pxor);
            prev = curr;
            curr = next;
        }
        next = XOR(prev, curr->pxor);

        while (curr != qback)
        {
            if (curr == head)
            {
                curr = tail;
                next = NULL;
                prev = XOR(next, curr->pxor);
            }
            else
            {
                prev = XOR(next, curr->pxor);
                next = curr;
                curr = prev;
            }
            count++;
        }
    }
    return count;
}

void AddBeg(Node** head, Node** tail, Node** qfront, Node** qback, int* lSize)
{
    int n;
    Node* new_node = (Node*)malloc(sizeof(Node));
    scanf("%i", &n);

    new_node->data = n;
    new_node->pxor = *head;

    if (*head != NULL)
        (*head)->pxor = XOR(new_node, (*head)->pxor);
    if (*head == NULL)
    {
        *tail = new_node;

        *qfront = new_node;
        *qback = new_node;
    }
    *head = new_node;

    (*lSize)++;
}

Node* Next(Node* relative_node, Node* tail, Node* head)
{
    if (relative_node == NULL) 
        return NULL;
    else if (relative_node == tail) 
        relative_node = head;
    else {
        Node* curr = head;
        Node* prev = NULL;
        Node* next;

        while (curr != relative_node)
        {
            next = XOR(prev, curr->pxor);
            prev = curr;
            curr = next;
        }
        relative_node = XOR(prev, curr->pxor);
    }
    return relative_node;
}

Node* Previous(Node* relative_node, Node* tail, Node* head)
{
    if (relative_node == NULL) 
        return NULL;
    else if (relative_node == head) 
        relative_node = tail;
    else {
        Node* curr = head;
        Node* prev = NULL;
        Node* next;

        while (curr != relative_node)
        {
            next = XOR(prev, curr->pxor);
            prev = curr;
            curr = next;
        }
        relative_node = prev;
    }
    return relative_node;
}

void AddEnd(Node** tail, Node** head, Node** qfront, Node** qback, int* lSize)
{
    int n;
    Node* new_node = (Node*)malloc(sizeof(struct Node));
    scanf("%i", &n);

    new_node->data = n;
    new_node->pxor = *tail;
    if (*tail == NULL)
    {
        *head = new_node;

        *qfront = new_node;
        *qback = new_node;
    }
    if (*tail != NULL) 
        (*tail)->pxor = XOR((*tail)->pxor, new_node);
    *tail = new_node;

    (*lSize)++;
}

void DelBeg(Node** head, Node** tail, Node** qfront, Node** qback, int* lSize, bool* isEmpty)
{
    if (*head == NULL) 
        return;
    else if ((*head)->pxor == NULL)
    {
        free(*head);
        *head = NULL;
        *tail = NULL;
        *isEmpty = true;

        (*lSize)--;
    }
    else
    {
        if ((*qfront == *head) && (*qback == *head))
        {
            *qfront = Next(*qfront, *tail, *head);
            *qback = *qfront;
            *isEmpty = true;
        }
        else
        {
            if (*qback == *head) 
                *qback = Next(*qback, *tail, *head);
            if (*qfront == *head) 
                *qfront = Previous(*qfront, *tail, *head);
        }
        Node* next = (*head)->pxor;
        next->pxor = XOR(*head, next->pxor);
        free(*head);
        *head = next;

        (*lSize)--;
    }
}

void DelEnd(Node** head, Node** tail, Node** qfront, Node** qback, int* lSize, bool* isEmpty)
{
    if (*head == NULL) 
        return;
    else if ((*head)->pxor == NULL)
    {
        free(*head);
        *head = NULL;
        *tail = NULL;
        *isEmpty = true;

        (*lSize)--;
    }
    else
    {
        if ((*qfront == *tail) && (*qback == *tail))
        {
            *qfront = Previous(*qfront, *tail, *head);
            *qback = *qfront;
            *isEmpty = true;
        }
        else
        {
            if (*qfront == *tail) 
                *qfront = Previous(*qfront, *tail, *head);
            if (*qback == *tail) 
                *qback = Next(*qback, *tail, *head);
        }
        Node* prev = (*tail)->pxor;
        prev->pxor = XOR(*tail, prev->pxor);
        free(*tail);
        *tail = prev;

        (*lSize)--;
    }
}

void AddNewPush(Node** qfront, Node** qback, int* lSize)
{
    int n;
    Node* new_node = (Node*)malloc(sizeof(Node));
    scanf("%i", &n);
    new_node->data = n;
    new_node->pxor = XOR(*qfront, *qback);

    (*qfront)->pxor = XOR(XOR((*qfront)->pxor, *qback), new_node);
    (*qback)->pxor = XOR(XOR((*qback)->pxor, *qfront), new_node);
    *qback = new_node;

    (*lSize)++;
}

void Push(Node** tail, Node** head, Node** qfront, Node** qback, int* lSize, bool* isEmpty)
{
    if (*head == NULL)
    {
        AddBeg(head, tail, qfront, qback, lSize);
        *isEmpty = false;
    }
    else
    {
        Node* prev = Previous(*qback, *tail, *head);
        if (*isEmpty == true)
        {
            int n;
            scanf("%i", &n);
            (*qfront)->data = n;
            *isEmpty = false;
        }
        else if ((*qfront == *tail) && (*head == *qback))
        {
            AddBeg(head, tail, qfront, qback, lSize);
            *qback = *head;
        }
        else if((prev != *qfront))
        {
            int n;
            scanf("%i", &n);
            prev->data = n;
            *qback = prev;
        }
        else if ((prev == *qfront) && (*qfront != *tail))
        {
            AddNewPush(qfront, qback, lSize);
        }
    }
}

void PrintQueue(Node* qfront, Node* qback, Node* head, Node* tail, bool* isEmpty)
{
    if (head == NULL) 
        printf("NULL\n");
    else
    {
        if (*isEmpty == true) printf("NULL\n");
        else
        {
            Node* curr = head;
            Node* prev = NULL;
            Node* next = NULL;

            while (curr != qfront)
            {
                next = XOR(prev, curr->pxor);
                prev = curr;
                curr = next;
            }
            next = XOR(prev, curr->pxor);

            while (curr != qback)
            {
                printf("%d ", curr->data);

                if (curr == head)
                {
                    curr = tail;
                    next = NULL;
                    prev = XOR(next, curr->pxor);
                }
                else
                {
                    prev = XOR(next, curr->pxor);
                    next = curr;
                    curr = prev;
                }
            }
            printf("%d\n", curr->data);
        }
    }
}

void Pop(Node* tail, Node* head, Node** qfront, Node** qback, bool* isEmpty)
{
    if (*isEmpty == true) 
        printf("NULL\n");
    else 
    {
        printf("%d\n", (*qfront)->data);
        if (*qfront == *qback) *isEmpty = true;
        else *qfront = Previous(*qfront, tail, head);
    }
}

void GarbageSoft(Node* tail, Node* head, Node** qfront, Node** qback, bool* isEmpty)
{
    if (*isEmpty == true)
    {
        Node* curr = head;
        Node* prev = NULL;
        Node* next;

        while (curr != tail)
        {
            curr->data = 0;
            next = XOR(prev, curr->pxor);
            prev = curr;
            curr = next;
        }
        curr->data = 0;
    }
    else
    {
        Node* curr = *qback;
        Node* prev = Previous(*qback, tail, head);
        Node* next = Next(*qback, tail, head);

        while (curr != *qfront)
        {
            if (curr == *qback);
            else
            {
                curr->data = 0;
            }

            if (curr == head)
            {
                curr = tail;
                next = NULL;
                prev = XOR(next, curr->pxor);
            }
            else
            {
                prev = XOR(next, curr->pxor);
                next = curr;
                curr = prev;
            }
        }
    }
}

void Free(Node** tail, Node** head, Node** qfront, Node** qback, int* lSize, bool* isEmpty)
{
    while (*head != NULL)
        DelEnd(head, tail, qfront, qback, lSize, isEmpty);
}

void GarbageHard(Node** tail, Node** head, Node** qfront, Node** qback, int* lSize, bool* isEmpty)
{
    if (*isEmpty)   
        Free(tail, head, qfront, qback, lSize, isEmpty);
    else
    {
        Node* curr = Previous(*qback, *tail, *head);
        Node* next = *qback;
        Node* prev = XOR(curr->pxor, next);

        while (curr != *qfront)
        {    
            if (curr == *qback);
            else if (curr == *head)
            {
                next = XOR(NULL, curr->pxor);
                next->pxor = XOR(NULL, XOR(curr, next->pxor));
                free(curr);
                *head = next;
                curr = *tail;
                next = NULL;
                prev = XOR(NULL, curr->pxor);
                (*lSize)--;
            }
            else if(curr == *tail)
            {
                prev = XOR(NULL, curr->pxor);
                next = NULL;
                curr = prev;
                DelEnd(head, tail, qfront, qback, lSize, isEmpty);
            }
            else
            {
                prev->pxor = XOR(XOR(prev->pxor, curr), next);
                next->pxor = XOR(XOR(next->pxor, curr), prev);
                free(curr);
                curr = prev;
                prev = XOR(curr->pxor, next);
                (*lSize)--;
            }
        }
    }
}

int main()
{
    Node* head = NULL;
    Node* tail = head;
    Node* qfront = NULL;
    Node* qback = NULL;
    bool isEmpty = true;
    int lSize = 0;

    char command[256];
    while (scanf("%s", command) != EOF)
    {
      
        if (strcmp(command, "ADD_BEG") == 0)
        {
            AddBeg(&head, &tail, &qfront, &qback, &lSize);
        }
        else if (strcmp(command, "PUSH") == 0)
        {
            Push(&tail, &head, &qfront, &qback, &lSize, &isEmpty);
        }
        else if (strcmp(command, "ADD_END") == 0)
        {
            AddEnd(&tail, &head, &qfront, &qback, &lSize);
        }
        else if (strcmp(command, "DEL_BEG") == 0)
        {
            DelBeg(&head, &tail, &qfront, &qback, &lSize, &isEmpty);
        }
        else if (strcmp(command, "DEL_END") == 0)
        {
            DelEnd(&head, &tail, &qfront, &qback, &lSize, &isEmpty);
        }
        else if (strcmp(command, "PRINT_FORWARD") == 0)
        {
            PrintForward(head);
        }
        else if (strcmp(command, "PRINT_BACKWARD") == 0)
        {
            PrintBackward(tail);
        }
        else if (strcmp(command, "PRINT_QUEUE") == 0)
        {
            PrintQueue(qfront, qback, head, tail, &isEmpty);
        }
        else if (strcmp(command, "POP") == 0)
        {
            Pop(tail, head, &qfront, &qback, &isEmpty);
        }
        else if (strcmp(command, "COUNT") == 0)
        {
            printf("%d\n", Count(qfront, qback, head, tail, &isEmpty));
        }
        else if (strcmp(command, "SIZE") == 0)
        {
            printf("%d\n", lSize);
        }
        else if (strcmp(command, "GARBAGE_SOFT") == 0)
        {
            GarbageSoft(tail, head, &qfront, &qback, &isEmpty);
        }
        else if (strcmp(command, "GARBAGE_HARD") == 0)
        {
            GarbageHard(&tail, &head, &qfront, &qback, &lSize, &isEmpty);
        }
        else printf("Couldn't recognized the command!\n");
    }
    Free(&tail, &head, &qfront, &qback, &lSize, &isEmpty);
}