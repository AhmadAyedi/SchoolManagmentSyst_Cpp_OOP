#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Base class: Person (encapsulates shared attributes of students and teachers)
class Person
{
protected:
    string name;
    int id;

public:
    // Constructor
    Person(const string &name, int id) : name(name), id(id) {}

    // Virtual function to display details (to be overridden in derived classes)
    virtual void displayDetails() const = 0;

    virtual ~Person() {}

    // Getters for name and id (used for deletion and updates)
    string getName() const { return name; }
    int getId() const { return id; }

    // Setters for name and id (used for updates)
    void setName(const string &newName) { name = newName; }
    void setId(int newId) { id = newId; }
};

// Derived class: Student
class Student : public Person
{
private:
    string studentClass;
    vector<string> subjects = {"Maths", "Physics", "CS", "Electronics"};
    vector<int> grades;
    double averageGrade;

public:
    // Constructor to initialize Student with name, id, class, and grades
    Student(const string &name, int id, const string &studentClass, const vector<int> &grades)
        : Person(name, id), studentClass(studentClass), grades(grades)
    {
        calculateAverage();
    }

    // Calculate the average grade
    void calculateAverage()
    {
        int total = 0;
        for (int grade : grades)
        {
            total += grade;
        }
        averageGrade = static_cast<double>(total) / grades.size();
    }

    // Check if the student passed
    string isPassed() const
    {
        return averageGrade >= 10.0 ? "Passed" : "Failed";
    }

    // Display student details (override)
    void displayDetails() const override
    {
        cout << "Name: " << name << "\nID: " << id << "\nClass: " << studentClass << endl;
        cout << "Subjects and Grades: \n";
        for (size_t i = 0; i < subjects.size(); ++i)
        {
            cout << subjects[i] << ": " << grades[i] << endl;
        }
        cout << "Average Grade: " << averageGrade << "\n"
             << "Result: " << isPassed() << endl;
    }

    // Save student details to file
    void saveToFile(ofstream &file) const
    {
        file << name << "," << id << "," << studentClass << ",";
        for (size_t i = 0; i < grades.size(); ++i)
        {
            file << grades[i];
            if (i != grades.size() - 1)
                file << ",";
        }
        file << "\n";
    }

    // Getters
    string getStudentClass() const { return studentClass; }
    vector<int> getGrades() const { return grades; }
};

// Derived class: Teacher
class Teacher : public Person
{
private:
    string subject;
    string grade; // professor, assistant, or conference

public:
    // Constructor to initialize Teacher
    Teacher(const string &name, int id, const string &subject, const string &grade)
        : Person(name, id), subject(subject), grade(grade) {}

    // Display teacher details (override)
    void displayDetails() const override
    {
        cout << "Name: " << name << "\nID: " << id << "\nSubject: " << subject << "\nGrade: " << grade << endl;
    }

    // Save teacher details to file
    void saveToFile(ofstream &file) const
    {
        file << name << "," << id << "," << subject << "," << grade << "\n";
    }

    // Getters and Setters for updates
    string getSubject() const { return subject; }
    string getGrade() const { return grade; }

    void setSubject(const string &newSubject) { subject = newSubject; }
    void setGrade(const string &newGrade) { grade = newGrade; }
};

// School class to manage students and teachers
class School
{
private:
    vector<Student *> students;
    vector<Teacher *> teachers;
    string filePath = "C:\\Users\\Mediatek\\Documents\\C++\\proj_school-mang_syst1\\school_DataBase.txt";

public:
    School()
    {
        loadFromFile(); // Load the data from file on start
    }

    // Add a new student
    void addStudent()
    {
        string name, studentClass;
        int id;
        vector<int> grades(4); // grades for Maths, Physics, CS, and Electronics

        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Student ID: ";
        cin >> id;
        cout << "Enter Student Class: ";
        cin.ignore();
        getline(cin, studentClass);

        for (size_t i = 0; i < grades.size(); ++i)
        {
            cout << "Enter grade for " << (i == 0 ? "Maths" : i == 1 ? "Physics"
                                                          : i == 2   ? "CS"
                                                                     : "Electronics")
                 << ": ";
            cin >> grades[i];
        }

        students.push_back(new Student(name, id, studentClass, grades));
        cout << "Student added successfully!\n";
        saveToFile(); // Save to file after adding
    }

    // Add a new teacher
    void addTeacher()
    {
        string name, subject, grade;
        int id;

        cout << "Enter Teacher Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Teacher ID: ";
        cin >> id;
        cout << "Enter Subject (Maths, Physics, CS, Electronics): ";
        cin.ignore();
        getline(cin, subject);
        cout << "Enter Grade (Professor, Assistant, Conference): ";
        getline(cin, grade);

        teachers.push_back(new Teacher(name, id, subject, grade));
        cout << "Teacher added successfully!\n";
        saveToFile(); // Save to file after adding
    }

