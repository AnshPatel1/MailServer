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
    struct MailGroup **mailGroups;
    int currentMailAccountsCount;
    int currentMailGroupsCount;
};

struct MailGroup {
    char *name;
    struct MailTreeNode *root;
};

struct Mail {
    char *from;
    char *to;
    char *subject;
    char *body;
};

struct Account {
    char *name;
    char *roll_no;
    char *password;
    struct MailServer *server;
};

struct MailAccount {
    struct Account account;
    char *email;
    struct Mail **inbox;
    int currentMailCount;
};

struct MailTreeNode {
    struct MailAccount *mailAccount;
    struct MailTreeNode **children;
    char *mailGroupName;
    int isMailGroup;
    struct MailGroup *mailGroup;
    int currentMemberCount;
};

// FUNCTION SIGNATURES
struct MailServer *createMailServer(char *domain);

struct MailGroup *createMailGroup(char *name, struct MailServer server);

struct Mail *createMail(char *from, char *to, char *subject, char *body);

struct Account *createAccount(char *name, char *roll_no, char *password, struct MailServer *server);

struct MailAccount *createMailAccount(struct Account *account, char *email);

struct MailTreeNode *createMailTreeNode(struct MailAccount *mailAccount, char *mailGroupName, int isMailGroup);

void addMailAccountToMailGroup(char *mailGroup, struct MailAccount *mailAccount);

struct MailGroup findMailGroupByName(struct MailServer *mailServer, char *name);

void addMailAccountToMailServer(struct MailServer *mailServer, struct MailAccount *mailAccount);

void addMailAccountToMailTreeNode(struct MailTreeNode *mailTreeNode, struct MailAccount *mailAccount);

void addMailGroupToMailServer(struct MailServer *mailServer, struct MailGroup *mailGroup);

void addMailGroupToMailGroup(struct MailGroup *parentMailGroup, struct MailGroup *mailGroupToAdd);


// Menu Function Signatures
void mainMenu();

void mailServerMenu();

void loggedInMenu();


// Helper Function Signatures for Menu Functions
void printMailServer(struct MailServer *mailServer);

void printMailGroup(struct MailGroup *mailGroup);

void printMailTreeNode(struct MailTreeNode *mailTreeNode);

void printMailAccount(struct MailAccount *mailAccount);

void printInbox(struct MailAccount *mailAccount);

void printMail(struct Mail *mail);

void printAccount(struct Account *account);

void printMailServerList();

void printMailGroupList();

void printMailAccountList();

void printMailTreeNodeList();

void printMailList();

void isEmailGroupOrAccount(char *email);

struct MailAccount *findMailAccountByEmail(struct MailServer *mailServer, char *email);


// Global Variables
struct MailServer *mailServerList[MAX_SERVERS];
int currentMailServerCount = 0;

struct MailGroup *mailGroupList[MAX_MAIL_GROUPS_PER_SERVER];
int currentMailGroupCount = 0;

struct MailServer *currentMailServer;


// Main Function
int main() {
    mainMenu();
    return 0;
}

// FUNCTION DEFINITIONS
struct MailServer *createMailServer(char *domain) {
    struct MailServer *mailServer = (struct MailServer *) malloc(sizeof(struct MailServer));
    mailServer->domain = domain;
    mailServer->root = createMailTreeNode(NULL, "ALL", 1);
    mailServer->mailAccounts = (struct MailAccount **) malloc(
            sizeof(struct MailAccount *) * MAX_MAIL_LIST_MEMBERS * 100);
    mailServer->currentMailAccountsCount = 0;
    mailServer->mailGroups = (struct MailGroup **) malloc(sizeof(struct MailGroup *) * MAX_MAIL_GROUPS_PER_SERVER);
    mailServer->currentMailGroupsCount = 0;
    mailServerList[currentMailServerCount++] = mailServer;
    return mailServer;
}

struct MailGroup *createMailGroup(char *name, struct MailServer server) {
    struct MailGroup *mailGroup = (struct MailGroup *) malloc(sizeof(struct MailGroup));
    mailGroup->name = name;
    mailGroup->root = createMailTreeNode(NULL, NULL, 0);
    mailGroupList[currentMailGroupCount++] = mailGroup;
    addMailGroupToMailServer(&server, mailGroup);
    return mailGroup;
}

struct Mail *createMail(char *from, char *to, char *subject, char *body) {
    struct Mail *mail = (struct Mail *) malloc(sizeof(struct Mail));
    mail->from = from;
    mail->to = to;
    mail->subject = subject;
    mail->body = body;
    return mail;
}

