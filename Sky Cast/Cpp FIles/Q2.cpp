#include <iostream>
#include <string>

using namespace std;

// ========== Base Class ==========
// Inheritance used: This class will be inherited by others
class WeatherInfo {
protected:
    string location;

public:
    // Abstraction: public interface hides details
    void setLocation(const string& loc) {
        location = loc;
    }

    // Virtual Function: enables Runtime Polymorphism
    virtual void display() const {
        cout << "Weather information for: " << location << endl;
    }

    // Virtual Destructor for proper cleanup in polymorphism
    virtual ~WeatherInfo() {}
};

// ========== Derived Class 1: Current Weather ==========
class CurrentWeather : public WeatherInfo {
private:
    float temperature;
    string condition;

public:
    // Encapsulation: data is private, access via methods
    void setCurrentWeather(float temp, const string& cond) {
        temperature = temp;
        condition = cond;
    }

    // Polymorphism: override base class virtual function
    void display() const override {
        cout << "Current Weather in " << location << ":\n";
        cout << "Temperature: " << temperature << "°C\n";
        cout << "Condition: " << condition << endl;
    }
};

// ========== Helper Class for Composition ==========
class DayWeather {
private:
    string day;
    float temp;
    string cond;

public:
    void setData(const string& d, float t, const string& c) {
        day = d;
        temp = t;
        cond = c;
    }

    void show() const {
        cout << day << ": " << temp << "°C, " << cond << endl;
    }
};

// ========== Derived Class 2: Forecast Weather ==========
class ForecastWeather : public WeatherInfo {
private:
    vector<DayWeather> weekForecast; // Composition

public:
    void addDayWeather(const DayWeather& day) {
        weekForecast.push_back(day);
    }

    void display() const override {
        cout << "Weekly Forecast for " << location << ":\n";
        for (const DayWeather& d : weekForecast) {
            d.show();
        }
    }
};

// ========== Main Program ==========
int main() {
    // Pointer to base class for polymorphism
    WeatherInfo* weather;

    // Current weather example
    CurrentWeather current;
    current.setLocation("Lahore");
    current.setCurrentWeather(33.5, "Sunny");

    weather = &current; // Base class pointer to derived object
    weather->display(); // Polymorphism in action

    cout << "\n-----------------------------\n";

    // Weekly forecast example
    ForecastWeather forecast;
    forecast.setLocation("Islamabad");

    DayWeather mon, tue, wed;
    mon.setData("Mon", 31.2, "Cloudy");
    tue.setData("Tue", 29.0, "Rainy");
    wed.setData("Wed", 30.5, "Sunny");

    forecast.addDayWeather(mon);
    forecast.addDayWeather(tue);
    forecast.addDayWeather(wed);

    weather = &forecast; // Base class pointer to another derived object
    weather->display(); // Polymorphism in action again

    return 0;
}

