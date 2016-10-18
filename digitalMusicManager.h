/*******************************************************************************************
* Student: Vikram Singh StudentID: 11491025                                                *
* Class: CptS 122, Fall  2016; Lab Section 5                                               *
* Programming Assignment: Programming Assignment 2					                       *
* Date: January 29, 2016                                                                   *
* Description: Writing a program that is a Digital music manager						   *
*******************************************************************************************/
//Gaurd Code
#ifndef DIGITAL_MUSIC_MANAGER_H
#define DIGITAL_MUSIC_MANAGER_H

#define _CRT_SECURE_NO_WARNINGS //Stops unwanted errors

//Standard libararies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for storing time
typedef struct songTime
{
	//time will be stored in to vars
	//the min and seconds as ints
	int minutes;
	int seconds;
}SongTime;

//struct stores song information
typedef struct musicdata
{
	char *artistName; //pointer for artist
	char *albumTitle; //pointer for album title
	char *songTitle; //pointer for song title
	char *genre; //pointer for the genre
	SongTime songLength; //struct that stores mins and seconds
	int timesPlayed; //times the song was played
	int rating; //rating of the song between 1 - 5
}MusicData;

//struct stores pointers and main data struct
typedef struct musicNode
{
	//Main Data
	MusicData mainData;

	//pointers
	struct musicNode *pNext; //if linear will hold n + 1 node
	struct musicNode *pPrevious; //due to double linked will hold pervious node n - 1
}MusicNode;

//Digital Music Managers' Prototypes
int mainMenu (void); //main menu of DMM
void mainMenuWrittenOptions(void); //print options for main menu
MusicNode* allocateMemory(char *artistName, char *albumTitle, char *songTitle,  char *genre,
							int minutes, int seconds, int timesPlayed, int rating); //Allocates memeory and places info in struct
int loadFromFile(MusicNode **pHeadDMM); //this function will load from file data into a double linked array not sorted (1: all is good 0: something happened)
int printMusicList(MusicNode *pHeadDMM); //print linked list
int storeMusicList(MusicNode *pHeadDMM); //prints the linked list to a file
void searchOptions(void); //print statements for search
int searchOptionSelector(void);//selector
void mainDigitalManager(void); //Main holder of logic and choices
void searchOptionHandler(MusicNode *mainHead); //handle searches
void searchByArtist(MusicNode *mainHead);//search by artist
void searchByAlbum(MusicNode *mainHead);//search by album
void searchBySong(MusicNode *mainHead);//search by song
void searchByGenre(MusicNode *mainHead);//search by genre
void searchByTimesPlayed(MusicNode *mainHead);//search by timesplayed
void searchByRating(MusicNode *mainHead);//search by ratting
void searchByTime(MusicNode *mainHead);//search by time
void rateMusic(MusicNode *mainHead); //rate music
void deleteSong(MusicNode **mainHead); //delete song
void insertOrDelete(MusicNode **pHeadmain); //options
void insertSong(MusicNode **mainHead);//try insert
void editAnythingOption(MusicNode **mainnode);//edit anything
void sortByAnything(MusicNode **mainnode, int *sort); //sort handler
void handleSortRequest(MusicNode **mainnode, int *sort);//handler
void swapData (MusicNode* struct1, MusicNode* struct2); //swaping


#endif /*DIGITAL_MUSIC_MANAGER_H: end gaurd code*/