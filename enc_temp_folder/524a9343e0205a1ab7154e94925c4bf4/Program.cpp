﻿#include "Program.h"
#include <iostream>
#include<fstream>
Program::Program()
{
	loadfile();
}
void Program::loop()
{
	loginMenu();

}

void Program::loginMenu()
{
	
	while (true) {
		printCentered("S a r a h a");
		printCentered("Annonymously send and receive messages.");
		cout << "Welcome!" << endl;
		cout << "Do you have an account?\n\n" << endl; //bug here, two newlines fix it but why?
		cout << "1. Yes (login)\n"
			<< "2. No (sign up)\n"
			<< "3. Exit\n";

		cin >> choice;

		switch (choice) {
		case 1:
		{
			//login
			printCentered("L o g   i n");
			liveUserID = login();
			if (liveUserID !=-1)
			{
				liveUser = &users[liveUserID];
				userMenu(*liveUser);
			}
			break;
		}
		case 2:
			printCentered("S i g n   u p");
			//signup
			signup();
			break;
		case 3:
			//exit
			return;
		default:
			cout << "Invalid entry, try again!\n";
				break;
		}

	}
	
}

void Program::userMenu(User &liveUser)
{
	while (true)
	{
		liveUser.notify();
		cout << "1. Send a message\n"
			<< "2. Inbox\n"
			<< "3. Favorites\n"
			<< "4. Sent messages\n"
			<< "5. My contacts\n"
			<< "6. Logout\n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			//send a message
			sendmessage(liveUser);
	
			break;
		case 2:
		{
			//inbox
			Inbox(liveUser);
			break;
		}
		case 3:
			//favorites
			liveUser.viewFavorites();
			break;
		case 4:
			//sent messages
			cout << "Sent messages from latest to oldest: " << endl;
			liveUser.viewSent();
			cout << "1. Undo the latest message\n"
				<< "0. Back to previous menu\n";
			cin >> choice;
			if (choice == 1)
			{
				undolastmessage(liveUser);
			}
	
			break;
		case 5:
			//contacts
			liveUser.viewcontacts();
			cout << "Enter a contact ID for more options \n"
				<< "0 to go back to the previous menu. \n";
			cin >> choice;
			if (choice == 0)
				break;
			else
			{
				contactMenu(liveUser, *idToUser(choice));
			}
			break;
		case 6:
			//logout
			return; //هيخرج برا اللوب وبرا الفنكشن
		default:
			cout << "Invalid entry, try again!\n";
		}

	}
}

void Program::Inbox(User &liveUser)
{
	int msgIndex;
	liveUser.viewReceived();
	cout << "Enter message index to view details and options. \n" << "0. Back to home. \n";
	cin >> msgIndex;
	msgIndex--;
	if (msgIndex == -1) // was 0 but 0 is an index in msg vector
	{
		return;
	}
	else
	{
		Message msg = liveUser.getInboxMessage(msgIndex);
		msg.viewAsReceived();
		cout << "1. Add/remove from favorites.\n"
			<< "2. Add sender to contacts\n"
			<< "3. Report sender\n"
			<< "0. Back to previous menu.\n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			liveUser.favourite(msg);
	
			break;
		case 2:
			addSendertoContacts(liveUser, msg);
	
			break;
		case 3:
			cout << endl << "Report the sender of this message? (y/n) ";
			cin >> check;
			// Send Message
			if (check == 'y')
			{
				idToUser(msg.getSenderID())->beReported();
				cout << "Sender reported.\n";
			}
			break;
		case 0:
			break;
		default:
			cout << "Invalid entry.";
			break;

		}

	}
}


/*User Program::idToUser(int id)
{
	User user;
	if (users.find(id) != users.end()) {
		user = *users.find(id);
	}
	return user;
}*/
User* Program::idToUser(int id) {//returns pointer to user in hashmap, or nullptr if not found
	auto it = users.find(id);
	if (it != users.end()) {
		return &(it->second);
	}
	return nullptr;
}

int Program::usernameToID(string username)
{
	for (auto& it : users)
	{
		if (it.second.getUsername() == username)
			return it.first;
	}
	return -1; //not found
}

void Program::addSendertoContacts(User& liveUser, Message msg)
{
	User sender = *idToUser(msg.getSenderID());
	liveUser.addcontact(sender);

}

void Program::printCentered(string str)
{
	int maxwidth = 70;
	stringstream ss;
	int padding = maxwidth - str.size() / 2;
	for (int i = 0; i < padding;  i++) 
	{
		ss << " ";
	}
	ss << str;
	for (int i = 0; i < padding; i++)
	{
		ss << " ";
	}
	cout << ss.str() << endl;
}

void Program::signup() { //wessal salah
	string username, pass;
	cout << "Please, enter your user name: \n";
	cin.ignore();
	getline(cin, username);
	cout << "Enter your password: \n";
	cin >> pass;
	if (usernameToID(username) != -1) {
		cout << "You already have an acount. please log in!";
	}
	else 
	{
		++userCount;
		users[userCount] = User(username, pass, userCount);
		cout << "Congratulation!!\nYou now have an account";
	}
}
	
	

int Program::login() { //wessal

	string name, pass;
	cout << "Enter your user name: \n";
	cin.ignore();
	getline(cin, name);
	cout << "Enter your password: \n";
	cin >> pass;
	liveUserID = usernameToID(name);
	if (liveUserID == -1)
	{
		cout << "The username is incorrect!\nplease try again\n";
		return -1;
	}
	else
	{
		auto it = users.find(liveUserID);
		User u = it->second;
		if (!u.comparePassword(pass))
		{
			cout << "The password is incorrect!\nplease try again\n";
			return -1;
		}
		else
		{
			if (u.isBanned())
			{
				cout << "Uh oh! Your account is banned :( \n"
					<< "Looks like you have been reported too many times.\n";
				return -1;
			}
			else
			{
				cout << "Welcome back," << u.getUsername() << "!\n";
				return liveUserID;
			}
		}
	}
}
	
