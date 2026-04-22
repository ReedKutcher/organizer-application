#include <gtk/gtk.h>

gboolean on_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer userData) {
    if (keyval == GDK_KEY_F4) {
        gtk_window_close(userData);
        return 1;
    }
    return 0;
}

static void printHello(GtkWidget *widget, gpointer data) {
    gtk_revealer_set_reveal_child(data, gtk_revealer_get_child_revealed(data) ? FALSE : TRUE);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *menuButton;
    GtkWidget *bannerSeparator;
    GtkWidget *menuSeparator;
    GtkWidget *bannerFrame;
    GtkWidget *menuFrame;
    GtkWidget *menuRevealer;

    GtkCssProvider *provider;

    GdkCursor *cursor;
    GtkEventController *controller;

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "styles.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Organizer Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);

    controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_pressed), GTK_WINDOW(window));
    gtk_widget_add_controller(window, controller);

    cursor = gdk_cursor_new_from_name("arrow", NULL);
    gtk_widget_set_cursor(window, cursor);

    bannerSeparator = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), bannerSeparator);

    bannerFrame = gtk_frame_new(NULL);
    gtk_box_append(GTK_BOX(bannerSeparator), bannerFrame);
    gtk_widget_add_css_class(bannerFrame, "banner");
    gtk_widget_set_hexpand(bannerFrame, TRUE);

    menuSeparator = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(bannerSeparator), menuSeparator);

    menuFrame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(menuFrame, "menu");
    gtk_widget_set_vexpand(menuFrame, TRUE);

    menuRevealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(menuRevealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_LEFT);
    gtk_revealer_set_child(GTK_REVEALER(menuRevealer), menuFrame);
    gtk_box_append(GTK_BOX(menuSeparator), menuRevealer);

    menuButton = gtk_button_new_from_icon_name("open-menu-symbolic");
    gtk_widget_set_halign(menuButton, GTK_ALIGN_START);
    gtk_widget_set_valign(menuButton, GTK_ALIGN_START);
    g_signal_connect(menuButton, "clicked", G_CALLBACK(printHello), GTK_REVEALER(menuRevealer));
    gtk_frame_set_child(GTK_FRAME(bannerFrame), menuButton);

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