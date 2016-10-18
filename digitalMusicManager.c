/*******************************************************************************************
* Student: Vikram Singh StudentID: 11491025                                                *
* Class: CptS 122, Fall  2016; Lab Section 5                                               *
* Programming Assignment: Programming Assignment 2					                       *
* Date: January 29, 2016                                                                   *
* Description: Writing a program that is a Digital music manager						   *
*******************************************************************************************/
//include local libraries
#include "digitalMusicManager.h"

/*************************************************************
* Function: void mainDigitalManager(void)                    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up main menu                             *
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void mainDigitalManager(void)
{
	//head of the link list
	int sortOption = 1;
	MusicNode *mainHead = NULL;

	while (1)
	{ 
		//Start main menu
		switch (mainMenu())
		{
		case 1:
			if (mainHead == NULL)
			{
				//Load from while error check for crash
				if (loadFromFile(&mainHead))
				{
					puts("Done loading from the file returning to main menu.");
					handleSortRequest(&mainHead, &sortOption);
				}
				else
					puts("Error: Couldn't load from file. Returning to main menu.");
			}
			else
			{
				puts("Music already loaded.");
			}
			system("pause");
			break;
		case 2:
			//save linked list to file
			if (mainHead != NULL)
			{
				//Load from while error check for crash
				if (storeMusicList(mainHead))
					puts("Done saving songs to savedList.txt returning to main menu.");
				else
					puts("Error: Couldn't save to file. Returning to main menu.");
			}
			else
			{
				puts("Please load music.");
			}
			system("pause");
			break;
		case 3:
			//search through the linked list
			searchOptionHandler(mainHead);
			break;
		case 4:
			//edit linked list
			if (mainHead != NULL)
			{
				insertOrDelete(&mainHead);
				handleSortRequest(&mainHead, &sortOption);
			}
			else
			{
				puts("Error: Please load from a file first.");
				system("pause");
			}

			break;
		case 5:
			//sort linked list
			if (mainHead != NULL)
				sortByAnything(&mainHead, &sortOption);
			else
				puts("Please load music.");
			system("pause");
			break;
		case 6:
			//Change rating to any song
			if (mainHead != NULL)
				rateMusic(mainHead);
			else
				puts("Please load music.");
			system("pause");
			break;
		case 7:
			//exit the program returning 0 save record if not empty
			if (mainHead != NULL)
				storeMusicList(mainHead);
			exit(0);
			break;
		default:
			break;
		}
		//clear screen before returning to main menu
		system("cls");
	}
}

/*************************************************************
* Function: int mainMenu (void)                              *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up main menu gets input                  *
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
int mainMenu (void)
{
	//local var
	int selectedByUser = 0;

	//do while for collection user data
	do{

		//print menu and collection option
		mainMenuWrittenOptions();
		printf("Please make a selection: ");
		scanf(" %d",& selectedByUser);

		//let user know of mistake ahd pause to read
		if ((selectedByUser <= 0) || (selectedByUser >= 8))
		{
			printf("Please select an option between 1-7. You selected: %d.\n", selectedByUser);
			system("pause");
		}

		//clear screeen
		system("cls");

	} while ((selectedByUser <= 0) || (selectedByUser >= 8));

	//return option
	return selectedByUser;


}

/*************************************************************
* Function: void mainMenuWrittenOptions (void)               *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: print options for menu                        *
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void mainMenuWrittenOptions(void)
{
	//prints options and brief and intro
	puts("This is Digital Music Manager this program will help you manage all your music!");

	//load, store, display, insert, delete, edit, sort, rate, and exit
	printf("Please select from one of these options.\n"
			"\t1. Load\n"
			"\t2. Store\n"
			"\t3. Display\n"
			"\t4. Edit(change song info/insert or delete)\n"
			"\t5. Sort\n"
			"\t6. Rate\n"
			"\t7. Exit\n");
}

/*************************************************************
* Function:musicNode* allocateMemory(artistName, albumTitle, *
*		songTitle, minutes, seconds, timesPlayed, rating)    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: allocates memory and places data in place     *
* Input parameters: info required to fill the structs        *
* Returns: musicNode*                                        *
* Preconditions: data must be passed                         *
* Postconditions: none                                       *
*************************************************************/
MusicNode* allocateMemory(char *artistName, char *albumTitle, char *songTitle, char *genre,
							int minutes, int seconds, int timesPlayed, int rating)
{
	//local vars
	MusicNode* temp = NULL;

	//allocate space for struct
	temp = (MusicNode *)malloc(sizeof(MusicNode));

	//dynamiclly get memory for for all strings
	temp->mainData.artistName = (char *)malloc(strlen(artistName)*sizeof(char) + 1);
	temp->mainData.albumTitle = (char *)malloc(strlen(albumTitle)*sizeof(char) + 1);
	temp->mainData.songTitle = (char *)malloc(strlen(songTitle)*sizeof(char) + 1);
	temp->mainData.genre = (char *)malloc(strlen(genre)*sizeof(char) + 1);

	//Set all values
	strcpy(temp->mainData.artistName, artistName);
	strcpy(temp->mainData.albumTitle, albumTitle);
	strcpy(temp->mainData.songTitle, songTitle);
	strcpy(temp->mainData.genre, genre);
	temp->mainData.songLength.minutes = minutes;
	temp->mainData.songLength.seconds = seconds;
	temp->mainData.timesPlayed = timesPlayed;
	temp->mainData.rating = rating;

	//set pointers values
	temp->pNext = NULL;
	temp->pPrevious = NULL;

	//return struct
	return temp;
}

