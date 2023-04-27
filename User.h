#pragma once

#include "Message.h"
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <stack>
#include <iostream>
using namespace std;

class User
{
public:
	int id;
	string username;
	string password;
	vector<User> contacts; 
	stack<Message> sent;
	vector<Message> inbox;
	vector<pair<User, int>>sortedContacts;
	
	
public:
	void addcontact(User u); //return boolean?
	void removecontact(User u);
	//void sendmessage();
	//void unsend();
	//void searchcontact(); //boolean?
	void viewcontacts();
	void viewSent();
	void viewReceived();
	//bool cmp(const pair<int, int>& a, const pair<int, int>& b);
	void msgcounter(User u);
	//void favourite();
	//void RemoveOldestavorite();
	//void viewFavorites();
	

	//helper functions
};

