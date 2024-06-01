#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int id;
    int age;
    string major;

public:
    Student() : name("No name"), id(0), age(0), major("No major") {}
    Student(string n, int i, int a, string m) : name(n), id(i), age(a), major(m) {}
    void Show() {
        cout << "ID: " << id << ", Name: " << name << ", Age: " << age << ", Major: " << major << endl;
    }
    void Input() {
        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter major: ";
        cin >> major;
    }
    void writeToFile(ofstream& fout) {
        if (fout.is_open()) {
            fout << id << " " << name << " " << age << " " << major << endl;
        }
        else {
            cout << "Could not write to file" << endl;
        }
    }
    static Student readFromFile(ifstream& fin) {
        int id, age;
        string name, major;
        fin >> id >> name >> age >> major;
        return Student(name, id, age, major);
    }
    int getId() { return id; }
    string getName() { return name; }
    int getAge() { return age; }
    string getMajor() { return major; }

    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setMajor(string m) { major = m; }
};
class StudentDatabase {
private:
    Student** students;
    int capacity;
    int count;
    void resize() {
        capacity *= 2;
        Student** newStudents = new Student * [capacity];
        for (int i = 0; i < count; ++i) {
            newStudents[i] = students[i];
        }
        delete[] students;
        students = newStudents;
    }
public:
    StudentDatabase(int capacity) : capacity(capacity), count(0) {
        students = new Student * [capacity];
    }
       ~StudentDatabase() {
        for (int i = 0; i < count; ++i) {
            delete students[i];
        }
        delete[] students;
    }
    void addStudent(Student& student) {
        if (count == capacity) {
            resize();
        }
        students[count++] = new Student(student);
    }
    void removeStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                delete students[i];
                for (int j = i; j < count - 1; ++j) {
                    students[j] = students[j + 1];
                }
                return;
            }
        }
        cout << "Student with ID " << id << " not found." << endl;
    }
    Student* findStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getId() == id) {
                return students[i];
            }
        }
        return nullptr;
    }
    void listStudents() {
        for (int i = 0; i < count; ++i) {
            students[i]->Show();
        }
    }
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cout << "Error opening file for writing." << endl;
            return;
        }
        for (int i = 0; i < count; ++i) {
            students[i]->writeToFile(outFile);
        }
        outFile.close();
    }
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "Error opening file for reading." << endl;
            return;
        }
        count = 0;
        while (!inFile.eof()) {
            Student student = Student::readFromFile(inFile);
            if (inFile.fail()) break;
            addStudent(student);
        }
        inFile.close();
    }
    void updateStudent(int id, Student& newData) {
        Student* student = findStudent(id);
        if (student) {
            student->setName(newData.getName());
            student->setAge(newData.getAge());
            student->setMajor(newData.getMajor());
        }
        else {
            cout << "Student with ID " << id << " not found." << endl;
        }
    }
    void filterByMajor(string major) {
        for (int i = 0; i < count; ++i) {
            if (students[i]->getMajor() == major) {
                students[i]->Show();
            }
        }
    }
};
void printMenu() {
    cout << "Menu:" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Remove Student" << endl;
    cout << "3. Find Student" << endl;
    cout << "4. List All Students" << endl;
    cout << "5. Save to File" << endl;
    cout << "6. Load from File" << endl;
    cout << "Enter your choice: ";
}
void run() {
    StudentDatabase db(10);
    string choice;
    while (true) {
        printMenu();
        cin >> choice;
        switch (stoi(choice)) {
        case 1: {
            Student student;
            student.Input();
            db.addStudent(student);
            break;
        }
        case 2: {
            int id;
            cout << "Enter ID of the student to remove: ";
            cin >> id;
            db.removeStudent(id);
            break;
        }
        case 3: {
            int id;
            cout << "Enter ID of the student to find: ";
            cin >> id;
            Student* student = db.findStudent(id);
            if (student) {
                student->Show();
            }
            else {
                cout << "Student with ID " << id << " not found." << endl;
            }
            break;
        }
        case 4: {
            db.listStudents();
            break;
        }
        case 5: {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            db.saveToFile(filename);
            break;
        }
        case 6: {
            string filename;
            cout << "Enter filename to load: ";
            cin >> filename;
            db.loadFromFile(filename);
            break;
        }
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    }
}
int main() {
    run();
}