/*************************************************************
* Function: int loadFromFile(MusicNode *pHeadDMM)			 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: allocates memory and places data in place     *
* Input parameters: head of list                             *
* Returns: True or False                                     *
* Preconditions: head must be passed in                      *
* Postconditions: none                                       *
*************************************************************/
int loadFromFile(MusicNode **pHeadDMM)
{
	//local vars
	char songInfoString[4][100] = {" "," "," ", " "}; //1: artistName 2: albumTitle 3: songTitle 4: genre
	int songTitleInt[4] = {0}; //0.song minutes 1. song seconds 2. timesplayed 3. rating
	int success = 1, stringLength = 0, counter = 0; //return variable for function
	MusicNode *temp = NULL, *lastNode = NULL; //try the double link use last node for it
	FILE *dataInFile = fopen("toBeLoaded.txt","r"); //file that needs to be opened

	//check file openning
	if (dataInFile != NULL)
	{
		//open file till end 
		while (!feof(dataInFile))
		{	
			//extract info from file
			fgets(&songInfoString[0][0], 99, dataInFile); //get artistName
			//magange size (dirty hack to manage to fgets)
			counter = 0;
			stringLength = strlen(&songInfoString[0][0]);
			for (counter; counter < stringLength; counter++)
			{
				if(songInfoString[0][counter] == '\n')
					songInfoString[0][counter] = '\0';
			}
			

			fgets(&songInfoString[1][0], 99, dataInFile); //get albumTitle
			//magange size (dirty hack to manage to fgets)
			counter = 0;
			stringLength = strlen(&songInfoString[1][0]);
			for (counter; counter < stringLength; counter++)
			{
				if(songInfoString[1][counter] == '\n')
					songInfoString[1][counter] = '\0';
			}

			fgets(&songInfoString[2][0], 99, dataInFile); //get songTitle
			//magange size (dirty hack to manage to fgets)
			counter = 0;
			stringLength = strlen(&songInfoString[2][0]);
			for (counter; counter < stringLength; counter++)
			{
				if(songInfoString[2][counter] == '\n')
					songInfoString[2][counter] = '\0';
			}

			fgets(&songInfoString[3][0], 99, dataInFile); //get genre
			//magange size (dirty hack to manage to fgets)
			counter = 0;
			stringLength = strlen(&songInfoString[3][0]);
			for (counter; counter < stringLength; counter++)
			{
				if(songInfoString[3][counter] == '\n')
					songInfoString[3][counter] = '\0';
			}
			//finish scanning in rest of variables
			fscanf(dataInFile, "\n%d:%d\n%d\n%d\n", &songTitleInt[0], &songTitleInt[1], //set the format for minutes, seconds,
									&songTitleInt[2], &songTitleInt[3]); 				//timesplayed, and rating.

			//Make struct for node 
			temp = allocateMemory(&songInfoString[0][0], &songInfoString[1][0], &songInfoString[2][0], &songInfoString[3][0],
									songTitleInt[0], songTitleInt[1], songTitleInt[2], songTitleInt[3]);

			if (temp != NULL)
			{
				//list is empty
				if (*pHeadDMM == NULL)
				{
					*pHeadDMM = temp;
					lastNode = temp;
				}
				else
				{
					//not empty so start linking
					temp->pNext = *pHeadDMM;
					(*pHeadDMM)->pPrevious = temp;
					*pHeadDMM = temp;

					//links everything together
					(*pHeadDMM)->pPrevious = lastNode;
					lastNode->pNext = temp;
				}
				

			}
			else
			{
				//show error message go back to menu
				puts("Error: Couldn't allocate memory for song data from file going back to menu.");
				system("pause");
				success = 0;
				return success;
			}
		}
	} else {
		//show error message go back to main menu
		puts("Error: Couldn't open file.");
		system("pause");
		success = 0;
		return success;
	}
	return success; //return success
}

/*************************************************************
* Function: void printMusicList(MusicNode *pHeadDMM)		 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: Prints linked list                            *
* Input parameters: head of list                             *
* Returns: nothing                                           *
* Preconditions: head must be passed in                      *
* Postconditions: none                                       *
*************************************************************/
int printMusicList(MusicNode *pHeadDMM)
{
	//local var
	int success = 0;
	MusicNode *pCurrentMusicNode = NULL;

	//set current to head
	pCurrentMusicNode = pHeadDMM;

	//print all content
	while(pCurrentMusicNode != NULL)
	{
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			   "Album: %s\n"
			   "Song: %s\n"
			   "Genre: %s\n"
			   "Time: %d:%d\n"
			   "Times played: %d\n"
			   "Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, 
			   				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			   				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, 
			   				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);
		//itterate to next one
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//stop print once looped around breaks out
		if (pCurrentMusicNode == pHeadDMM)
		{
			success = 1;
			break;
		}
	}

	//return success
	return success;

}

