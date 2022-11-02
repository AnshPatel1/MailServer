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
struct MailServer createAndInitializeMailServer(char *domain);
struct MailAccount createMailAccount(char *name, char *email);
struct MailGroup createMailGroup(char *name);

//