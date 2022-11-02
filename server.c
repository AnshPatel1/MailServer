#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// CONFIGURATION FOR THE PROGRAM
#define MAX_INBOX_CAPACITY 100
#define MAX_MAIL_LIST_MEMBERS 100
#define MAX_SERVERS 100
#define MAX_MAIL_GROUPS_PER_SERVER 100


// STRUCTURES
struct MailServer {
    char *domain;
    struct MailTreeNode *root;
    struct MailAccount **mailAccounts;
    int currentMailAccountsCount;
    int currentMailGroupsCount;
};


struct Mail {
    char *sender;
    char *receiver;
    char *subject;
    char *body;
};

struct MailAccount {
    char *name;
    char *email;
    struct Mail **inbox;
    int currentInboxCount;

};

struct MailTreeNode {
    int isMailGroup;
    char *mailGroupName;
    struct MailTreeNode **members;
    int currentMemberCount;
    int isMailAccount;
    struct MailAccount account;
};

// FUNCTION SIGNATURES FOR MAIN USE CASES
struct MailServer *createAndInitializeMailServer(char *domain);

struct MailAccount createMailAccount(char *name, char *email);

struct MailTreeNode *createMailGroup(char *name);

// FUNCTION SIGNATURES FOR CONFIGURATIONS
void addMailAccountToMailServer(struct MailServer *server, struct MailAccount account);

void addMailGroupToMailServer(struct MailServer *server, struct MailTreeNode group);

// FUNCTION SIGNATURES FOR MAILING
void sendMail(struct Mail *mail);

void sendMailToMailGroup(struct Mail *mail, char *group);

// FUNCTION SIGNATURES FOR MAIL TREE
struct MailTreeNode *createMailTreeNode();

void addMailAccountToMailTreeNode(struct MailTreeNode *node, struct MailAccount *account);

void addMailGroupToMailTreeNode(struct MailTreeNode *node, struct MailTreeNode *group);

void addMailTreeNodeToMailTreeNode(struct MailTreeNode *parent, struct MailTreeNode *child);

// FUNCTION SIGNATURES FOR MAIL ACCOUNTS
void addMailToMailAccount(struct MailAccount *account, struct Mail mail);

void printMailAccount(struct MailAccount account);

struct MailTreeNode *findMailGroup(struct MailTreeNode *root, char *groupName);

struct MailTreeNode *findMailAccount(struct MailTreeNode *root, char *email);

struct MailServer *findMailServerByDomain(char *domain);

void printInbox(char *email);

// GLOBAL VARIABLES
struct MailServer *mailServers[MAX_SERVERS];
int currentMailServerCount = 0;
struct MailServer *selectedMailServer;

void mailServerMenu() {
    int isExit = 0;
    do {
        int choice;
        printf("1. Create Mail Account\n");
        printf("2. Create Mail Group\n");
        printf("3. Create Mail Group in an Existing Group\n");
        printf("4. Add Mail Account to Mail Group\n");
        printf("5. Add Mail Group to Mail Group\n");
        printf("6. Send Mail\n");
        printf("7. Send Mail To Mail Group\n");
        printf("8. Print Inbox of an Email\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                char name[100], roll_no[100], password[100], email[100];
                printf("Enter name: ");
                scanf("%s", name);
                printf("Enter email: ");
                scanf("%s", email);
                struct MailAccount account = createMailAccount(name, email);
                addMailAccountToMailServer(selectedMailServer, account);
            }
                break;
            case 2: {
                char name[100];
                printf("Enter name: ");
                scanf("%s", name);
                struct MailTreeNode *group = createMailGroup(name);
                addMailGroupToMailTreeNode(selectedMailServer->root, group);
            }
                break;
            case 3: {
                char mailGroup[100], parent[100];
                printf("Enter mail group name: ");
                scanf("%s", mailGroup);
                printf("Enter parent mail group: ");
                scanf("%s", parent);
                struct MailTreeNode *group = createMailGroup(mailGroup);
                addMailGroupToMailTreeNode(findMailGroup(selectedMailServer->root, parent), group);
            }
                break;
            case 4: {
                char name[100], email[100];
                printf("Enter email: ");
                scanf("%s", name);
                printf("Enter group to add: ");
                scanf("%s", email);
                struct MailAccount account = findMailAccount(selectedMailServer->root, email)->account;
                addMailAccountToMailTreeNode(findMailGroup(selectedMailServer->root, email), &account);
            }
                break;
            case 5: {
                char groupName[100], parent[100];
                printf("Enter group name: ");
                scanf("%s", groupName);
                printf("Enter parent group: ");
                scanf("%s", parent);
                struct MailTreeNode *group = findMailGroup(selectedMailServer->root, groupName);
                addMailGroupToMailTreeNode(findMailGroup(selectedMailServer->root, parent), group);
            }
                break;
            case 6: {
                char sender[100], receiver[100], subject[100], body[100];
                printf("Enter sender: ");
                scanf("%s", sender);
                printf("Enter receiver: ");
                scanf("%s", receiver);
                printf("Enter subject: ");
                scanf("%s", subject);
                printf("Enter body: ");
                scanf("%s", body);
                struct Mail mail = {sender, receiver, subject, body};
                sendMail(&mail);
            }
                break;
            case 7: {
                char sender[100], group[100], subject[100], body[100];
                printf("Enter sender: ");
                scanf("%s", sender);
                printf("Enter group name: ");
                scanf("%s", group);
                printf("Enter subject: ");
                scanf("%s", subject);
                printf("Enter body: ");
                scanf("%s", body);
                struct Mail mail = {sender, group, subject, body};
                sendMailToMailGroup(&mail, group);
            }
            case 8: {
                char email[100];
                printf("Enter email: ");
                scanf("%s", email);
                printInbox(email);
            }
                break;
            case 9:
                isExit = 1;
                break;
            default:
                printf("Invalid Choice\n");
                break;
        }
    } while (!isExit);
}