/*************************************************************
* Function: void storeMusicList(MusicNode *pHeadDMM)		 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: Prints linked list to a file                  *
* Input parameters: head of list                             *
* Returns: nothing                                           *
* Preconditions: head must be passed in                      *
* Postconditions: none                                       *
*************************************************************/
int storeMusicList(MusicNode *pHeadDMM)
{
	//local var
	int successs = 0;
	MusicNode *pCurrentMusicNode = pHeadDMM;
	FILE *outputFIle = fopen("toBeLoaded.txt", "w");

	while (pCurrentMusicNode != NULL)
	{
		//print everything proprely
		fprintf( outputFIle, "%s\n"
			"%s\n"
			"%s\n"
			"%s\n"
			"%d:%d\n"
			"%d\n"
			"%d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		//itterate to next one
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//stop print once looped around breaks out
		if (pCurrentMusicNode == pHeadDMM)
		{
			successs = 1;
			break;
		}
	}

	return successs;

}

/*************************************************************
* Function: void searchOptions(void)						 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options							 *
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchOptions(void)
{
	//prints options and brief and intro
	puts("Please select from the following search critrias.");

	//load, store, display, insert, delete, edit, sort, rate, and exit
	printf("Please select from one of these options.\n"
		"\t1. Artist\n"
		"\t2. Album title\n"
		"\t3. Song title\n"
		"\t4. Genre\n"
		"\t5. Song Legth\n"
		"\t6. Times played\n"
		"\t7. Rating\n"
		"\t8. Print All\n"
		"\t9. Main Menu\n" );
}

/*************************************************************
* Function: int searchOptionSelector(void)					 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
int searchOptionSelector(void)
{
	//local var
	int selectedByUser = 0;

	//do while for collection user data
	do {

		//print menu and collection option
		searchOptions();
		printf("Please make a selection: ");
		scanf(" %d", &selectedByUser);

		//let user know of mistake ahd pause to read
		if ((selectedByUser <= 0) || (selectedByUser >= 10))
		{
			printf("Please select an option between 1-10. You selected: %d.\n", selectedByUser);
			system("pause");
		}

		//clear screeen
		system("cls");

	} while ((selectedByUser <= 0) || (selectedByUser >= 10));

	//return option
	return selectedByUser;
}

/*************************************************************
* Function: int searchOptionHandler(void)					 *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchOptionHandler(MusicNode *mainHead)
{
	//local var
	int toExit = 0;

	//set toExit sanity check
	toExit = 1;

	//Run to have this till exit
	while(toExit)
	{
		switch (searchOptionSelector())
		{
		case 1:
			//Search by artist error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByArtist(mainHead);//search by artist
			break;
		case 2:
			//Search by album error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByAlbum(mainHead);
			break;
		case 3:
			//Search by song error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchBySong(mainHead);
			break;
		case 4:
			//Search by artist error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByGenre(mainHead);
			break;
		case 5:
			//Search by song length error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByTime(mainHead);
			break;
		case 6:
			//Search by times played error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByTimesPlayed(mainHead);
			break;
		case 7:
			//Search by rating error check for empty head pointer
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				searchByRating(mainHead);
			break;
		case 8:
			//Print all list error check for empty list
			if (mainHead == NULL)
				puts("Please load from file or enter info");
			else
				printMusicList(mainHead);
			break;
		case 9:
			//Return to main menu set exit to false
			puts("Returning to main menu.");
			toExit = 0;
			break;
		}
		//pause and than clear
		system("pause");
		system("cls");
	}
}

/*************************************************************
* Function: void searchByArtist(MusicNode *mainHead)	     *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByArtist(MusicNode *mainHead)
{
	//local vars
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Artist: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.artistName, searchFor) != 0)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.artistName) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find artist.");
	}
}

/*************************************************************
* Function: void searchByAlbum(MusicNode *mainHead)    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByAlbum(MusicNode *mainHead)
{
	//local vars
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Album: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.albumTitle, searchFor) != 0)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.albumTitle) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find album.");
	}
}

/*************************************************************
* Function: void searchBySong(MusicNode *mainHead)           *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchBySong(MusicNode *mainHead)
{
	//local vars
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Song: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.songTitle, searchFor) != 0)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.songTitle) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find song.");
	}
}

/*************************************************************
* Function: void searchByGenre(MusicNode *mainHead)           *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByGenre(MusicNode *mainHead)
{
	//local vars
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Genre: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.genre, searchFor) != 0)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.genre) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find genre.");
	}
}

/*************************************************************
* Function: void searchByTimesPlayed(MusicNode *mainHead)    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByTimesPlayed(MusicNode *mainHead)
{
	//local vars
	int searchFor = 0;
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Times played: ");
	scanf("%d", &searchFor);

	while (searchFor != pCurrentMusicNode->mainData.timesPlayed)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (searchFor == pCurrentMusicNode->mainData.timesPlayed)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find times played.");
	}
}

/*************************************************************
* Function: void searchByRating(MusicNode *mainHead)    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByRating(MusicNode *mainHead)
{
	//local vars
	int searchFor = 0;
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter rating to search for: ");
	scanf("%d", &searchFor);

	while (searchFor != pCurrentMusicNode->mainData.rating)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (searchFor == pCurrentMusicNode->mainData.rating)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find that rating.");
	}
}

/*************************************************************
* Function: void searchByTime(MusicNode *mainHead)    *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:Prints search options and return user selection*
* Input parameters: none                                     *
* Returns: user choice                                       *
* Preconditions:											 *
* Postconditions: none                                       *
*************************************************************/
void searchByTime(MusicNode *mainHead)
{
	//local vars
	int minutes = 0, seconds = 0;
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter length of song to search for(Example: 3:24 would be 3 24): ");
	scanf("%d %d", &minutes, &seconds);

	while ((minutes != pCurrentMusicNode->mainData.songLength.minutes) && (seconds != pCurrentMusicNode->mainData.songLength.seconds))
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if ((minutes == pCurrentMusicNode->mainData.songLength.minutes) && (seconds == pCurrentMusicNode->mainData.songLength.seconds))
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

	}
	else
	{
		puts("Couldn't find that time.");
	}
}

