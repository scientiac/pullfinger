#include "TextFileApp.h"
#include <fstream>
#include <gtk/gtk.h>

TextFileApp::TextFileApp() {
  // Initialize GTK
  gtk_init(NULL, NULL);

  // Create the main window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Text File App");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400); // Set default size
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  // Create the main layout
  mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  // Create the text view for multiple lines of text
  textView = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(textView), 5);
  gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(textView), 5);
  gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(textView), 5);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

  // Create a scrolled window to contain the text view
  GtkScrolledWindow *scrolledWindow =
      GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
  gtk_scrolled_window_set_policy(
      scrolledWindow, GTK_POLICY_AUTOMATIC,
      GTK_POLICY_AUTOMATIC); // Add this line to make it scrollable

  // Add the text view to the scrolled window
  gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);

  // Create the save button
  saveButton = gtk_button_new_with_label("Save");

  // Add widgets to the main layout
  gtk_box_pack_start(GTK_BOX(mainBox), GTK_WIDGET(scrolledWindow), TRUE, TRUE,
                     0); // This line makes the scrolled window expand
  gtk_container_add(GTK_CONTAINER(mainBox), saveButton);

  // Connect callback for the save button
  g_signal_connect(saveButton, "clicked", G_CALLBACK(onSaveButtonClicked),
                   this);

  // Add a callback to detect changes in the text buffer
  g_signal_connect(buffer, "changed", G_CALLBACK(onTextChanged), this);

  // Add the main layout to the window
  gtk_container_add(GTK_CONTAINER(window), mainBox);

  // Load text from a text file
  loadText();

  // Initialize the text changed flag
  textChanged = false;

  // Show all widgets
  gtk_widget_show_all(window);
}

TextFileApp::~TextFileApp() {
  // Cleanup and finalize GTK
  gtk_widget_destroy(window);
}

void TextFileApp::run() { gtk_main(); }

void TextFileApp::onSaveButtonClicked(GtkWidget *widget, gpointer data) {
  TextFileApp *app = static_cast<TextFileApp *>(data);
  if (app->textChanged) {
    app->setText();               // Save the text only if it has changed
    app->setButtonLabel("Saved"); // Change the button label to "Saved"
  }
}

void TextFileApp::onTextChanged(GtkTextBuffer *buffer, gpointer data) {
  TextFileApp *app = static_cast<TextFileApp *>(data);
  app->textChanged = true;     // Set the text changed flag
  app->setButtonLabel("Save"); // Change the button label to "Save"
}

void TextFileApp::setText() {
  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  saveText();          // Save text to a text file
  textChanged = false; // Reset the text changed flag
}

void TextFileApp::setButtonLabel(const char *label) {
  gtk_button_set_label(GTK_BUTTON(saveButton), label);
}

void TextFileApp::loadText() {
  std::ifstream file("text.txt");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      text += line + "\n";
    }
    file.close();
    gtk_text_buffer_set_text(buffer, text.c_str(), -1);
  }
}

void TextFileApp::saveText() {
  std::ofstream file("text.txt");
  if (file.is_open()) {
    file << text;
    file.close();
  }
}