void mainMenu() {
    int isExit = 0;


    do {
        int choice;
        printf("1. Create Mail Server\n");
        printf("2. Select Mail Server\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                char domain[100];
                printf("Enter domain name: ");
                scanf("%s", domain);
                mailServers[currentMailServerCount++] = createAndInitializeMailServer(domain);
            }
                break;
            case 2: {
                char domain_choice[100];
                printf("Enter your choice: ");
                scanf("%s", domain_choice);
                selectedMailServer = findMailServerByDomain(domain_choice);
                mailServerMenu();
            }
                break;
            case 3:
                isExit = 1;
                break;
            default:
                printf("Invalid Choice\n");
        }

    } while (!isExit);
}


// MAIN FUNCTION
int main() {
    mainMenu();
}

// FUNCTION DEFINITIONS FOR MAIN USE CASES

struct MailServer *findMailServerByDomain(char *domain) {
    for (int i = 0; i < currentMailServerCount; ++i) {
        if (strcmp(mailServers[i]->domain, domain) == 0) {
            return mailServers[i];
        }
    }
    return NULL;
}

struct MailTreeNode *findMailGroup(struct MailTreeNode *root, char *groupName) {
    if (root->isMailGroup && strcmp(root->mailGroupName, groupName) == 0) {
        return root;
    }
    for (int i = 0; i < root->currentMemberCount; i++) {
        struct MailTreeNode *node = root->members[i];
        if (node->isMailGroup) {
            struct MailTreeNode *foundNode = findMailGroup(node, groupName);
            if (foundNode->isMailGroup && strcmp(foundNode->mailGroupName, groupName) == 0) {
                return foundNode;
            }
        }
    }
    return NULL;
}

struct MailTreeNode *findMailAccount(struct MailTreeNode *root, char *email) {
    if (root->isMailAccount && strcmp(root->account.email, email) == 0) {
        return root;
    }
    for (int i = 0; i < root->currentMemberCount; i++) {
        struct MailTreeNode *node = root->members[i];
        if (node->isMailGroup) {
            struct MailTreeNode *foundNode = findMailAccount(node, email);
            if (foundNode->isMailAccount && strcmp(foundNode->account.email, email) == 0) {
                return foundNode;
            }
        }
    }
    struct MailTreeNode *node = malloc(sizeof(struct MailTreeNode));
    node->isMailAccount = 0;
    return node;
}

struct MailServer *createAndInitializeMailServer(char *domain) {
    struct MailServer *server = (struct MailServer *) malloc(sizeof(struct MailServer));
    server->domain = domain;
    server->root = createMailGroup("ALL");
    server->mailAccounts = malloc(sizeof(struct MailAccount *) * MAX_MAIL_LIST_MEMBERS);
    server->currentMailAccountsCount = 0;
    server->currentMailGroupsCount = 0;
    return server;
}

