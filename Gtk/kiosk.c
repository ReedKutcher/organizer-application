#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct DateTimeFmt {
    GtkWidget *dateLabel;
    GtkWidget *timeLabel;

    char *dateFmt;
    char *timeFmt;
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

gboolean everySecond(gpointer data) {
    GDateTime *dateTime;
    struct DateTimeFmt *formatData;
    
    formatData = (struct DateTimeFmt *)data;
    dateTime = g_date_time_new_now_local();

    gtk_label_set_text(GTK_LABEL(formatData->dateLabel), g_date_time_format(dateTime, formatData->dateFmt));
    gtk_label_set_text(GTK_LABEL(formatData->timeLabel), g_date_time_format(dateTime, formatData->timeFmt));

    return TRUE;
}

void freeEverySecond(gpointer data) {
    g_free(data);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkCssProvider *provider = NULL;

    GtkWidget *window = NULL;

    GdkCursor *cursor = NULL;
    GtkEventController *controller = NULL;
    
    GtkWidget *contentFrame = NULL;

    GtkWidget *bannerSeparator = NULL;
    GtkWidget *bannerFrame = NULL;
    GtkWidget *bannerBox = NULL;
    GtkWidget *menuButton = NULL;
    GtkWidget *dateLabel = NULL;
    GtkWidget *timeLabel = NULL;

    GtkWidget *menuSeparator = NULL;
    GtkWidget *menuRevealer = NULL;
    GtkWidget *menuFrame = NULL;

    struct DateTimeFmt *defaultDateTime = (struct DateTimeFmt *)malloc(sizeof(struct DateTimeFmt));


    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "gtk.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Organizer Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 1080, 1920);

    controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(onKeyPressed), GTK_WINDOW(window));
    gtk_widget_add_controller(window, controller);

    cursor = gdk_cursor_new_from_name("arrow", NULL);
    gtk_widget_set_cursor(window, cursor);


    bannerSeparator = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), bannerSeparator);

    bannerFrame = gtk_frame_new(NULL);
    gtk_box_append(GTK_BOX(bannerSeparator), bannerFrame);
    gtk_widget_add_css_class(bannerFrame, "banner");
    gtk_widget_set_hexpand(bannerFrame, TRUE);

    bannerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_frame_set_child(GTK_FRAME(bannerFrame), bannerBox);


    menuSeparator = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(bannerSeparator), menuSeparator);

    menuFrame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(menuFrame, "menu");
    gtk_widget_set_vexpand(menuFrame, TRUE);

    menuRevealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(menuRevealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_LEFT);
    gtk_revealer_set_child(GTK_REVEALER(menuRevealer), menuFrame);
    gtk_box_append(GTK_BOX(menuSeparator), menuRevealer);

    contentFrame = gtk_frame_new(NULL);
    gtk_widget_set_hexpand(contentFrame, TRUE);
    gtk_widget_set_vexpand(contentFrame, TRUE);
    gtk_box_append(GTK_BOX(menuSeparator), contentFrame);
    gtk_widget_add_css_class(contentFrame, "content");


    menuButton = gtk_button_new_from_icon_name("open-menu-symbolic");
    gtk_widget_set_halign(menuButton, GTK_ALIGN_START);
    gtk_widget_set_valign(menuButton, GTK_ALIGN_START);
    gtk_widget_add_css_class(menuButton, "menu-button");
    g_signal_connect(menuButton, "clicked", G_CALLBACK(printHello), GTK_REVEALER(menuRevealer));
    gtk_box_append(GTK_BOX(bannerBox), menuButton);

    dateLabel = gtk_label_new("Date Loading...");
    gtk_widget_set_hexpand(dateLabel, TRUE);
    gtk_widget_add_css_class(dateLabel, "date");

    timeLabel = gtk_label_new("Time Loading...");
    gtk_widget_add_css_class(timeLabel, "time");

    defaultDateTime->dateLabel = dateLabel;
    defaultDateTime->timeLabel = timeLabel;
    defaultDateTime->dateFmt = "%A, %B %d, %Y";
    defaultDateTime->timeFmt = "%I:%M %p";
    g_timeout_add_full(G_PRIORITY_DEFAULT, 1000, G_SOURCE_FUNC(everySecond), defaultDateTime, freeEverySecond);

    gtk_box_append(GTK_BOX(bannerBox), dateLabel);
    gtk_box_append(GTK_BOX(bannerBox), timeLabel);

    
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