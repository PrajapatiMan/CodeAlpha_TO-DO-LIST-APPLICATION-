#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Task {
public:
    string description;
    bool completed;
    string category;

    Task(string desc, string cat = "") {
        description = desc;
        completed = false;
        category = cat;
    }

    void markCompleted() {
        completed = true;
    }

    void display() const {
        cout << (completed ? "[✅] " : "[ ] ") << description;
        if (!category.empty()) {
            cout << " (Category: " << category << ")";
        }
        cout << endl;
    }
};

class TaskManager {
private:
    vector<Task> tasks;
    const string filename = "tasks.txt";

public:
    void addTask(const string& desc, const string& category = "") {
        tasks.push_back(Task(desc, category));
    }

    void markTaskCompleted(int index) {
        if (index >= 0 && index < tasks.size()) {
            tasks[index].markCompleted();
        } else {
            cout << "Invalid task number." << endl;
        }
    }

    void viewTasks(bool completed) const {
        cout << (completed ? "\nCompleted Tasks:\n" : "\nPending Tasks:\n");
        for (int i = 0; i < tasks.size(); ++i) {
            if (tasks[i].completed == completed) {
                cout << i + 1 << ". ";
                tasks[i].display();
            }
        }
    }

    void saveTasks() const {
        ofstream file(filename);
        for (const auto& task : tasks) {
            file << task.description << "|" << task.completed << "|" << task.category << endl;
        }
        file.close();
    }

    void loadTasks() {
        tasks.clear();
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.rfind('|');
            string desc = line.substr(0, pos1);
            bool comp = line.substr(pos1 + 1, pos2 - pos1 - 1) == "1";
            string cat = line.substr(pos2 + 1);
            Task t(desc, cat);
            if (comp) t.markCompleted();
            tasks.push_back(t);
        }
        file.close();
    }
};

int main() {
    TaskManager manager;
    manager.loadTasks();

    int choice;
    do {
        cout << "\n--- TO-DO LIST MENU ---\n";
        cout << "1. Add Task\n";
        cout << "2. Mark Task as Completed\n";
        cout << "3. View Pending Tasks\n";
        cout << "4. View Completed Tasks\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string desc, cat;
            cout << "Enter task description: ";
            getline(cin, desc);
            cout << "Enter category (optional): ";
            getline(cin, cat);
            manager.addTask(desc, cat);
        } else if (choice == 2) {
            int num;
            manager.viewTasks(false);
            cout << "Enter task number to mark as completed: ";
            cin >> num;
            manager.markTaskCompleted(num - 1);
        } else if (choice == 3) {
            manager.viewTasks(false);
        } else if (choice == 4) {
            manager.viewTasks(true);
        } else if (choice != 5) {
            cout << "Invalid choice. Try again." << endl;
        }

        manager.saveTasks();

    } while (choice != 5);

    cout << "Goodbye!\n";
    return 0;
}