void Program::sendmessage(User &liveUser) {
	// Data
	int receiverID;
	string username_receiver, msg; 
	// Create Message

	cout << endl << "Enter your message:" << " ";
	cin.ignore();
	getline(cin, msg);

	cout << "Enter receiver username:" << " ";
	cin >> username_receiver;

	receiverID = usernameToID(username_receiver);

	
	if (receiverID == -1) 
	{
		cout << endl << "User does not exist, please try again." << endl;
	}
	else
	{
		User *receiver = &users[receiverID];

		Message msgg_object(liveUser.getid(), receiverID, username_receiver, msg);

		// Check
		cout << "Send message? (y/n)" << " ";
		cin >> check;

		// Send Message
		if (check == 'y')
		{

			// Push in Sender messages
			liveUser.addToSent(msgg_object, *receiver);

			// push in reciver inbox
			receiver->addToInbox(msgg_object, *receiver);
			cout << "Message sent successfully." << " " << endl;
		}
		else
		{
			cout << "Message canceled." << " " << endl;
		}
	}


}



void Program::undolastmessage(User &liveUser) {
	cout << endl << "Do You Want To Delete Last Message ? (y/n)" << " " << endl;
	char c;
	int cc;
	Message lastMsg;
	cin >> c;
	if (c == 'y') {
		// Pop in Sender messages and store popped message
		lastMsg = liveUser.popSent();
		cout << "1. Delete for me\n"
			<< "2. Delete for everyone\n";
		cin >> cc;
		switch (cc)
		{
		case 1:
			cout << "Message removed for you" << endl;
			break;
		case 2:
		{
			//Pop in receiver inbox //BUG HERE
			if (idToUser(lastMsg.getReceiverID()) != nullptr) {
				User* receiver = idToUser(lastMsg.getReceiverID());
				receiver->removeFromInbox(lastMsg);
				break;
			}
		}
		default:
			cout << "Invalid choice, please try again!";
		}

	}
	else
		return;
}

//void Program::savefile() {
//	ofstream ourfile("ourdata.txt", ios::app);
//	if (ourfile.is_open()) {
//		for (auto it : users)
//		{
//			ourfile << it.second.getid() << ',' << it.second.getUsername() << ',' << it.second.password << "\n";
//			//for (auto msg : it.second.sent) {
//			//	ourfile << msg.getSenderID() << " " << msg.getReceiverID() << " " << msg.getReceiverUsername() << " " << msg.getContent() << "\n";
//			//}
//			//for (auto msg : it.second.inbox) {
//			//	ourfile << msg.getSenderID() << " " << msg.getReceiverID() << " " << msg.getReceiverUsername() << " " << msg.getContent() << "\n";
//			//	/*	}
//			//		for (auto elem : it.second.contacts) {
//			//			ourfile << elem.second << endl;
//			//		}*/
//			//	for (auto elem : it.second.FavouriteMessages) {
//			//		ourfile << msg.getSenderID() << " " << msg.getReceiverID() << " " << msg.getReceiverUsername() << " " << msg.getContent() << "\n";
//			//	}
//				ourfile << endl;
//			}
//		}
//		ourfile.close();
//}

//void Program::loadfile()
//{
//	ifstream ourfile("ourdata.txt");
//	string line;
//	int id;
//	string username;
//	string password;
//	stack<string> splitted;
//	while (ourfile) {
//		getline(ourfile, line);
//		splitted = split(line, ',');
//		while (!splitted.empty())
//		{
//			id = stoi(splitted.top());
//			splitted.pop();
//			username = splitted.top();
//			splitted.pop();
//			password = splitted.top();
//			splitted.pop();
//			//users.insert(make_pair(id, obj));
//			users[id] = User(username, password, id);
//			/*for (auto it : users) {
//				cout << it.first << it.second.username << " " << it.second.password;
//			}*/
//		}
//	}
//	ourfile.close();
//}
void Program::savefile()
{
	ofstream file("data.txt");
	for (auto it : users)
	{
		file << it.second.getid() << " " << it.second.getUsername() << " " << it.second.getPassword() << endl;
		file << endl;

	}
}
void Program::loadfile()
{
	ifstream file("data.txt");
	int id;
	string username;
	string password;

	int senderID, receiverID;
	string receiverUsername, content;

	while (file >> id >> username >> password)
	{
		users[id] = User(username, password, id);
	}
}
Program::~Program()
{
	savefile();
}

void Program::contactMenu(User &liveUser, User &contact) {

	cout << "1. View sent messages\n"
		<< "2. Report\n"
		<< "3. Remove\n"
		//<< "4. Block\n"
		<< "0. Back to previous menu \n";

	int choice; 
	cin >> choice;

	switch (choice) {
	case 1:
		// view messages from contact
		liveUser.viewContactMessages(contact);
		break;
	case 2:
		// report 
		cout << endl << "Report this contact? (y/n) ";
		cin >> check;
		if (check == 'y')
		{
			contact.beReported();
			cout << "Contact reported.\n";
		}
		break;
	case 3:
		liveUser.removecontact(contact);
		break;
	case 4:
		// block function
		break;
	case 0:
		break;
	default:
		cout << "Invalid entry! Please try again.";
	}
}


//stack<string> Program::split(string s, char delim) 
//{
//	stack<string> result;
//	stringstream ss(s);
//	string item;
//
//	while (getline(ss, item, delim)) {
//		result.push(item);
//	}
//
//	return result;
//}
//usage
//vector<string> v = split(str, delimiter);
//for (auto i : v) cout << i << endl;