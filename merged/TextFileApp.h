#ifndef TEXTFILEAPP_H
#define TEXTFILEAPP_H

#include <gtk/gtk.h>
#include <string>

class TextFileApp {
public:
  TextFileApp();
  ~TextFileApp();

  void run();

  void setText(const std::string &text);
  std::string getText() const;
  void saveTextToFile(const std::string &filename);
  void loadTextFromFile(const std::string &filename);

private:
  GtkWidget *window;
  GtkWidget *mainBox;
  GtkWidget *textView;
  GtkTextBuffer *buffer;
  GtkWidget *saveButton;
  std::string text; // Store text
  bool textChanged; // Flag to track text changes

  static void onSaveButtonClicked(GtkWidget *widget, gpointer data);
  static void onTextChanged(GtkTextBuffer *buffer, gpointer data);
  void setText();
  void setButtonLabel(const char *label);
  void loadText();
  void saveText();
};

#endif // TEXTFILEAPP_H