struct Account *createAccount(char *name, char *roll_no, char *password, struct MailServer *server) {
    struct Account *account = (struct Account *) malloc(sizeof(struct Account));
    account->name = name;
    account->roll_no = roll_no;
    account->password = password;
    account->server = server;
    return account;
}

struct MailAccount *createMailAccount(struct Account *account, char *email) {
    struct MailAccount *mailAccount = (struct MailAccount *) malloc(sizeof(struct MailAccount));
    mailAccount->account = *account;
    mailAccount->email = email;
    mailAccount->inbox = (struct Mail **) malloc(sizeof(struct Mail *) * MAX_INBOX_CAPACITY);
    mailAccount->currentMailCount = 0;
    return mailAccount;
}

struct MailTreeNode *createMailTreeNode(struct MailAccount *mailAccount, char *mailGroupName, int isMailGroup) {
    struct MailTreeNode *mailTreeNode = (struct MailTreeNode *) malloc(sizeof(struct MailTreeNode));
    mailTreeNode->mailAccount = mailAccount;
    mailTreeNode->mailGroupName = mailGroupName;
    mailTreeNode->isMailGroup = isMailGroup;
    mailTreeNode->mailGroup = (struct MailGroup *) malloc(sizeof(struct MailGroup));
    mailTreeNode->currentMemberCount = 0;
    mailTreeNode->children = (struct MailTreeNode **) malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
    return mailTreeNode;
}

void addMailAccountToMailGroup(char *mailGroup, struct MailAccount *mailAccount) {
    struct MailGroup mailGroupToAdd = findMailGroupByName(mailAccount->account.server, mailGroup);
    if (mailGroupToAdd.root == NULL) {
        printf("Mail Group not found\n");
        return;
    } else if (mailGroupToAdd.root->currentMemberCount == MAX_MAIL_LIST_MEMBERS) {
        printf("Mail Group is full\n");
        return;
    } else {
        addMailAccountToMailTreeNode(mailGroupToAdd.root, mailAccount);
    }
}

// TODO: Add this to helper functions
struct MailGroup findMailGroupByName(struct MailServer *mailServer, char *name) {
    for (int i = 0; i < mailServer->currentMailGroupsCount; i++) {
        if (strcmp(mailServer->mailGroups[i]->name, name) == 0) {
            return *mailServer->mailGroups[i];
        }
    }
}

void addMailAccountToMailServer(struct MailServer *mailServer, struct MailAccount *mailAccount) {
    if (mailServer->root->currentMemberCount == MAX_MAIL_LIST_MEMBERS) {
        printf("Mail Server is full\n");
        return;
    } else {
        addMailAccountToMailTreeNode(mailServer->root, mailAccount);
    }
}

void addMailAccountToMailTreeNode(struct MailTreeNode *mailTreeNode, struct MailAccount *mailAccount) {
    mailTreeNode->children[mailTreeNode->currentMemberCount++] = createMailTreeNode(mailAccount, NULL, 0);
}

void addMailGroupToMailServer(struct MailServer *mailServer, struct MailGroup *mailGroup) {
    if (mailServer->root->currentMemberCount == MAX_MAIL_LIST_MEMBERS) {
        printf("Mail Server is full\n");
        return;
    } else {
        mailServer->root->children[mailServer->root->currentMemberCount++] = createMailTreeNode(NULL, mailGroup->name,
                                                                                                1);
    }
}

void addMailGroupToMailGroup(struct MailGroup *parentMailGroup, struct MailGroup *mailGroupToAdd) {
    parentMailGroup->root->children[parentMailGroup->root->currentMemberCount++] = createMailTreeNode(NULL,
                                                                                                      mailGroupToAdd->name,
                                                                                                      1);
}


// TODO: Add this to helper functions
void printMailServerList() {
    for (int i = 0; i < currentMailServerCount; i++) {
        printf("Mail Server %d: %s\n", i + 1, mailServerList[i]->domain);
    }
}

struct MailServer *findMailServerByDomain(char *domain) {
    for (int i = 0; i < currentMailServerCount; i++) {
        if (strcmp(mailServerList[i]->domain, domain) == 0) {
            return (mailServerList[i]);
        }
    }
    printf("Mail Server not found\n");
}


