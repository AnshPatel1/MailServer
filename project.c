#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_CHILDREN 100
#define INBOX_SIZE 100
#define DOMAIN "sot.pdpu.ac.in"

struct Node *root;

struct Mail {
    char *from;
    char *to;
    char *subject;
    char *body;
};

struct Node {
    char *email;
    struct Node **children;
    int numChildren;
    int isAccount;
    int isGroup;
    struct Mail **inbox;
};


char *concat(char *s1, char *s2, char *s3) {
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

char getUsernameFromEmail(char *email) {
    char *username = malloc(strlen(email) + 1);
    strcpy(username, email);
    char *token = strtok(username, "@");
    return token;
}

struct Node *initializeRoot(char *email) {
    struct Node *root = malloc(sizeof(struct Node));
    root->email = concat(email, "@", DOMAIN);
    root->children = malloc(sizeof(struct Node *) * MAX_CHILDREN);
    root->numChildren = 0;
    root->isAccount = 0;
    root->isGroup = 1;
    root->inbox = NULL;
    return root;
}

struct Node *initializeAccount(char *email) {
    struct Node *account = malloc(sizeof(struct Node));
    account->email = concat(email, "@", DOMAIN);
    account->children = NULL;
    account->numChildren = 0;
    account->isAccount = 1;
    account->isGroup = 0;
    account->inbox = malloc(sizeof(struct Mail *) * INBOX_SIZE);
    return account;
}

void addNodeToParent(struct Node *parent, struct Node *child) {
    parent->children[parent->numChildren] = child;
    parent->numChildren++;
}

struct Node *initializeGroup(char *email) {
    struct Node *group = malloc(sizeof(struct Node));
    group->email = concat(email, "@", DOMAIN);
    group->children = malloc(sizeof(struct Node *) * MAX_CHILDREN);
    group->numChildren = 0;
    group->isAccount = 0;
    group->isGroup = 1;
    group->inbox = NULL;
    return group;
}

struct Node *findNode(struct Node *root, char *email) {
    if (strcmp(root->email, email) == 0) {
        return root;
    }
    for (int i = 0; i < root->numChildren; i++) {
        printf("%d", i);
        struct Node *result = findNode(root->children[i], email);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

struct Node *findGroup(struct Node *root, char *email) {
    struct Node *group = findNode(root, email);
    if (group != NULL && group->isGroup) {
        return group;
    } else if (group == NULL && !group->isAccount) {
        printf("Not a group\n");
        return NULL;
    } else {
        printf("Group not found\n");
        return NULL;
    }
}

struct Node *findAccount(struct Node *root, char *email) {
    struct Node *account = findNode(root, email);
    if (account != NULL && account->isAccount) {
        return account;
    } else if (account == NULL && !account->isAccount) {
        printf("Not an account\n");
        return NULL;
    } else {
        printf("Account not found\n");
        return NULL;
    }
}

void sendMailToAccount(struct Node *from, struct Node *to, char *subject, char *body) {
    struct Mail *mail = malloc(sizeof(struct Mail));
    mail->from = from->email;
    mail->to = to->email;
    mail->subject = subject;
    mail->body = body;
    if (to->inbox == NULL) {
        to->inbox = malloc(sizeof(struct Mail *) * INBOX_SIZE);
    }
    to->inbox[to->numChildren] = mail;
    to->numChildren++;
}

//void sendMailToGroup(struct Node *from, struct Node *group, char *subject, char *body) {
//    for (int i = 0; i < group->numChildren; i++) {
//        sendMail(from, group->children[i], subject, body);
//    }
//}


void printMail(struct Mail *mail) {
    printf("From: %s\n", mail->from);
    printf("To: %s\n", mail->to);
    printf("Subject: %s\n", mail->subject);
    printf("Body: %s\n", mail->body);
}

void printInbox(char *email) {
    struct Node *node = findNode(root, email);
    if (node->isAccount) {
        if (node != NULL) {
            for (int i = 0; i < node->numChildren; i++) {
                printMail(node->inbox[i]);
            }
        }
    } else {
        printf("Not an account\n");
    }
}

void printAccountsInGroup(char *email) {
    struct Node *group = findGroup(root, email);
    if (group != NULL) {
        for (int i = 0; i < group->numChildren; i++) {
            if (group->children[i]->isAccount) {
                printf("%s\n", group->children[i]->email);
            }
        }
    }
}

void sendMailToGroup(char *from, char *email, char *subject, char *body) {
    struct Node *group = findGroup(root, email);
    if (group != NULL) {
        for (int i = 0; i < group->numChildren; i++) {
            if (group->children[i]->isAccount) {
                sendMailToAccount(findAccount(root, from), group->children[i], subject, body);
            }
        }
    }
}
void sendMail(struct Node *from, struct Node *to, char *subject, char *body) {
    if (to->isAccount) {
        sendMailToAccount(from, to, subject, body);
    }
    else if (to->isGroup) {
        sendMailToGroup(from->email, to->email, subject, body);
    }
}
int menu() {
    printf("1. Create Account\n");
    printf("2. Create Group\n\n");
    printf("3. Send Mail\n\n");
    printf("4. Create Account in Group\n");
    printf("5. Print Inbox\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1: {
            char *email = malloc(100);
            printf("Enter Username: ");
            scanf("%s", email);
            struct Node *account = initializeAccount(email);
            root->children[root->numChildren] = account;
            root->numChildren++;
            printf("Account created successfully\n");
            break;
        }
        case 2: {
            char *username = malloc(100);
            printf("Enter Group Name: ");
            scanf("%s", username);
            struct Node *group = initializeGroup(username);
            root->children[root->numChildren] = group;
            root->numChildren++;
            printf("Group created successfully\n");
            break;
        }
        case 3: {
            char *from = malloc(100);
            char *to = malloc(100);
            char *subject = malloc(100);
            char *body = malloc(100);
            printf("Enter from: ");
            scanf("%s", from);
            printf("Enter to: ");
            scanf("%s", to);
            printf("Enter subject: ");
            scanf("%s", subject);
            printf("Enter body: ");
            scanf("%s", body);
            struct Node *fromNode = findAccount(root, from);
            struct Node *toNode = findNode(root, to);
            sendMail(fromNode, toNode, subject, body);
            printf("Mail sent successfully\n");
            break;
        }
        case 4: {
            char *email = malloc(100);
            char *username = malloc(100);
            char *groupEmail;
            printf("Enter Username: ");
            scanf("%s", email);
            printf("Enter Group Name: ");
            scanf("%s", username);
            groupEmail = concat(username, "@", DOMAIN);
            printAccountsInGroup(groupEmail);
            struct Node *group = findNode(root, groupEmail);
            if (group != NULL && group->isGroup) {
                addNodeToParent(group, initializeAccount(email));

                printf("Account created successfully\n");
            } else {
                printf("Group not found\n");
            }
            break;
        }

        case 5: {
            char *email = malloc(100);
            printf("Enter Email: ");
            scanf("%s", email);
            printInbox(email);
            break;
        }
        case 6: {
            return 1;
        }
        default: {
            printf("Invalid choice\n");
            break;
        }
    }
    return 0;
}

int main() {
    root = initializeRoot("ALL");
    int status = 0;
    do {
        status = menu();
    } while (status != 1);
    return 0;
}
