#include "TextFileApp.h"

#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// Class responsible for fetching Finger information
class FingerClient {
public:
  FingerClient() {
    // Initialize variables
    host = "";
    user = "";
    lastAddress = "";
  }

  std::string fetchFingerInfo() {
    try {
      // Create a socket for communication
      int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
      if (clientSocket == -1) {
        throw std::runtime_error("Error creating socket.");
      }

      // Get host information
      hostent *server = gethostbyname(host.c_str());
      if (server == nullptr) {
        throw std::runtime_error("Host not found.");
      }

      sockaddr_in serverAddr;
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(79);
      serverAddr.sin_addr = *(struct in_addr *)server->h_addr;

      // Connect to the server
      if (connect(clientSocket, (struct sockaddr *)&serverAddr,
                  sizeof(serverAddr)) == -1) {
        close(clientSocket);
        throw std::runtime_error("Error connecting to the server.");
      }

      // Send query
      std::string query = user + "\r\n";
      send(clientSocket, query.c_str(), query.size(), 0);

      // Receive response
      std::string response;
      char buffer[1024];
      while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
          break;
        }
        response.append(buffer, bytesRead);
      }

      // Close the socket and return the response
      close(clientSocket);
      return response;
    } catch (const std::exception &e) {
      return "Error: " + std::string(e.what());
    }
  }

  std::string validateAndSetURL(const std::string &input) {
    // Check for "finger " format
    size_t fingerPos = input.find("finger ");
    if (fingerPos != std::string::npos) {
      std::string address = input.substr(fingerPos + 7);
      if (address.empty()) {
        return "The address is invalid, use finger://example.com/user or "
               "example.com/user";
      }

      // Parse user and host
      size_t atIndex = address.find('@');
      if (atIndex != std::string::npos) {
        user = address.substr(0, atIndex);
        host = address.substr(atIndex + 1);
        lastAddress = "finger://" + address;
        return "";
      }
    }

    // Check for "://" format
    size_t schemePos = input.find("://");
    if (schemePos != std::string::npos) {
      std::string hostUser = input.substr(schemePos + 3);
      size_t slashPos = hostUser.find('/');
      if (slashPos == std::string::npos) {
        return "Invalid URL format.";
      }
      host = hostUser.substr(0, slashPos);
      user = hostUser.substr(slashPos + 1);
      lastAddress = input;
      return "";
    } else {
      // Check for "host/user" format
      size_t slashPos = input.find('/');
      if (slashPos == std::string::npos) {
        return "Invalid URL format.";
      }
      host = input.substr(0, slashPos);
      user = input.substr(slashPos + 1);
      lastAddress = "finger://" + input;
      return "";
    }
  }

  std::string getLastAddress() const { return lastAddress; }

private:
  std::string host;
  std::string user;
  std::string lastAddress;
};

// Class responsible for managing the GUI application
class FingerApp {
public:
  FingerApp() {
    // Initialize GTK
    gtk_init(NULL, NULL);

    // Create main window and layout
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    input_entry = gtk_entry_new();
    response_label = gtk_label_new(NULL);
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // Configure window
    gtk_window_set_title(GTK_WINDOW(window), "Pullfinger");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Initialize FingerClient and GUI components
    fingerClient = FingerClient();
    gtk_container_add(GTK_CONTAINER(vbox), input_entry);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(scrolled_window), response_label);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create the bookmarks button
    bookmarksButton = gtk_button_new_with_label("Bookmarks");
    g_signal_connect(bookmarksButton, "clicked", G_CALLBACK(openBookmarks),
                     this);
    gtk_container_add(GTK_CONTAINER(vbox), bookmarksButton);

    // Connect callback for input entry activation
    g_signal_connect(input_entry, "activate", G_CALLBACK(input_entry_activated),
                     this);

    // Update initial response label
    updateResponseLabel(
        "Enter 'finger user@host' or 'finger://host/user' and press Enter.");
    // Show all GUI elements
    gtk_widget_show_all(window);
  }

  // Start GTK main loop
  void run() { gtk_main(); }

private:
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *input_entry;
  GtkWidget *response_label;
  GtkWidget *scrolled_window;
  GtkWidget *bookmarksButton;
  FingerClient fingerClient;

  // Update the response label with text
  void updateResponseLabel(const std::string &text) {
    gtk_label_set_text(GTK_LABEL(response_label), text.c_str());

    // Get the PangoContext for the response_label
    PangoContext *context = gtk_widget_get_pango_context(response_label);

    // Get the system's monospaced font
    PangoFontDescription *fontDesc =
        pango_context_get_font_description(context);

    // Set the font description to be monospaced (you can set other properties
    // as needed)
    pango_font_description_set_family(fontDesc, "Monospace");

    // Apply the font description to the response_label
    pango_context_set_font_description(context, fontDesc);

    gtk_widget_show_all(window);
  }

  // Callback function for input entry activation
  static void input_entry_activated(GtkEntry *entry, gpointer data) {
    FingerApp *app = static_cast<FingerApp *>(data);
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(entry));

    // Validate input and set URL
    std::string error_message = app->fingerClient.validateAndSetURL(input);
    if (!error_message.empty()) {
      app->updateResponseLabel(error_message);
      return;
    }

    // Fetch and update response if the address is changed
    if (app->fingerClient.getLastAddress() !=
        app->fingerClient.validateAndSetURL(input)) {
      std::string response = app->fingerClient.fetchFingerInfo();
      app->updateResponseLabel(response);
    }
  }

  // Callback function for the bookmarks button
  static void openBookmarks(GtkButton *button, gpointer data) {
    // Open the TextFileApp window
    TextFileApp textFileApp;
    textFileApp.run();
  }
};

// Main function
int main(int argc, char *argv[]) {
  FingerApp app;
  app.run();
  return 0;
}
