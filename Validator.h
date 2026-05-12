#pragma once
#pragma once
#include <cctype>
using namespace std;

class Validator
{
public:
    static bool validateID(int id)
    {
        return id > 0;
    }

    static bool validateMenuChoice(int c, int min, int max)
    {
        return c >= min && c <= max;
    }

    static bool validateAmount(double amount)
    {
        return amount > 0;
    }

    static bool validateAge(int age)
    {
        return age > 0 && age < 150;
    }

    static bool validateGender(const char* g)
    {
        return (g[0] == 'M' || g[0] == 'F') && g[1] == '\0';
    }

    static bool validatePassword(const char* p)
    {
        return p[0] != '\0';
    }

    static bool validateTimeSlot(const char* time)
    {
        const char* valid[8] = {
            "09:00","10:00","11:00","12:00",
            "13:00","14:00","15:00","16:00"
        };
        for (int i = 0; i < 8; i++)
        {
            int k = 0; bool match = true;
            while (valid[i][k] != '\0' && time[k] != '\0')
            {
                if (valid[i][k] != time[k]) { match = false; break; }
                k++;
            }
            if (match && valid[i][k] == '\0' && time[k] == '\0')
                return true;
        }
        return false;
    }

    static bool validateDate(const char* date)
    {
        // check format DD-MM-YYYY
        if (date[2] != '-' || date[5] != '-') return false;

        int d = (date[0] - '0') * 10 + (date[1] - '0');
        int m = (date[3] - '0') * 10 + (date[4] - '0');
        int y = (date[6] - '0') * 1000 + (date[7] - '0') * 100 +
            (date[8] - '0') * 10 + (date[9] - '0');

        if (d < 1 || d > 31) return false;
        if (m < 5 || m > 12) return false;
        if (y < 2026)        return false;

        return true;
    }

    static bool validateContact(const char* contact)
    {
        int len = 0;
        while (contact[len] != '\0') len++;
        if (len != 11) return false;
        for (int i = 0; i < len; i++)
            if (contact[i] < '0' || contact[i] > '9') return false;
        return true;
    }
};