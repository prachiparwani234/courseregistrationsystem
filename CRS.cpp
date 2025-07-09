#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<cctype>
#include<regex>
#include<sstream>

using namespace std;



bool isValidPhone(string phone){
    return phone.length() == 10 && (phone[0] >='6' && phone[0] <= '9') &&
    all_of(phone.begin(), phone.end(), ::isdigit);
}

bool isValidName(const string& name){
    if(name.empty()) return false;

    for(char c : name){
        if  (!isalpha(c) && c != ' '){
            return false;
        }
    }
    return true;
}

bool isValidEmail(const string& email) {
        const regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return regex_match(email, pattern);
    }

bool isValidPercentile(float p) {
        return p >= 0 && p <= 100;
    }

class Course{
public:
    string coursename;
    string coursecode;
    float minpercentile;
    int seat;
    int enrolled;

    Course(string coursename,string coursecode,float minpercentile,int seat)
         : coursename(coursename), coursecode(coursecode), 
           minpercentile(minpercentile),seat(seat), enrolled(0) {}

    bool isEligible(float percentile) const{
        return percentile >= minpercentile;
    }

    bool isVacant() const {
        return seat > enrolled;
    }

    bool EnrollStudent(float percentile){
        if(isEligible(percentile) && isVacant()){
            enrolled++;
            cout << "Enrollment successful! Seats remaining: " << (seat-enrolled) <<endl;
            return true;
        }
        else{
            cout<<"Enrollment failed, ";
            if(!isEligible(percentile)) cout<<"Not Eligible.";
            if(!isVacant()) cout<<"No seats available.";
            cout << endl;
            return false;
        }
    }

    void Display() const {
        cout <<"Course Name: "<<coursename<<endl;
        cout <<"Course code: "<<coursecode<<endl;
        cout <<"Minimum JEE percentile required: "<<minpercentile<<endl;
        cout <<"Total seats: "<<seat<<endl;
        cout <<"students Enrolled: "<<enrolled<<endl;
        cout <<"seats remaining: "<<(seat-enrolled)<<endl;
        cout <<"________________________________________\n";

    }
};

class Student{
public:
    string name;
    string phone;
    float JEEpercentile;
    string email;
    string courseName;

    Student(string name,string email,string phone,float JEEpercentile,string courseName)
       : name(name), email(email),phone(phone), JEEpercentile(JEEpercentile), courseName(courseName){}

    static Student createstudent(){
        string name,email,phone;
        float jee;
        
        

        do{
            cout<<"Enter Name:";
            getline(cin>>ws, name);
            if(!isValidName(name)) cout << " Invalid name.\n";
        }while(!isValidName(name));

        do{
            cout << "Enter email:";
            cin>>email;
            if(!isValidEmail(email)) cout<<"Invalid email.\n";
        }while(!isValidEmail(email));

        do {
            cout<<"Enter phone number:";
            cin>>phone;
            if(!isValidPhone(phone)) cout << "Invalid phone number.\n";
        }while(!isValidPhone(phone));
        
        do{
            cout<<"Enter JEE percentile:";
            cin>>jee;
            if(!isValidPercentile(jee)) cout << "Invalid Percentile.\n";
        }while(!isValidPercentile(jee));
        
        return Student(name, email, phone, jee, "");
    }

    void Display() const {
        cout << "Name: "<<name
             <<" | Email: " <<email 
             <<" | phone: " <<phone
             <<"| JEE: "<< JEEpercentile<<"%"
             <<"| Course: "<< courseName <<"\n";
    }

};

bool isDuplicateRegistration(const vector<Student>& students,const string& email,const string& phone) {
    for(const auto& s : students){
        if(s.email == email || s.phone == phone ){
            return true;
        }
    }
    return false;
}

void loadRegistrationFromFile(vector<Student>& students,vector<Course>& courses){
    ifstream infile("registrations.txt");
    if(!infile){
        cout << "No pervious registrations found.\n";
        return;

    }

    string line;
    while(getline(infile, line)){
        stringstream ss(line);
        string name, email, phone, percentileStr, courseName;

        getline(ss,name,',');
        getline(ss,email,',');
        getline(ss,phone,',');
        getline(ss,percentileStr,',');
        getline(ss,courseName,',');

        float percentile = stof(percentileStr);
        students.emplace_back(name, email, phone, percentile, courseName);

        for(auto& c : courses){
            if(c.coursename == courseName){
                c.enrolled++;
                break;

            }
        }
    }

    infile.close();
    cout << students.size() << " previous registrations loaded.\n";
}

