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
    int currentMemberCount;
};


// FUNCTION DECLARATIONS
struct MailServer *createNewServer(char *domain) {
    struct MailTreeNode mailTreeNode;
    struct MailServer *mailServer = (struct MailServer *) malloc(sizeof(struct MailServer));
    mailServer->domain = domain;
    mailTreeNode.mailAccount = NULL;
    mailTreeNode.mailGroupName = domain;
    mailTreeNode.isMailGroup = 1;
    mailTreeNode.currentMemberCount = 0;
    mailTreeNode.children = (struct MailTreeNode **) malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
    mailServer->root = &mailTreeNode;
    return mailServer;
}

struct MailGroup *createNewMailGroup(char *name, struct MailTreeNode *parent) {
    struct MailTreeNode mailTreeNode;
    struct MailGroup *mailGroup = (struct MailGroup *) malloc(sizeof(struct MailGroup));
    mailGroup->name = name;
    mailTreeNode.mailAccount = NULL;
    mailTreeNode.mailGroupName = name;
    mailTreeNode.isMailGroup = 1;
    mailTreeNode.currentMemberCount = 0;
    mailTreeNode.children = (struct MailTreeNode **) malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
    mailGroup->root = &mailTreeNode;
    return mailGroup;
}

//struct MailGroup *createNewMailGroup(char *mailGroupName, struct MailServer *server_root) {
//    printf("Creating new mail group %s", mailGroupName);
//    struct MailGroup *mailGroup = (struct MailGroup *) malloc(sizeof(struct MailGroup));
//    mailGroup->name = mailGroupName;
//    struct MailTreeNode mailTreeNode;
//    mailTreeNode.mailAccount = NULL;
//    mailTreeNode.mailGroupName = mailGroupName;
//    mailTreeNode.isMailGroup = 1;
//    mailTreeNode.currentMemberCount = 0;
//    mailTreeNode.children = (struct MailTreeNode **) malloc(sizeof(struct MailTreeNode *) * MAX_MAIL_LIST_MEMBERS);
//    server_root->root->children[server_root->root->currentMemberCount] = &mailTreeNode;
//    server_root->root->currentMemberCount++;
//    mailGroup->root = &mailTreeNode;
//    return mailGroup;
//}

void createNewMailAccount(char *name, char *roll_no, char *username, char *password, struct MailServer server,
                          struct MailTreeNode *parent) {
    struct MailTreeNode mailTreeNode;
    struct MailAccount *mailAccount = (struct MailAccount *) malloc(sizeof(struct MailAccount));
    mailAccount->account.name = name;
    mailAccount->account.roll_no = roll_no;
    mailAccount->account.password = password;
    mailAccount->account.server = &server;
    strcat(username, "@");
    strcat(username, server.domain);
    mailAccount->email = username;
    mailAccount->currentMailCount = 0;
    mailAccount->inbox = (struct Mail **) malloc(sizeof(struct Mail *) * MAX_INBOX_CAPACITY);
    mailTreeNode.mailAccount = mailAccount;
    mailTreeNode.mailGroupName = NULL;
    mailTreeNode.isMailGroup = 0;
    mailTreeNode.currentMemberCount = 0;
    mailTreeNode.children = NULL;
    parent->children[parent->currentMemberCount] = &mailTreeNode;
    parent->currentMemberCount++;
    printf("Account Details: \n");
    printf("Name: %s\n", mailAccount->account.name);
    printf("Roll No: %s\n", mailAccount->account.roll_no);
    printf("Email: %s\n", mailAccount->email);
    printf("Password: %s\n", mailAccount->account.password);
    printf("Server: @%s\n", mailAccount->account.server->domain);
}

char *splitEmailGetUsername(char *email) {
    int i = 0;
    while (email[i] != '@') i++;
    char *username = (char *) malloc(sizeof(char) * i);
    i = 0;
    while (email[i] != '@') {
        username[i] = email[i];
        i++;
    }
    return username;
}

void sendMail(char *from, char *to, char *subject, char *body, struct MailTreeNode *mailTreeNode) {
    if (mailTreeNode->isMailGroup) {
        for (int i = 0; i < mailTreeNode->currentMemberCount; i++) {
            sendMail(from, to, subject, body, mailTreeNode->children[i]);
        }
    } else {
        struct Mail *mail = (struct Mail *) malloc(sizeof(struct Mail));
        mail->from = from;
        mail->to = to;
        mail->subject = subject;
        mail->body = body;
        mailTreeNode->mailAccount->inbox[mailTreeNode->mailAccount->currentMailCount] = mail;
        mailTreeNode->mailAccount->currentMailCount++;
    }
}

struct MailAccount findMailAccount(char *email, struct MailTreeNode *root) {
    if (root->isMailGroup) {
        for (int i = 0; i < root->currentMemberCount; i++) {
            struct MailAccount mailAccount = findMailAccount(email, root->children[i]);
            if (mailAccount.account.name != NULL) {
                return mailAccount;
            }
        }
    } else {
        if (strcmp(root->mailAccount->email, email) == 0) {
            return *root->mailAccount;
        }
    }
    struct MailAccount mailAccount;
    mailAccount.account.name = NULL;
    return mailAccount;
}