    // Delete a student by ID
    void deleteStudent()
    {
        int id;
        cout << "Enter Student ID to delete: ";
        cin >> id;

        auto it = students.begin();
        while (it != students.end())
        {
            if ((*it)->getId() == id)
            {
                delete *it;         // Free memory
                students.erase(it); // Remove from vector
                cout << "Student deleted successfully!\n";
                saveToFile(); // Save changes to file
                return;
            }
            ++it;
        }
        cout << "Student not found!\n";
    }

    // Delete a teacher by ID
    void deleteTeacher()
    {
        int id;
        cout << "Enter Teacher ID to delete: ";
        cin >> id;

        auto it = teachers.begin();
        while (it != teachers.end())
        {
            if ((*it)->getId() == id)
            {
                delete *it;         // Free memory
                teachers.erase(it); // Remove from vector
                cout << "Teacher deleted successfully!\n";
                saveToFile(); // Save changes to file
                return;
            }
            ++it;
        }
        cout << "Teacher not found!\n";
    }

    // Display all students
    void displayStudentsDashboard() const
    {
        if (students.empty())
        {
            cout << "No students available.\n";
            return;
        }

        cout << "\n--- Students Dashboard ---\n";
        for (const Student *student : students)
        {
            student->displayDetails();
            cout << "----------------------\n";
        }
    }

    // Display all teachers
    void displayTeachersDashboard() const
    {
        if (teachers.empty())
        {
            cout << "No teachers available.\n";
            return;
        }

        cout << "\n--- Teachers Dashboard ---\n";
        for (const Teacher *teacher : teachers)
        {
            teacher->displayDetails();
            cout << "----------------------\n";
        }
    }

    // Save all data to file
    void saveToFile() const
    {
        ofstream file(filePath);
        if (!file)
        {
            cout << "Error opening file for saving!\n";
            return;
        }

        file << "Students\n";
        for (const Student *student : students)
        {
            student->saveToFile(file);
        }

        file << "Teachers\n";
        for (const Teacher *teacher : teachers)
        {
            teacher->saveToFile(file);
        }
    }

    // Load all data from file
    void loadFromFile()
    {
        string filePath = "school_DataBase.txt"; // Path to the file
        ifstream file(filePath);                 // Declare ifstream to read from the file

        if (!file)
        {
            cout << "No existing data found. Starting fresh.\n";
            return;
        }

        string type, line;
        while (getline(file, type))
        {
            if (type == "Students")
            {
                while (getline(file, line) && !line.empty())
                {
                    string name, studentClass;
                    int id, grade;
                    vector<int> grades(4);
                    size_t pos = 0;

                    pos = line.find(",");
                    name = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    pos = line.find(",");
                    id = stoi(line.substr(0, pos));
                    line.erase(0, pos + 1);

                    pos = line.find(",");
                    studentClass = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    for (int i = 0; i < 4; ++i)
                    {
                        pos = line.find(",");
                        grades[i] = stoi(line.substr(0, pos));
                        line.erase(0, pos + 1);
                    }

                    students.push_back(new Student(name, id, studentClass, grades));
                }
            }
            else if (type == "Teachers")
            {
                while (getline(file, line) && !line.empty())
                {
                    string name, subject, grade;
                    int id;
                    size_t pos = 0;

                    pos = line.find(",");
                    name = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    pos = line.find(",");
                    id = stoi(line.substr(0, pos));
                    line.erase(0, pos + 1);

                    pos = line.find(",");
                    subject = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    grade = line;

                    teachers.push_back(new Teacher(name, id, subject, grade));
                }
            }
        }

        cout << "Data loaded from file successfully!\n";
    }

    ~School()
    {
        // Free dynamically allocated memory
        for (Student *student : students)
        {
            delete student;
        }
        for (Teacher *teacher : teachers)
        {
            delete teacher;
        }
    }
};

// Main menu
void displayMenu()
{
    cout << "\n--- School Management System ---\n";
    cout << "1. Add Student\n";
    cout << "2. Add Teacher\n";
    cout << "3. View Student Dashboard\n";
    cout << "4. View Teacher Dashboard\n";
    cout << "5. Delete Student\n";
    cout << "6. Delete Teacher\n";
    cout << "7. Exit\n";
}

int main()
{
    School school;
    int choice;

    while (true)
    {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            school.addStudent();
            break;
        case 2:
            school.addTeacher();
            break;
        case 3:
            school.displayStudentsDashboard();
            break;
        case 4:
            school.displayTeachersDashboard();
            break;
        case 5:
            school.deleteStudent();
            break;
        case 6:
            school.deleteTeacher();
            break;
        case 7:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