/*************************************************************
* Function: int rateMusi (MusicNode* mainnode)               *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up song search gets input replace ratting*
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void rateMusic(MusicNode *mainHead)
{
	//local vars
	int rating = 0;
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = mainHead;

	//Get user search query
	printf("Please enter Song name that you want to chanage the rating for: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.songTitle, searchFor) != 0)
	{
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.songTitle) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		//ask for rating
		printf("Please give new rating: ");
		scanf("%d", &rating);

		//clear screen
		system("cls");

		//change rating
		pCurrentMusicNode->mainData.rating = rating;

		puts("Ratting Changed.");

		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);
	}
	else
	{
		puts("Couldn't find song.");
	}
}
/*************************************************************
* Function: void insertOrDelete (MusicNode* mainnode)        *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description:                                               *
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void insertOrDelete(MusicNode **pHeadmain)
{
	//local var
	int choice = 0;

	do
	{
		//get if they want to insert or delete
		printf("Please make a selection\n"
			"\t1. Insert.\n"
			"\t2. Delete\n"
			"\t3. Edit\n");
		printf("Choice: ");
		scanf("%d", &choice);
	} while ((choice <= 0)||(choice >= 4));
	

	switch (choice)
	{
	case 1:
		//clear screen you selected insert
		system("cls");
		if (*pHeadmain != NULL)
			insertSong(pHeadmain);
		else
			puts("Please load music first from file.");
		break;
	case 2:
		//clear you selected to delete
		system("cls");
		if (*pHeadmain != NULL)
			deleteSong(pHeadmain);
		else
			puts("Please load music first from file.");
		break;
	case 3:
		//clear you selected to delete
		system("cls");
		if (*pHeadmain != NULL)
			editAnythingOption(pHeadmain);
		else
			puts("Please load music first from file.");
		break;
	}
	system("pause");
}
/*************************************************************
* Function: int deleteSong (MusicNode **mainnode)            *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up song search gets input replace ratting*
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void deleteSong(MusicNode **mainHead)
{
	//local vars
	int rating = 0;
	char searchFor[100] = " ";
	MusicNode *pCurrentMusicNode = *mainHead, *previousMusicNode = NULL, *temp = NULL;

	//Get user search query
	printf("Please enter Song name that you want to delete: ");
	getc(stdin);
	fgets(searchFor, 99, stdin);
	searchFor[strlen(searchFor) - 1] = '\0';

	while (strcmp(pCurrentMusicNode->mainData.songTitle, searchFor) != 0)
	{
		previousMusicNode = pCurrentMusicNode;
		pCurrentMusicNode = pCurrentMusicNode->pNext;

		//break loop once checked
		if (pCurrentMusicNode == *mainHead)
			break;
	}

	if (strcmp(searchFor, pCurrentMusicNode->mainData.songTitle) == 0)
	{
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		//ask for rating
		printf("Are you sure you want to delete this song?(Enter: (1 for yes) (2 for no)): ");
		scanf("%d", &rating);

		if (rating)
		{
			if (pCurrentMusicNode == *mainHead)
			{
				temp = pCurrentMusicNode->pPrevious;
				*mainHead = pCurrentMusicNode->pNext;
				(*mainHead)->pPrevious = pCurrentMusicNode->pPrevious;
				temp->pNext = pCurrentMusicNode->pNext;
				free(pCurrentMusicNode);
			}
			else
			{
				previousMusicNode->pNext = pCurrentMusicNode->pNext;
				temp = pCurrentMusicNode->pNext;
				temp->pPrevious = previousMusicNode;
			}
		}

		
	}
	else
	{
		puts("Couldn't find song.");
	}
}
/*************************************************************
* Function: int insertSong (MusicNode **mainnode)            *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up song search gets input replace ratting*
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void insertSong(MusicNode **mainHead)
{
	char songName[100] = " ", albumTitle[100] = " ", artist[100] = " ", genre[100] = " ";
	int minutes = 0, seconds = 0, timesPlayed = 0, ratting = 0;
	MusicNode* temp = NULL, *previousTemp = NULL;

	puts("Please provide information to add to your library.");
	printf("Artist: ");
	getc(stdin);
	fgets(artist, 99, stdin);
	artist[strlen(artist) - 1] = '\0';

	printf("Album Title: ");
	fgets(albumTitle, 99, stdin);
	albumTitle[strlen(albumTitle) - 1] = '\0';

	printf("Song Title: ");
	fgets(songName, 99, stdin);
	songName[strlen(songName) - 1] = '\0';

	printf("Genre: ");
	fgets(genre, 99, stdin);
	genre[strlen(genre) - 1] = '\0';

	printf("Length(Example: 04:34 would be enterted as 4 34): ");
	scanf(" %d %d", &minutes, &seconds);

	printf("Enter timed previous played: ");
	scanf("%d", &timesPlayed);

	printf("Enter ratting of the song between 1 and 5: ");
	scanf("%d", &ratting);

	temp = allocateMemory(artist, albumTitle, songName, genre, minutes, seconds, timesPlayed, ratting);

	previousTemp = (*mainHead)->pPrevious;
	(*mainHead)->pPrevious = temp;
	temp->pNext = *mainHead;
	temp->pPrevious = previousTemp;
	previousTemp->pNext = temp;
}

/*************************************************************
* Function: int editAnythingOption (MusicNode *mainnode)     *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: sets up song search gets input replace ratting*
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void editAnythingOption(MusicNode **mainnode)
{
	//local vars
	int choice = 0;
	char searchQuery[100] = " ";
	MusicNode *pCurrentMusicNode = *mainnode;

	//ask user what they want to search by very lazy no error checking
	do
	{
		printf("Please choose a search option.\n"
			"\t1. Artist\n"
			"\t2. Album\n"
			"\t3. Song\n"
			"\t4. Genre\n"
			"Choice: ");
		scanf("%d", &choice);
	} while ((choice <= 0)||(choice >= 5));

	switch (choice)
	{
	case 1:
		//get info
		printf("Artist: ");
		getc(stdin);
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';

		while (strcmp(pCurrentMusicNode->mainData.artistName, searchQuery) != 0)
		{
			pCurrentMusicNode = pCurrentMusicNode->pNext;

			//break loop once checked
			if (pCurrentMusicNode == *mainnode)
				break;
		}
		if (strcmp(searchQuery, pCurrentMusicNode->mainData.artistName) == 0)
		{
		//clears screen
		system("cls");
		puts("Found artist.");
		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		puts("Type new info all information must be provided!");
		printf("Artist: ");
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';
		free(pCurrentMusicNode->mainData.artistName);
		pCurrentMusicNode->mainData.artistName = (char *)malloc(strlen(searchQuery)*sizeof(char) + 1);
		strcpy(pCurrentMusicNode->mainData.artistName, searchQuery);



		printf("Album: ");
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';
		free(pCurrentMusicNode->mainData.albumTitle);
		pCurrentMusicNode->mainData.albumTitle = (char *)malloc(strlen(searchQuery)*sizeof(char) + 1);
		strcpy(pCurrentMusicNode->mainData.albumTitle, searchQuery);

		printf("Song: ");
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';
		free(pCurrentMusicNode->mainData.songTitle);
		pCurrentMusicNode->mainData.songTitle = (char *)malloc(strlen(searchQuery)*sizeof(char) + 1);
		strcpy(pCurrentMusicNode->mainData.songTitle, searchQuery);

		printf("Genre: ");
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';
		free(pCurrentMusicNode->mainData.genre);
		pCurrentMusicNode->mainData.genre = (char *)malloc(strlen(searchQuery)*sizeof(char) + 1);
		strcpy(pCurrentMusicNode->mainData.genre, searchQuery);

		printf("Time(Example 4:50 would be 4 50): ");
		scanf("%d %d", &pCurrentMusicNode->mainData.songLength.minutes, &pCurrentMusicNode->mainData.songLength.seconds);

		printf("Time played: ");
		scanf("%d", &pCurrentMusicNode->mainData.timesPlayed);

		printf("Rating: ");
		scanf("%d", &pCurrentMusicNode->mainData.rating);

		//print everything proprely
		printf("=================================\n"
			"Artist Name: %s\n"
			"Album: %s\n"
			"Song: %s\n"
			"Genre: %s\n"
			"Time: %d:%d\n"
			"Times played: %d\n"
			"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
			pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
			pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
			pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		}
		else
		{
		puts("Couldn't find artist.");
		}

		break;
	case 2:
		//get info
		printf("Album: ");
		getc(stdin);
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';

		while (strcmp(pCurrentMusicNode->mainData.albumTitle, searchQuery) != 0)
		{
			pCurrentMusicNode = pCurrentMusicNode->pNext;

			//break loop once checked
			if (pCurrentMusicNode == *mainnode)
				break;
		}
		if (strcmp(searchQuery, pCurrentMusicNode->mainData.albumTitle) == 0)
		{
			//clears screen
			system("cls");
			puts("Found artist.");
			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			puts("Type new info all information must be provided!");
			printf("Artist: ");
			fgets(pCurrentMusicNode->mainData.artistName, 99, stdin);
			pCurrentMusicNode->mainData.artistName[strlen(pCurrentMusicNode->mainData.artistName) - 1] = '\0';

			printf("Album: ");
			fgets(pCurrentMusicNode->mainData.albumTitle, 99, stdin);
			pCurrentMusicNode->mainData.albumTitle[strlen(pCurrentMusicNode->mainData.albumTitle) - 1] = '\0';

			printf("Song: ");
			fgets(pCurrentMusicNode->mainData.songTitle, 99, stdin);
			pCurrentMusicNode->mainData.songTitle[strlen(pCurrentMusicNode->mainData.songTitle) - 1] = '\0';

			printf("Genre: ");
			fgets(pCurrentMusicNode->mainData.genre, 99, stdin);
			pCurrentMusicNode->mainData.genre[strlen(pCurrentMusicNode->mainData.genre) - 1] = '\0';

			printf("Time(Example 4:50 would be 4 50): ");
			scanf("%d %d", &pCurrentMusicNode->mainData.songLength.minutes, &pCurrentMusicNode->mainData.songLength.seconds);

			printf("Time played: ");
			scanf("%d", &pCurrentMusicNode->mainData.timesPlayed);

			printf("Rating: ");
			scanf("%d", &pCurrentMusicNode->mainData.rating);

			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		}
		else
		{
			puts("Couldn't find Album.");
		}


		break;
	case 3:
		//get info
		printf("Song Title: ");
		getc(stdin);
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';

		while (strcmp(pCurrentMusicNode->mainData.songTitle, searchQuery) != 0)
		{
			pCurrentMusicNode = pCurrentMusicNode->pNext;

			//break loop once checked
			if (pCurrentMusicNode == *mainnode)
				break;
		}
		if (strcmp(searchQuery, pCurrentMusicNode->mainData.songTitle) == 0)
		{
			//clears screen
			system("cls");
			puts("Found artist.");
			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			puts("Type new info all information must be provided!");
			printf("Artist: ");
			fgets(pCurrentMusicNode->mainData.artistName, 99, stdin);
			pCurrentMusicNode->mainData.artistName[strlen(pCurrentMusicNode->mainData.artistName) - 1] = '\0';

			printf("Album: ");
			fgets(pCurrentMusicNode->mainData.albumTitle, 99, stdin);
			pCurrentMusicNode->mainData.albumTitle[strlen(pCurrentMusicNode->mainData.albumTitle) - 1] = '\0';

			printf("Song: ");
			fgets(pCurrentMusicNode->mainData.songTitle, 99, stdin);
			pCurrentMusicNode->mainData.songTitle[strlen(pCurrentMusicNode->mainData.songTitle) - 1] = '\0';

			printf("Genre: ");
			fgets(pCurrentMusicNode->mainData.genre, 99, stdin);
			pCurrentMusicNode->mainData.genre[strlen(pCurrentMusicNode->mainData.genre) - 1] = '\0';

			printf("Time(Example 4:50 would be 4 50): ");
			scanf("%d %d", &pCurrentMusicNode->mainData.songLength.minutes, &pCurrentMusicNode->mainData.songLength.seconds);

			printf("Time played: ");
			scanf("%d", &pCurrentMusicNode->mainData.timesPlayed);

			printf("Rating: ");
			scanf("%d", &pCurrentMusicNode->mainData.rating);

			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		}
		else
		{
			puts("Couldn't find Song.");
		}
		break;
	case 4:
		//get info
		printf("Genre: ");
		getc(stdin);
		fgets(searchQuery, 99, stdin);
		searchQuery[strlen(searchQuery) - 1] = '\0';

		while (strcmp(pCurrentMusicNode->mainData.genre, searchQuery) != 0)
		{
			pCurrentMusicNode = pCurrentMusicNode->pNext;

			//break loop once checked
			if (pCurrentMusicNode == *mainnode)
				break;
		}
		if (strcmp(searchQuery, pCurrentMusicNode->mainData.genre) == 0)
		{
			//clears screen
			system("cls");
			puts("Found artist.");
			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			puts("Type new info all information must be provided!");
			printf("Artist: ");
			fgets(pCurrentMusicNode->mainData.artistName, 99, stdin);
			pCurrentMusicNode->mainData.artistName[strlen(pCurrentMusicNode->mainData.artistName) - 1] = '\0';

			printf("Album: ");
			fgets(pCurrentMusicNode->mainData.albumTitle, 99, stdin);
			pCurrentMusicNode->mainData.albumTitle[strlen(pCurrentMusicNode->mainData.albumTitle) - 1] = '\0';

			printf("Song: ");
			fgets(pCurrentMusicNode->mainData.songTitle, 99, stdin);
			pCurrentMusicNode->mainData.songTitle[strlen(pCurrentMusicNode->mainData.songTitle) - 1] = '\0';

			printf("Genre: ");
			fgets(pCurrentMusicNode->mainData.genre, 99, stdin);
			pCurrentMusicNode->mainData.genre[strlen(pCurrentMusicNode->mainData.genre) - 1] = '\0';

			printf("Time(Example 4:50 would be 4 50): ");
			scanf("%d %d", &pCurrentMusicNode->mainData.songLength.minutes, &pCurrentMusicNode->mainData.songLength.seconds);

			printf("Time played: ");
			scanf("%d", &pCurrentMusicNode->mainData.timesPlayed);

			printf("Rating: ");
			scanf("%d", &pCurrentMusicNode->mainData.rating);

			//print everything proprely
			printf("=================================\n"
				"Artist Name: %s\n"
				"Album: %s\n"
				"Song: %s\n"
				"Genre: %s\n"
				"Time: %d:%d\n"
				"Times played: %d\n"
				"Rating: %d\n", pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle,
				pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre,
				pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds,
				pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

		}
		else
		{
			puts("Couldn't find Album.");
		}
		break;
	default:
		break;
	}
}

/*************************************************************
* Function: void sortByAnything (MusicNode **mainnode, 		 *
									int *sort)               *
* Date Created: 01/28/2015                                   *
* Date Last Modified: 01/28/2015                             *
* Description: Set up facualty to sort 	                     *
* Input parameters: none                                     *
* Returns: Nothing                                           *
* Preconditions: Nothing                                     *
* Postconditions: none                                       *
*************************************************************/
void sortByAnything(MusicNode **mainnode, int *sort)
{
	//find what you want to sort by
	int choice = 0;

	if (choice == 0)
	{
		do {
			puts("Please make a selection on what you would like to sort by.");
			printf("\t1. Artist\n"
				"\t2. Album\n"
				"\t3. Song\n"
				"\t4. Rating\n"
				"\t5. Times played\n"
				"Choice: ");
			scanf("%d", &choice);
		} while ((choice <= 0) || (choice >= 6));
	}

	switch (choice)
	{
	case 1:
		//set sort to artist
		*sort = 1;
		handleSortRequest(mainnode,sort);
		break;
	case 2:
		//set sort to album
		*sort = 2;
		handleSortRequest(mainnode,sort);
		break;
	case 3:
		//set sort to song
		*sort = 3;
		handleSortRequest(mainnode,sort);
		break;
	case 4:
		//set sort to rating
		*sort = 4;
		handleSortRequest(mainnode,sort);
		break;
	case 5:
		//set sort to Times Played
		*sort = 5;
		handleSortRequest(mainnode,sort);
		break;
	default:
		break;
	}
}
	/*************************************************************
	* Function: void handleSortRequest (MusicNode **mainnode, 	 *
	int *sort)													 *
	* Date Created: 01/28/2015                                   *
	* Date Last Modified: 01/28/2015                             *
	* Description: Set to sort 	                                 *
	* Input parameters: none                                     *
	* Returns: Nothing                                           *
	* Preconditions: Nothing                                     *
	* Postconditions: none                                       *
	*************************************************************/
