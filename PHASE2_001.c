#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <unistd.h> // For sleep function

// ANSI color codes for colorful output
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

// Admin credentials
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "password" // Change to secure password

// Structure to store item details
struct Item {
    char name[50];
    char description[100];
    char location[50];
    char phone[15];
    char date[11]; // Format: YYYY-MM-DD
    char day[10];  // Day of the week
    char reporterName[50];
    int isLost; // 1 for lost, 0 for found
};

// Global array to store items
struct Item items[100];
int itemCount = 0;

// Function prototypes
void addItem();
void viewItems();
void searchItem();
void deleteItem();
void clearScreen();
void printHeader();
void loadingEffect();
void updateItemStatus();
void requestContactInfo();
void saveItemsToFile();
void loadItemsFromFile();
void logContactRequest(const char *requesterName, const char *requesterPhone, const char *itemName);
void viewContactRequests();
int adminLogin();
void logNotification(const char *message);
void viewNotifications();

int main() {
    loadItemsFromFile(); // Load items from file at startup

    int choice;
    int isAdmin = 0; // Track admin login status

    while (1) {
        clearScreen();
        printHeader();
        printf(YELLOW "1. Add Item\n" RESET);
        printf(YELLOW "2. See All Items\n" RESET);
        printf(YELLOW "3. Search Item\n" RESET);
        printf(YELLOW "4. Delete Item (Admin Only)\n" RESET);
        printf(YELLOW "5. Update Item Status\n" RESET);
        printf(YELLOW "6. Request Owner Contact Info\n" RESET);
        printf(YELLOW "7. View Contact Requests (Admin)\n" RESET);
        printf(YELLOW "8. View Notifications (Admin)\n" RESET);
        printf(RED "9. Exit\n" RESET);
        printf(GREEN "Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            printf(RED "Invalid input! Please enter a number.\n" RESET);
            sleep(1);
            continue;
        }
        getchar(); // Clear newline

        switch (choice) {
            case 1: addItem(); break;
            case 2: viewItems(); break;
            case 3: searchItem(); break;
            case 4:
                if (!isAdmin) {
                    printf(RED "Admin access required to delete items. Please log in.\n" RESET);
                    if (adminLogin()) {
                        isAdmin = 1;
                        deleteItem();
                    } else {
                        printf(RED "Invalid admin credentials!\n" RESET);
                        sleep(1);
                    }
                } else {
                    deleteItem();
                }
                break;
            case 5: updateItemStatus(); break;
            case 6: requestContactInfo(); break;
            case 7:
                if (adminLogin()) {
                    viewContactRequests();
                    isAdmin = 1;
                } else {
                    printf(RED "Invalid admin credentials!\n" RESET);
                    sleep(1);
                }
                break;
            case 8:
                if (adminLogin()) {
                    viewNotifications();
                    isAdmin = 1;
                } else {
                    printf(RED "Invalid admin credentials!\n" RESET);
                    sleep(1);
                }
                break;
            case 9:
                saveItemsToFile(); // Save items to file before exiting
                printf(GREEN "Thank you for using Lost and Found Portal!\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice! Try again.\n" RESET);
                sleep(1);
        }
        printf(YELLOW "\nPress Enter to continue..." RESET);
        getchar();
    }
    return 0;
}

// Load items from a file
void loadItemsFromFile() {
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        printf(YELLOW "No previous data found. Starting fresh.\n" RESET);
        return; // Handle the absence of the file gracefully
    }

    // Read items from the file
    while (fscanf(file, "%49[^|]|%99[^|]|%49[^|]|%14[^|]|%10[^|]|%9[^|]|%49[^|]|%d\n",
                  items[itemCount].name,
                  items[itemCount].description,
                  items[itemCount].location,
                  items[itemCount].phone,
                  items[itemCount].date,
                  items[itemCount].day,
                  items[itemCount].reporterName,
                  &items[itemCount].isLost) == 8) {
        itemCount++;
        if (itemCount >= 100) break; // Prevent overflow
    }
    fclose(file);
    printf(GREEN "Items loaded from file successfully!\n" RESET);
}

