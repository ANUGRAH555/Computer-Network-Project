#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

string getTimestamp()
{
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

bool pingHost(const string &host)
{
    string command;

#ifdef _WIN32
    command = "ping -n 1 " + host + " > nul";
#else
    command = "ping -c 1 " + host + " > /dev/null 2>&1";
#endif

    int response = system(command.c_str());
    return response == 0;
}

int main()
{

    vector<string> hosts = {
        "8.8.8.8",        // Google DNS
        "1.1.1.1",        // Cloudflare DNS
        "9.9.9.9",        // Quad9 DNS
        "208.67.222.222", // OpenDNS
        "8.8.4.4",        // Secondary Google DNS
        "1.0.0.1",        // Secondary Cloudflare DNS
        "google.com",
        "cloudflare.com",
        "facebook.com",
        "youtube.com",
        "twitter.com",
        "github.com",
        "amazon.com",
        "microsoft.com",
        "apple.com",
        "wikipedia.org",
        "bing.com",
        "reddit.com",
        "linkedin.com",
        "192.168.1.1", // Local gateway (router)
        "192.168.0.1", // Common alternate local gateway
        "example.com"  // Test domain (always online)
    };

    string logFile = "ping_log.txt";

    int interval = 60;

    cout << "🔍 Starting Automated Ping Test..." << endl;

    while (true)
    {
        for (const string &host : hosts)
        {
            bool status = pingHost(host);
            string result = getTimestamp() + " | " + host + " | " + (status ? "UP" : "DOWN");

            // Log to file
            ofstream log(logFile, ios::app);
            if (log.is_open())
            {
                log << result << endl;
                log.close();
            }

            // Print on screen
            cout << result << endl;

            // Alert if down
            if (!status)
            {
                cout << "⚠️ ALERT: " << host << " is not reachable!" << endl;
            }
        }

        cout << "Waiting for next check...\n"
             << endl;
        this_thread::sleep_for(chrono::seconds(interval));
    }

    return 0;
}
