#ifndef STYLE_H
#define STYLE_H

#include <gtk/gtk.h>
#include <sys/stat.h>

#define ROTE_DEFAULT_CSS \
"window {" \
"  background: transparent;" \
"}" \
".panel {" \
"  background-color: @window_bg_color;" \
"  border-radius: 12px;" \
"  padding: 12px;" \
"  margin: 8px;" \
"}"

void initialize_css();

#endif

