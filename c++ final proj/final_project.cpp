#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class Notes {
private:
    map<string, vector<string>> subjectNotes;      			    //subjectNotes is a map having keys as string and key values as vector of strings.(In other words each subject is associated with a list of notes)

public:
    void addNote(const string& subject, const vector<string>& notes) 
	{
        subjectNotes[subject].insert(subjectNotes[subject].end(), notes.begin(), notes.end());			//This allows for adding multiple notes to the same subject without overwriting existing ones.
    }

    void displayNotes(const string& subject) const 
	{
        auto it = subjectNotes.find(subject);						//to find a particular subject exists or not.
        if (it != subjectNotes.end()) 
		{
            cout << "Notes for Subject '" << subject << "':\n";
            for (const auto& note : it->second) 						//This line iterates over each note in the vector of notes associated with the subject.
			{
                cout << note << "\n";
            }
        } 
		else 
		{
            cout << "No notes found for Subject '" << subject << "'.\n";
        }
    }

    void displayAllNotes() const 
	{
        cout << "All Subject-wise Notes:\n";
        for (const auto& pair : subjectNotes) 						 //pair is a variable that represents each key-value pair in the subjectNotes.
		{
			printf("\n");
            cout << "Subject: " << pair.first << "\n";
            for (const auto& note : pair.second) 
			{
                cout << note << "\n";
            }
        }
    }

    void saveNotesToFile(const string& username) const 
	{
        ofstream file(username + "_notes.txt");
        for (const auto& pair : subjectNotes) 
		{
            file << pair.first << ":\n";
            for (const auto& note : pair.second) 
			{
                file << note << "\n";
            }
            file << '\n';
        }
        file.close();
    }

    void loadNotesFromFile(const string& username) 
	{
        ifstream file(username + "_notes.txt");
        if (file.is_open()) 
		{
            string line;											//this variable line is used to store each line read from the file.
            string currentSubject;
            vector<string> currentNotes;
            
            while (getline(file, line)) 							//this loop reads each line from the file until the end of the file is reached.
			{
                if (line.back() == ':')							//this condition checks if the current line ends with a colon, indicating a new subject.
				{
                    currentSubject = line.substr(0, line.size() - 1);		//If the condition is true, it extracts the subject name from the line and assigns it to currentSubject
                    currentNotes.clear();									// It clears the currentNotes vector to prepare it for storing notes for the new subject.
                } 
				else if (!line.empty()) 
				{
                    currentNotes.push_back(line);						//If the condition is true, it adds the line (note) to the currentNotes vector.
                    subjectNotes[currentSubject] = currentNotes;		//It assigns the currentNotes vector to the currentSubject key in the subjectNotes map
                }
            }
            file.close();
        }
    }

    // function to delete notes for a particular subject
	void deleteNotes(const string& username, const string& subject) 
	{
  	  auto it = subjectNotes.find(subject);
  	  if (it != subjectNotes.end()) 
	  {
  	      subjectNotes.erase(it);
      	  cout << "Notes for Subject '" << subject << "' deleted.\n";

        // Update the file by rewriting it without the deleted subject's notes
        saveNotesToFile(username);
   	 } 
	 else
	 {
         cout << "No notes found for Subject '" << subject << "'.\n";
     }	
	}

};

class User {
private:
    string username;
    string password;
    Notes userNotes;

public:
    User(const string& uname, const string& pwd)
        : username(uname), password(pwd) {}

    bool login(const string& enteredPassword) const 
	{
        return password == enteredPassword;
    }

    void addNote() 
	{
        string subject, note;
        vector<string> notes;
        cout << "Enter subject: ";
        cin >> subject;
        cin.ignore();                                            // Clear newline character from the buffer
        cout << "Enter notes for " << subject << " (end input with 'end' on a new line):\n";
        while (getline(cin, note)) 
		{
            if (note == "end") 
			{
                break;
            }
            notes.push_back(note);
        }
        userNotes.addNote(subject, notes);					//calling object again to append added notes to the user's existing notes for a particular subject.
    }

    void viewNotes() const 
	{
        string subject;
        cout << "Enter subject to view notes: ";
        cin >> subject;
        userNotes.displayNotes(subject);
    }

    void viewAllNotes() const {
        userNotes.displayAllNotes();
    }

    void saveNotes() const {
        userNotes.saveNotesToFile(username);
    }

    void loadNotes() {
        userNotes.loadNotesFromFile(username);
    }

	void deleteNotes() 
	{
  	  string subject;
  	  cout << "Enter subject to delete notes: ";
  	  cin >> subject;
  	  userNotes.deleteNotes(username, subject);
	}

};

int main() {
    cout << "--- MyNotes Software ---\n";
    string username, password;
    cout << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    User user(username, password);

    // Simulating login (In a real application, you would validate the login credentials)
    string enteredPassword;
    cout << "Enter password to login: ";
    cin >> enteredPassword;

    if (user.login(enteredPassword)) {
        cout << "Login successful!\n";

        // Load existing notes
        user.loadNotes();

        int choice;
        do {
            cout << "\nMenu:\n"
                 << "1. Add Note\n"
                 << "2. View Notes for a Subject\n"
                 << "3. View All Notes\n"
                 << "4. Save Notes\n"
                 << "5. Load Notes\n"
                 << "6. Delete Notes for a Subject\n"
                 << "7. Exit\n"
                 << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    user.addNote();
                    break;
                case 2:
                    user.viewNotes();
                    break;
                case 3:
                    user.viewAllNotes();
                    break;
                case 4:
                    user.saveNotes();
                    break;
                case 5:
                    user.loadNotes();
                    cout << "Notes loaded.\n";
                    break;
                case 6:
                    user.deleteNotes();
                    break;
                case 7:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 7);
    } 
	else 
	{
        cout << "Login failed. Incorrect password.\n";
    }

    return 0;
}
