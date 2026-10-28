#ifndef APP_H
#define APP_H

#define ROTE_APP_ID "org.chskzl.rote"
#define ROTE_APP_NAME "Rote"

typedef struct _RoteApp RoteApp;

RoteApp *rote_app_new();
int rote_app_run(RoteApp *app, int argc, char **argv);

#endif
