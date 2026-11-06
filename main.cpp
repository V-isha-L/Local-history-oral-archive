#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <cstdlib>
using namespace std;

const int MAX_RECORDS = 30;
const int STEP_COUNT = 8;
const string STEP_NAMES[STEP_COUNT] = {
    "Consent Form Signed",
    "Audio Digitized",
    "Transcription Done",
    "Transcript Proofread",
    "Keyword Tagging",
    "Place and Date Verified",
    "Metadata Entered",
    "Archive Published"};

class OralRecord
{
private:
    int recordId = 0;
    string title;
    string narrator;
    string interviewer;
    string location;
    string interviewDate;
    string keywords;
    int durationMinutes = 0;
    bool indexed[STEP_COUNT] = {};
    static int totalRecords;

public:
    void setDetails(int id, const string &recordTitle,
                    const string &narratorName,
                    const string &interviewerName,
                    const string &place,
                    const string &date,
                    const string &tags,
                    int duration)
    {
        this->recordId = id;
        title = recordTitle;
        narrator = narratorName;
        interviewer = interviewerName;
        location = place;
        interviewDate = date;
        keywords = tags;
        durationMinutes = duration;
        for (int i = 0; i < STEP_COUNT; i++)
        {
            indexed[i] = false;
        }
        totalRecords++;
    }

    inline int getId() const
    {
        return recordId;
    }

    inline string getNarrator() const
    {
        return narrator;
    }

    inline string getKeywords() const
    {
        return keywords;
    }

    static int getTotalRecords()
    {
        return totalRecords;
    }

    void showSummary() const
    {
        cout << "ID: " << recordId
             << " | Title: " << title
             << " | Narrator: " << narrator
             << " | Place: " << location << '\n';
    }

    void showDetails() const
    {
        cout << "\nRecord ID: " << recordId
             << "\nTitle: " << title
             << "\nNarrator: " << narrator
             << "\nInterviewer: " << interviewer
             << "\nLocation: " << location
             << "\nInterview date: " << interviewDate
             << "\nKeywords: " << keywords
             << "\nDuration: " << durationMinutes << " minutes\n";
    }

    void showChecklist() const
    {
        cout << "\nIndexing checklist for Record "
             << recordId << " (" << title << ")\n";
        for (int i = 0; i < STEP_COUNT; i++)
        {
            cout << i + 1 << ". ["
                 << (indexed[i] ? 'X' : ' ')
                 << "] " << STEP_NAMES[i] << '\n';
        }
        cout << "Checklist is read-only in CA1.\n";
    }

    friend void displayIndexingStatus(const OralRecord &record);
};

int OralRecord::totalRecords = 0;

void displayIndexingStatus(const OralRecord &record)
{
    int done = 0;
    for (int i = 0; i < STEP_COUNT; i++)
    {
        if (record.indexed[i])
        {
            done++;
        }
    }
    cout << "Indexing progress: "
         << done << "/" << STEP_COUNT
         << " steps complete\n";
}

int readInt(const string &prompt, int minimum, int maximum)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof())
        {
            cout << "\nInput ended. Exiting.\n";
            exit(0);
        }
        cout << "Invalid number. Enter a value from "
             << minimum << " to " << maximum << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string &prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, value))
        {
            cout << "\nInput ended. Exiting.\n";
            exit(0);
        }
        if (value.find_first_not_of(" \t\r\n") != string::npos)
        {
            return value;
        }
        cout << "This field cannot be empty.\n";
    }
}

bool validDateFormat(const string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
    {
        return false;
    }
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }
    int month = (date[5] - '0') * 10 + (date[6] - '0');
    int day = (date[8] - '0') * 10 + (date[9] - '0');
    return month >= 1 && month <= 12 &&
           day >= 1 && day <= 31;
}

string readDate()
{
    while (true)
    {
        string date = readText("Interview date (YYYY-MM-DD): ");
        if (validDateFormat(date))
        {
            return date;
        }
        cout << "Invalid date format or month/day range.\n";
    }
}

