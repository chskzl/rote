#include "style.h"

static void
on_css_parsing_error(GtkCssProvider *provider, GtkCssSection *section, const GError *error, gpointer user_data)
{
    (void) provider;
    (void) section;
    (void) user_data;
    g_printerr("Error parsing CSS: %s\n", error->message);
}

void initialize_css()
{
  GtkCssProvider *provider = NULL;
  gulong parsing_error_handler;
  GdkDisplay *display = gdk_display_get_default();
  char *user_css_path = g_build_filename(g_get_user_config_dir(), "rote", "rote.css", NULL);
  struct stat st;

  if (display) {
    provider = gtk_css_provider_new();
    parsing_error_handler = g_signal_connect(provider, "parsing-error", G_CALLBACK(on_css_parsing_error), &provider);
    gtk_css_provider_load_from_string( provider, ROTE_DEFAULT_CSS);
    
    if (stat(user_css_path, &st) == 0) {
      if (S_ISREG(st.st_mode)) {
        if (access(user_css_path, R_OK) == 0) {
          gtk_css_provider_load_from_path(provider, user_css_path);
        }
      }
    }

    g_signal_handler_disconnect(provider, parsing_error_handler);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  }
}