// Clear screen (works on Windows and Linux)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Print header for the portal
void printHeader() {
    printf(BLUE "====================================\n" RESET);
    printf(BLUE "     LOST AND FOUND PORTAL\n" RESET);
    printf(BLUE "====================================\n" RESET);
    printf(GREEN "Location: Graphic Era University, Dehradun\n" RESET);
    printf(GREEN "Contact: +91-9876543210 (Replace with your contact number)\n" RESET);
}

// Show loading effect
void loadingEffect() {
    printf(GREEN "Loading");
    for (int i = 0; i < 3; i++) {
        printf(". ");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

// Add a new item
void addItem() {
    clearScreen();
    printHeader();
    if (itemCount >= 100) {
        printf(RED "Storage full! Cannot add more items.\n" RESET);
        return;
    }
    printf(GREEN "Enter item name (e.g., Mobile, Bottle): " RESET);
    if (fgets(items[itemCount].name, 50, stdin) == NULL) return;
    items[itemCount].name[strcspn(items[itemCount].name, "\n")] = 0;
    if (strlen(items[itemCount].name) == 0) {
        printf(RED "Name cannot be empty!\n" RESET);
        return;
    }
    printf(GREEN "Enter description (e.g., Color, Brand, Material): " RESET);
    if (fgets(items[itemCount].description, 100, stdin) == NULL) return;
    items[itemCount].description[strcspn(items[itemCount].description, "\n")] = 0;
    printf(GREEN "Enter location found/lost: " RESET);
    if (fgets(items[itemCount].location, 50, stdin) == NULL) return;
    items[itemCount].location[strcspn(items[itemCount].location, "\n")] = 0;
    printf(GREEN "Enter contact phone number: " RESET);
    if (fgets(items[itemCount].phone, 15, stdin) == NULL) return;
    items[itemCount].phone[strcspn(items[itemCount].phone, "\n")] = 0;
    printf(GREEN "Enter date (YYYY-MM-DD): " RESET);
    if (fgets(items[itemCount].date, 11, stdin) == NULL) return;
    items[itemCount].date[strcspn(items[itemCount].date, "\n")] = 0;
    fflush(stdin);
    printf(GREEN "Enter reporter's name: " RESET);
    if (fgets(items[itemCount].reporterName, 50, stdin) == NULL) return;
    items[itemCount].reporterName[strcspn(items[itemCount].reporterName, "\n")] = 0;
    printf(GREEN "Is this item lost (1) or found (0)? " RESET);
    if (scanf("%d", &items[itemCount].isLost) != 1 || (items[itemCount].isLost != 0 && items[itemCount].isLost != 1)) {
        while (getchar() != '\n');
        printf(RED "Invalid input! Assuming found (0).\n" RESET);
        items[itemCount].isLost = 0;
    }
    getchar();
    loadingEffect(); // Show loading effect
    itemCount++;
    printf(GREEN "Item added successfully!\n" RESET);
}

// See all items
void viewItems() {
    clearScreen();
    printHeader();
    if (itemCount == 0) {
        printf(RED "No items available!\n" RESET);
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        printf(YELLOW "Item %d:\n" RESET, i + 1);
        printf("Name: %s\n", items[i].name);
        printf("Description: %s\n", items[i].description);
        printf("Location: %s\n", items[i].location);
        printf("Phone: %s\n", items[i].phone);
        printf("Date: %s\n", items[i].date);
        printf("Reporter: %s\n", items[i].reporterName);
        printf("Status: %s\n", items[i].isLost ? "Lost" : "Found");
        printf("------------------------\n");
    }
}

// Search item by name and description with selection
void searchItem() {
    clearScreen();
    printHeader();
    char searchName[50];
    char searchDesc[100];
    printf(GREEN "Enter item name to search (e.g., Mobile, Bottle): " RESET);
    if (fgets(searchName, 50, stdin) == NULL) return;
    searchName[strcspn(searchName, "\n")] = 0;
    if (strlen(searchName) == 0) {
        printf(RED "Search name cannot be empty!\n" RESET);
        return;
    }
    printf(GREEN "Enter description keywords (e.g., Black, Samsung, Plastic, or leave blank): " RESET);
    if (fgets(searchDesc, 100, stdin) == NULL) return;
    searchDesc[strcspn(searchDesc, "\n")] = 0;
    int matches[100], matchCount = 0;
    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].name, searchName) != NULL &&
            (strlen(searchDesc) == 0 || strstr(items[i].description, searchDesc) != NULL)) {
            matches[matchCount] = i;
            matchCount++;
        }
    }
    if (matchCount == 0) {
        printf(RED "No items found with name '%s' and description containing '%s'!\n" RESET, searchName, searchDesc);
        return;
    }
    printf(YELLOW "\nFound %d matching items:\n" RESET, matchCount);
    for (int i = 0; i < matchCount; i++) {
        int idx = matches[i];
        printf(YELLOW "%d. %s (Description: %s)\n" RESET, i + 1, items[idx].name, items[idx].description);
    }
    int choice;
    printf(GREEN "\nEnter the number of the item to view details (1-%d, or 0 to cancel): " RESET, matchCount);
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf(RED "Invalid input! Cancelled.\n" RESET);
        return;
    }
    getchar();
    if (choice == 0 || choice < 1 || choice > matchCount) {
        printf(RED "Cancelled or invalid selection!\n" RESET);
        return;
    }
    int selectedIdx = matches[choice - 1];
    printf(YELLOW "\nSelected Item Details:\n" RESET);
    printf("Name: %s\n", items[selectedIdx].name);
    printf("Description: %s\n", items[selectedIdx].description);
    printf("Location: %s\n", items[selectedIdx].location);
    printf("Phone: %s\n", items[selectedIdx].phone);
    printf("Date: %s\n", items[selectedIdx].date);
    printf("Day: %s\n", items[selectedIdx].day);
    printf("Reporter: %s\n", items[selectedIdx].reporterName);
    printf("Status: %s\n", items[selectedIdx].isLost ? "Lost" : "Found");
    printf("------------------------\n");
}