void handleSortRequest(MusicNode **mainNode, int *sort)
{
	// strat make a new list add 1 item at a time and check with linked list
	MusicNode *pCurrentMusicNode = NULL, *temp = NULL, *newList = NULL, *counterN = NULL, *counterP = NULL;

	

	switch (*sort)
	{
	case 1:
		//set up starting point
		pCurrentMusicNode = *mainNode;

		//break list
		(pCurrentMusicNode->pPrevious)->pNext = NULL;
		pCurrentMusicNode->pPrevious = NULL;
		
		while(pCurrentMusicNode != NULL)
		{
			temp = allocateMemory(pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre, pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);
			
			if (newList == NULL)
				newList = temp;
			else
			{
				counterN = newList;

				while ((counterN != NULL) && (strcmp(temp->mainData.artistName, counterN->mainData.artistName) > 0))
				{
					counterP = counterN;
					counterN = counterN->pNext;
				}
				if (counterN == newList)
				{
					temp->pNext = newList;
					newList->pPrevious = temp;
					newList = temp;
				}
				else {
					temp->pNext = counterN;
					if (counterN != NULL)
						counterN->pPrevious = temp;
					counterP->pNext = temp;
					temp->pPrevious = counterP;
				}

			}

			pCurrentMusicNode = pCurrentMusicNode->pNext;
		}

		//make the list circular
		counterN = newList;

		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
		}

		newList->pPrevious = counterP;
		counterP->pNext = newList;

		counterN = *mainNode;
		//free old list
		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
			if (counterP != NULL)
				free(counterP);
		}

		*mainNode = newList;



		// while (pUnsorted->pPrevious != NULL)
		// {
		// 	pCurrentMusicNode = mainNode->pNext;

		// 	while(pCurrentMusicNode != pUnsorted)
		// 	{
		// 		if (strcmp(pCurrentMusicNode->mainData.artistName, (pCurrentMusicNode->pPrevious)->mainData.artistName) < 0)
		// 		{
		// 			swapData(pCurrentMusicNode, pCurrentMusicNode->pPrevious);
		// 		}
				
		// 		pCurrentMusicNode = pCurrentMusicNode->pNext;

		// 	}

		// 	pUnsorted = pUnsorted->pPrevious;
		// }

		
		
		

	
		break;
	case 2:
		//set up starting point
		pCurrentMusicNode = *mainNode;

		//break list
		(pCurrentMusicNode->pPrevious)->pNext = NULL;
		pCurrentMusicNode->pPrevious = NULL;

		while (pCurrentMusicNode != NULL)
		{
			temp = allocateMemory(pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre, pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			if (newList == NULL)
				newList = temp;
			else
			{
				counterN = newList;

				while ((counterN != NULL) && (strcmp(temp->mainData.albumTitle, counterN->mainData.albumTitle) > 0))
				{
					counterP = counterN;
					counterN = counterN->pNext;
				}
				if (counterN == newList)
				{
					temp->pNext = newList;
					newList->pPrevious = temp;
					newList = temp;
				}
				else {
					temp->pNext = counterN;
					if (counterN != NULL)
						counterN->pPrevious = temp;
					counterP->pNext = temp;
					temp->pPrevious = counterP;
				}

			}

			pCurrentMusicNode = pCurrentMusicNode->pNext;
		}

		//make the list circular
		counterN = newList;

		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
		}

		newList->pPrevious = counterP;
		counterP->pNext = newList;
		//free old memory
		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
			if (counterP != NULL)
				free(counterP);
		}

		*mainNode = newList;

		
		break;
	case 3:
		//set up starting point
		pCurrentMusicNode = *mainNode;

		//break list
		(pCurrentMusicNode->pPrevious)->pNext = NULL;
		pCurrentMusicNode->pPrevious = NULL;

		while (pCurrentMusicNode != NULL)
		{
			temp = allocateMemory(pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre, pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			if (newList == NULL)
				newList = temp;
			else
			{
				counterN = newList;

				while ((counterN != NULL) && (strcmp(temp->mainData.songTitle, counterN->mainData.songTitle) > 0))
				{
					counterP = counterN;
					counterN = counterN->pNext;
				}
				if (counterN == newList)
				{
					temp->pNext = newList;
					newList->pPrevious = temp;
					newList = temp;
				}
				else {
					temp->pNext = counterN;
					if (counterN != NULL)
						counterN->pPrevious = temp;
					counterP->pNext = temp;
					temp->pPrevious = counterP;
				}

			}

			pCurrentMusicNode = pCurrentMusicNode->pNext;
		}

		//make the list circular
		counterN = newList;

		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
		}

		newList->pPrevious = counterP;
		counterP->pNext = newList;
		//free old memory
		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
			if (counterP != NULL)
				free(counterP);
		}

		*mainNode = newList;


		break;
	case 4:
		//set up starting point
		pCurrentMusicNode = *mainNode;

		//break list
		(pCurrentMusicNode->pPrevious)->pNext = NULL;
		pCurrentMusicNode->pPrevious = NULL;

		while (pCurrentMusicNode != NULL)
		{
			temp = allocateMemory(pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre, pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			if (newList == NULL)
				newList = temp;
			else
			{
				counterN = newList;

				while ((counterN != NULL) && (temp->mainData.rating < counterN->mainData.rating))
				{
					counterP = counterN;
					counterN = counterN->pNext;
				}
				if (counterN == newList)
				{
					temp->pNext = newList;
					newList->pPrevious = temp;
					newList = temp;
				}
				else {
					temp->pNext = counterN;
					if (counterN != NULL)
						counterN->pPrevious = temp;
					counterP->pNext = temp;
					temp->pPrevious = counterP;
				}

			}

			pCurrentMusicNode = pCurrentMusicNode->pNext;
		}

		//make the list circular
		counterN = newList;

		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
		}

		newList->pPrevious = counterP;
		counterP->pNext = newList;
		//free old memory
		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
			if (counterP != NULL)
				free(counterP);
		}

		*mainNode = newList;


		break;
	case 5:
		//set up starting point
		pCurrentMusicNode = *mainNode;

		//break list
		(pCurrentMusicNode->pPrevious)->pNext = NULL;
		pCurrentMusicNode->pPrevious = NULL;

		while (pCurrentMusicNode != NULL)
		{
			temp = allocateMemory(pCurrentMusicNode->mainData.artistName, pCurrentMusicNode->mainData.albumTitle, pCurrentMusicNode->mainData.songTitle, pCurrentMusicNode->mainData.genre, pCurrentMusicNode->mainData.songLength.minutes, pCurrentMusicNode->mainData.songLength.seconds, pCurrentMusicNode->mainData.timesPlayed, pCurrentMusicNode->mainData.rating);

			if (newList == NULL)
				newList = temp;
			else
			{
				counterN = newList;

				while ((counterN != NULL) && (temp->mainData.timesPlayed < counterN->mainData.timesPlayed))
				{
					counterP = counterN;
					counterN = counterN->pNext;
				}
				if (counterN == newList)
				{
					temp->pNext = newList;
					newList->pPrevious = temp;
					newList = temp;
				}
				else {
					temp->pNext = counterN;
					if (counterN != NULL)
						counterN->pPrevious = temp;
					counterP->pNext = temp;
					temp->pPrevious = counterP;
				}

			}

			pCurrentMusicNode = pCurrentMusicNode->pNext;
		}

		//make the list circular
		counterN = newList;

		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
		}

		newList->pPrevious = counterP;
		counterP->pNext = newList;


		//free old list
		while (counterN != NULL)
		{
			counterP = counterN;
			counterN = counterN->pNext;
			if (counterP != NULL)
				free(counterP);
		}

		*mainNode = newList;


		break;
	default:
		break;
	}
}