void sendMail(struct Mail *mail) {
    struct MailAccount *mailAccount = findMailAccountByEmail(currentMailServer, mail->to);
    if (mailAccount == NULL) {
        printf("Mail Account not found\n");
        return;
    } else {
        mailAccount->inbox[mailAccount->currentMailCount++] = mail;
    }
}

// MENU FUNCTIONS DEFINITIONS

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
                createMailServer(domain);
            }
                break;
            case 2: {
                printMailServerList();

                char domain_choice[100];
                printf("Enter your choice: ");
                scanf("%s", domain_choice);
                currentMailServer = findMailServerByDomain(domain_choice);
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

void mailServerMenu() {
    int isExit = 0;
    do {
        int choice;
        printf("1. Create Mail Account\n");
        printf("2. Create Mail Group\n");
        printf("3. Add Mail Account to Mail Group\n");
        printf("4. Add Mail Group to Mail Group\n");
        printf("5. Add Mail Account to Mail Server\n");
        printf("6. Add Mail Group to Mail Server\n");
        printf("7. Send Mail\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                char name[100], roll_no[100], password[100], email[100];
                printf("Enter name: ");
                scanf("%s", name);
                printf("Enter roll no: ");
                scanf("%s", roll_no);
                printf("Enter password: ");
                scanf("%s", password);
                printf("Enter email: ");
                scanf("%s", email);
                struct Account *account = createAccount(name, roll_no, password, currentMailServer);
                struct MailAccount *mailAccount = createMailAccount(account, email);
                currentMailServer->mailAccounts[currentMailServer->currentMailAccountsCount++] = mailAccount;
            }
                break;
            case 2: {
                char name[100];
                printf("Enter name: ");
                scanf("%s", name);
                mailGroupList[currentMailGroupCount++] = createMailGroup(name, *currentMailServer);
            }
                break;
            case 3: {
                char mailGroup[100], email[100];
                printf("Enter mail group name: ");
                scanf("%s", mailGroup);
                printf("Enter email: ");
                scanf("%s", email);
                struct MailAccount *mailAccount = findMailAccountByEmail(currentMailServer, email);
                addMailAccountToMailGroup(mailGroup, mailAccount);
            }
                break;
            case 4: {
                char parentMailGroup[100], mailGroup[100];
                printf("Enter parent mail group name: ");
                scanf("%s", parentMailGroup);
                printf("Enter mail group name: ");
                scanf("%s", mailGroup);
                struct MailGroup parentMailGroupToAdd = findMailGroupByName(currentMailServer, parentMailGroup);
                struct MailGroup mailGroupToAdd = findMailGroupByName(currentMailServer, mailGroup);
                parentMailGroupToAdd.root->children[parentMailGroupToAdd.root->currentMemberCount++] = createMailTreeNode(
                        NULL,
                        mailGroupToAdd.name,
                        1);
            }
                break;
            case 5: {
                char email[100];
                printf("Enter email: ");
                scanf("%s", email);
                struct MailAccount *mailAccount = findMailAccountByEmail(currentMailServer, email);
                addMailAccountToMailServer(currentMailServer, mailAccount);
            }
                break;
            case 6: {
                char mailGroup[100];
                printf("Enter mail group name: ");
                scanf("%s", mailGroup);
                struct MailGroup mailGroupToAdd = findMailGroupByName(currentMailServer, mailGroup);
                addMailGroupToMailServer(currentMailServer, &mailGroupToAdd);
            }
                break;
            case 7: {
                char from[100], to[100], subject[100], body[100];
                printf("Enter from: ");
                scanf("%s", from);
                printf("Enter to: ");
                scanf("%s", to);
                printf("Enter subject: ");
                scanf("%s", subject);
                printf("Enter body: ");
                scanf("%s", body);
                struct MailAccount *fromMailAccount = findMailAccountByEmail(currentMailServer, from);
                struct MailAccount *toMailAccount = findMailAccountByEmail(currentMailServer, to);
                struct Mail *mail = createMail(fromMailAccount, toMailAccount, subject, body);
                sendMail(mail);
            }
                break;
            case 8: {
                isExit = 1;
            }
                break;
            default:
                printf("Invalid Choice\n");
                break;
        }
    } while (!isExit);
}

struct MailAccount *findMailAccountByEmail(struct MailServer *mailServer, char *email) {
    for (int i = 0; i < mailServer->currentMailAccountsCount; i++) {
        if (strcmp(mailServer->mailAccounts[i]->email, email) == 0) {
            return mailServer->mailAccounts[i];
        }
    }
}