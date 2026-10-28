#include "include/app.h"

int
main(int argc, char **argv)
{
  RoteApp *app = rote_app_new();
  return rote_app_run(app, argc, argv);
}