void swapData (MusicNode* struct1, MusicNode* struct2)
{
	//get temp stores
	char artistName[100] = " ", albumName[100]= " ", songTitle[100]= " ", genre[100]= " ";
	int minSECtpRat[4] = {0};

	//store data in temp
	strcpy(artistName, struct1->mainData.artistName);
	strcpy(albumName, struct1->mainData.albumTitle);
	strcpy(songTitle, struct1->mainData.songTitle);
	strcpy(genre, struct1->mainData.genre);
	minSECtpRat[0] = struct1->mainData.songLength.minutes;
	minSECtpRat[1] = struct1->mainData.songLength.seconds;
	minSECtpRat[2] = struct1->mainData.timesPlayed;
	minSECtpRat[3] = struct1->mainData.rating;



	//move struct 2 to struct 1
	strcpy(struct1->mainData.artistName, struct2->mainData.artistName);
	strcpy(struct1->mainData.albumTitle, struct2->mainData.albumTitle);
	strcpy(struct1->mainData.songTitle, struct2->mainData.songTitle);
	strcpy(struct1->mainData.genre, struct2->mainData.genre);
	struct1->mainData.songLength.minutes = struct2->mainData.songLength.minutes;
	struct1->mainData.songLength.seconds = struct2->mainData.songLength.seconds;
	struct1->mainData.timesPlayed = struct2->mainData.timesPlayed;
	struct1->mainData.rating = struct2->mainData.rating;

	//move temp to struct 2
	strcpy(struct2->mainData.artistName, artistName);
	strcpy(struct2->mainData.albumTitle, albumName);
	strcpy(struct2->mainData.songTitle, songTitle);
	strcpy(struct2->mainData.genre, genre);
	struct2->mainData.songLength.minutes = minSECtpRat[0];
	struct2->mainData.songLength.seconds = minSECtpRat[1];
	struct2->mainData.timesPlayed = minSECtpRat[2];
	struct2->mainData.rating = minSECtpRat[3];
}