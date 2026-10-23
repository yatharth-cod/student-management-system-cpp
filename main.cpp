#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

const string FILE_NAME = "students.txt";

void addStudent() {
    int roll;
    string name, course;
    cout << "Roll number: ";
    cin >> roll;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Name: ";
    getline(cin, name);
    cout << "Course: ";
    getline(cin, course);

    ofstream out(FILE_NAME, ios::app);
    if (!out) { cout << "Could not open student file.\n"; return; }
    out << roll << '|' << name << '|' << course << '\n';
    cout << "Student added.\n";
}

void displayStudents() {
    ifstream in(FILE_NAME);
    if (!in) { cout << "No records yet.\n"; return; }
    string line;
    bool found = false;
    cout << "\n--- Student Records ---\n";
    while (getline(in, line)) {
        stringstream ss(line);
        string roll, name, course;
        if (getline(ss, roll, '|') && getline(ss, name, '|') && getline(ss, course)) {
            cout << "Roll: " << roll << " | Name: " << name << " | Course: " << course << '\n';
            found = true;
        }
    }
    if (!found) cout << "No records yet.\n";
}

void updateStudent() {
    int target;
    cout << "Enter roll number to update: ";
    cin >> target;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ifstream in(FILE_NAME);
    if (!in) { cout << "No records yet.\n"; return; }
    ofstream temp("students_temp.txt");
    string line;
    bool updated = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string rollText, name, course;
        if (!getline(ss, rollText, '|') || !getline(ss, name, '|') || !getline(ss, course)) continue;
        try {
            if (stoi(rollText) == target) {
                cout << "New name: "; getline(cin, name);
                cout << "New course: "; getline(cin, course);
                updated = true;
            }
        } catch (...) {}
        temp << rollText << '|' << name << '|' << course << '\n';
    }
    in.close(); temp.close();
    remove(FILE_NAME.c_str());
    rename("students_temp.txt", FILE_NAME.c_str());
    cout << (updated ? "Student updated.\n" : "Roll number not found.\n");
}

void deleteStudent() {
    int target;
    cout << "Enter roll number to delete: ";
    cin >> target;
    ifstream in(FILE_NAME);
    if (!in) { cout << "No records yet.\n"; return; }
    ofstream temp("students_temp.txt");
    string line;
    bool deleted = false;
    while (getline(in, line)) {
        stringstream ss(line);
        string rollText, name, course;
        if (!getline(ss, rollText, '|') || !getline(ss, name, '|') || !getline(ss, course)) continue;
        bool match = false;
        try { match = stoi(rollText) == target; } catch (...) {}
        if (match) deleted = true;
        else temp << line << '\n';
    }
    in.close(); temp.close();
    remove(FILE_NAME.c_str());
    rename("students_temp.txt", FILE_NAME.c_str());
    cout << (deleted ? "Student deleted.\n" : "Roll number not found.\n");
}

int main() {
    int choice;
    do {
        cout << "\n=== Student Management System ===\n"
             << "1. Add student\n2. Display students\n3. Update student\n4. Delete student\n0. Exit\nChoice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); choice = -1; }
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 0: cout << "Goodbye.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
