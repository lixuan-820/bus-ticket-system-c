#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // exit() function
#include <ctype.h>   // toupper()
#include <stdbool.h> // bool type - true & false
#include <time.h>

#define MAX_SCHEDULES 100
#define MAX_SEATS 40
#define MAX_TICKETS 100

// Struct for Admin and Passenger login
typedef struct {
    char username[50];
    char password[50];
    char email[50];
} user_psw;

typedef struct {
    char departure[50];  
    char arrival[50];     
    char dep_time[10];    
    char arr_time[10];    
    char bus_number[20];  
    float price;
    char date[15];
    int num_of_seats;
} BusSchedule;

typedef struct {
    char ticketID[20];
    char departure_location[50];
    char destination[50];
    char date[20];    
    char time[20];   
    char bus_number[20]; 
    int seat;
    char username[20];
    char invoiceID[20];
    char status[20];
} Ticket;

typedef struct {
    char invoiceID[20];
    float price;
    char payment_date[20];
    char payment_method[20];
    char status[20];
} Invoice;

typedef struct {
    char username[50];
    char fav_depart[50];
    char fav_arrival[50];
    char fav_paymentMethod[50];
} user_preference;

// Admin function declare
void login_admin();
void Admin_Menu();
int Load_Bus_Schedules(BusSchedule *schedules);
void Save_Bus_Schedules(BusSchedule *schedules, int count);
void Add_Bus_Schedule();
void Update_Bus_Schedule();
void Delete_Bus_Schedule();
void Display_Bus_Schedules(BusSchedule *schedules, int count);
void viewBookings();
void viewCancellations();
void viewIncome();
void generateReport();

// Passenger function declare
void login_signup_psg();
void login_passenger();
void sign_up_passenger();
void passenger_menu(char username[]);
void loadBookedSeats(char bus_number[], char date[], int bookedSeats[]);
void saveBookedSeats(char *bus_number, char *date, int seat);
void displaySeats(char *bus_number, char *date, int bookedSeats[], int numSeats);
void SearchAndBook(char username[]);
int load_tickets(Ticket ticket[]);
void view_tickets(char username[]);
void print_tickets(char username[]);
void send_notification(char username[], char message[]);
void get_current_date(char *dateStr);
void cancel_booking(char username[]);
void process_refund(char invoiceID[]);
void manage_profile(char username[]);
void update_profile(char username[]);
void update_preferences(char username[]);

// Function to convert a string to uppercase
void toUpperCase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

void main_menu() {
    int choice;
    
    while (true) {   
        printf("\nWelcome to Bus Reservation System\n");
        printf("-------------------------------------\n");
        printf("1. Login as Administrator\n");
        printf("2. Login as Passenger\n");
        printf("3. Logout\n");

        printf("\nEnter option: ");
        if (scanf("%d", &choice) != 1) {   // != 1 means unsuccessful attempt to read an integer datatype
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');   // Clear input buffer to prevent infinite loop
            continue;
        }

        if (choice == 1) {   
            login_admin();
        } else if (choice == 2) {
            login_signup_psg();
        } else if (choice == 3) {
            printf("System exited.\n");
            exit(0);
        } else {
            printf("Please enter a valid number.\n");
        }        
    }
}

void login_admin() {
    user_psw admin;
    char admin_id[50], password[50];
    FILE *file = fopen("data/admin.txt", "r");

    if (!file) {
        printf("No admin accounts found! Please create one.\n");
        return;
    }
    
    printf("Enter Admin ID: ");
    scanf("%s", admin_id);
    toUpperCase(admin_id);
    printf("Enter password: ");
    scanf("%s", password);
    
    while (fscanf(file, "%49[^;];%49[^\n]\n", admin.username, admin.password) != EOF) {
        // Compare input credentials with those stored in the file
        if (strcmp(admin_id, admin.username) == 0 && strcmp(password, admin.password) == 0) {
            printf("Login successful!\n");
            fclose(file);
            Admin_Menu();
            return;
        }
    }
    
    fclose(file);
    printf("Invalid Admin ID or password!\n");
}

void Admin_Menu() {
    int choice;
    while (true) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Bus Schedule\n");
        printf("2. Update Bus Schedule\n");
        printf("3. Delete Bus Schedule\n");
        printf("4. Display Bus Schedules\n");
        printf("5. View Bookings\n");
        printf("6. View Cancellations\n");
        printf("7. View Income\n");
        printf("8. Generate Report\n");
        printf("9. Back to Main Menu\n");

        printf("\nChoose an option: ");
        if (scanf("%d", &choice) != 1) {  
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');  
            continue;
        }
        
        switch (choice) {
            case 1: 
                Add_Bus_Schedule(); 
                break;
            case 2: 
                Update_Bus_Schedule(); 
                break;
            case 3: 
                Delete_Bus_Schedule(); 
                break;
            case 4: {
                BusSchedule schedules[100];
                int count = Load_Bus_Schedules(schedules);
                Display_Bus_Schedules(schedules, count);
                break;
            }
            case 5: 
                viewBookings(); 
                break;
            case 6: 
                viewCancellations(); 
                break;
            case 7: 
                viewIncome(); 
                break;
            case 8: 
                generateReport(); 
                break;
            case 9: 
                printf("Logging out to main menu...\n");
                return;
            default: 
                printf("Invalid option! Please enter a valid choice.\n");
        }
    }
}

int Load_Bus_Schedules(BusSchedule *schedules) {
    FILE *file = fopen("data/schedules.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 0;
     }

    int count = 0;
    while (fscanf(file, "%49[^;];%49[^;];%9[^;];%9[^;];%9[^;];%f;%14[^;];%d\n",
                  schedules[count].departure, schedules[count].arrival, schedules[count].dep_time, schedules[count].arr_time, 
                  schedules[count].bus_number, &schedules[count].price, schedules[count].date, &schedules[count].num_of_seats) != EOF) {
        count++;
        if (count >= 100) break;
    }
    
    fclose(file);
    return count;
}

void Save_Bus_Schedules(BusSchedule *schedules, int count) {
    FILE *file = fopen("data/schedules.txt", "w");
    if (!file) {
       printf("Error opening file for writing!\n");
       return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s;%s;%s;%.2f;%s;%d\n", 
                schedules[i].departure, schedules[i].arrival, schedules[i].dep_time, schedules[i].arr_time, 
                schedules[i].bus_number, schedules[i].price, schedules[i].date, schedules[i].num_of_seats);  
    }
    
    fclose(file);
}

void Add_Bus_Schedule() {
    BusSchedule schedules[100];
    int count = Load_Bus_Schedules(schedules);   // Load current schedules from file
    BusSchedule newSchedule;
    FILE *file = fopen("data/schedules.txt", "a");
    if (!file) {
        printf("Error opening schedule file!\n");
        return;
    }
    printf("Enter bus number: ");
    scanf("%s", newSchedule.bus_number);
    toUpperCase(newSchedule.bus_number);
    int duplicate = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(schedules[i].bus_number, newSchedule.bus_number) == 0) {
            duplicate = 1;
            break;
        }
    }

    // If a duplicate bus number is found, abort the addition process
    if (duplicate) {
        printf("Error: Bus number already exists! Update aborted.\n");
        fclose(file);
        return;
    }

    printf("Enter departure location: ");
    scanf("%s", newSchedule.departure);
    toUpperCase(newSchedule.departure);
    printf("Enter arrival location: ");
    scanf("%s", newSchedule.arrival);
    toUpperCase(newSchedule.arrival);
    printf("Enter departure time (HH:MM): ");
    scanf("%s", newSchedule.dep_time);
    printf("Enter arrival time (HH:MM): ");
    scanf("%s", newSchedule.arr_time);
    printf("Enter price: ");
    scanf("%f", &newSchedule.price);
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", newSchedule.date);
    printf("Enter number of seats of the bus: ");
    scanf("%d", &newSchedule.num_of_seats);

    fprintf(file, "%s;%s;%s;%s;%s;%.2f;%s;%d\n", newSchedule.departure, newSchedule.arrival, newSchedule.dep_time, 
        newSchedule.arr_time, newSchedule.bus_number, newSchedule.price, newSchedule.date, newSchedule.num_of_seats);

    fclose(file);
    printf("Bus schedule added successfully!\n");
}

