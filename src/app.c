#include "include/app.h"
#include <gtk/gtk.h>

struct _RoteApp {
  GtkApplication *application;
  GtkWidget *window;
  GtkWidget *textview;
  GFile *datafile;
};

static gboolean
on_key_pressed(GtkEventControllerKey *controller,
               guint keyval,
               guint keycode,
               GdkModifierType state,
               gpointer user_data)
{
  RoteApp *self = user_data;
  (void) controller;
  (void) keycode;
  (void) state;

  if (keyval == GDK_KEY_Escape) {
    gtk_window_close(GTK_WINDOW(self->window));
    return TRUE;
  }
  return FALSE;
}

static void
on_activate(GApplication *application, gpointer user_data)
{
  RoteApp *self = user_data;
  (void) application;
  self->window = gtk_application_window_new(GTK_APPLICATION(self->application));
  gtk_window_set_title(GTK_WINDOW(self->window), ROTE_APP_NAME);
  gtk_window_present(GTK_WINDOW(self->window));

  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), self);
  gtk_widget_add_controller(self->window, key_controller);
}

RoteApp *
rote_app_new()
{
  RoteApp *self = g_new(RoteApp, 1);
  self->application = gtk_application_new(ROTE_APP_ID, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(self->application, "activate", G_CALLBACK(on_activate), self);
  return self;
}

int
rote_app_run(RoteApp *self, int argc, char **argv)
{
  int status;
  status = g_application_run(G_APPLICATION(self->application), argc, argv);
  g_object_unref(self->application);
  g_free(self);
  return status;
}