// Update item status
void updateItemStatus() {
    clearScreen();
    printHeader();
    char updateName[50];
    printf(GREEN "Enter item name to update status: " RESET);
    if (fgets(updateName, 50, stdin) == NULL) return;
    updateName[strcspn(updateName, "\n")] = 0;

    for (int i = 0; i < itemCount; i++) {
        if (strcmp(items[i].name, updateName) == 0) {
            int oldStatus = items[i].isLost;
            items[i].isLost = !items[i].isLost; // Toggle status
            printf(GREEN "Status updated successfully! Item is now %s.\n", items[i].isLost ? "Lost" : "Found");

            // If status changed to Found, notify admin
            if (oldStatus == 1 && items[i].isLost == 0) {
                char notification[256];
                time_t now = time(NULL);
                struct tm *tm_info = localtime(&now);
                char timeStr[26];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
                snprintf(notification, sizeof(notification), "Item '%s' was reported FOUND on %s.", items[i].name, timeStr);
                logNotification(notification);
            }
            return;
        }
    }
    printf(RED "Item '%s' not found!\n" RESET, updateName);
}

// Request owner contact info
void requestContactInfo() {
    clearScreen();
    printHeader();
    char requestName[50];
    char requestPhone[15];
    char itemName[50];

    printf(GREEN "Enter your name: " RESET);
    if (fgets(requestName, 50, stdin) == NULL) return;
    requestName[strcspn(requestName, "\n")] = 0;

    printf(GREEN "Enter your phone number: " RESET);
    if (fgets(requestPhone, 15, stdin) == NULL) return;
    requestPhone[strcspn(requestPhone, "\n")] = 0;

    printf(GREEN "Enter the name of the item you want to contact the owner about: " RESET);
    if (fgets(itemName, 50, stdin) == NULL) return;
    itemName[strcspn(itemName, "\n")] = 0;

    // Log the contact request
    logContactRequest(requestName, requestPhone, itemName);
    printf(GREEN "Request for contact info logged. Admin will review it.\n" RESET);
}

