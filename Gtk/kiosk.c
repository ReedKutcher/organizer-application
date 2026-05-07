#include <gtk/gtk.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

struct TimeStructure {
    char *dateFmt;
    char *timeFmt;

    GtkWidget *dateLabel;
    GtkWidget *timeLabel;
};

gboolean onKeyPressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer userData) {
    if (keyval == GDK_KEY_F4) {
        gtk_window_close(userData);
        return 1;
    }
    return 0;
}

static void printHello(GtkWidget *widget, gpointer data) {
    gtk_revealer_set_reveal_child(data, gtk_revealer_get_child_revealed(data) ? FALSE : TRUE);
}

gboolean everySecond(gpointer userData) {
    struct TimeStructure * ts = (struct TimeStructure *)userData;
    GDateTime *dt = g_date_time_new_now_local();

    gtk_label_set_text(GTK_LABEL(ts->dateLabel), g_date_time_format(dt, ts->dateFmt));
    gtk_label_set_text(GTK_LABEL(ts->timeLabel), g_date_time_format(dt, ts->timeFmt));
}

GtkWidget *fdev_menu_item_new_from_name(const char *icon_name, const char *label_text) {
    GtkWidget *button = gtk_button_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *icon = gtk_image_new_from_icon_name(icon_name);
    GtkWidget *label = gtk_label_new(label_text);

    gtk_box_append(GTK_BOX(box), icon);
    gtk_box_append(GTK_BOX(box), label);

    gtk_button_set_child(GTK_BUTTON(button), box);

    gtk_widget_add_css_class(button, "menu-item");

    return button;
}

GtkWidget *fdev_menu_item_new_from_file(const char *file_path, const char *label_text) {
    GtkWidget *button = gtk_button_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *icon = gtk_image_new_from_file(file_path);
    GtkWidget *label = gtk_label_new(label_text);

    gtk_box_append(GTK_BOX(box), icon);
    gtk_box_append(GTK_BOX(box), label);

    gtk_button_set_child(GTK_BUTTON(button), box);

    gtk_widget_add_css_class(button, "menu-item");

    return button;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "gtk.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Organizer Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 1080, 1920);

    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(onKeyPressed), GTK_WINDOW(window));
    gtk_widget_add_controller(window, controller);

    GdkCursor *cursor = gdk_cursor_new_from_name("arrow", NULL);
    gtk_widget_set_cursor(window, cursor);


    GtkWidget *bannerSeparator = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), bannerSeparator);

    GtkWidget *bannerFrame = gtk_frame_new(NULL);
    gtk_box_append(GTK_BOX(bannerSeparator), bannerFrame);
    gtk_widget_add_css_class(bannerFrame, "banner");
    gtk_widget_set_hexpand(bannerFrame, TRUE);

    GtkWidget *bannerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_frame_set_child(GTK_FRAME(bannerFrame), bannerBox);


    GtkWidget *menuSeparator = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(bannerSeparator), menuSeparator);

    GtkWidget *menuFrame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(menuFrame, "menu");
    gtk_widget_set_vexpand(menuFrame, TRUE);

    GtkWidget *menuRevealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(menuRevealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_LEFT);
    gtk_revealer_set_child(GTK_REVEALER(menuRevealer), menuFrame);
    gtk_box_append(GTK_BOX(menuSeparator), menuRevealer);

    GtkWidget *contentFrame = gtk_frame_new(NULL);
    gtk_widget_set_hexpand(contentFrame, TRUE);
    gtk_widget_set_vexpand(contentFrame, TRUE);
    gtk_box_append(GTK_BOX(menuSeparator), contentFrame);
    gtk_widget_add_css_class(contentFrame, "content");

    GtkWidget *menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_frame_set_child(GTK_FRAME(menuFrame), menuBox);


    GtkWidget *menuButton = gtk_button_new_from_icon_name("open-menu-symbolic");
    gtk_widget_set_halign(menuButton, GTK_ALIGN_START);
    gtk_widget_set_valign(menuButton, GTK_ALIGN_START);
    gtk_widget_add_css_class(menuButton, "menu-button");
    g_signal_connect(menuButton, "clicked", G_CALLBACK(printHello), GTK_REVEALER(menuRevealer));
    gtk_box_append(GTK_BOX(bannerBox), menuButton);

    struct TimeStructure *infoDateTime = (struct TimeStructure *)malloc(sizeof(struct TimeStructure));
    infoDateTime->dateFmt = "%A, %B %d, %Y";
    infoDateTime->timeFmt = "%l:%M %P";

    infoDateTime->dateLabel = gtk_label_new("Getting Date...");
    infoDateTime->timeLabel = gtk_label_new("Getting Time...");

    gtk_widget_add_css_class(infoDateTime->dateLabel, "date");
    gtk_widget_add_css_class(infoDateTime->timeLabel, "time");

    gtk_widget_set_hexpand(infoDateTime->dateLabel, TRUE);

    g_timeout_add(1000, G_SOURCE_FUNC(everySecond), infoDateTime);

    gtk_box_append(GTK_BOX(bannerBox), infoDateTime->dateLabel);
    gtk_box_append(GTK_BOX(bannerBox), infoDateTime->timeLabel);


    GtkWidget *calendarButton = fdev_menu_item_new_from_file("icons/calendar.svg", "Calendar");
    GtkWidget *tasksButton = fdev_menu_item_new_from_file("icons/tasks.svg", "Tasks");
    GtkWidget *settingsButton = fdev_menu_item_new_from_file("icons/settings.svg", "Settings");
    GtkWidget *connectButton = fdev_menu_item_new_from_file("icons/connect.svg", "Connect");

    gtk_box_append(GTK_BOX(menuBox), calendarButton);
    gtk_box_append(GTK_BOX(menuBox), tasksButton);
    gtk_box_append(GTK_BOX(menuBox), settingsButton);
    gtk_box_append(GTK_BOX(menuBox), connectButton);
    
    
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}