string toLowerText(string text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
    return text;
}

OralRecord *findRecord(OralRecord records[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (records[i].getId() == id)
        {
            return &records[i];
        }
    }
    return nullptr;
}

void addRecord(OralRecord records[], int &count)
{
    if (count >= MAX_RECORDS)
    {
        cout << "Archive full. Maximum "
             << MAX_RECORDS << " records.\n";
        return;
    }
    int id = readInt("Unique Record ID (1-999999): ", 1, 999999);
    if (findRecord(records, count, id) != nullptr)
    {
        cout << "Duplicate Record ID. Nothing was added.\n";
        return;
    }
    string title = readText("Recording title: ");
    string narrator = readText("Narrator name: ");
    string interviewer = readText("Interviewer name: ");
    string location = readText("Location / locality: ");
    string date = readDate();
    string tags = readText("Keywords (comma separated): ");
    int duration = readInt("Duration in minutes (1-1440): ", 1, 1440);

    records[count].setDetails(
        id, title, narrator, interviewer, location, date, tags, duration);
    count++;
    cout << "\nRecord indexed successfully.\n";
}

void viewAll(const OralRecord records[], int count)
{
    if (count == 0)
    {
        cout << "No records found.\n";
        return;
    }
    cout << "\nAll Records:\n";
    for (int i = 0; i < count; i++)
    {
        records[i].showSummary();
    }
}

void viewRecordDetails(OralRecord records[], int count)
{
    int id = readInt("Enter Record ID: ", 1, 999999);
    OralRecord *found = findRecord(records, count, id);
    if (found == nullptr)
    {
        cout << "Record not found.\n";
    }
    else
    {
        found->showDetails();
    }
}

void viewChecklist(OralRecord records[], int count)
{
    int id = readInt("Enter Record ID: ", 1, 999999);
    OralRecord *found = findRecord(records, count, id);
    if (found == nullptr)
    {
        cout << "Record not found.\n";
    }
    else
    {
        found->showChecklist();
        displayIndexingStatus(*found);
    }
}

void searchByKeyword(const OralRecord records[], int count)
{
    string term = toLowerText(readText("Search word (keyword or narrator): "));
    int matches = 0;
    for (int i = 0; i < count; i++)
    {
        string tags = toLowerText(records[i].getKeywords());
        string name = toLowerText(records[i].getNarrator());
        if (tags.find(term) != string::npos ||
            name.find(term) != string::npos)
        {
            if (matches == 0)
            {
                cout << "\nMatching Records:\n";
            }
            records[i].showSummary();
            matches++;
        }
    }
    if (matches == 0)
    {
        cout << "No matching records.\n";
    }
}

int main()
{
    OralRecord records[MAX_RECORDS];
    int count = 0;

    while (true)
    {
        cout << "\n===== LOCAL HISTORY ORAL ARCHIVE INDEX =====\n";
        cout << "1. Add new record\n";
        cout << "2. View all records\n";
        cout << "3. View record details\n";
        cout << "4. View indexing checklist\n";
        cout << "5. Search by keyword or narrator\n";
        cout << "6. View total records\n";
        cout << "7. Exit\n";

        int choice = readInt("Choose (1-7): ", 1, 7);

        if (choice == 1)
        {
            addRecord(records, count);
        }
        else if (choice == 2)
        {
            viewAll(records, count);
        }
        else if (choice == 3)
        {
            viewRecordDetails(records, count);
        }
        else if (choice == 4)
        {
            viewChecklist(records, count);
        }
        else if (choice == 5)
        {
            searchByKeyword(records, count);
        }
        else if (choice == 6)
        {
            cout << "Total records: "
                 << OralRecord::getTotalRecords() << '\n';
        }
        else
        {
            cout << "Goodbye...\n";
            break;
        }
    }
    return 0;
}