struct MailAccount login(char *email, char *password, struct MailTreeNode *root) {
    struct MailAccount mailAccount = findMailAccount(email, root);
    if (mailAccount.account.name != NULL) {
        if (strcmp(mailAccount.account.password, password) == 0) {
            return mailAccount;
        }
    }
    mailAccount.account.name = NULL;
    return mailAccount;
}

struct MailTreeNode *findMailGroupByName(char *mailGroupName, struct MailTreeNode *root) {
    if (root->isMailGroup) {
        if (strcmp(root->mailGroupName, mailGroupName) == 0) {
            return root;
        }
        for (int i = 0; i < root->currentMemberCount; i++) {
            struct MailTreeNode *mailTreeNode = findMailGroupByName(mailGroupName, root->children[i]);
            if (mailTreeNode != NULL) {
                return mailTreeNode;
            }
        }
    }
    return NULL;
}

int menuL1() {
    int choice;
    printf("1. Create New Mail Server\n");
    printf("2. Select Mail Server\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int menuL2() {
    int choice;
    printf("1. Create New Mail Account\n");
    printf("2. Create New Mailing Group\n");
    printf("3. Login\n");
    printf("4. Return to previous menu\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int menuL3() {
    int choice;
    printf("1. Send Mail\n");
    printf("2. View Inbox\n");
    printf("3. Return to previous menu\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// GLOBAL VARIABLES
struct MailServer servers[MAX_SERVERS];
struct MailGroup **mailGroups;
int currentMailGroupCount = 0;
int currentServerCount = 0;
struct MailServer selectedServer;

int main() {
    mailGroups = (struct MailGroup **) malloc(sizeof(struct MailGroup *));
    int choiceL1, choiceL2;
    while (1) {
        choiceL1 = menuL1();
        switch (choiceL1) {
            case 1: {
                char domain[100];
                printf("Enter the domain name: ");
                scanf("%s", domain);
                servers[currentServerCount] = *createNewServer(domain);
                currentServerCount++;
                break;
            }
            case 2: {
                char domain[100];
                printf("Enter the domain name: ");
                scanf("%s", domain);
                for (int i = 0; i < currentServerCount; i++) {
                    if (strcmp(servers[i].domain, domain) == 0) {
                        selectedServer = servers[i];
                        while (1) {
                            choiceL2 = menuL2();
                            if (choiceL2 == 1) {
                                char name1[100], roll_no[100], username[100], password[100], group[100];
                                struct MailTreeNode *parent;
                                printf("Enter your name1: ");
                                scanf("%s", name1);
                                printf("Enter your roll no: ");
                                scanf("%s", roll_no);
                                printf("Enter your username: ");
                                scanf("%s", username);
                                printf("Enter your password: ");
                                scanf("%s", password);
                                printf("Enter the name1 of the group you want to add this account to: ");
                                scanf("%s", group);
//                                parent = findMailGroupByName(group, selectedServer.root);
//                                printf("Creating new mail account...\n");
                                createNewMailAccount(name1, roll_no, username, password, selectedServer,
                                                     selectedServer.root);
                                break;
                            } else if (choiceL2 == 2) {
                                char name[100];
                                printf("Enter the name1 of the group: ");
                                scanf("%s", name);
                                struct MailGroup *m = createNewMailGroup(name, selectedServer.root);
                                mailGroups[currentMailGroupCount] = m;
                                currentMailGroupCount++;
                                printf("Mail group created successfully!\n");
                            } else if (choiceL2 == 3) {
                                char email[100], password[100];
                                printf("Enter your email: ");
                                scanf("%s", email);
                                printf("Enter your password: ");
                                scanf("%s", password);
                                struct MailAccount loggedInAccount = login(email, password, selectedServer.root);
                                if (loggedInAccount.account.name != NULL) {
                                    printf("Login Successful\n");
                                    printf("Welcome %s\n", loggedInAccount.account.name);
                                    int choiceL3 = menuL3();
                                    while (1) {
                                        switch (choiceL3) {
                                            case 1: {
                                                char to[100], subject[100], body[100];
                                                printf("Enter the recipient: ");
                                                scanf("%s", to);
                                                printf("Enter the subject: ");
                                                scanf("%s", subject);
                                                printf("Enter the body: ");
                                                scanf("%s", body);
                                                sendMail(loggedInAccount.email, to, subject, body, selectedServer.root);
                                                break;
                                            }
                                            case 2: {
                                                for (int i = 0; i < loggedInAccount.currentMailCount; i++) {
                                                    printf("%d.\n   From: %s\n", i, loggedInAccount.inbox[i]->from);
                                                    printf("    To: %s\n", loggedInAccount.inbox[i]->to);
                                                    printf("    Subject: %s\n", loggedInAccount.inbox[i]->subject);
                                                    printf("    Body: %s\n\n\n\n", loggedInAccount.inbox[i]->body);
                                                }
                                                break;
                                            }
                                            case 3: {
                                                break;
                                            }
                                            case 4: {
                                                exit(0);
                                                break;
                                            }
                                            default: {
                                                printf("Invalid choice!\n");
                                                break;
                                            }
                                        }
                                    }
                                }
                            } else if (choiceL2 == 4) {
                                break;
                            } else if (choiceL2 == 5) {
                                exit(0);
                            } else {
                                printf("Invalid choice!\n");
                                break;
                            }
                        }
                    }
                }
            }
        }
//        default: {
//            printf("Invalid choice!\n");
//            break;
//        }
    }


}