void Update_Bus_Schedule() {
    BusSchedule schedules[100];
    int count = Load_Bus_Schedules(schedules);
    char bus_number[10];
    int found = 0;

    // If no schedules exist, notify the admin and exit to Admin Menu
    if (count == 0) {
        printf("No bus schedules found!\n");
        return;
    }

    Display_Bus_Schedules(schedules, count);   // Show all current bus schedules

    printf("\nEnter bus number to update: ");
    scanf("%s", bus_number);
    toUpperCase(bus_number);
    getchar(); 

    for (int i = 0; i < count; i++) {
        if (strcmp(schedules[i].bus_number, bus_number) == 0) {
            char temp[50];   // Temporary buffer for new input values

            printf("Enter new departure location (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';   // Remove trailing newline
            if (strlen(temp) > 0) {    // If input is not empty
                toUpperCase(temp);    // Convert to uppercase for uniformity
                strcpy(schedules[i].departure, temp);   // Update departure location
            }

            printf("Enter new arrival location (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) {
                toUpperCase(temp);
                strcpy(schedules[i].arrival, temp);
            }

            printf("Enter new departure time (HH:MM) (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) strcpy(schedules[i].dep_time, temp);

            printf("Enter new arrival time (HH:MM) (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) strcpy(schedules[i].arr_time, temp);

            printf("Enter new bus number (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0) {
            
                int duplicate = 0;
                for (int j = 0; j < count; j++) {
                    // Ensure new bus number doesn't already exist (excluding current bus)
                    if (j != i && strcmp(schedules[j].bus_number, temp) == 0) {
                        duplicate = 1;
                        break;
                    }
                }

                if (duplicate) {     // If duplicate found, abort update
                    printf("Error: Bus number already exists! Update aborted.\n");
                    return;
                } else {
                    toUpperCase(temp);
                    strcpy(schedules[i].bus_number, temp);
                }
            }

            printf("Enter new price (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0'; 
            if (strlen(temp) > 0) {
                double new_price = atof(temp);
                if (new_price >= 0) {    // Validate that price is non-negative value
                    schedules[i].price = new_price;
                } 
            }

            printf("Enter new date (YYYY-MM-DD) (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            if (strcmp(temp, "\n") != 0) {
                temp[strcspn(temp, "\n")] = '\0';
                strcpy(schedules[i].date, temp);
            }

            printf("Enter new number of seats (or press Enter to skip): ");
            fgets(temp, sizeof(temp), stdin);
            if (strcmp(temp, "\n") != 0) {
                int new_seats = atoi(temp);   // Convert string to integer
                if (new_seats > 0) {
                    schedules[i].num_of_seats = new_seats;
                }
            }

            found = 1;   // Set flag to indicate the bus was found
            break;
        }
    }

    if (found) {
        Save_Bus_Schedules(schedules, count);
        printf("Bus schedule updated successfully!\n");
        printf("\n|| Alerts of bus schedule changes have been sent to all respective passengers' emails. ||\n");
    } else {
        printf("Bus number not found!\n");
    }
}

void Delete_Bus_Schedule() {
    BusSchedule schedules[100];
    int count = Load_Bus_Schedules(schedules);
    char bus_number[10];
    int found = 0;

    if (count == 0) {
        printf("No bus schedules found!\n");
        return;
    }

    Display_Bus_Schedules(schedules, count);

    printf("\nEnter bus number to delete: ");
    scanf("%s", bus_number);
    toUpperCase(bus_number);

    for (int i = 0; i < count; i++) {
        if (strcmp(schedules[i].bus_number, bus_number) == 0) {
            // Shift all subsequent schedules one position left to overwrite the deleted one
            for (int j = i; j < count - 1; j++) {
                schedules[j] = schedules[j + 1];
            }
            count--;   // Decrease schedule count after deletion
            found = 1;
            break;
        }
    }

    if (found) {
        Save_Bus_Schedules(schedules, count);
        printf("Bus schedule deleted successfully!\n");
    } else {
        printf("Bus number not found!\n");
    }
}

void Display_Bus_Schedules(BusSchedule *schedules, int count) {
    if (count == 0) {
        printf("No bus schedules available!\n");
        return;
    }

    printf("\n--- Bus Schedules ---\n");
    printf("%-15s %-15s %-10s %-10s %-10s %-10s %-12s %-10s\n", "Departure", "Arrival", "Dep Time", "Arr Time", "Bus No.", "Price", "Date", "Number of Seats");
    // %-ns means print the string, left-aligned, using n spaces (n → Reserve n character spaces for the string, - → Left-align the string)
    printf("---------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-15s %-15s %-10s %-10s %-10s RM %-8.2f %-12s %-10d\n", schedules[i].departure, schedules[i].arrival, schedules[i].dep_time, 
               schedules[i].arr_time, schedules[i].bus_number, schedules[i].price, schedules[i].date, schedules[i].num_of_seats);
    }
}

void viewBookings() {
    int choice;
    Ticket ticket;
    char searchDate[20];
    
    printf("\n--- Booking Management ---\n");
    printf("1. View All Bookings\n");
    printf("2. View Bookings by Date\n");
    printf("3. Return to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            // View all bookings with departure and arrival locations
            FILE *ticketFile = fopen("data/ticket.txt", "r");
            
            if (ticketFile == NULL) {
                printf("Error opening ticket file!\n");
                return;
            }
            
            printf("\n%-13s %-18s %-15s %-15s %-20s\n", 
                   "Ticket ID", "Departure Date", "Departure", "Destination", "Username");
            printf("--------------------------------------------------------------------------\n");
            
            char ticketLine[256];   // Buffer to read each line from the file
            
            while (fgets(ticketLine, sizeof(ticketLine), ticketFile)) {
                ticketLine[strcspn(ticketLine, "\n")] = 0;    // Remove newline character if present
                
                // Parse ticket line
                char *token = strtok(ticketLine, ";");
                if (token != NULL) {
                    strcpy(ticket.ticketID, token);
                    
                    token = strtok(NULL, ";");
                    if (token != NULL) {
                        strcpy(ticket.departure_location, token);
                        
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strcpy(ticket.destination, token);
                            
                            token = strtok(NULL, ";");
                            if (token != NULL) {
                                strcpy(ticket.date, token);
                                
                                token = strtok(NULL, ";");    // Skip time
                                token = strtok(NULL, ";");    //Skip Bus Number
                                token = strtok(NULL, ";");   // Skip seat
                                
                                token = strtok(NULL, ";");
                                if (token != NULL) {
                                    strcpy(ticket.username, token);
                                    
                                    token = strtok(NULL, ";");
                                    if (token != NULL) {
                                        strcpy(ticket.invoiceID, token);

                                        token = strtok(NULL, ";"); 
                                        if (token != NULL && strcmp(token, "Canceled") == 0) {
                                            continue;   // Skip canceled tickets
                                        }

                                        printf("%-13s %-18s %-15s %-15s %-20s\n", 
                                               ticket.ticketID, ticket.date, ticket.departure_location, ticket.destination, ticket.username);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            fclose(ticketFile);
            break;
            
        case 2:
            // View bookings by date and group by route
            printf("Enter date (YYYY-MM-DD): ");
            scanf("%s", searchDate);
            
            ticketFile = fopen("data/ticket.txt", "r");
            
            if (ticketFile == NULL) {
                printf("Error opening ticket file!\n");
                return;
            }

            // Define a structure to hold route statistics (unique departure-arrival combinations)
            struct RouteStats {
                char departure[50];
                char arrival[50];
                int passengerCount;
                bool used;   // Flag to mark if this route entry is in use
            } routes[50];
            
            // Initialize the routes array
            for (int i = 0; i < 50; i++) {
                routes[i].used = false;
                routes[i].passengerCount = 0;
            }
            
            int routeCount = 0, totalPassengers = 0;
            
            // Process all tickets for the specified date
            char dateLine[256];
            while (fgets(dateLine, sizeof(dateLine), ticketFile)) {
                dateLine[strcspn(dateLine, "\n")] = 0;
                
                char *token = strtok(dateLine, ";");
                if (token != NULL) {
                    strcpy(ticket.ticketID, token);
                    
                    token = strtok(NULL, ";");
                    if (token != NULL) {
                        strcpy(ticket.departure_location, token);
                        
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strcpy(ticket.destination, token);
                            
                            token = strtok(NULL, ";");
                            if (token != NULL) {
                                strcpy(ticket.date, token);

                                token = strtok(NULL, ";"); // Skip time
                                token = strtok(NULL, ";");  // Skip Bus Number
                                token = strtok(NULL, ";"); // Skip seat
                                token = strtok(NULL, ";"); // Skip username
                                token = strtok(NULL, ";"); // Skip invoiceID

                                token = strtok(NULL, ";"); 
                                if (token != NULL && strcmp(token, "Canceled") == 0) {
                                    continue;   // Skip canceled tickets
                                }
                                
                                // Check if this ticket matches the search date
                                if (strcmp(ticket.date, searchDate) == 0) {
                                    // Check if this route (departure-arrival combination) already exists in our statistics
                                    bool routeExists = false;
                                    for (int i = 0; i < routeCount; i++) {
                                        if (routes[i].used && 
                                            strcmp(routes[i].departure, ticket.departure_location) == 0 && 
                                            strcmp(routes[i].arrival, ticket.destination) == 0) {
                                        
                                            routes[i].passengerCount++;   // Route exists, increment passenger count
                                            routeExists = true;
                                            break;
                                        }
                                    }
                                    
                                    // If route doesn't exist, add it as a new entry
                                    if (!routeExists && routeCount < 50) {
                                        strcpy(routes[routeCount].departure, ticket.departure_location);
                                        strcpy(routes[routeCount].arrival, ticket.destination);
                                        routes[routeCount].passengerCount = 1;
                                        routes[routeCount].used = true;
                                        routeCount++;
                                    }

                                    totalPassengers++;   // Increment total passenger count
                                }
                            }
                        }
                    }
                }
            }
            
            // Display the summarized results
            printf("\n=== Bookings for Date: %s ===\n", searchDate);
            printf("\n%-15s %-15s %-15s\n", "Departure", "Arrival", "Passengers");
            printf("-------------------------------------------\n");
            if (totalPassengers == 0) {
                printf("No bookings found for this date.\n");
            } else {
                for (int i = 0; i < routeCount; i++) {
                    if (routes[i].used) {
                        printf("%-15s %-15s %-15d\n", routes[i].departure, routes[i].arrival, routes[i].passengerCount);
                    }
                }
                printf("-------------------------------------------\n");
                printf("Total Passengers: %d\n", totalPassengers);
            }
            
            fclose(ticketFile);
            break;
            
        case 3:
            return;   // // Return to Admin Menu
            
        default:
            printf("Invalid choice. Try again.\n");
    }
}

void viewCancellations() {
    Invoice invoice;
    int cancelCount = 0;   // Counter for the number of cancellations
    float totalLoss = 0.0;   // Accumulator for the total financial loss
    int choice;   // Admin menu choice
    int targetMonth = 0, targetYear = 0;   // Variables to store month/year filter criteria
    char dateStr[20];   // Buffer for date string
    int month, year;   // Variables to hold parsed date components
    
    printf("\n--- Cancellation Report ---\n");
    printf("1. View All Cancellations\n");
    printf("2. View Cancellations by Month\n");
    printf("3. Return to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if (choice == 3) {
        return; // Return to Admin Menu
    }
    
    // Filter cancellations by month and year
    if (choice == 2) {
        printf("Enter month (1-12): ");
        scanf("%d", &targetMonth);
        printf("Enter year (YYYY): ");
        scanf("%d", &targetYear);
        
        if (targetMonth < 1 || targetMonth > 12) {
            printf("Invalid month. Please enter a value between 1 and 12.\n");
            return;
        }
    } else if (choice != 1) {   // Handle invalid choices (not 1, 2, or 3)
        printf("Invalid choice. Returning to Admin Menu.\n");
        return;
    }
    
    FILE *file = fopen("data/invoice.txt", "r");
    if (file == NULL) {
        printf("Error opening invoice file!\n");
        return;
    }
    
    printf("\n%-15s %-10s %-15s %-15s\n", 
           "Invoice ID", "Amount", "Payment Date", "Payment Method");
    printf("---------------------------------------------------------\n");
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        char *token = strtok(line, ";");
        if (token != NULL) {
            strcpy(invoice.invoiceID, token);
            
            token = strtok(NULL, ";");
            if (token != NULL) {
                invoice.price = atof(token);   // Extract price and convert it to float datatype
                
                token = strtok(NULL, ";");
                if (token != NULL) {
                    strcpy(invoice.payment_date, token);
                    
                    token = strtok(NULL, ";");
                    if (token != NULL) {
                        strcpy(invoice.payment_method, token);
                        
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strcpy(invoice.status, token);
                            
                            if (strcmp(invoice.status, "Canceled") == 0) {
                                // Parse the date from format YYYY-MM-DD into year and month
                                if (choice == 2) {
                                    sscanf(invoice.payment_date, "%d-%d-%*d", &year, &month);   // %*d means to read and discard a number (in this case, the day)
                                    
                                    if (month != targetMonth || year != targetYear) {
                                        continue;   // Skip if not matching the selected month
                                    }
                                }
                                
                                // Display the canceled invoice information
                                printf("%-15s %-10.2f %-15s %-15s\n", 
                                       invoice.invoiceID, invoice.price, invoice.payment_date, invoice.payment_method);
                                
                                cancelCount++;   // Increment cancellation counter
                                totalLoss += invoice.price;   // Add price to total loss
                            }
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    if (cancelCount == 0) {
        if (choice == 2) {
            printf("\nNo cancellations found for month %d/%d.\n", targetMonth, targetYear);
        } else {
            printf("\nNo cancellations found.\n");
        }
    } else {
        // Display summary statistics if cancellations were found
        printf("\n--------------------------------------------------\n");
        if (choice == 2) {
            printf("Cancellations for %d/%d:\n", targetMonth, targetYear);
        }
        printf("Total cancellations: %d\n", cancelCount);
        printf("Total revenue loss: RM %.2f\n", totalLoss);   // Display with RM currency prefix
    }
}

void viewIncome() {
    Invoice invoice;
    int successCount = 0;   // Counter for the number of successful transactions
    float totalIncome = 0.0;   // Accumulator for the total income
    int choice;
    int targetMonth = 0, targetYear = 0;
    int month, year;
    
    printf("\n--- Income Report ---\n");
    printf("1. View All Income\n");
    printf("2. View Income by Month\n");
    printf("3. Return to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if (choice == 3) {
        return; 
    }
    
    if (choice == 2) {
        printf("Enter month (1-12): ");
        scanf("%d", &targetMonth);
        printf("Enter year (YYYY): ");
        scanf("%d", &targetYear);
        
        if (targetMonth < 1 || targetMonth > 12) {
            printf("Invalid month. Please enter a value between 1 and 12.\n");
            return;
        }
    } else if (choice != 1) {
        printf("Invalid choice. Returning to main menu.\n");
        return;
    }
    
    FILE *file = fopen("data/invoice.txt", "r");
    if (file == NULL) {
        printf("Error opening invoice file!\n");
        return;
    }
    
    printf("\n%-15s %-10s %-15s %-15s\n", 
           "Invoice ID", "Amount", "Payment Date", "Payment Method");
    printf("---------------------------------------------------------\n");
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        char *token = strtok(line, ";");
        if (token != NULL) {
            strcpy(invoice.invoiceID, token);
            
            token = strtok(NULL, ";");
            if (token != NULL) {
                invoice.price = atof(token);
                
                token = strtok(NULL, ";");
                if (token != NULL) {
                    strcpy(invoice.payment_date, token);
                    
                    token = strtok(NULL, ";");
                    if (token != NULL) {
                        strcpy(invoice.payment_method, token);
                        
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strcpy(invoice.status, token);
                            
                            if (strcmp(invoice.status, "Successful") == 0) {
                                if (choice == 2) {
                                    sscanf(invoice.payment_date, "%d-%d-%*d", &year, &month);
                                    
                                    if (month != targetMonth || year != targetYear) {
                                        continue;
                                    }
                                }
                                
                                // Display the successful invoice information
                                printf("%-15s %-10.2f %-15s %-15s\n", 
                                       invoice.invoiceID, invoice.price, invoice.payment_date, invoice.payment_method);
                                
                                successCount++;   // Increment successful transaction counter
                                totalIncome += invoice.price;   // Add price to total income
                            }
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    if (successCount == 0) {
        if (choice == 2) {
            printf("\nNo successful transactions found for month %d/%d.\n", targetMonth, targetYear);
        } else {
            printf("\nNo successful transactions found.\n");
        }
    } else {
        // Display summary statistics if successful transactions were found
        printf("\n---------------------------------------------------------\n");
        if (choice == 2) {
            printf("Income for %d/%d:\n", targetMonth, targetYear);
        }
        printf("Total successful transactions: %d\n", successCount);
        printf("Total income: RM %.2f\n", totalIncome);
    }
}

void generateReport() {
    Invoice invoice;
    // Financial tracking variables
    float totalIncome = 0.0, totalLoss = 0.0;
    int successCount = 0, cancelCount = 0;
    int targetMonth, targetYear;
    float monthlyTarget = 30000.0;
    int month, year;
    float percentageComplete;
    float gapWithTarget;

    // Payment method counters
    int tngCount = 0;
    int creditCardCount = 0;
    int debitCardCount = 0;
    int otherPaymentCount = 0;
    
    printf("\n--- Monthly Performance Report ---\n");
    printf("Enter month (1-12): ");
    scanf("%d", &targetMonth);
    printf("Enter year (YYYY): ");
    scanf("%d", &targetYear);
    
    if (targetMonth < 1 || targetMonth > 12) {
        printf("Invalid month. Please enter a value between 1 and 12.\n");
        return;
    }
    
    FILE *file = fopen("data/invoice.txt", "r");
    if (file == NULL) {
        printf("Error opening invoice file!\n");
        return;
    }
    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, ";");
        if (token != NULL) {
            strcpy(invoice.invoiceID, token);
            
            token = strtok(NULL, ";");
            if (token != NULL) {
                invoice.price = atof(token);
                
                token = strtok(NULL, ";");
                if (token != NULL) {
                    strcpy(invoice.payment_date, token);
                    sscanf(invoice.payment_date, "%d-%d-%*d", &year, &month);

                    if (month != targetMonth || year != targetYear) {
                        continue;   // Skip records that don't match target month/year
                    }
                    
                    token = strtok(NULL, ";");
                    if (token != NULL) {
                        strcpy(invoice.payment_method, token);
                        
                        if (strcmp(invoice.payment_method, "TNG") == 0) {
                            tngCount++;
                        } else if (strcmp(invoice.payment_method, "CREDIT CARD") == 0) {
                            creditCardCount++;
                        } else if (strcmp(invoice.payment_method, "DEBIT CARD") == 0) {
                            debitCardCount++;
                        } else {
                            otherPaymentCount++;
                        }
                        
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strcpy(invoice.status, token);
                            
                            if (strcmp(invoice.status, "Successful") == 0) {
                                totalIncome += invoice.price;
                                successCount++;
                            } else if (strcmp(invoice.status, "Canceled") == 0) {
                                totalLoss += invoice.price;
                                cancelCount++;
                            }
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    percentageComplete = (totalIncome / monthlyTarget) * 100;
    gapWithTarget = monthlyTarget - totalIncome;

    int totalPaymentMethods = tngCount + creditCardCount + debitCardCount + otherPaymentCount;

    // Array of month names for report header
    char *monthNames[] = {"", "January", "February", "March", "April", "May", "June", 
                          "July", "August", "September", "October", "November", "December"};
    
    printf("\n=================================================\n");
    printf("           MONTHLY PERFORMANCE REPORT            \n");
    printf("                   %s %d                  \n", monthNames[targetMonth], targetYear);
    printf("=================================================\n\n");
    
    printf("1. FINANCIAL SUMMARY\n");
    printf("   Monthly Target:            RM %.2f\n", monthlyTarget);
    printf("   Total Income:              RM %.2f\n", totalIncome);
    printf("   Total Loss:                RM %.2f\n", totalLoss);
    printf("   Gap with Target:           RM %.2f\n", (gapWithTarget > 0) ? gapWithTarget : 0);
    printf("   Target Completion:         %.1f%%\n\n", percentageComplete);
    
    printf("2. TRANSACTION BREAKDOWN\n");
    printf("   Successful Transactions:   %d\n", successCount);
    printf("   Cancelled Transactions:    %d\n", cancelCount);
    printf("   Total Transactions:        %d\n\n", successCount + cancelCount);
    
    printf("3. PAYMENT METHOD ANALYSIS\n");
    if (totalPaymentMethods > 0) {
        printf("   TNG:                      %d (%.1f%%)\n", 
               tngCount, (float)tngCount / totalPaymentMethods * 100);
        printf("   Credit Card:              %d (%.1f%%)\n", 
               creditCardCount, (float)creditCardCount / totalPaymentMethods * 100);
        printf("   Debit Card:               %d (%.1f%%)\n", 
               debitCardCount, (float)debitCardCount / totalPaymentMethods * 100);
        printf("   Other Methods:            %d (%.1f%%)\n\n", 
               otherPaymentCount, (float)otherPaymentCount / totalPaymentMethods * 100);
    } else {
        printf("   No transactions recorded for this month.\n\n");
    }
    
    printf("4. PERFORMANCE ASSESSMENT\n");
    printf("   Completion Rate: %.1f%%\n", percentageComplete);
    printf("   Assessment: ");

    if (percentageComplete <= 25) {
        printf("It is still far from the goal, please adjust the plan.\n");
    } else if (percentageComplete <= 50) {
        printf("Only half done, please keep going.\n");
    } else if (percentageComplete <= 75) {
        printf("More than halfway through, good performance.\n");
    } else if (percentageComplete <= 100) {
        printf("You are only one step away from your goal. You can do it!\n");
    } else {
        printf("Excellent! You exceeded your goal.\n");
    }
    
    printf("\n=============================================\n");
    printf("Report generated on: %s", __DATE__);
    printf("\n=============================================\n");
}

// Passenger Login or Sign Up Menu Funtion
void login_signup_psg() {
    int choice;
    printf("\n===== Passenger Login Menu =====\n");
    printf("1. Login passenger account\n");
    printf("2. Sign up as a new passenger\n");
    printf("\nChoose an option: ");
    scanf("%d", &choice);
    getchar();    
    switch (choice) {
        case 1:
            login_passenger();
            break;
        case 2:
            sign_up_passenger();
            break;
        default:
            printf("Invalid option! Returning to main menu...\n");
    }
}

// Function to check if username exists 
int username_exists(const char *username) {
    FILE *file = fopen("data/passenger.txt", "r");
    if (!file) {
        printf("Failed to open user file!\n");
        return 0;
    }
    
    char existing_username[50];
    char existing_password[50];
    char existing_email[50];
    while (fscanf(file, "%[^;];%[^;];%s\n", existing_username, existing_password,existing_email) != EOF) {
        if (strcmp(existing_username, username) == 0) {
            fclose(file);
            return 1; 
        }
    }
    
    fclose(file);
    return 0; 
}

void sign_up_passenger() {
    user_psw user;

    while (1) {
        printf("\nEnter new username: ");
        scanf("%s", user.username);
        
        if (username_exists(user.username) == 1) {
            printf("Username already exists! Please enter a different username.\n");
        } else {
            break; 
        }
    }
    
    printf("Enter new password: ");
    scanf("%s", user.password);
    printf("Enter email: ");
    scanf("%s", user.email);
    
    FILE *passenger_file = fopen("data/passenger.txt", "a");
    if (!passenger_file) {
        printf("Failed to open user file!\n");
        return;
    }
    fprintf(passenger_file, "%s;%s;%s\n", user.username, user.password, user.email);
    fclose(passenger_file);

    printf("Sign up successful! You can now log into the Bus Reservation System.\n");
    login_passenger();
}

void login_passenger() {
    user_psw passenger[100]; 
    int passenger_count = 0;
    FILE *file = fopen("data/passenger.txt", "r");

    if (!file) {
        printf("Could not open passenger file.\n");
        return;
    }

    while (passenger_count < 100 &&
           fscanf(file, " %49[^;];%49[^;];%49[^\n]\n",
                 passenger[passenger_count].username,
                 passenger[passenger_count].password,
                 passenger[passenger_count].email) == 3) {
        passenger_count++;
    }
    fclose(file);

    char username[50], password[50];

    printf("\nEnter your username: ");
    scanf("%49s", username);
    printf("Enter your password: ");
    scanf("%49s", password);

    for (int i = 0; i < passenger_count; i++) {
        if (strcmp(passenger[i].username, username) == 0 && strcmp(passenger[i].password, password) == 0) {
            printf("Passenger login successful!\n");
            passenger_menu(username);
            return;   
        }
    }
    printf("Invalid username or password.\n");
}

void passenger_menu(char username[]) {
    int choice;

    while (true) {
        printf("\nPassenger Menu\n");
        printf("------------------------\n");
        printf("1. Search and Book\n");
        printf("2. View and Print Ticket\n");
        printf("3. Cancel Booking\n");
        printf("4. Manage Profile\n");
        printf("5. Back to main menu\n");
        printf("\nEnter option: ");
        if (scanf("%d", &choice) != 1) {  
            printf("Invalid input! Please enter a valid number.\n");
            while (getchar() != '\n');   
            continue;
        }

        if (choice == 1) {
            SearchAndBook(username);
        } else if (choice == 2) {
            print_tickets(username);
        } else if (choice == 3) {
            cancel_booking(username);
        } else if (choice == 4) {
            manage_profile(username);
        } else if (choice == 5) {
            printf("Logging out to main menu...\n");
            return;    
        } else {
            printf("Please enter a valid number.\n");                
        }
    }
}

void loadBookedSeats(char bus_number[], char date[], int bookedSeats[]) {
    FILE *file = fopen("data/seats.txt", "r");
    if (file) {
        char busNo[20], seatDate[20];
        int seat;
        while (fscanf(file, "%[^;];%[^;];%d\n", busNo, seatDate, &seat) != EOF) {
            if (strcmp(busNo, bus_number) == 0 && strcmp(seatDate, date) == 0 && seat >= 1 && seat <= MAX_SEATS) {
                bookedSeats[seat - 1] = 1;   // Marks the corresponding seat as booked
            }
        }
        fclose(file);
    }
}

void saveBookedSeats(char *bus_number, char *date, int seat) {
    FILE *file = fopen("data/seats.txt", "a");
    if (file) {
        fprintf(file, "%s;%s;%d\n", bus_number, date, seat);
        fclose(file);
    }
}

// Displays the available and booked seats for a bus
void displaySeats(char *bus_number, char *date, int bookedSeats[], int numSeats){
    printf("\nAvailable Seats for %s on %s:\n", bus_number, date);
    for (int i = 0; i < numSeats; i++) {
        if (i % 4 == 0 && i != 0) printf("\n");
        if (bookedSeats[i] == 1)
            printf("[X] ");   // Marks [X] for booked seats
        else
            printf("[%d] ", i + 1);
    }
    printf("\n");
}

// Searches for bus schedules that match the departure point, arrival point, and date
int findSchedules(BusSchedule schedules[], char departure[], char arrival[], char date[]) {
    FILE *file = fopen("data/schedules.txt", "r");
    if (!file) {
        printf("Error opening schedule file!\n");
        return 0;
    }

    int schedule_count = 0;
    char line[200];

    while (fgets(line, sizeof(line), file)) {
        BusSchedule schedule;
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%f;%[^;];%d", 
               schedule.departure, schedule.arrival, schedule.dep_time, schedule.arr_time,
               schedule.bus_number, &schedule.price, schedule.date, &schedule.num_of_seats);

        if (strcmp(schedule.departure, departure) == 0 &&
            strcmp(schedule.arrival, arrival) == 0 &&
            strcmp(schedule.date, date) == 0) {
            
            schedules[schedule_count++] = schedule;
        }
    }
    fclose(file);
    return schedule_count;
}

// Function to get the next ticket ID based on the last ID in the file
int get_next_ticket_id() {
    FILE *file = fopen("data/ticket.txt", "r");
    if (file == NULL) {
        return 1;   // If no file exists, start from T0001
    }

    char line[512];
    int last_ticket_id = 0;

    // Read through the file and track the highest ticket ID
    while (fgets(line, sizeof(line), file)) {
        char ticketID[20] = {0};

        if (sscanf(line, " %19[^;];", ticketID) == 1) {
            int current_id;
            // Extract the numeric part of the ticket ID
            if (sscanf(ticketID, "T%04d", &current_id) == 1) {
                if (current_id > last_ticket_id) {
                    last_ticket_id = current_id;   // Track the highest ticket ID
                }
            }
        }
    }

    fclose(file);
    return last_ticket_id + 1;   // Return the next ticket ID in the sequence
}

// Function to generate the invoice ID based on the ticket ID
void generate_ticket_and_invoice_ids(char *ticketID, char *invoiceID, int sequence) {
    int next_ticket_id = get_next_ticket_id() + sequence;
    sprintf(ticketID, "T%04d", next_ticket_id);
    sprintf(invoiceID, "I%04d", next_ticket_id);
}

// Helper function to compare time strings in HH:MM format
int is_time_after(const char* time1, const char* time2) {
    int h1, m1, h2, m2;
    sscanf(time1, "%d:%d", &h1, &m1);
    sscanf(time2, "%d:%d", &h2, &m2);
    
    if (h1 > h2) return 1;
    if (h1 == h2 && m1 > m2) return 1;
    return 0;
}

// Function to count number of booked seats for a specific bus on a given date
int countBookedSeats(const char* bus_number, const char* date) {
    FILE *file = fopen("data/seats.txt", "r");
    int count = 0;
    if (file) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            char current_bus[20], current_date[20];
            int seat;
            if (sscanf(line, "%[^;];%[^;];%d", current_bus, current_date, &seat) == 3) {
                if (strcmp(current_bus, bus_number) == 0 && strcmp(current_date, date) == 0) {
                    count++;
                }
            }
        }
        fclose(file);
    }
    return count;
}

void SearchAndBook(char username[]) {
    int trip_type, selected_schedule, seat, confirm_payment, add_more;
    char departure_location[50], destination[50],date[20];
    char payment_method[30];
    BusSchedule schedules[MAX_SCHEDULES];
    int search_again=1;
    float totalPrice = 0.0;  // Initialize total price

    char fav_depart[50], fav_arrival[50], fav_payment[50];
    int use_fav_route = 0, use_fav_payment = 0;
    int found = 0;

    char return_date[20];  
    BusSchedule return_schedules[MAX_SCHEDULES];  
    int return_selection = 0;  
    int totalSeatsBooked = 0;  

    int departure_seat_numbers[MAX_SEATS] = {0};   // To store booked seat numbers
    int return_seat_numbers[MAX_SEATS] = {0};    // For return trip seats
    int departure_seats_booked = 0;   // Count of booked seats
    int return_seats_booked = 0;

    int temp_bookedSeats[MAX_SEATS] = {0};   // Temporary departure seat storage
    int temp_departure_seat_numbers[MAX_SEATS] = {0};   // Temporary departure seat numbers
    int temp_departure_seats_booked = 0;    // Temporary count of departure seats
    int temp_return_bookedSeats[MAX_SEATS] = {0};    // Ttemporary return seat storage
    int temp_return_seat_numbers[MAX_SEATS] = {0};   // Temporary return seat numbers
    int temp_return_seats_booked = 0;   // Temporary count of return seats

    // Read user preferences from passengerFav.txt
    FILE *pass_fav = fopen("data/passengerFav.txt", "r");
    if (pass_fav) {
        char line[200];
        while (fgets(line, sizeof(line), pass_fav)) {
            char user[50];
            sscanf(line, "%[^;];%[^;];%[^;];%s", user, fav_depart, fav_arrival, fav_payment);
            if (strcmp(user, username) == 0) {
                found = 1;
                break;
            }
        }
        fclose(pass_fav);
    }

    do {
        // Ask for trip type
        do {
            printf("\nEnter trip type (1=One-Way, 2=Round Trip): ");
            if(scanf("%d", &trip_type) != 1) {   
                printf("Invalid input! Please enter 1 (One-Way) or 2 (Round Trip).\n");
                while(getchar() != '\n');  
                continue;
            }
            if(trip_type != 1 && trip_type != 2) {
                printf("Invalid input! Please enter 1 (One-Way) or 2 (Round Trip).\n");
            }
        } while(trip_type != 1 && trip_type != 2);

        // Ask if user wants to use saved frequent route
        if (found) {
            do {
                printf("\nDo you want to use your frequent route (%s -> %s)? (1=Yes, 0=No): ", fav_depart, fav_arrival);
                if (scanf("%d", &use_fav_route) != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
                    while(getchar() != '\n'); 
                    continue;
                }
                if (use_fav_route != 0 && use_fav_route != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
                }
            } while (use_fav_route != 0 && use_fav_route != 1);
        }

        if (found && use_fav_route) {
            strcpy(departure_location, fav_depart);
            strcpy(destination, fav_arrival);
        } else {
            printf("Enter departure point: ");
            scanf("%s", departure_location);
            printf("Enter arrival point: ");
            scanf("%s", destination);
            toUpperCase(departure_location);
            toUpperCase(destination);
        }
   
        // Store original trip info before swapping
        char original_departure[50], original_destination[50];
        strcpy(original_departure, departure_location);
        strcpy(original_destination, destination);

        printf("Enter journey date (YYYY-MM-DD): ");
        scanf("%s", date);

        // Find matching schedules
        int count = findSchedules(schedules, departure_location, destination, date);
        if (count == 0) {
            printf("No matching schedules found.\n");
            printf("\nDo you want to search another route? (1=Yes, 0=Return to Menu): ");
            scanf("%d", &search_again);
            continue; 
        } else {
            printf("\nAvailable schedules:\n");
            for (int i = 0; i < count; i++) {
            printf("\n%d. %s -> %s | %s - %s | Bus No: %s | Price: RM %.2f\n", i + 1, schedules[i].departure, 
                    schedules[i].arrival,schedules[i].dep_time, schedules[i].arr_time,schedules[i].bus_number, schedules[i].price);
            }
        }

        // Ask user if they want to book
        int book_choice;
        do {
            printf("\nDo you want to book a schedule? (1=Yes, 0=No): ");
            if (scanf("%d", &book_choice) != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
                while(getchar() != '\n'); 
                continue;
            }
            if (book_choice != 0 && book_choice != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
            }
        } while (book_choice != 0 && book_choice != 1);

        if (book_choice != 1) {
            do {
                printf("\nDo you want to search another route? (1=Yes, 0=Return to Menu): ");
                if (scanf("%d", &search_again) != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                    while(getchar() != '\n');
                    continue;
                }
                if (search_again != 0 && search_again != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                }
            } while (search_again != 0 && search_again != 1);
            
            if (search_again == 0) {
                return;   // Return to passenger menu if user selects 0
            }
            continue; // Continue to search again if user selects 1
        }

        // Select a schedule
        int valid_selection = 0;
        do {
            printf("Enter serial number to book: ", count);
            if (scanf("%d", &selected_schedule) != 1) {
                printf("Invalid input! Please enter a valid serial number.\n");
                while(getchar() != '\n');
                continue;
            }
            if (selected_schedule < 1 || selected_schedule > count) {
                printf("Invalid input! Please enter a valid serial number.\n", count);
            } else {
                valid_selection = 1;
            }
        } while (!valid_selection);
        selected_schedule--;   // Adjust to 0-based index

        // Reset temporary variables
        temp_departure_seats_booked = 0;
        for (int i = 0; i < MAX_SEATS; i++) {
            temp_bookedSeats[i] = 0;
            temp_departure_seat_numbers[i] = 0;
        }

        // Load actual booked seats
        int actual_bookedSeats[MAX_SEATS] = {0};
        loadBookedSeats(schedules[selected_schedule].bus_number, date, actual_bookedSeats);

        // Show available seats 
        displaySeats(schedules[selected_schedule].bus_number, date, actual_bookedSeats, schedules[selected_schedule].num_of_seats);

        // Seat selection loop
        do {
            int valid_seat = 0;
            while (!valid_seat) {
                printf("\nEnter seat number to book: ");
                scanf("%d", &seat);
                
                // Check if seat is valid and available
                if (seat < 1 || seat > schedules[selected_schedule].num_of_seats) {
                    printf("Invalid seat number. Please choose again.\n", 
                        schedules[selected_schedule].num_of_seats);
                } 
                else if (actual_bookedSeats[seat-1] == 1) {
                    printf("Seat %d is booked. Choose another seat.\n", seat);
                }
                else if (temp_bookedSeats[seat-1] == 1) {
                    printf("Seat %d is already selected in this session. Choose another seat.\n", seat);
                }
                else {
                    valid_seat = 1;
                }
            }

            // Mark seat as temporarily booked
            temp_bookedSeats[seat-1] = 1;
            temp_departure_seat_numbers[temp_departure_seats_booked++] = seat;

            // Show updated availability
            int combinedDisplay[MAX_SEATS] = {0};
            for (int i = 0; i < MAX_SEATS; i++) {
                combinedDisplay[i] = actual_bookedSeats[i] || temp_bookedSeats[i];
            }
            printf("\nCurrent seat availability:\n");
            displaySeats(schedules[selected_schedule].bus_number, date, combinedDisplay, schedules[selected_schedule].num_of_seats);

            do {
                printf("\nBook another seat? (1=Yes, 0=No): ");
                if(scanf("%d", &add_more) != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
                    while(getchar() != '\n');
                    continue;
                }
                if(add_more != 0 && add_more != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No).\n");
                }
            } while(add_more != 0 && add_more != 1);
        } while (add_more == 1);

        // Calculate departure price
        totalPrice = schedules[selected_schedule].price * temp_departure_seats_booked;
            
        // Round trip handling
        if (trip_type == 2) {
            printf("\n--- Booking Return Ticket ---\n");
            char temp_loc[50];
            strcpy(temp_loc, departure_location);
            strcpy(departure_location, destination);
            strcpy(destination, temp_loc);

            // Initialize temporary return variables
            temp_return_seats_booked = 0;
            for (int i = 0; i < MAX_SEATS; i++) {
                temp_return_seat_numbers[i] = 0;
                temp_return_bookedSeats[i] = 0;
            }

            // Prompt for return date
            do {
                printf("Enter return date (YYYY-MM-DD, must be on or after %s): ", date);
                scanf("%s", return_date);
                
                if (strcmp(return_date, date) < 0) {
                    printf("Error: Return date cannot be before departure. Please try again.\n");
                    continue;
                }
                break;
            } while (1);

            int return_count = findSchedules(return_schedules, departure_location, destination, return_date);

            // Filter and count only available schedules
            int available_schedules = 0;
            BusSchedule available_return_schedules[MAX_SCHEDULES];
            
            for (int i = 0; i < return_count; i++) {
                // Check time validity (if same day)
                if (strcmp(return_date, date) == 0 && !is_time_after(return_schedules[i].dep_time, schedules[selected_schedule].arr_time)) {
                    continue;
                }
                
                // Check seat availability
                int booked = countBookedSeats(return_schedules[i].bus_number, return_date);
                if (booked >= return_schedules[i].num_of_seats) {
                    continue;  // Skip fully booked schedules
                }
                
                // If we get here, the schedule is valid and has available seats
                memcpy(&available_return_schedules[available_schedules], &return_schedules[i], sizeof(BusSchedule));
                // memcpy() used to copy a specified number of bytes from one memory location to another memory location
                available_schedules++;
            }

            if (available_schedules == 0) {
                printf("No available return trips found.\n");
                trip_type = 1;   // Fall back to one-way
                continue;
            }

            // Display only available schedules
            printf("\nAvailable return trips:\n");
            for (int i = 0; i < available_schedules; i++) {
                int booked = countBookedSeats(available_return_schedules[i].bus_number, return_date);
                int available_seats = available_return_schedules[i].num_of_seats - booked;
                
                printf("\n%d. %s -> %s | %s - %s | Bus No: %s | Price: RM %.2f | Seats Available: %d\n", i+1, available_return_schedules[i].departure,
                       available_return_schedules[i].arrival, available_return_schedules[i].dep_time, available_return_schedules[i].arr_time,
                       available_return_schedules[i].bus_number, available_return_schedules[i].price, available_seats);
            }

            // Get user selection
            int valid_return_selection = 0;
            while (!valid_return_selection) {
                printf("\nEnter serial number of return schedule: ");
                if (scanf("%d", &return_selection) != 1) {
                    printf("Invalid input! Please enter a valid serial number.\n");
                    while(getchar() != '\n');
                    continue;
                }
                if (return_selection < 1 || return_selection > available_schedules) {
                    printf("Invalid input! Please enter a valid serial number.\n");
                } else {
                    valid_return_selection = 1;
                    return_selection--;   // Convert to 0-based index
                    
                    // Load actual booked seats for selected schedule
                    loadBookedSeats(available_return_schedules[return_selection].bus_number, return_date, temp_return_bookedSeats);
                    
                    // Display available seats
                    displaySeats(available_return_schedules[return_selection].bus_number, return_date, temp_return_bookedSeats, 
                                available_return_schedules[return_selection].num_of_seats);
                    
                    // Seat selection process
                    int add_more_return = 0;
                    do {
                        int return_seat;
                        int valid_seat = 0;
                        while (!valid_seat) {
                            printf("Enter seat number for return: ");
                            if (scanf("%d", &return_seat) != 1) {
                                printf("Invalid input! Please enter a number.\n");
                                while(getchar() != '\n');
                                continue;
                            }
                            
                            if (return_seat < 1 || return_seat > available_return_schedules[return_selection].num_of_seats) {
                                printf("Invalid seat. Choose again.\n");
                            } else if (temp_return_bookedSeats[return_seat - 1] == 1) {
                                printf("Seat %d is already booked. Choose another.\n", return_seat);
                            } else {
                                valid_seat = 1;
                            }
                        }

                        // Mark seat as temporarily booked
                        temp_return_bookedSeats[return_seat - 1] = 1;
                        temp_return_seat_numbers[temp_return_seats_booked] = return_seat;
                        temp_return_seats_booked++;

                        // Show updated availability
                        displaySeats(available_return_schedules[return_selection].bus_number, return_date, temp_return_bookedSeats, 
                                     available_return_schedules[return_selection].num_of_seats);
                        
                        // Ask if user wants to book another seat
                        do {
                            printf("Book another return seat? (1=Yes, 0=No): ");
                            if (scanf("%d", &add_more_return) != 1) {
                                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                                while(getchar() != '\n');
                                continue;
                            }
                            if (add_more_return != 0 && add_more_return != 1) {
                                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                            }
                        } while (add_more_return != 0 && add_more_return != 1);
                    } while (add_more_return == 1);

                    // Update total price with return trip cost
                    totalPrice += available_return_schedules[return_selection].price * temp_return_seats_booked;
                }
            }
        }

        // After payment confirmation (replace the existing code)
        do {
            printf("\nProceed to payment? (1=Yes, 0=No): ");
            if (scanf("%d", &confirm_payment) != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                while(getchar() != '\n'); 
                continue;
            }
            if (confirm_payment != 0 && confirm_payment != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
            }
        } while (confirm_payment != 0 && confirm_payment != 1);

        int base_ticket_id = get_next_ticket_id();

        if (confirm_payment == 0) {
            printf("Booking cancelled. Returning to menu.\n");
            return;   // Only return to menu on payment cancel
        }

        // Ask if user wants to use saved frequent payment method
        if (found) {
            do {
                printf("\nUse frequent payment method (%s)? (1=Yes, 0=No): ", fav_payment);
                if (scanf("%d", &use_fav_payment) != 1) {
                    printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                    while(getchar() != '\n');
                    continue;
                }
                if (use_fav_payment != 0 && use_fav_payment != 1) {
                    printf("Invalid choice! Please enter 1 (Yes) or 0 (No): ");
                }
            } while (use_fav_payment != 0 && use_fav_payment != 1);
        }

        if (found && use_fav_payment) {
            strcpy(payment_method, fav_payment);
        } else {
            int payment_choice;
            do {
                printf("\nChoose a payment method (1=TNG, 2=Online Banking, 3=Debit Card, 4=Credit Card): ");
                if (scanf("%d", &payment_choice) != 1) {
                    printf("Invalid input! Please enter 1-4\n");
                    while(getchar() != '\n');
                    continue;
                }
                if (payment_choice < 1 || payment_choice > 4) {
                    printf("Invalid choice! Please enter 1-4: ");
                }
            } while (payment_choice < 1 || payment_choice > 4);
                    
            if (payment_choice == 1) {
                strcpy(payment_method, "TNG");
            } else if (payment_choice == 2) {
                strcpy(payment_method, "Online Banking");
            } else if (payment_choice == 3) {
                strcpy(payment_method, "Debit Card");
                char card_number[20], expiry_date[10], cvv[5];
                printf("Enter card number: ");
                scanf("%s", card_number);
                printf("Enter expiry date (MM/YY): ");
                scanf("%s", expiry_date);
                printf("Enter CVV: ");
                scanf("%s", cvv);
            } else if (payment_choice == 4) {  
                strcpy(payment_method, "Credit Card");
                char card_number[20], expiry_date[10], cvv[5];
                printf("Enter card number: ");
                scanf("%s", card_number);
                printf("Enter expiry date (MM/YY): ");
                scanf("%s", expiry_date);
                printf("Enter CVV: ");
                scanf("%s", cvv);
            }
        }

        // Get current time for all tickets
        time_t current_time = time(NULL);
        struct tm *tm_info = localtime(&current_time);

        // Save departure seats with unique tickets
        departure_seats_booked = 0;
        for (int i = 0; i < temp_departure_seats_booked; i++) {
            char ticketID[10], invoiceID[10];
            sprintf(ticketID, "T%04d", base_ticket_id + i);
            sprintf(invoiceID, "I%04d", base_ticket_id + i);
            
            saveBookedSeats(schedules[selected_schedule].bus_number, date, temp_departure_seat_numbers[i]);
            departure_seat_numbers[departure_seats_booked++] = temp_departure_seat_numbers[i];
            
            FILE *ticket_file = fopen("data/ticket.txt", "a");
            if (ticket_file) {
                fprintf(ticket_file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s\n", ticketID,original_departure, original_destination, date,
                        schedules[selected_schedule].dep_time,schedules[selected_schedule].bus_number,temp_departure_seat_numbers[i], username, invoiceID, "Successful");
                fclose(ticket_file);
            }
            
            // Save per-seat invoice for departure
            Invoice newInvoice;
            strcpy(newInvoice.invoiceID, invoiceID);   // Unique per seat
            newInvoice.price = schedules[selected_schedule].price;   // Price for ONE seat
            strftime(newInvoice.payment_date, sizeof(newInvoice.payment_date), "%Y-%m-%d", tm_info);
            strcpy(newInvoice.payment_method, payment_method);
            strcpy(newInvoice.status, "Successful");

            FILE *invoice_file = fopen("data/invoice.txt", "a");
            if (invoice_file) {
                fprintf(invoice_file, "%s;%.2f;%s;%s;%s\n", newInvoice.invoiceID, newInvoice.price,
                        newInvoice.payment_date, newInvoice.payment_method, newInvoice.status);
                fclose(invoice_file);
            }
        }

        // Save return seats if applicable (similar to departure seats)
        return_seats_booked = 0;
        if (trip_type == 2) {
            for (int i = 0; i < temp_return_seats_booked; i++) {
                char ticketID[10], invoiceID[10];
                int seq = departure_seats_booked + i;
                sprintf(ticketID, "T%04d", base_ticket_id + seq);
                sprintf(invoiceID, "I%04d", base_ticket_id + seq);
                
                saveBookedSeats(return_schedules[return_selection].bus_number, return_date, temp_return_seat_numbers[i]);
                return_seat_numbers[return_seats_booked++] = temp_return_seat_numbers[i];
                
                FILE *ticket_file = fopen("data/ticket.txt", "a");
                if (ticket_file) {
                    fprintf(ticket_file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s\n", ticketID, original_destination, original_departure, return_date,  
                            return_schedules[return_selection].dep_time, return_schedules[return_selection].bus_number, temp_return_seat_numbers[i], username, invoiceID, "Successful");
                    fclose(ticket_file);
                }

                Invoice newInvoice;
                strcpy(newInvoice.invoiceID, invoiceID);
                newInvoice.price = return_schedules[return_selection].price;
                strftime(newInvoice.payment_date, sizeof(newInvoice.payment_date), "%Y-%m-%d", tm_info);
                strcpy(newInvoice.payment_method, payment_method);
                strcpy(newInvoice.status, "Successful");

                FILE *invoice_file = fopen("data/invoice.txt", "a");
                if (invoice_file) {
                    fprintf(invoice_file, "%s;%.2f;%s;%s;%s\n", newInvoice.invoiceID, newInvoice.price,
                            newInvoice.payment_date, newInvoice.payment_method, newInvoice.status);
                    fclose(invoice_file);
                }
            }
        }

        // Show confirmation message
        printf("\n------- Booking Confirmation ------\n");
        printf("Status: Successful\n");  
        printf("Departure Trip:\n");
        printf("From %s to %s on %s\n", original_departure, original_destination, date);
        printf("Bus No.: %s | Departure: %s | Arrival: %s\n", schedules[selected_schedule].bus_number,
               schedules[selected_schedule].dep_time, schedules[selected_schedule].arr_time);
        printf("Seat(s) Booked: ");
        for (int i = 0; i < departure_seats_booked; i++) {
            printf("%d (T%04d)", departure_seat_numbers[i], base_ticket_id + i);
            if (i < departure_seats_booked - 1) printf(", ");
        }
        printf("   (Total: %d)\n", departure_seats_booked);

        // Return trip info (if applicable)
        if (trip_type == 2 && return_seats_booked > 0) {
            printf("\nReturn Trip:\n");
            printf("From %s to %s on %s\n",original_destination, original_departure, return_date);
            printf("Bus No.: %s | Departure: %s | Arrival: %s\n", return_schedules[return_selection].bus_number,
                    return_schedules[return_selection].dep_time, return_schedules[return_selection].arr_time);
            printf("Return Seat(s) Booked: ");
            for (int i = 0; i < return_seats_booked; i++) {
                printf("%d (T%04d)", return_seat_numbers[i], base_ticket_id + departure_seats_booked + i);
                if (i < return_seats_booked - 1) printf(", ");
            }
            printf("   (Total: %d)\n", return_seats_booked);
        }

        // Final totals
        printf("\nTotal Seats Booked: %d\n", departure_seats_booked + return_seats_booked);
        printf("Total Price: RM %.2f\n", totalPrice);

        do {
            printf("\nMake another booking? (1=Yes, 0=No): ");
            if (scanf("%d", &search_again) != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
                while(getchar() != '\n');
                continue;
            }
            if (search_again != 0 && search_again != 1) {
                printf("Invalid input! Please enter 1 (Yes) or 0 (No): ");
            }
        } while (search_again != 0 && search_again != 1);
        
    } while (search_again == 1);
}

int load_tickets(Ticket ticket[]){
    int ticket_count = 0;
    FILE *file = fopen("data/ticket.txt", "r");
    if (!file) {
        printf("Error: Could not open ticket.txt\n");
        return 0;
    }

    // Read ticket data until end of file
    while (!feof(file)) {   
        // fscanf reads and parses semicolon-separated values into the corresponding struct fields
        fscanf(file, " %19[^;];%49[^;];%49[^;];%19[^;];%19[^;];%19[^;];%d;%19[^;];%19[^;];%19[^\n]\n", 
               ticket[ticket_count].ticketID, ticket[ticket_count].departure_location, ticket[ticket_count].destination, ticket[ticket_count].date, 
               ticket[ticket_count].time, ticket[ticket_count].bus_number, &ticket[ticket_count].seat, ticket[ticket_count].username, ticket[ticket_count].invoiceID, ticket[ticket_count].status);
        ticket_count++;
        if (ticket_count >= MAX_TICKETS) {
            break;
        }
    }
    fclose(file);
    return ticket_count;   // Return total number of tickets loaded
}

// Function to display all tickets belonging to a specific user
void view_tickets(char username[]) {
    Ticket ticket[100];
    int ticket_count = load_tickets(ticket);

    if (ticket_count == 0) {
        printf("No tickets found.\n");
        return;
    }

    int found = 0;   // Flag to check if user's ticket is found

    printf("\n%-13s %-12s %-12s %-13s %-10s %-12s %-8s %-13s %-13s", "Ticket ID", "From", "To", "Date", "Time", "Bus No", "Seat", "Invoice ID", "Status");
    printf("\n-------------------------------------------------------------------------------------------------------------------------------------\n");
    // Loop through all tickets to find those matching the username
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(ticket[i].username, username) == 0) {
            printf("%-13s %-12s %-12s %-13s %-10s %-12s %-8d %-13s %-13s\n", ticket[i].ticketID, ticket[i].departure_location, ticket[i].destination, 
                ticket[i].date, ticket[i].time, ticket[i].bus_number, ticket[i].seat, ticket[i].invoiceID, ticket[i].status);
            found = 1;   // Mark that at least one ticket is found
        }
    }
    if (!found) {
        printf("You never booked a ticket.\n");
        return;
    }
}

void print_tickets(char username[]){
    Ticket ticket[100];
    view_tickets(username);    // Display all tickets for the given user
    int ticket_count = load_tickets(ticket);

    int has_tickets = 0;   // Flag to check if user has any tickets
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(ticket[i].username, username) == 0) {
            has_tickets = 1;   
            break;  
        }
    }

    if (!has_tickets) {
        return;  // Return to Passenger Menu if no tickets exist
    }

    char choice[10];
    // Ask if user wants to print a ticket
    while (1) {
        printf("\nDo you want to print a ticket? (Y/N): ");
        scanf(" %9s", choice);
        toUpperCase(choice);

        if (strcmp(choice, "N") == 0) {
            return;
        }
        if (strcmp(choice, "Y") != 0) {
            printf("Invalid input. Please enter 'Y' or 'N'.\n");
            continue;
        }

        // Loop for ticket printing process
        while (1) {
            char ticket_ID[20];
            int found_ticket = 0;
            
            printf("Enter the Ticket ID: ");
            scanf(" %19s", ticket_ID);
            toUpperCase(ticket_ID);

            // Search for the ticket by ID and username
            for (int i = 0; i < ticket_count; i++) {
                if (strcmp(ticket[i].ticketID, ticket_ID) == 0 && strcmp(ticket[i].username, username) == 0) {
                    printf("\n-------------- Ticket Details --------------\n");
                    printf("Ticket ID: %s      Status: %s\n", ticket[i].ticketID, ticket[i].status);
                    printf("Username: %s\n", ticket[i].username);
                    printf("Route: %s - %s\n", ticket[i].departure_location, ticket[i].destination);
                    printf("Date: %s      Time: %s\n", ticket[i].date, ticket[i].time);
                    printf("Bus No: %s       Seat: %d\n", ticket[i].bus_number, ticket[i].seat);
                    printf("Invoice ID: %s\n", ticket[i].invoiceID);
                    printf("--------------------------------------------\n");
                    found_ticket = 1;
                    break;
                }
            }

            if (!found_ticket) {
                printf("The ticket ID '%s' is not found or does not belong to you.\n", ticket_ID);
            }

            while (1) {
                printf("\nDo you want to print another ticket? (Y/N): ");
                scanf(" %9s", choice);
                toUpperCase(choice);

                if (strcmp(choice, "Y") == 0) {
                    break; 
                } else if (strcmp(choice, "N") == 0) {
                    return; 
                } else {
                    printf("Invalid input. Please enter 'Y' or 'N'.\n");
                }
            }
        }
    }
}

// Sends a notification message to a specific user
void send_notification(char username[], char message[]) {
    printf("\n**Notification for %s: %s\n", username, message);
}

void get_current_date(char *dateStr) {
    time_t t = time(NULL);    // Get current time
                              // time_t is a long integer data type used to store the number of seconds since January 1, 1970
                              // time(NULL) is a standard library function that returns the number of seconds from January 1, 1970 to the current moment
                              // If a NULL parameter is passed, it means getting the current time.
    struct tm tm = *localtime(&t);    // Convert to local time structure
                                      // struct tm is a structure used to store specific time information (year, month, day, hour, minute, second, etc.)
                                      // localtime(&t) is a standard library function that converts time_t type time to local time 
                                      // It returns a pointer to struct tm, which contains detailed local time information, and assign it to tm.
    strftime(dateStr, 20, "%Y-%m-%d", &tm);    // Format the date into a string; 20 is the buffer size to safely store the formatted 
                                               // dateStr is the output character array used to store the formatted date string
                                               // "%Y-%m-%d" is a format string that specifies the display format of the date
}

// Compares two date strings in the format "YYYY-MM-DD"
int compare_dates(const char *date1, const char *date2) {
    return strcmp(date1, date2); 
}

void cancel_booking(char username[]) {
    Ticket tickets[100];
    view_tickets(username);
    int ticket_count = load_tickets(tickets);

    char ticketID[20];
    printf("\n**Dear passenger, kindly be informed that you can only cancel the tickets for dates after today (today excluded).\n");
    printf("\nEnter Ticket ID to cancel: ");
    scanf("%s", ticketID);
    toUpperCase(ticketID);

    char current_date[20];
    get_current_date(current_date);   // Get today's date in YYYY-MM-DD format

    FILE *file = fopen("data/ticket.txt", "w");
    if (!file) {
        printf("Error: Could not open ticket file for writing.\n");
        return;
    }

    int found = 0;   // Flag to track if the ticket was found
    for (int i = 0; i < ticket_count; i++) {
        if (strcmp(tickets[i].ticketID, ticketID) == 0 && strcmp(tickets[i].username, username) == 0) {
            found = 1;
            // Check if the departure date of the booked ticket has passed or is today
            if (compare_dates(current_date, tickets[i].date) >= 0) {
                printf("Ticket %s cannot be canceled as the travel date has already passed or is today.\n", ticketID);
                // Write the original ticket back to the file (unchanged)
                fprintf(file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s\n", tickets[i].ticketID, tickets[i].departure_location, tickets[i].destination, tickets[i].date, 
                        tickets[i].time, tickets[i].bus_number, tickets[i].seat, tickets[i].username, tickets[i].invoiceID, tickets[i].status);
                continue;
            }

            // Check if the ticket is already canceled
            if (strcmp(tickets[i].status, "Canceled") == 0) {
                printf("Ticket %s has already been canceled. Duplicate cancellation is not allowed.\n", ticketID);
                // Write the original ticket back (unchanged)
                fprintf(file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s\n", tickets[i].ticketID, tickets[i].departure_location, tickets[i].destination, 
                        tickets[i].date, tickets[i].time, tickets[i].bus_number, tickets[i].seat, tickets[i].username, tickets[i].invoiceID, tickets[i].status);
                continue;
            }

            // If cancel is allowed, notify the user and mark the ticket as "Canceled"
            printf("Ticket %s canceled successfully.\n", ticketID);
            send_notification(username, "Your ticket has been successfully canceled.");
            process_refund(tickets[i].invoiceID);
            // Save the ticket info with updated status
            fprintf(file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;Canceled\n", tickets[i].ticketID, tickets[i].departure_location, tickets[i].destination,
                tickets[i].date, tickets[i].time, tickets[i].bus_number, tickets[i].seat, tickets[i].username, tickets[i].invoiceID);

            // Remove seat from seats.txt 
            FILE *seat_file = fopen("data/seats.txt", "r");
            FILE *temp_seat_file = fopen("data/temp_seats.txt", "w");
            
            if (seat_file && temp_seat_file) {
                char bus_no[20], dep_date[20];
                int seat_count = 0, seat_no;
            
                while (fscanf(seat_file, "%[^;];%[^;];%d\n", bus_no, dep_date, &seat_no) == 3) {
                    // If the record does NOT match the canceled seat, write it to the temp file
                    if (!(strcmp(bus_no, tickets[i].bus_number) == 0 && strcmp(dep_date, tickets[i].date) == 0 && seat_no == tickets[i].seat)) {
                        fprintf(temp_seat_file, "%s;%s;%d\n", bus_no, dep_date, seat_no);
                    }
                    seat_count ++;
                    if (seat_count > MAX_SEATS) {
                        break;
                    }
                }

                fclose(seat_file);
                fclose(temp_seat_file);
                // Replace original seats.txt with updated version
                remove("data/seats.txt");    
                rename("data/temp_seats.txt", "data/seats.txt");
            } else {
                // If file opening fails, safely close any opened file to prevent errors
                if (seat_file) fclose(seat_file);
                if (temp_seat_file) fclose(temp_seat_file);
                printf("Could not update seat records.\n");
            }                    
        } else {
            // Write all other tickets (not being canceled) back to the file
            fprintf(file, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s\n", tickets[i].ticketID, tickets[i].departure_location, tickets[i].destination, tickets[i].date, 
                tickets[i].time, tickets[i].bus_number, tickets[i].seat, tickets[i].username, tickets[i].invoiceID, tickets[i].status);
        }
    }
    fclose(file);

    if (!found) {
        printf("Ticket ID not found or does not belong to you.\n");
    }
}

void process_refund(char invoiceID[]) {
    FILE *file = fopen("data/invoice.txt", "r");
    if (!file) {
        printf("Error: Could not open invoice file.\n");
        return;
    }

    Invoice invoices[100];  
    int invoice_count = 0;
    int found = 0;

    while (fscanf(file, " %19[^;];%f;%19[^;];%19[^;];%19[^\n]\n", invoices[invoice_count].invoiceID, &invoices[invoice_count].price, 
                  invoices[invoice_count].payment_date, invoices[invoice_count].payment_method, invoices[invoice_count].status) == 5) {
        invoice_count++;
        if (invoice_count >= 100) {
            break;
        } 
    }
    fclose(file);

    // Search for the invoice to be refunded
    for (int i = 0; i < invoice_count; i++) {
        if (strcmp(invoices[i].invoiceID, invoiceID) == 0) {
            strcpy(invoices[i].status, "Canceled");  
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Invoice ID not found.\n");
        return;
    }

    file = fopen("data/invoice.txt", "w");
    if (!file) {
        printf("Error: Could not open invoice file for writing.\n");
        return;
    }

    for (int i = 0; i < invoice_count; i++) {
        fprintf(file, "%s;%.2f;%s;%s;%s\n", invoices[i].invoiceID, invoices[i].price, invoices[i].payment_date, invoices[i].payment_method, invoices[i].status);
    }
    fclose(file);

    printf("Refund processed successfully for Invoice ID: %s\n", invoiceID);
}

void manage_profile(char username[]) {
    int choice;
    do {
        printf("\n----- Profile Management -----\n");
        printf("1. Update Profile\n");
        printf("2. Update Frequent Route and Payment Method\n");
        printf("3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: update_profile(username); break;
            case 2: update_preferences(username); break;
            case 3: return;  
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);  
}

void update_profile(char username[]) {
    user_psw users[100];
    int count = 0, found = 0;

    FILE *file = fopen("data/passenger.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%49[^;];%49[^;];%49[^\n]\n", users[count].username, users[count].password, users[count].email) != EOF) {
        count++;
    }
    fclose(file);

    // Find the target user and update their profile information
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            found = 1;

            printf("\nUpdate new username (press Enter to keep [%s]): ", users[i].username);
            char new_username[50];
            fgets(new_username, sizeof(new_username), stdin);
            if (new_username[0] != '\n') {   // Check if user entered something (not just Enter) 
                new_username[strcspn(new_username, "\n")] = 0;   // Remove newline character 
                strcpy(users[i].username, new_username);
            }

            printf("Update new password (press Enter to keep current): ");
            char new_password[50];
            fgets(new_password, sizeof(new_password), stdin);
            if (new_password[0] != '\n') {
                new_password[strcspn(new_password, "\n")] = 0;
                strcpy(users[i].password, new_password);
            }

            printf("Update new email (press Enter to keep [%s]): ", users[i].email);
            char new_email[50];
            fgets(new_email, sizeof(new_email), stdin);
            if (new_email[0] != '\n') {
                new_email[strcspn(new_email, "\n")] = 0;
                strcpy(users[i].email, new_email);
            }

            printf("\nProfile updated successfully!\n");
            break;
        }
    }

    // Write all user records back to file, including any updates
    file = fopen("data/passenger.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s\n", users[i].username, users[i].password, users[i].email);
    }
    fclose(file);
}

void update_preferences(char username[]) {
    user_preference users[100];
    int count = 0, found = 0;

    FILE *pass_fav = fopen("data/passengerFav.txt", "r");
    if (!pass_fav) {
        printf("Error: passengerFav.txt not found.\n");
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), pass_fav)) {
        char *token = strtok(line, ";\n");   // Parse each field using semicolon as delimiter
        if (token != NULL) {
            strcpy(users[count].username, token);
        }

        token = strtok(NULL, ";\n");
        if (token != NULL) {
            strcpy(users[count].fav_depart, token);
        }

        token = strtok(NULL, ";\n");
        if (token != NULL) {
            strcpy(users[count].fav_arrival, token);
        }

        token = strtok(NULL, ";\n");
        if (token != NULL) {
            strcpy(users[count].fav_paymentMethod, token);
        }

        count++;
    }
    fclose(pass_fav);

    for (int i = 0; i < count; i++) {
        // Look for existing user to update their preferences
        if (strcmp(users[i].username, username) == 0) {
            found = 1;
            char temp[50];   // Temporary variable to store input 
    
            printf("Enter new frequent departure location (or press enter to keep [%s]): ", users[i].fav_depart);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;  // 
            if (strlen(temp) > 0) {    // Only update if input is not empty
                strcpy(users[i].fav_depart, temp);
                toUpperCase(users[i].fav_depart);
            }
    
            printf("Enter new frequent arrival location (or press enter to keep [%s]): ", users[i].fav_arrival);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(users[i].fav_arrival, temp);
                toUpperCase(users[i].fav_arrival);
            }
    
            printf("Enter new payment method (or press enter to keep [%s]): ", users[i].fav_paymentMethod);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(users[i].fav_paymentMethod, temp);
                toUpperCase(users[i].fav_paymentMethod);
            }
    
            break;
        }
    }    

    // If user not found, append new user to the file
    if (!found) {
        strcpy(users[count].username, username);
    
        do {
            printf("Enter frequent departure location (cannot be empty): ");
            fgets(users[count].fav_depart, sizeof(users[count].fav_depart), stdin);
            users[count].fav_depart[strcspn(users[count].fav_depart, "\n")] = 0;
          // Continue loop if input is empty or contains only spaces
        } while (strlen(users[count].fav_depart) == 0 || strspn(users[count].fav_depart, " ") == strlen(users[count].fav_depart));
        toUpperCase(users[count].fav_depart);
    
        do {
            printf("Enter frequent arrival location (cannot be empty): ");
            fgets(users[count].fav_arrival, sizeof(users[count].fav_arrival), stdin);
            users[count].fav_arrival[strcspn(users[count].fav_arrival, "\n")] = 0;
        } while (strlen(users[count].fav_arrival) == 0 || strspn(users[count].fav_arrival, " ") == strlen(users[count].fav_arrival));
        toUpperCase(users[count].fav_arrival);
    
        do {
            printf("Enter payment method (cannot be empty): ");
            fgets(users[count].fav_paymentMethod, sizeof(users[count].fav_paymentMethod), stdin);
            users[count].fav_paymentMethod[strcspn(users[count].fav_paymentMethod, "\n")] = 0;
        } while (strlen(users[count].fav_paymentMethod) == 0 || strspn(users[count].fav_paymentMethod, " ") == strlen(users[count].fav_paymentMethod));
        toUpperCase(users[count].fav_paymentMethod);
    
        count++;   // Increment count to include the new user
    }    

    pass_fav = fopen("data/passengerFav.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(pass_fav, "%s;%s;%s;%s\n", users[i].username, users[i].fav_depart, users[i].fav_arrival, users[i].fav_paymentMethod);
    }
    fclose(pass_fav);

    printf("\nUser's preferences have been successfully updated!\n");
}

int main() {
    main_menu();
    return 0;
}
