#include <gtk/gtk.h>


static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *main_menu;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Tic Tac Toe");
  gtk_window_set_default_size (GTK_WINDOW (window), 250, 250);

  main_menu = gtk_label_new("#################\n   Tic-Tac-Toe  \n#################\n=== New Game ===\n1. Single Player\n2. Two Player\n3. Exit\n");
  gtk_label_set_justify(GTK_LABEL(main_menu), GTK_JUSTIFY_LEFT);
  gtk_window_set_child (GTK_WINDOW (window), main_menu);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}