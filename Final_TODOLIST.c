#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define len 256
#define LIST_SIZE 50
#define RED     "\x1b[31m"   //defining ASCII colours
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
void signup(FILE *username_file, FILE *password_file); //prototypes
bool login(FILE *username_file, FILE *password_file, bool *login_successful);

//To-do list functions created before main
void addToList(char* list[], int* listSize)
{
    char str[100];
    fgets(str, sizeof str, stdin); //gets string input
    strcpy(list[*listSize], str); //puts it into the list array
    *listSize += 1; //records increase in list size
    printf("Added item to list\n");
}

void printList(char* list[], int listSize)
{

    if(listSize == 0)
    {
        printf("List is empty\n");
    }
    for(int i = 0; i < listSize; i++) //prints out the list
    {
        printf("%d: %s", i+1, list[i]);
    }
}

void removeItem(char* list[], int* listSize, int index)
{
    if(index > *listSize || index == 0)
    {
        printf("not within bounds\n");
        return;
    }

    char temp[100];
    for(int i = index; i < *listSize; i++) //shuffles item to the end of the array
    {
        strcpy(temp, list[i]);
        strcpy(list[i], list[i-1]);
        strcpy(list[i-1],temp);
    }
    strcpy(list[*listSize-1],""); //after it has been moved to the end of the array it is made empty
    *listSize -= 1; //decreases list size so the removed item is not included
    printf("Removed index %d\n", index);
}

void writeToFile(FILE* filePtr, char* list[], int listSize)
{
  for(int i = 0; i < listSize; i++)
    {
      fprintf(filePtr,"%s",list[i]); //writes each element of the array to the file
    }
}

void fileToArray(FILE* filePtr, char* list[], int* listSize)
{
  char str[100];
  while(fgets(str,100,filePtr) != NULL) //gets each line from file
    {
      strcpy(list[*listSize], str); //copies line to list array
      *listSize += 1; //records the list size
    }
}

void clearList(char* list[], int* listSize)
{
  for(int i = 0; i < *listSize; i++)
    {
      strcpy(list[i],"");
    }
  *listSize = 0;
}

void printMenu()
{
    
    printf(YELLOW"***********\n");
    printf("[1] Display tasks\n[2] Add item\n[3] Remove item\n[4] Clear list\n[5] Exit program\n\n");
    printf("***********\n"RESET);
}

