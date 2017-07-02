/*
    Date created: June 18th, 2017
    Author: Dung Harry
    Compiler: GCC for C++ version 6.3.0
    Contest: Top Career Code Challenge Summer 2017

    Description: this is the solution for the first task in C++ programming language

    The task:
            James is a businessman. He is on a tight schedule this week.
            The week starts on Monday at 00:00 and ends on Sunday at 24:00.
            His schedule consists of M meetings he needs to take part in.
            Each of them will take place in a period of time, beginning and ending on the same day
        (there are no two ongoing meetings at the same time).
            James is very tired thus he needs to find the longest period of time there are no ongoing meetings.
            The sleeping break can begin and end on different days and should begin and end in the same week.
            You are given a string containing M lines.
            Each line is a substring representing one meeting in the schedule, in the format "Ddd hh:mm-hh:mm".
            "Ddd" is a three-letter abbreviation for the day of the week when the meeting takes place: "Mon"(Monday),
        "Tue", "Web", "Thu", "Fri", "Sat", "Sun".
            "hh:mm-hh:mm" means the beginning time and the ending time of the meeting(from 00:00 to 24:00 inclusive).
            The given times represent exact moments of time.
            So, there are exactly five minutes between 13:40 and 13:45.


    Compile and run by the following commands:
        g++ -std=c++0x -o task1 task1.cpp
        ./task1
*/

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <utility>

using namespace std;

enum WeekDay {
    WEEK_DAY_SUNDAY = 0,
    WEEK_DAY_MONDAY,
    WEEK_DAY_TUESDAY,
    WEEK_DAY_WEDNESDAY,
    WEEK_DAY_THURSDAY,
    WEEK_DAY_FRIDAY,
    WEEK_DAY_SATURDAY,
    WEEK_DAY_COUNT
};

class Duration {
public:
    Duration();
    Duration(int32_t iBegin, int32_t iEnd);
    Duration(const Duration &duration);

    virtual ~Duration();

    int32_t getBeginPoint() const;
    void setBeginPoint(int32_t iBeginPoint);

    int32_t getEndPoint() const;
    void setEndPoint(int32_t iEndPoint);

    bool operator<(const Duration &duration) const;
private:
    int32_t iBeginPoint;
    int32_t iEndPoint;
};

class TimeConverter {
public:
    TimeConverter();

    virtual ~TimeConverter();

    static TimeConverter* getInstance();

    Duration convertToDuration(const char *cpcTime);
    char* convertToChars(Duration &duration);
private:
    static TimeConverter *pInstance;
};

class FileIO {
public:
    FileIO(const char *cpcFileName);
    virtual ~FileIO();

    bool handle();

    const map<int32_t, set<Duration> *>* getDurations();

    const char* getFileName();
    void setFileName(const char *cpcFileName);
private:
    string *psFileName;
    ifstream *pStream;

    map<int32_t, set<Duration> *> *pDurations;
};

class RestHandler {
public:
    RestHandler();

    virtual ~RestHandler();

    bool handle(const map<int32_t, set<Duration> *> *pDurations);

    int32_t getLongestDuration();

    const map<int32_t, set<int32_t> *> *getRests();
private:
    map<int32_t, set<int32_t> *> *pRests;
};

TimeConverter * TimeConverter::pInstance = NULL;

int32_t main(int32_t argc, char **argv) {
    FileIO *pFileIO = new FileIO("schedules.txt");
    RestHandler *pRestHandler = new RestHandler();

    pFileIO->handle();

    pRestHandler->handle(pFileIO->getDurations());

    cout<<"The longest sleep duration is: "<<pRestHandler->getLongestDuration()<<endl;

    delete pFileIO;
    delete pRestHandler;
}

Duration::Duration() :
    iBeginPoint {0},
    iEndPoint {0}
{

}

Duration::Duration(int32_t iBegin, int32_t iEnd) :
    iBeginPoint {iBegin},
    iEndPoint {iEnd}
{

}

Duration::Duration(const Duration &duration) :
    iBeginPoint {duration.iBeginPoint},
    iEndPoint {duration.iEndPoint}
{

}

Duration::~Duration() {

}

int32_t Duration::getBeginPoint() const {
    return this->iBeginPoint;
}

void Duration::setBeginPoint(int32_t iBeginPoint) {
    if(iBeginPoint <= 0)
        return;

    this->iBeginPoint = iBeginPoint;
}

