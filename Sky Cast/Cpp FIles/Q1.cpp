// SKY CAST - Weather Forecast System (Options 1 to 6 Fully Implemented)
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <map>
#include <fstream>
#include <rapidjson/document.h>

using namespace std;

const string API_KEY = "0127742c55ae8c8c6b9dc1008ec2697a";
const string COUNTRY_CODE = ",PK";

const string pakistaniCities[] = {
    "Karachi", "Lahore", "Islamabad", "Rawalpindi", "Multan", "Faisalabad",
    "Peshawar", "Quetta", "Sialkot", "Gujranwala", "Hyderabad",
    "Bahawalpur", "Sargodha", "Abbottabad", "Muzaffarabad"
};
const int cityCount = sizeof(pakistaniCities) / sizeof(pakistaniCities[0]);

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string fetchWeatherData(const string& city) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        string url = "https://api.openweathermap.org/data/2.5/weather?q=" + city + COUNTRY_CODE + "&appid=" + API_KEY + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

string toLower(const string& s) {
    string result = s;
    for (char& c : result) if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    return result;
}

bool parseAndDisplayWeather(const string& data, const string& city, bool show = true) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());

    if (!doc.HasParseError() && doc.HasMember("main")) {
        double temp = doc["main"]["temp"].GetDouble();
        int humidity = doc["main"]["humidity"].GetInt();
        string weather = doc["weather"][0]["description"].GetString();

        if (show) {
            cout << "Weather in " << city << ":\n";
            cout << "Temperature: " << temp << "°C\n";
            cout << "Humidity: " << humidity << "%\n";
            cout << "Condition: " << weather << "\n\n";
        }
        return true;
    }
    return false;
}

double extractTemp(const string& data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    if (!doc.HasParseError() && doc.HasMember("main"))
        return doc["main"]["temp"].GetDouble();
    return NAN;
}

void option1() {
    string city;
    cout << "Enter city name in Pakistan: ";
    cin.ignore();
    getline(cin, city);
    string data = fetchWeatherData(city);
    parseAndDisplayWeather(data, city);
}

void option2() {
    string condition;
    cout << "Enter weather condition (clear, rain, clouds, etc.): ";
    cin.ignore();
    getline(cin, condition);

    condition = toLower(condition);
    bool foundAny = false;
    cout << "\nCities matching condition '" << condition << "':\n";

    for (int i = 0; i < cityCount; ++i) {
        string data = fetchWeatherData(pakistaniCities[i]);
        rapidjson::Document doc;
        doc.Parse(data.c_str());

        if (doc.HasParseError() || !doc.HasMember("weather")) continue;

        string desc = doc["weather"][0]["main"].GetString();
        desc = toLower(desc);

        if (desc.find(condition) != string::npos) {
            foundAny = true;
            double temp = doc["main"]["temp"].GetDouble();
            cout << " - " << pakistaniCities[i] << ": " << desc << ", Temp: " << fixed << setprecision(1) << temp << " °C\n";
        }
    }

    if (!foundAny) {
        cout << "No cities found with the weather condition '" << condition << "'.\n";
    }
}

void option3() {
    double targetTemp;
    cout << "Enter target temperature (°C): ";
    cin >> targetTemp;

    const double TOLERANCE = 1.0;
    bool found = false;

    for (int i = 0; i < cityCount; ++i) {
        string city = pakistaniCities[i];
        string data = fetchWeatherData(city);
        double temp = extractTemp(data);

        if (!isnan(temp) && fabs(temp - targetTemp) <= TOLERANCE) {
            parseAndDisplayWeather(data, city);
            found = true;
        }
    }

    if (!found) {
        cout << "No cities in Pakistan found with temperature near " << targetTemp << "°C.\n";
    }
}

string fetchForecastData(const string& city) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        string url = "https://api.openweathermap.org/data/2.5/forecast?q=" + city + COUNTRY_CODE + "&appid=" + API_KEY + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