struct MailAccount createMailAccount(char *name, char *email) {
    struct MailAccount account;
    account.name = name;
    account.email = email;
    account.inbox = malloc(sizeof(struct Mail *) * MAX_INBOX_CAPACITY);
    for (int i = 0; i < MAX_INBOX_CAPACITY; i++) {
        account.inbox[i] = malloc(sizeof(struct Mail));
    }
    account.currentInboxCount = 0;
    return account;
}

struct MailTreeNode *createMailGroup(char *name) {
    struct MailTreeNode *group = (struct MailTreeNode *) malloc(sizeof(struct MailTreeNode));
    group->mailGroupName = name;
    group->members = malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
    group->currentMemberCount = 0;
    group->isMailGroup = 1;
    group->isMailAccount = 0;

    return group;
}

struct MailTreeNode *createMailTreeNode() {
    struct MailTreeNode *node = (struct MailTreeNode *) malloc(sizeof(struct MailTreeNode));
    node->isMailGroup = 1;
    strcpy(node->mailGroupName, "ALL");
    node->members = malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
    node->currentMemberCount = 0;
    node->isMailAccount = 0;
    return node;
}

void addMailAccountToMailServer(struct MailServer *server, struct MailAccount account) {
    server->mailAccounts[server->currentMailAccountsCount++] = &account;
}

void addMailToMailGroup(char *email, char *group) {
    struct MailTreeNode *groupNode = findMailGroup(selectedMailServer->root, group);
    struct MailTreeNode *accountNode = findMailAccount(selectedMailServer->root, email);
    addMailAccountToMailTreeNode(groupNode, &accountNode->account);
}

void addMailAccountToMailTreeNode(struct MailTreeNode *node, struct MailAccount *account) {
    struct MailTreeNode *mailTreeNode = (struct MailTreeNode *) malloc(sizeof(struct MailTreeNode));
    mailTreeNode->isMailGroup = 0;
    mailTreeNode->account = *account;
    mailTreeNode->isMailAccount = 1;
    node->members[node->currentMemberCount] = mailTreeNode;
    node->currentMemberCount++;
}

void addMailGroupToMailTreeNode(struct MailTreeNode *node, struct MailTreeNode *groupName) {
    struct MailTreeNode *mailTreeNode = (struct MailTreeNode *) malloc(sizeof(struct MailTreeNode));
    mailTreeNode->isMailGroup = 1;
    mailTreeNode->mailGroupName = groupName->mailGroupName;
    mailTreeNode->isMailAccount = 0;
    node->members[node->currentMemberCount] = mailTreeNode;
    node->currentMemberCount++;
}

void sendMail(struct Mail *mail) {
    struct MailTreeNode *receiverNode = findMailAccount(selectedMailServer->root, mail->receiver);
    if (receiverNode) {
        receiverNode->account.inbox[receiverNode->account.currentInboxCount++] = mail;
    } else {
        printf("No such account found\n");
    }
//    strcpy(receiverNode->account.inbox[receiverNode->account.currentInboxCount++]->sender, mail->sender);
//    strcpy(receiverNode->account.inbox[receiverNode->account.currentInboxCount++]->subject , mail->subject);
//    strcpy(receiverNode->account.inbox[receiverNode->account.currentInboxCount++]->body , mail->body);
//    strcpy(receiverNode->account.inbox[receiverNode->account.currentInboxCount++]->receiver, mail->receiver);
}

void sendMailToMailGroup(struct Mail *mail, char *group) {
    struct MailTreeNode *groupNode = findMailGroup(selectedMailServer->root, mail->receiver);
    for (int i = 0; i < groupNode->currentMemberCount; i++) {
        struct MailTreeNode *node = groupNode->members[i];
        if (node->isMailAccount) {
            node->account.inbox[node->account.currentInboxCount++] = &mail;
        }
        if (node->isMailGroup) {
            sendMailToMailGroup(mail, node->mailGroupName);
        }
    }
}

void printMail(struct Mail *mail) {
    printf("Sender: %s\n", mail->sender);
    printf("Receiver: %s\n", mail->receiver);
    printf("Subject: %s\n", mail->subject);
    printf("Body: %s\n", mail->body);
    printf("\n\n\n\n");
}

void printInbox(char *email) {
    struct MailTreeNode *accountNode = findMailAccount(selectedMailServer->root, email);
    for (int i = 0; i < accountNode->account.currentInboxCount; i++) {
        struct Mail *mail = accountNode->account.inbox[i];
        printMail(mail);
    }
}