int32_t Duration::getEndPoint() const {
    return this->iEndPoint;
}

void Duration::setEndPoint(int32_t iEndPoint) {
    if(iEndPoint <= 0)
        return;

    this->iEndPoint = iEndPoint;
}

bool Duration::operator<(const Duration &duration) const {
    return iEndPoint < duration.iBeginPoint;
}

TimeConverter::TimeConverter() {

}

TimeConverter::~TimeConverter() {

}

TimeConverter* TimeConverter::getInstance() {
    return (pInstance == NULL ? (pInstance = new TimeConverter()) : pInstance);
}

Duration TimeConverter::convertToDuration(const char *cpcTime) {
    int32_t iBeginHour = 0, iBeginMinute = 0, iEndHour = 0, iEndMinute = 0;
    Duration duration;

    if(cpcTime == NULL)
        return Duration();

    sscanf(cpcTime, "%d:%d-%d:%d", &iBeginHour, &iBeginMinute, &iEndHour, &iEndMinute);

    duration.setBeginPoint(iBeginHour * 60 + iBeginMinute);
    duration.setEndPoint(iEndHour * 60 + iEndMinute);

    return duration;
}

char* TimeConverter::convertToChars(Duration &duration) {
    int32_t iBeginHour = 0, iBeginMinute = 0, iEndHour = 0, iEndMinute = 0;
    char *pcResult;

    if(duration.getBeginPoint() <= 0 || duration.getEndPoint() <= 0)
        return NULL;

    pcResult = (char *) malloc(sizeof(char) * (strlen("hh:mm-hh:mm") + 1));

    iBeginHour = duration.getBeginPoint() / 60;
    iBeginMinute = duration.getBeginPoint() % 60;
    iEndHour = duration.getEndPoint() / 60;
    iEndMinute = duration.getEndPoint() % 60;

    return pcResult;
}

FileIO::FileIO(const char *cpcFileName) :
    psFileName {cpcFileName == NULL ? new string() : new string(cpcFileName)},
    pDurations {new map<int32_t, set<Duration> *>()},
    pStream {new ifstream()}
{
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_SUNDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_MONDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_TUESDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_WEDNESDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_THURSDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_FRIDAY, new set<Duration>()));
    this->pDurations->insert(pair<int32_t, set<Duration> *>(WEEK_DAY_SATURDAY, new set<Duration>()));

    if(this->psFileName->size() > 0)
        this->pStream->open(this->psFileName->c_str());
}

FileIO::~FileIO() {
    int32_t i;

    if(this->psFileName != NULL)
        delete this->psFileName;

    if(this->pDurations != NULL) {
        for(i = 0; i < this->pDurations->size(); i ++)
            if(this->pDurations->at(i) != NULL) {
                this->pDurations->at(i)->clear();

                delete this->pDurations->at(i);
            }

        this->pDurations->clear();

        delete this->pDurations;
    }

    if(this->pStream != NULL) {
        if(this->pStream->is_open())
            this->pStream->close();

        delete this->pStream;
    }
}

bool FileIO::handle() {
    int32_t i, iLength;
    char *pcBuffer, *pcDay, *pcDuration;

    if(((this->psFileName == NULL || this->psFileName->size() <= 0) && this->pStream->is_open() == false) || this->pDurations == NULL || this->pDurations->size() < WEEK_DAY_COUNT)
        return false;

    if(this->pStream->is_open() == false) {
        this->pStream->open(this->psFileName->c_str());

        if(this->pStream->is_open() == false)
            return false;
    } else {
        this->pStream->clear();
        this->pStream->seekg(0, this->pStream->beg);
    }

    for(i = 0; i < WEEK_DAY_COUNT; i ++)
        if(this->pDurations->at(i) != NULL)
            this->pDurations->at(i)->clear();

    pcBuffer = new char[100];
    pcDay = new char[10];
    pcDuration = new char[50];

    memset(pcBuffer, 0, 100);
    memset(pcDay, 0, 10);
    memset(pcDuration, 0, 50);

    while(this->pStream->eof() == false) {
        for(i = 0; this->pStream->get(*(pcBuffer + i)) && *(pcBuffer + i) != '\n' && this->pStream->eof() == false && i < 100; i ++);
        *(pcBuffer + (i < 100 ? i : 99)) = '\0';

        sscanf(pcBuffer, "%s %s", pcDay, pcDuration);

        iLength = strlen(pcDay);

        for(i = 0; i < iLength; i ++)
            *(pcDay + i) = tolower(*(pcDay + i));

        if(strcmp(pcDay, "sun") == 0)
            i = WEEK_DAY_SUNDAY;
        else if(strcmp(pcDay, "mon") == 0)
            i = WEEK_DAY_MONDAY;
        else if(strcmp(pcDay, "tue") == 0)
            i = WEEK_DAY_TUESDAY;
        else if(strcmp(pcDay, "wed") == 0)
            i = WEEK_DAY_WEDNESDAY;
        else if(strcmp(pcDay, "thu") == 0)
            i = WEEK_DAY_THURSDAY;
        else if(strcmp(pcDay, "fri") == 0)
            i = WEEK_DAY_FRIDAY;
        else if(strcmp(pcDay, "sat") == 0)
            i = WEEK_DAY_SATURDAY;
        else
            i = -1;

        if(i >= 0 && this->pDurations->at(i) != NULL) {
            this->pDurations->at(i)->insert(TimeConverter::getInstance()->convertToDuration(pcDuration));
        }
    }

    return true;
}

