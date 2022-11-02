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

struct MailGroup *createMailGroup(char *name);

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


// Global Variables
struct MailServer *mailServerList[MAX_SERVERS];
int currentMailServerCount = 0;

struct MailGroup *mailGroupList[MAX_MAIL_GROUPS_PER_SERVER];
int currentMailGroupCount = 0;


// Main Function
int main() {
    mainMenu();
    return 0;
}

// FUNCTION DEFINITIONS
struct MailServer *createMailServer(char *domain) {
    struct MailServer *mailServer = (struct MailServer *) malloc(sizeof(struct MailServer));
    mailServer->domain = domain;
    mailServer->root = createMailTreeNode(NULL, NULL, 0);
    mailServerList[currentMailServerCount++] = mailServer;
    return mailServer;
}

struct MailGroup *createMailGroup(char *name) {
    struct MailGroup *mailGroup = (struct MailGroup *) malloc(sizeof(struct MailGroup));
    mailGroup->name = name;
    mailGroup->root = createMailTreeNode(NULL, NULL, 0);
    mailGroupList[currentMailGroupCount++] = mailGroup;
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
    for (int i = 0; i < currentMailGroupCount; i++) {
        if (strcmp(mailGroupList[i]->name, name) == 0) {
            return *mailGroupList[i];
        }
    }
}

void addMailAccountToMailTreeNode(struct MailTreeNode *mailTreeNode, struct MailAccount *mailAccount) {
    mailTreeNode->children[mailTreeNode->currentMemberCount++] = createMailTreeNode(mailAccount, NULL, 0);
}

void addMailAccountToMailServer(struct MailServer *mailServer, struct MailAccount *mailAccount) {
    addMailAccountToMailTreeNode(mailServer->root, mailAccount);
}

