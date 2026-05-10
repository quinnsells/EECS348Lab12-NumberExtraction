#include <iostream>
#include <iomanip>
#include <string>
#include <climits>
#include <cfloat>

using namespace std;

const double INVALID = -999999.99;

bool hasMoreDigits(const string& str, int from) {
    for (int i = from; i < (int)str.size(); i++)
        if (isdigit(str[i])) return true;
    return false;
}
 
// Attempts to parse a string with number starting at str[i].
// On success: sets 'value' and advances 'i' past the number, returns true.
// On failure: returns false. 'i' is undefined on failure.
bool tryParse(const string& str, int& i, double& value) {
    int n = (int)str.size();
    // Sign 
    int sign = 1;
    if (i < n && (str[i] == '+' || str[i] == '-')) {
        if (str[i] == '-') sign = -1;
        i++;
    }
    // Integer part 
    long long intPart = 0;
    bool hasIntDigits = false;
    while (i < n && isdigit(str[i])) {
        int d = str[i] - '0';
        // Check for overflow before multiplying
        if (intPart > (LLONG_MAX - d) / 10)
            return false; // number too large
        intPart = intPart * 10 + d;
        hasIntDigits = true;
        i++;
    }
    // Fractional part
    double fracPart = 0.0;
    bool hasFracDigits = false;
    if (i < n && str[i] == '.') {
        i++; // consume '.'
        double mult = 0.1;
        while (i < n && isdigit(str[i])) {
            fracPart += (str[i] - '0') * mult;
            mult *= 0.1;
            hasFracDigits = true;
            i++;
        }
    }
    // Must have at least one digit total
    if (!hasIntDigits && !hasFracDigits)
        return false;
 
    // Exponent part
    int expSign = 1;
    long long expVal = 0;
    if (i < n && (str[i] == 'e' || str[i] == 'E')) {
        i++; // consume 'e'/'E'
 
        // sign on exponent
        if (i < n && (str[i] == '+' || str[i] == '-')) {
            if (str[i] == '-') expSign = -1;
            i++;
            if (i >= n || !isdigit(str[i]))
                return false;
        } else {
            if (i >= n || !isdigit(str[i]))
                return false;
        }
 
        while (i < n && isdigit(str[i])) {
            int d = str[i] - '0';
            if (expVal <= 400)
                expVal = expVal * 10 + d;
            else
                expVal = 401; // too large
            i++;
        }
    }
    // base value
    double base = sign * ((double)intPart + fracPart);
    // exponent
    long long exp = expSign * expVal;
    if (exp > 308 || exp < -324) {
        // Out of double range
        if (base == 0.0) {
            value = 0.0;
            return true;
        }
        return false;
    }
 
    double result = base;
    if (exp >= 0) {
        for (long long e = 0; e < exp; e++) {
            result *= 10.0;
            if (result > DBL_MAX || result < -DBL_MAX)
                return false;
        }
    } else {
        for (long long e = 0; e > exp; e--) {
            result /= 10.0;
        }
    }
 
    value = result;
    return true;
}
 
double extractNumeric(const string& str) {
    int n = (int)str.size();
 
    for (int i = 0; i < n; i++) {
        char c = str[i];
        bool isDig  = isdigit(c) != 0;
        bool isSign = (c == '+' || c == '-');
        bool isDot  = (c == '.');

        bool canStart = false;
 
        if (isDig) {
            canStart = true;
        } else if (isDot) {
            // can start only if the next char is a digit (e.g. ".75")
            canStart = (i + 1 < n && isdigit(str[i + 1]));
        } else if (isSign) {

            if (i + 1 < n && isdigit(str[i + 1])) {
                canStart = true;
            } else if (i + 1 < n && str[i + 1] == '.' && i + 2 < n && isdigit(str[i + 2])) {
                canStart = true;
            }
        }
 
        if (!canStart) continue;
 
        // Attempt parse from position i
        int parsePos = i;
        double value = 0.0;
        bool ok = tryParse(str, parsePos, value);
 
        if (!ok)
            return INVALID;

        if (hasMoreDigits(str, parsePos))
            return INVALID;
 
        return value;
    }
 
    return INVALID; 
}
 
int main() {
    string input;
 
    while (true) {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);
 
        if (input == "END") {
            cout << "Program terminated." << endl;
            break;
        }
        double result = extractNumeric(input);
 
        if (result == INVALID) {
            cout << "Invalid input: no valid floating-point number found" << endl;
        } else {
            cout << "Extracted number: "
                      << fixed << setprecision(4)
                      << result << endl;
        }
 
        cout << endl;
    }
 
    return 0;
}