const map<int32_t, set<Duration> *>* FileIO::getDurations() {
    return this->pDurations;
}

const char* FileIO::getFileName() {
    return this->psFileName == NULL ? NULL : this->psFileName->c_str();
}

void FileIO::setFileName(const char *cpcFileName) {
    if(cpcFileName == NULL)
        return;

    if(this->psFileName != NULL)
        delete this->psFileName;

    this->psFileName = new string(cpcFileName);

    if(this->pStream != NULL && this->pStream->is_open() == true)
        this->pStream->close();
}

RestHandler::RestHandler() :
    pRests {new map<int32_t, set<int32_t>*>()}
{
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_SUNDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_MONDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_TUESDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_WEDNESDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_THURSDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_FRIDAY, new set<int32_t>()));
    this->pRests->insert(pair<int32_t, set<int32_t>*>(WEEK_DAY_SATURDAY, new set<int32_t>()));
}

RestHandler::~RestHandler() {
    int32_t i;

    if(this->pRests != NULL) {
        for(i = 0; i < WEEK_DAY_COUNT; i ++)
            if(this->pRests->at(i) != NULL) {
                this->pRests->at(i)->clear();

                delete this->pRests->at(i);
            }

        this->pRests->clear();

        delete this->pRests;
    }
}

bool RestHandler::handle(const map<int32_t, set<Duration> *> *pDurations) {
    int32_t i, iRestDuration;
    set<Duration>::iterator iter, nextIter;

    if(pDurations == NULL || pDurations->size() < WEEK_DAY_COUNT)
        return false;

    if(this->pRests != NULL)
        for(i = 0; i < WEEK_DAY_COUNT; i ++)
            if(this->pRests->at(i) != NULL && this->pRests->at(i)->size() > 0)
                this->pRests->at(i)->clear();

    for(i = 0; i < WEEK_DAY_COUNT; i ++) {
        if(pDurations->at(i) != NULL && this->pRests->at(i) != NULL) {
            if(pDurations->at(i)->begin() != pDurations->at(i)->end()) {
                this->pRests->at(i)->insert(pDurations->at(i)->begin()->getBeginPoint());

                for(iter = pDurations->at(i)->begin(), nextIter = iter, ++ nextIter; nextIter != pDurations->at(i)->end(); ++ iter, ++ nextIter) {
                    iRestDuration = nextIter->getBeginPoint() - iter->getEndPoint();

                    this->pRests->at(i)->insert(iRestDuration);
                }

                this->pRests->at(i)->insert(24 * 60 - iter->getEndPoint());
            } else
                this->pRests->at(i)->insert(24 * 60);
        }
    }

    return true;
}

const map<int32_t, set<int32_t>*>* RestHandler::getRests() {
    return this->pRests;
}

int32_t RestHandler::getLongestDuration() {
    int32_t iLongestDuration = 0;
    int32_t i;

    if(this->pRests == NULL || this->pRests->size() < WEEK_DAY_COUNT)
        return 0;

    for(i = 0; i < WEEK_DAY_COUNT; i ++)
        if(this->pRests->at(i) != NULL && iLongestDuration < *(this->pRests->at(i)->rbegin()))
            iLongestDuration = *(this->pRests->at(i)->rbegin());

    return iLongestDuration;
}