void exportToCSV(const vector<Student>& students, const string& filename) {
    ofstream out(filename);
    if(!out){
        cout << "Error : Could not open " << filename <<"for writing.\n";
        return;
    }
    out << "Name,Email,Phone,JEE percentile,Course Name\n";
    for(const auto & s : students){
        out <<"\"" << s.name <<"\","
            << s.email <<","
            << s.phone <<","
            << s.JEEpercentile << ","
            << "\"" << s.courseName << "\"\n";
    }
    out.close();
    cout << "registration data exported to " <<filename << endl;
}

void rewriteFile(const vector<Student>& students) {
    ofstream out("registrations.txt");
    for(const auto& s : students){
        out << s.name << "," << s.email << "," << s.phone << ","
        << s.JEEpercentile << "," << s.courseName << "\n";

    }
    out.close();
}


int main(){
    cout << "----------------------------------------" << endl;
    cout << " Welcome to our course registration system" << endl;
    cout << "----------------------------------------\n" << endl;

    vector<Course> courses = {
        Course("JEE Advanced Batch", "ADVJEE", 90, 20),
        Course("Main + Advanced Combo Batch", "COMBOJEE", 85, 30),
        Course("JEE Mains Only Batch", "MAINJEE", 75, 35),
        Course("Crash Course (30 Days)", "CRASH30", 65, 40),
        Course("One-Shot Revision Batch", "ONESHOT", 60, 50),
        Course("Question Bank Solving Course", "QBSOLVE", 55, 50),
        Course("PYQ Practice Batch", "PYQBATCH", 50, 60)
    };

    vector<Student> registeredstudents;
    loadRegistrationFromFile(registeredstudents, courses);

    ofstream file("registrations.txt", ios::app);
    if(!file){
        cout <<"Could not open file to save registrations!\n";
        return 1;
    }

    int option;
    do{
        cout << "\n-----------COURSE REGISTRATION SYSTEM-------------\n";
        cout <<"1. Register Student.\n";
        cout <<"2. View Registered Students.\n";
        cout <<"3. View course details.\n";
        cout <<"4. Exit.\n";
        cout <<"5. Sort Students Alphabetically by Name.\n";
        cout <<"6. Filter Students by Course.\n";
        cout <<"7. Export Registration to CSV.\n";
        cout <<"8. Modify Student.\n";
        cout <<"9. Delete Student.\n";
        cout <<"Enter your choice:";
        cin >> option;


    switch (option){
        case 1: {
            Student s = Student::createstudent();

            if(isDuplicateRegistration(registeredstudents, s.email, s.phone)) {
                cout << "Student with this email or phone number is already registered.\n";
                break;
            }

            cout << "\nAvailable Courses:\n";
            for(size_t i = 0; i <courses.size(); ++i){
                cout << i+1<< ". ";
                courses[i].Display();

            }

            int courseChoice;
            cout << "Choose a course (number):";
            cin>>courseChoice;

            if(courseChoice >= 1 && courseChoice <= courses.size()) {
                Course& selected = courses[courseChoice -1];
                if(selected.EnrollStudent(s.JEEpercentile)) {
                    s.courseName = selected.coursename;
                    

                    registeredstudents.push_back(s);
                    cout << "Successfully registered!\n";
                    file << s.name << "," << s.email << "," << s.phone << ","
                             << s.JEEpercentile << "," << s.courseName << "\n";
                    file.flush();
                }
        } else {
            cout << "Invalid course Choice.\n";

        }
        break;
    }

    case 2:
            cout << "\n== Registered Students ==\n";
                if (registeredstudents.empty()) {
                    cout << "No students registered yet.\n";
                } else {
                    for (const auto& s : registeredstudents) {
                        s.Display();
                    }
                }
                break;

    case 3:
            cout << "\n== Course Details ==\n";
                for (const auto& c : courses) {
                    c.Display();
                }
                break;

    case 4:
            cout << "Exiting program.\n";
                break;
    case 5: 
            if(registeredstudents.empty()){
                cout << "No students registered yet.\n";
                break;
            }

            sort(registeredstudents.begin(), registeredstudents.end(), [](const Student& a, const Student& b) {
                string nameA = a.name, nameB = b.name;
                transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
                transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
                return nameA < nameB;
            });

            cout << "\n== Students Sorted Alphabetically ==\n";
            for(const auto& s : registeredstudents){
                s.Display();
            }
            break;

    case 6: {
            if(registeredstudents.empty()) {
                cout << "No students registered yet.\n";
                break;
            }

            cout <<"\nAvailable Courses:\n";
            for(size_t i = 0; i < courses.size(); ++i ) {
                cout << i+1 << ". "<< courses[i].coursename << endl;
            }

            int courseChoice;
            cout << "Enter course number to filter: ";
            cin >> courseChoice;

            if(courseChoice >= 1 && courseChoice <= courses.size()) {
                string selectedCourse = courses[courseChoice - 1].coursename;
                cout << "\n== Students in \""<<selectedCourse << "\" ==\n";
                bool found = false;
                for (const auto& s : registeredstudents){
                    if(s.courseName == selectedCourse ) {
                        s.Display();
                        found = true;
                    }
                }
                if(!found) cout << "No students registered in this course yet.\n";
            }else{
                cout << "Invalid course selection.\n";
            }
            break;
        }

    case 7:
            if(registeredstudents.empty()) {
                cout << "No students registered yet.\n";
            }else {
                exportToCSV(registeredstudents, "registrations_export.csv");
            }
            break;
    
    case 8:{
        if(registeredstudents.empty()){
            cout << "No students registered yet.\n";
            break;
        }
        string emailToModify;
        cout << "Enter email of student to modify: ";
        cin >> emailToModify;

        auto it = find_if(registeredstudents.begin(), registeredstudents.end(),
                          [&emailToModify](const Student& s) { return s.email == emailToModify;});

        if (it != registeredstudents.end()){
            cout << "Editing student:\n";
            it->Display();

            string newName, newPhone, newEmail;
            float newJEE;

            do{
                cout << "Enter new name (or press Enter to keep current): ";
                getline(cin >> ws, newName);
                if(newName.empty()) break;
            } while (!isValidName(newName));
            if(!newName.empty()) it->name = newName;

            do{
                cout << "Enter new email (or press Enter to keep current): ";
                getline(cin, newEmail);
                if(newEmail.empty()) break;
                if(!isValidEmail(newEmail)) cout <<"Invalid email.\n";
            } while (!newEmail.empty() && !isValidEmail(newEmail));
            if(!newEmail.empty()) it->email = newEmail;

            do{
                cout << "Enter new phone number (or press Enter to keep current): ";
                getline(cin, newPhone);
                if(newPhone.empty()) break;
                if(!isValidPhone(newPhone)) cout <<"Invalid phone.\n";
            } while (!newPhone.empty() && !isValidPhone(newPhone));
            if(!newPhone.empty()) it->phone = newPhone;

            cout << "Enter new JEE percentile (-1 to keep current): ";
            cin >> newJEE;
            if(newJEE >= 0 && isValidPercentile(newJEE)) {
                it->JEEpercentile = newJEE;

            }

            cout << "Student details updated.\n";
            rewriteFile(registeredstudents);
        }else {
            cout << "Student with this email not found.\n";
        }
        break;
    }

    case 9: {
        if(registeredstudents.empty()) {
            cout << "No students registered yet.\n";
            break;
        }

        string emailToDelete;
        cout << "Enter email of students to delete: ";
        cin >> emailToDelete;

        auto it = find_if(registeredstudents.begin(), registeredstudents.end(),
                         [ &emailToDelete](const Student& s) { return s.email == emailToDelete; });

        if(it != registeredstudents.end()){
            cout << "Deleting student:\n";
            it->Display();

            for(auto& c : courses) {
                if(c.coursename == it->courseName) {
                    c.enrolled--;
                    break;
                }
            }

            registeredstudents.erase(it);
            cout << "Student deleted.\n";
            rewriteFile(registeredstudents);

        }else{
            cout << "Student with this email not found.\n";
        }
        break;
    }

        
    default:
            cout << "Invalid option. Please try again.\n";
        }

    } while (option != 4);


file.close();
cout<<"\nAll registrations saved to registrations.txt\n";
return 0;
}