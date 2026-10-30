#include "include/app.h"
#include <gtk/gtk.h>
#include <gtk4-layer-shell/gtk4-layer-shell.h>

struct _RoteApp {
  GtkApplication *application;
  GtkWidget *window;
  GtkWidget *scrolled;
  GtkWidget *textview;
  GFile *datafile;
};

static void
initialize_textview(RoteApp* self)
{
  GError *err;
  self->textview = gtk_text_view_new();
  self->datafile = g_file_new_for_path(g_build_filename(g_get_user_data_dir(), "rote.txt", NULL));
  GFileInputStream* stream = g_file_read(self->datafile, NULL, &err);
  gsize bytes_read = 0;
  gsize bytes_requested = 4096;
  char* buf = malloc(bytes_requested);
  // TODO: remove the limit of 4096 bytes, or provide the user with feedback
  bytes_read = g_input_stream_read(G_INPUT_STREAM(stream), buf, bytes_requested, NULL, &err);
  buf[bytes_read] = 0;
  g_input_stream_close(G_INPUT_STREAM(stream), NULL, NULL);
  GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->textview));
  gtk_text_buffer_set_text(textbuf, buf, bytes_read);
  free(buf);
}

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

static gboolean
on_close_request(GtkWindow *window, gpointer user_data)
{
  RoteApp *self = user_data;
  (void) window;
  GError *err;
  GtkTextIter start, end;
  GtkTextBuffer *textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->textview));
  gtk_text_buffer_get_bounds(textbuf, &start, &end);
  gchar *buf = gtk_text_buffer_get_text(textbuf, &start, &end, true);
  GFileOutputStream* stream = g_file_replace(self->datafile, NULL, false, G_FILE_CREATE_NONE, NULL, &err);
  gboolean success = g_output_stream_write_all(G_OUTPUT_STREAM(stream), buf, strlen(buf), NULL, NULL, &err);
  if (!success)
    g_printerr("Failed to write to output file");
  g_output_stream_close(G_OUTPUT_STREAM(stream), NULL, NULL);
  g_application_quit(G_APPLICATION(self->application));
  return TRUE;
}

static void
on_activate(GApplication *application, gpointer user_data)
{
  RoteApp *self = user_data;
  (void) application;

  self->window = gtk_application_window_new(GTK_APPLICATION(self->application));
  gtk_window_set_title(GTK_WINDOW(self->window), ROTE_APP_NAME);
  gtk_window_present(GTK_WINDOW(self->window));
  gtk_layer_init_for_window(GTK_WINDOW(self->window));
  gtk_layer_set_namespace(GTK_WINDOW(self->window), "rote");
  gtk_layer_set_respect_close(GTK_WINDOW(self->window), TRUE);
  gtk_layer_set_layer(GTK_WINDOW(self->window), GTK_LAYER_SHELL_LAYER_TOP);
  gtk_layer_set_exclusive_zone(GTK_WINDOW(self->window), 0);
  gtk_layer_set_keyboard_mode(GTK_WINDOW(self->window), GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);
  gtk_layer_set_anchor(GTK_WINDOW(self->window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(self->window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(self->window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(self->window), GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);

  self->scrolled = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(self->scrolled, TRUE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(self->scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_window_set_child(GTK_WINDOW(self->window), self->scrolled);

  initialize_textview(self);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(self->scrolled), self->textview);
  gtk_widget_grab_focus(self->textview);

  GtkEventController *key_controller = gtk_event_controller_key_new();
  g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), self);
  gtk_widget_add_controller(self->window, key_controller);

  g_signal_connect(self->window, "close-request", G_CALLBACK(on_close_request), self);
}

RoteApp *
rote_app_new()
{
  RoteApp *self = g_new(RoteApp, 1);
  self->application = gtk_application_new(ROTE_APP_ID, G_APPLICATION_DEFAULT_FLAGS);
  if (!gtk_layer_is_supported())
    g_application_quit(G_APPLICATION(self->application));
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

