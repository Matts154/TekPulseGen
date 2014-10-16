#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <iomanip>  //setprecision

using namespace std;

struct Pulse {
    double startTime, duration;
    bool positive;
};

/**
 * displayPulse(const Pulse&) - Outputs passed pulse structure to stdout. Used mainly for debug purposes.
 */
void displayPulse(const Pulse &p){
    cout << fixed << "Start time:\t" << p.startTime << endl
         << "Duration:\t" << p.duration << endl
         << "Positive?\t" << (p.positive ? "True" : "False") << endl << endl;
}

/**
 * displayAllPulses(queue<pulse>&) - Outputs contents of the queue to the screen. Used for debug purposes.
 */
void displayAllPulses(queue<Pulse> &q){
    while(!q.empty()){
        displayPulse(q.front());
        q.pop();
    }
}

/**
 * fillQueue(queue<Pulse>&, int, const double, const double, const bool) - Fills the queue based on given arguments.
 * 	q:	The queue used to store file output.
 *	n:	The number of pulses in this group.
 *	pT:	Pulse time.
 *	pW:	Pulse width.
 *	pos:	Determining a positive or negative group of pulses.
 */
void fillQueue(queue<Pulse> &q, int n, const double &pT, const double &pW, const bool &pos){
    double time;

    if(q.empty())
        time = 0.01;
    else
        time = q.back().startTime + q.back().duration + 0.01;

    for(; n > 0; n--){
        Pulse p;

        p.startTime = time;
        p.duration = pW;
        p.positive = pos;

        q.push(p);

        time += pT;
    }
}

/**
 * exportTime(queue<Pulse>&) - Exports a CSV with start/end pulse points. No longer used in this version.
 */
void exportTime(queue<Pulse> &q){
    ofstream file;
    string fileName;

    cin.ignore();

    do {
        cout << "File name: ";

        getline(cin, fileName);

        file.open(fileName.c_str());

        if(!file.good())
            cout << "Invalid file name.\n";

    } while(!file.good());

    file << fixed;

    while(!q.empty()){
        Pulse p = q.front();

        //Output pulse
        file << p.startTime << ",0\n"
             << p.startTime << ",1\n";

        file << p.startTime + p.duration << ",1\n"
             << p.startTime + p.duration << ",0\n";

        q.pop();
    }
}

/**
 * exportPoints(queue<Pulse>&, const double&) - Exports a CSV filled with points based on the smallest pulse width (s).
 */
void exportPoints(queue<Pulse> &q, const double &s){
    ofstream file;
    string fileName;
    double time = 0.0;

    cin.ignore();

    do {
        cout << "File name: ";

        getline(cin, fileName);

        file.open(fileName.c_str());

        if(!file.good())
            cout << "Invalid file name.\n";

    } while(!file.good());

    file << fixed;

    while(!q.empty()){
        Pulse p = q.front();
        bool first = true;

        do {
            if(time < p.startTime){
                file << time << ",0\n";
                time += s;
            }
            else if(time >= p.startTime && time <= p.startTime + p.duration){
                if(first){
                    file << time << ",0\n";
                    first = false;
                }

                file << time << "," << (p.positive ? "" : "-") << "1\n";
                time += s;
            }

        } while(time <= p.startTime + p.duration);

        file << (time-s) << ",0\n";

        q.pop();
    }

    file.close();
}

/**
 * main() - Reads user input and outputs status messages.
 */
int main(){
    int segNum;
    double smallest = 1;
    queue<Pulse> pulses;
    Pulse last;

    fillQueue(pulses, 1, 0, 0.535, true);

    cout << "Number of segments: ";
    cin >> segNum;

    for(int i = 1; i <= segNum; i++){
        int pulseNum;
        double pulseTime, pulseWidth;
        bool pos = true;
        char temp;

        cout << "\nSegment #" << i << endl;
        cout << "Number of pulses: ";
        cin >> pulseNum;

        cout << "PRF: ";
        cin >> pulseTime;

        cout << "Pulse Width (in microseconds): ";
        cin >> pulseWidth;

        cout << "Positive or negative pulse? [+/-]: ";
        cin >> temp;

        pulseTime = 1/pulseTime;

        pulseWidth *= 0.000001;

        if(pulseWidth < smallest)
            smallest = pulseWidth;

        if(temp == '-')
            pos = false;

        fillQueue(pulses, pulseNum, pulseTime, pulseWidth, pos);
    }

    last = pulses.back();
    cout << fixed
         << "\nLast Pulse Time: " << (last.startTime + last.duration) << endl;
    cout << "Number of Points: " << setprecision(0) << ((last.startTime + last.duration) / smallest) + (pulses.size() * 2) << endl << endl;

    exportPoints(pulses, smallest);

    cout << "\nPress enter to exit. ";
    getchar();

    return 0;
}