void option4() {
    string city;
    cout << "Enter a city in Pakistan to get a 5-day weather forecast: ";
    cin.ignore();
    getline(cin, city);

    string data = fetchForecastData(city);
    rapidjson::Document doc;
    doc.Parse(data.c_str());

    if (doc.HasParseError() || !doc.HasMember("list")) {
        cout << "Error: Unable to parse forecast data for " << city << ".\n";
        return;
    }

    const rapidjson::Value& forecastList = doc["list"];
    map<string, string[10]> forecastsByDate;
    map<string, int> entryCount;

    for (rapidjson::SizeType i = 0; i < forecastList.Size(); ++i) {
        const auto& item = forecastList[i];

        time_t rawTime = item["dt"].GetInt64();
        tm* timeInfo = localtime(&rawTime);
        char dateStr[11], timeStr[6];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
        strftime(timeStr, sizeof(timeStr), "%H:%M", timeInfo);

        double temp = item["main"]["temp"].GetDouble();
        int humidity = item["main"]["humidity"].GetInt();
        string description = item["weather"][0]["description"].GetString();

        char line[200];
        snprintf(line, sizeof(line), "%-6s | Temp: %5.1f°C | Humidity: %3d%% | Condition: %s",
                 timeStr, temp, humidity, description.c_str());

        string dateKey = dateStr;
        int& count = entryCount[dateKey];
        if (count < 10) {
            forecastsByDate[dateKey][count++] = line;
        }
    }

    cout << "\n========= 5-Day Weather Forecast for " << city << " =========\n";
    for (const auto& pair : forecastsByDate) {
        cout << "\n🗓 Date: " << pair.first << "\n";
        cout << "-----------------------------------------------\n";
        int count = entryCount[pair.first];
        for (int i = 0; i < count; ++i) {
            cout << pair.second[i] << "\n";
        }
    }
    cout << "\n=============================================================\n";
}

class DayWeather {
private:
    string date, dayName, condition;
    float temperature, humidity;
    int uvIndex;

public:
    void setData(const string& d, const string& dn, float temp, float hum, const string& cond, int uv) {
        date = d; dayName = dn; temperature = temp; humidity = hum; condition = cond; uvIndex = uv;
    }
    string getDate() const { return date; }
    string getDayName() const { return dayName; }
    float getTemperature() const { return temperature; }
    float getHumidity() const { return humidity; }
    string getCondition() const { return condition; }
    int getUVIndex() const { return uvIndex; }

    friend void displayDayWeather(const DayWeather&);
};

void displayDayWeather(const DayWeather& day) {
    cout << left << setw(12) << day.date << setw(10) << day.dayName
         << setw(8) << day.temperature << setw(10) << day.humidity
         << setw(10) << day.condition << setw(5) << day.uvIndex << endl;
}

class WeekWeather {
public:
    DayWeather week[7];
    int count = 0;
};

bool loadFromFile(WeekWeather& week, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    string d, dn, cond;
    float temp, hum;
    int uv;
    week.count = 0;

    while (file >> d >> dn >> temp >> hum >> cond >> uv && week.count < 7) {
        DayWeather day;
        day.setData(d, dn, temp, hum, cond, uv);
        week.week[week.count++] = day;
    }
    file.close();
    return true;
}

void displayWeekWeather(const WeekWeather& week) {
    cout << "\n===== Past Week's Weather History =====\n\n";
    cout << left << setw(12) << "Date" << setw(10) << "Day"
         << setw(8) << "Temp" << setw(10) << "Humidity"
         << setw(10) << "Weather" << setw(5) << "UV" << "\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < week.count; ++i)
        displayDayWeather(week.week[i]);
}

void displaySummary(const WeekWeather& week) {
    if (week.count == 0) return;
    float totalTemp = 0, totalHumidity = 0;
    int hottest = 0, coldest = 0;

    for (int i = 0; i < week.count; ++i) {
        DayWeather day = week.week[i];
        totalTemp += day.getTemperature();
        totalHumidity += day.getHumidity();
        if (day.getTemperature() > week.week[hottest].getTemperature()) hottest = i;
        if (day.getTemperature() < week.week[coldest].getTemperature()) coldest = i;
    }

    cout << "\n--- Weekly Summary ---\n";
    cout << "Average Temperature: " << totalTemp / week.count << "°C\n";
    cout << "Average Humidity: " << totalHumidity / week.count << "%\n";
    cout << "Hottest Day: " << week.week[hottest].getDate() << " (" << week.week[hottest].getDayName() << ") - " << week.week[hottest].getTemperature() << "°C\n";
    cout << "Coldest Day: " << week.week[coldest].getDate() << " (" << week.week[coldest].getDayName() << ") - " << week.week[coldest].getTemperature() << "°C\n";
}

