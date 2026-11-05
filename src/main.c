#include "include/app.h"

int
main(int argc, char **argv)
{
  RoteApp *app = rote_app_new(argc, argv);
  return rote_app_run(app, argc, argv);
}