int main()
{
    // Creating pointers to username and password files
    FILE *username_file;
    FILE *password_file;
    int choice;
    bool login_successful = false;     //Creating a bool variable that will be used later to check if login was succesfull

    //printing a welcome sign
    printf(CYAN"///////////////////////////////////////////////////////////////////////////////////////////\n"RESET);
    printf(YELLOW"     //   //   //   ////////   //        ////////    /////    //////////    ////////\n"RESET);
    printf(GREEN"     //   //   //   //         //        //        //    //   //  //  //    //\n"RESET);
    printf(RED"     //   //   //   ////////   //        //        //    //   //  //  //    ////////\n"RESET);
    printf(BLUE"     //   //   //   //         //        //        //    //   //  //  //    //\n"RESET);
    printf(MAGENTA"     ////////////   ////////   ////////  ////////    /////    //  //  //    ////////\n" RESET);
    printf(YELLOW"///////////////////////////////////////////////////////////////////////////////////////////\n"RESET);

    // Open username and password files in append mode to create if not exist
    username_file = fopen("username.txt", "a+");
    if (username_file == NULL) 
    {
        printf(RED"Error opening username file!\n");    //check if file opens
        return 1;
    }

    password_file = fopen("password.txt", "a+");
    if (password_file == NULL) 
    {
        printf(RED"Error opening password file!\n");
        fclose(username_file);
        return 1;
    }

    // ask user if they would like to sign up or login
    for(;;)
    {
    printf(CYAN "Would you like to:\n\n1) Sign up\n2) Login\n3) Quit\n\nYour choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); // Clear input buffer to prevent infinte loop

    if(choice==1)
    {
        signup(username_file, password_file);
        break;
    }
    else if(choice==2)
    {
        login_successful = login(username_file, password_file, &login_successful); // Pass login_successful(bool variable to gauge if login was succesfull)
        break;
    }
    else if(choice==3)
    {
    printf("\nYou choose to quit- Goodbye\n");
    exit(0);
    }
    else
    { 
    system("clear");
    printf("Invalid choice. Try again\n\n");
    continue;
    }
    

}

    // Close files
    fclose(username_file);
    fclose(password_file);
/////////////////////////////////////////////////////////////
    // If login was successful, proceed to the to-do list
    if (login_successful)
    {
        printf(CYAN   "\n///////////////////////////////////////////////////////////////////////////////////////////\n\n"RESET);
    printf(YELLOW "//////////  ////////         /////     ////////     //       ////////  ////////  ////////// \n"RESET);
    printf(GREEN  "   ///      //    //         //   ///  //    //     //          //     //            ///    \n"RESET);
    printf(RED    "   ///      //    //  /////  //    //  //    //     //          //     ////////      ///    \n"RESET);
    printf(BLUE   "   ///      //    //         //    //  //    //     //          //           //      ///    \n"RESET);
    printf(MAGENTA"   ///      ////////         //////    ////////     //////// ////////  ////////      ///    \n"RESET);
    printf(YELLOW "\n///////////////////////////////////////////////////////////////////////////////////////////\n\n"RESET);
    char* list[LIST_SIZE];
    char input[10];
    int listSize = 0;
    int fileFound = 0;
    int keepGoing = 1;

    for(int i = 0; i < LIST_SIZE; i++) //give memory to each pointer in array. allows for adding strings to array
      {
        list[i] = malloc(100*sizeof(char));
      }

    FILE* filePtr;
    char fileName[100];

    while(!fileFound)
      {
        printf(YELLOW"****************************\n\n");
        printf("[1] Create new\n[2] Load existing file (name of file, do not include .txt)\n\n****************************\nInput: "RESET);

        int choice;
        scanf("%d",&choice);
        while(getchar() != '\n');
        system("clear");
        switch(choice)
          {
            case 1:
              printf("Name the file: ");
              fgets(fileName, 100, stdin); //gets the file name
              fileName[strcspn(fileName,"\n")] = 0; //removes newline
              strcat(fileName,".txt"); //adds .txt to the end of the file name
              filePtr = fopen(fileName,"w+"); //opens file to be written in
              fileFound = 1; //ends the while loop
              system("clear"); //clears screen
              break;
            case 2:
              printf("Name of file: ");
              fgets(fileName, 100, stdin); //gets file name
              fileName[strcspn(fileName,"\n")] = 0; //removes newline
              strcat(fileName,".txt"); //adds .txt to end of file name
              filePtr = fopen(fileName,"r+"); //opens file for reading in order to check if file exists
              system("clear"); //clears screen

              if(filePtr == NULL) //if file does not exists it closes the program
              {
                printf("file does not exist");
                exit(0);
              }
              fileFound = 2; //if file is found then it writes the file data into the list
              break;
            default:
              system("clear");
              printf("try again\n");
              break;
          }
      }
  
    if(fileFound == 2)
    {
      fileToArray(filePtr, list, &listSize); //function writes the data to the list array
    }


    //switches to write mode. rewrites it because fopen w+ clears file
    filePtr = fopen(fileName,"w+");
    writeToFile(filePtr, list, listSize);
    fclose(filePtr);



    printMenu(); //prints the menu

    while(keepGoing)
    {
        printf("Input: ");
        int choice;
        scanf("%d",&choice);
        while(getchar()!='\n'); //waits for user to press enter to prevent error
        switch(choice)
        {
            case 1:
                system("clear");
                printMenu();
              
                printf("\nTasks:\n");
                printList(list,listSize);
              
                printf("\n***********\n");
                printf("\n");
                break;
            case 2:
                system("clear");
                printMenu();
              
                printf("Input task: ");
                addToList(list, &listSize);


                
                filePtr = fopen(fileName,"w+"); //clears contents of the file
                writeToFile(filePtr, list, listSize); //rewrites all data from list array to file
                fclose(filePtr);
              
                break;
            case 3:
                system("clear");
                printMenu();
                
                printList(list,listSize);

                printf("***********\n");
                printf("Type index that you want to remove\n");
              
                int index;
                scanf("%d",&index);
                while(getchar() != '\n');
              
                removeItem(list, &listSize, index);

                
                filePtr = fopen(fileName,"w+"); //clears contents of file
                writeToFile(filePtr, list, listSize); //rewrites all data from list array to file
                fclose(filePtr);
              
                break;
            case 4:
              system("clear");
              int choiceToClear;
              printf("Are you sure you want to clear everything? 1 for yes, 0 for no: ");
              scanf("%d",&choiceToClear);

              while(getchar() != '\n');

              if(choiceToClear == 1)
              {
                clearList(list,&listSize); //clears list array
                filePtr = fopen(fileName,"w+"); //clears file
                fclose(filePtr);
              }
              system("clear");
              printMenu();
              break;
            case 5:
              keepGoing = 0; //ends program
              system("clear");
              printf("Program ended\n");
              break;
            default:
                system("clear");
                printMenu();
                printf("Not a valid input\n");
        }
    }

    for(int i = 0; i < LIST_SIZE; i++)
    {
        free(list[i]); //frees all memory that was allocated to list array in the beginning
    }
       
    }

    return 0;
}

void signup(FILE *username_file, FILE *password_file) 
{
    char inname[len];
    char inpass[len];

    printf("Enter new username: ");
    fgets(inname, sizeof(inname), stdin);
    strtok(inname, "\n");      //removes newline character

    printf("Set new password: ");
    fgets(inpass, sizeof(inpass), stdin);
    strtok(inpass, "\n");

    // Write username and password to files
    fprintf(username_file, "%s\n", inname);
    fprintf(password_file, "%s\n", inpass);

    printf("User successfully registered\n");

    // Rewind file pointers to the beginning
    rewind(username_file);
    rewind(password_file);
}


bool login(FILE *username_file, FILE *password_file, bool *login_successful)    //Login function with data type bool as it returns a bool value
{
    char username[len];
    char password[len];
    char stored_username[len];
    char stored_password[len];
    bool found = false;

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");   //removes newline character

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n");

    // Rewind files to the beginning so all file elements can be checked with the corresponding element in order
    rewind(username_file);    
    rewind(password_file);

    // Check if the entered username exists in the username file
    while (fgets(stored_username, sizeof(stored_username), username_file) != NULL) 
    {
        strtok(stored_username, "\n");
        // Read the corresponding password from the password file
        if (fgets(stored_password, sizeof(stored_password), password_file) != NULL)
         {
            strtok(stored_password, "\n");
            // Check if the entered username and password match the stored ones
            if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0)
            {
                found = true;
                break;
            }
        }
    }

    if (found)
     {
        system("clear");
        printf("Login successful\n");
        *login_successful = true; // Set login_successful to true
    } else 
    {
        printf("Invalid username or password\n");
    }

    return found; // Return whether login was successful
}