void option5() {
    map<int, string> files = {
        {1, "punjab_weather.txt"},
        {2, "sindh_weather.txt"},
        {3, "kp_weather.txt"},
        {4, "balochistan_weather.txt"}
    };
    int choice;
    cout << "\nSelect a province to view weather history:\n";
    cout << "1. Punjab\n2. Sindh\n3. KP\n4. Balochistan\nEnter choice: ";
    cin >> choice;

    if (files.find(choice) != files.end()) {
        WeekWeather week;
        if (loadFromFile(week, files[choice])) {
            displayWeekWeather(week);
            displaySummary(week);
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

void option6() {
    ifstream file("historical_alerts.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open 'historical_alerts.txt'.\n";
        return;
    }

    cout << "\n======= Historical Weather Alerts & Safety Measures =======\n\n";
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
    cout << "\n===========================================================\n";
}
void option7() {
    int subChoice;
    while (true) {
        cout << "\n=== Learn About Weather ===\n";
        cout << "[1] Learn Weather Types\n";
        cout << "[2] Fun Weather Facts\n";
        cout << "[3] Back to Main Menu\n";
        cout << "Choose an option: ";
        cin >> subChoice;

        if (subChoice == 1) {
            ifstream file("weather_types.txt");
            if (file.is_open()) {
                cout << "\n--- Weather Types ---\n";
                string line;
                while (getline(file, line)) {
                    cout << line << "\n";
                }
                file.close();
            } else {
                cout << "Error: 'weather_types.txt' not found.\n";
            }
        } else if (subChoice == 2) {
            ifstream file("weather_facts.txt");
            if (file.is_open()) {
                cout << "\n--- Fun Weather Facts ---\n";
                string line;
                while (getline(file, line)) {
                    cout << line << "\n";
                }
                file.close();
            } else {
                cout << "Error: 'weather_facts.txt' not found.\n";
            }
        } else if (subChoice == 3) {
            break;
        } else {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }
}

int main() {
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        cout << "============================================================" << endl;
        cout << "                      ☁️  S K Y   C A S T  ☀️                " << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "         Real-time Weather and Temperature Updates          " << endl;
        cout << "============================================================" << endl;
        cout << endl;

        cout << "                [1] Check Weather by City Name" << endl;
        cout << "                [2] Search by Weather Condition" << endl;
        cout << "                [3] Search by Temperature" << endl;
        cout << "                [4] 5-Day Weather Forecast" << endl;
        cout << "                [5] Weather History" << endl;
        cout << "                [6] Alerts & Precautions" << endl;
        cout << "                [7] Learn About Weather Types" << endl;
        cout << "                [8] Exit" << endl;

        cout << endl;
        cout << "          Please select an option (1-8): ";
        int choice;
        cin >> choice;

        cout << "\n------------------------------------------------------------\n";
        switch (choice) {
            case 1:
                cout << "You selected: Check Weather by City Name\n";
                option1();
                break;
            case 2:
                cout << "You selected: Search by Weather Condition\n";
                option2();
                break;
            case 3:
                cout << "You selected: Search by Temperature\n";
                option3();
                break;
            case 4:
                cout << "You selected: 5-Day Weather Forecast\n";
                option4();
                break;
            case 5:
                cout << "You selected: Weather History\n";
                option5();
                break;
            case 6:
                cout << "You selected: Alerts & Precautions\n";
                option6();
                break;
            case 7:
                cout << "You selected: Learn About Weather Types\n";
                option7();
                break;

            case 8:
                cout << "Exiting Sky Cast. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option. Please choose a number between 1 and 8.\n";
                break;
        }
        cout << "------------------------------------------------------------\n";
        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();
    }
    return 0;
}