// Log contact request to a file
void logContactRequest(const char *requesterName, const char *requesterPhone, const char *itemName) {
    FILE *file = fopen("contact_requests.txt", "a");
    if (file == NULL) {
        printf(RED "Error opening file for logging contact requests!\n" RESET);
        return;
    }
    fprintf(file, "%s|%s|%s\n", requesterName, requesterPhone, itemName);
    fclose(file);
}

// View contact requests for admin
void viewContactRequests() {
    clearScreen();
    printHeader();
    FILE *file = fopen("contact_requests.txt", "r");
    if (file == NULL) {
        printf(RED "No contact requests found!\n" RESET);
        return;
    }

    char line[150];
    printf(YELLOW "Pending Contact Requests:\n" RESET);
    while (fgets(line, sizeof(line), file)) {
        char requesterName[50], requesterPhone[15], itemName[50];
        sscanf(line, "%49[^|]|%14[^|]|%49[^\n]", requesterName, requesterPhone, itemName);
        printf("Requester: %s, Phone: %s, Item: %s\n", requesterName, requesterPhone, itemName);
    }
    fclose(file);
}

// Log notification to a file for admin
void logNotification(const char *message) {
    FILE *file = fopen("notifications.txt", "a");
    if (file == NULL) {
        printf(RED "Error opening file for logging notifications!\n" RESET);
        return;
    }
    fprintf(file, "%s\n", message);
    fclose(file);
}

// View notifications for the admin
void viewNotifications() {
    clearScreen();
    printHeader();
    FILE *file = fopen("notifications.txt", "r");
    if (file == NULL) {
        printf(RED "No notifications found!\n" RESET);
        return;
    }

    char line[256];
    printf(YELLOW "Admin Notifications:\n" RESET);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Save items to a file with numbering
void saveItemsToFile() {
    FILE *file = fopen("items.txt", "w");
    if (file == NULL) {
        printf(RED "Error opening file for saving items!\n" RESET);
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d|%s|%s|%s|%s|%s|%s|%s|%d\n",
                i + 1,  // Numbering starting at 1
                items[i].name,
                items[i].description,
                items[i].location,
                items[i].phone,
                items[i].date,
                items[i].day,
                items[i].reporterName,
                items[i].isLost);
    }
    fclose(file);
    printf(GREEN "Items saved to file successfully!\n" RESET);
}

// Delete an item by name
void deleteItem() {
    clearScreen();
    printHeader();
    char itemName[50];
    printf(GREEN "Enter the name of the item to delete: " RESET);
    if (fgets(itemName, 50, stdin) == NULL) return;
    itemName[strcspn(itemName, "\n")] = 0;

    for (int i = 0; i < itemCount; i++) {
        if (strcmp(items[i].name, itemName) == 0) {
            // Shift items to remove the deleted item
            for (int j = i; j < itemCount - 1; j++) {
                items[j] = items[j + 1];
            }
            itemCount--; // Decrease item count
            printf(GREEN "Item '%s' deleted successfully!\n" RESET, itemName);
            return;
        }
    }
    printf(RED "Item '%s' not found!\n" RESET, itemName);
}

// Admin login function
int adminLogin() {
    char username[50];
    char password[50];

    printf(GREEN "Enter admin username: " RESET);
    if (fgets(username, sizeof(username), stdin) == NULL) return 0;
    username[strcspn(username, "\n")] = 0; // Remove newline

    printf(GREEN "Enter admin password: " RESET);
    if (fgets(password, sizeof(password), stdin) == NULL) return 0;
    password[strcspn(password, "\n")] = 0; // Remove newline

    return (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0);
}

