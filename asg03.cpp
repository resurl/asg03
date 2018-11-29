#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// all input functions
void getFlatRate(int&, double&, double&, double&); // gets days, airfare, registration, hotel
void getTimes(int[], int[]);                        // gets departure and arrival times
void getMeal(double[], int, string);                 // gets the amount paid for a meal time
void getCarUse(int&, double&, int);                 // gets # of rentals, miles   
void getVehFees(double[], int, string);              // gets parking & taxi fees

// the following arrays calculate total expense of argument 
double calcArr(const double[], int);                // calculates total of array
double calcRate(double, int);                       // calculates total from a rate

// following arrays calculate total reimbursement limit of args 
double allowBfast(const int[], const int[], const double[], int);  // calculates company allowed expenses for meal
double allowLunch(const int[], const int[], const double[], int);
double allowDinner(const int[], const int[], const double[], int);
double allowRate(double, int);                                   // calculates allowance given limit & unit

// all display the input & calculations 
void displayTravel(int, double, double);                      // displays input: days, airfare, registration
void displayTimes(const int[], const int[]);                 // displays departure & arrival times
void displayHotel(double, double);                           // displays hotel rate & total charge
void displayMeals(const double[], const double[], const double[], int);  // displays expenses for one meal
void displayCar(int, double, double);                         // displays # of rentals, miles, and cost from miles
void displayVeh(const double[], const double[], int);        // displays parking & taxi fees
void displayExpenses(double, double);                       // displays total expenses, allowance, saved money/personal expense

const int DAYS = 31,                // for array length
TIME_LEN = 2;             // for departure/arrival array length
const double PARK_RATE = 10,        // parking allowance/day
TAXI_RATE = 20,        // taxi allowance/day
MILEAGE_RATE = 0.5,    // amount charged/km
HOTEL_LIMIT = 90;      // lodging allowance/night

int main() {
	int length, carRents;                                   // day & # of rentals
	int depart[TIME_LEN], arrive[TIME_LEN];                 // arrays storing hours & minutes of departure and arrival time
	double parkingFees[DAYS], taxiFees[DAYS],               // stores daily expenses of parking & taxi
		breakfast[DAYS], lunch[DAYS], dinner[DAYS];      // stores daily expenses of meals
	double totalExpenses, totalAllow,                       // total spent & total that will be reimbursed by the company
		bAllow, lunAllow, dinAllow,                      // total of meal expenses that will be reimbursed 
		taxiAllow = 0, parkAllow = 0, hotelAllow,        // total of taxi, parking, and hotel fees that will be reimbursed
		bfastExp, lunchExp, dinnerExp,                   // meal expenses
		taxiExp, parkExp, carExp,                        // taxi, parking, mileage expenses
		regFees, hotelRate, hotelExp, airfare, carKM;    // registration for conferences/seminars, hotel cost/night, roundtrip airfare, total km driven

	getFlatRate(length, airfare, regFees, hotelRate);
	getTimes(depart, arrive);
	getMeal(breakfast, length, "breakfast");
	getMeal(lunch, length, "lunch");
	getMeal(dinner, length, "dinner");
	getCarUse(carRents, carKM, length);
	if (carRents > 0)                                        // if at least one car was rented, ask for distance driven
		getVehFees(parkingFees, length, "parking");
	if (carRents != length) {                                // if a car was not rented for every day of the trip
		getVehFees(taxiFees, length, "taxi");
	}

	bfastExp = calcArr(breakfast, length);
	lunchExp = calcArr(lunch, length);
	dinnerExp = calcArr(dinner, length);
	if (carRents > 0)
		parkExp = calcArr(parkingFees, length);
	if (carRents != length)
		taxiExp = calcArr(taxiFees, length);
	carExp = calcRate(MILEAGE_RATE, carKM);
	hotelExp = calcRate(hotelRate, length);

	totalExpenses = bfastExp + lunchExp + dinnerExp
		+ parkExp + taxiExp + carExp + hotelExp
		+ regFees + airfare;                            // add up all expenses

	bAllow = allowBfast(depart, arrive, breakfast, length);
	lunAllow = allowLunch(depart, arrive, lunch, length);
	dinAllow = allowDinner(depart, arrive, dinner, length);
	if (carRents != length)
		taxiAllow = allowRate(TAXI_RATE, length);
	if (carRents > 0)
		parkAllow = allowRate(PARK_RATE, length);
	hotelAllow = allowRate(HOTEL_LIMIT, length);

	totalAllow = bAllow + lunAllow + dinAllow + taxiAllow
		+ parkAllow + hotelAllow + carExp + regFees
		+ airfare;                                         // add up all expenses that will be reimbursed

	displayTravel(length, airfare, regFees);
	displayTimes(depart, arrive);
	displayHotel(hotelRate, hotelExp);
	displayMeals(breakfast, lunch, dinner, length);
	displayCar(carRents, carKM, carExp);
	displayVeh(parkingFees, taxiFees, length);
	displayExpenses(totalExpenses, totalAllow);

	return 0;
}

void getFlatRate(int& day, double& airfare, double& regi, double& hotel) {
	do {
		cout << "\nEnter the number of days: ";
		cin >> day;
		cout << "Enter the cost of the airfare: $";
		cin >> airfare;
		cout << "Enter any registration costs: $";
		cin >> regi;
		cout << "Enter the nightly hotel rate: $";
		cin >> hotel;
	} while (day < 0 || airfare < 0 || regi < 0 || hotel < 0);      // do this while all input is positive
}

void getTimes(int dep[], int arr[]) {
	do {
		cout << "\nEnter the time of departure (ex. 9 30, 15 00): ";
		cin >> dep[0];  // departure hours
		cin >> dep[1];  // departure minutes

		cout << "Enter the time of arrival (ex. 3 00, 20 30): ";
		cin >> arr[0];  // arrival hours
		cin >> arr[1];  // arrival minutes
	} while (dep[0] < 0 || dep[0] > 23 || dep[1] < 0 || dep[1] > 59 || arr[0] < 0 || arr[0] > 23 || arr[1] < 0 || arr[1] > 59);
	// the above statement limits the input to follow clock 24hr notation
}

void getMeal(double meal[], int len, string str) {
	cout << " " << endl;                    // line break for presentation                               
	for (int i = 0; i < len; i++) {
		do {
			cout << "Enter the " << str << " amount for day " << (i + 1) << ": $";
			cin >> meal[i];
		} while (meal[i] < 0);          // money value cannot be 0
	}
}

void getCarUse(int& rent, double& km, int len) {
	do {
		cout << "\nEnter number of cars rented: ";
		cin >> rent;
	} while (rent > len || rent < 0);                   // keep asking input if number of rentals > number of days, rent must be 0 or greater
	if (rent > 0) {                                     // if rent exists, ask for mileage
		do {
			cout << "Total kilometers driven with car: ";
			cin >> km;
		} while (km < 0);                               // make sure mileage is positive
	}
}

void getVehFees(double fees[], int len, string str) {
	for (int n = 0; n < len; n++) {                      // keep asking a value for each day
		do {
			cout << "Day " << (n + 1) << " " << str << " fees: $";
			cin >> fees[n];
		} while (fees[n] < 0);                          // expense cannot be negative or 0 (not realistic if it's 0)
	}
}

double calcArr(const double fees[], int day) {
	double total = 0;

	for (int i = 0; i < day; i++)                        // for each day,
		total += fees[i];                               // add the amount spent on that day to the total

	return total;
}

double calcRate(double charge, int unit) {
	double expense = 0;
	expense = charge * unit;
	return expense;
}

double allowBfast(const int dep[], const int arr[], const double meal[], int len) {
	double allow = 0;

	if (dep[0] < 7)                  // if the departure time is before 7am,
		allow += meal[0];           // add the meal expense of the first day to amt to be reimbursed for bfast
	if (arr[0] >= 8)                 // if arrival time is at or after 8am
		allow += meal[len - 1];       // add meal expense of last day to amt 

	return allow;
}

double allowLunch(const int dep[], const int arr[], const double meal[], int len) {
	double allow = 0;

	if (dep[0] < 12)                     // if departure time is before 12 noon
		allow += meal[0];
	if (arr[0] >= 13)                    // if arrival time is at 1pm or after
		allow += meal[len - 1];

	return allow;
}

double allowDinner(const int dep[], const int arr[], const double meal[], int len) {
	double allow = 0;

	if (dep[0] < 18)                      // if departure time is before 6pm
		allow += meal[0];
	if (arr[0] >= 19)                     // if arrival time is at or after 7pm
		allow += meal[len - 1];

	return allow;
}

double allowRate(double limit, int unit) {
	double allow = 0;
	allow = limit * unit;
	return allow;
}

void displayTravel(int days, double airfare, double regis) {
	cout << "\nNumber of days on trip: " << days << endl;
	cout << fixed << showpoint << setprecision(2);
	cout << "Cost of round-trip airfare: $" << airfare << endl;
	cout << "Cost of any registration fees: $" << regis << endl;
}

void displayTimes(const int dep[], const int arr[]) {   // converts 24 hour time to 12 hour time and displays it using correct clock notation

	if (dep[0] <= 12 && dep[1] < 10)                                                           // if departure hour is before/at 12 noon & minutes <less than> 10
		cout << "Time of departure: " << (dep[0]) << ":" << "0" << dep[1] << "AM" << endl;    // keep hours as is and put a 0 before the minutes
	else if (dep[0] > 12 && dep[1] < 10)                                                       // if departure hour is after 12 & minutes < 10
		cout << "Time of departure: " << (dep[0] % 12) << ":" << "0" << dep[1] << "PM" << endl; // start hours from 1 again and put 0 before minutes
	else if (dep[0] <= 12 && dep[1] >= 10)                                                     // if departure hours is <= 12 & minutes are >= 10
		cout << "Time of departure: " << (dep[0]) << ":" << dep[1] << "AM" << endl;           // hours unchanged, minutes unchanged
	else                                                                                      // if all else fails, means that hour > 12noon and minutes > 10
		cout << "Time of departure: " << (dep[0] % 12) << ":" << dep[1] << "PM" << endl;        // start hours from 1 again, minutes unchanged

	if (arr[0] <= 12 && arr[1] < 10)                                                           // same logic applies to arrival time
		cout << "Time of arrival: " << (arr[0]) << ":" << "0" << arr[1] << "AM" << endl;
	else if (arr[0] > 12 && arr[1] < 10)
		cout << "Time of arrival: " << (arr[0] % 12) << ":" << "0" << arr[1] << "PM" << endl;
	else if (arr[0] <= 12 && arr[1] > 10)
		cout << "Time of arrival: " << (arr[0]) << ":" << arr[1] << "AM" << endl << endl;
	else
		cout << "Time of arrival: " << (arr[0] % 12) << ":" << arr[1] << "PM" << endl;
}

void displayHotel(double rate, double fees) {
	cout << fixed << showpoint << setprecision(2);
	cout << "\nHotel price per night: $" << rate << endl;
	cout << "Total hotel expenses: $" << fees << endl << endl;
}

void displayMeals(const double br[], const double lun[], const double din[], int len) {
	cout << fixed << showpoint << setprecision(2);
	cout << "Breakfast expenses" << endl;
	for (int i = 0; i < len; i++)
		cout << "Day " << (i + 1) << "\t$" << br[i] << endl;

	cout << "\nLunch expenses" << endl;
	for (int n = 0; n < len; n++)
		cout << "Day " << (n + 1) << "\t$" << lun[n] << endl;

	cout << "\nDinner expenses" << endl;
	for (int t = 0; t < len; t++)
		cout << "Day " << (t + 1) << "\t$" << din[t] << endl;
}

void displayCar(int rents, double miles, double cost) {
	cout << "\nNumber of car rentals: " << rents << endl;
	cout << fixed << showpoint << setprecision(2);
	cout << "Number of miles driven: " << miles << endl;
	cout << "Total mileage cost: $" << cost << endl;
}

void displayVeh(const double park[], const double taxi[], int len) {
	cout << fixed << showpoint << setprecision(2);
	cout << "\nParking fees" << endl;
	for (int i = 0; i < len; i++)
		cout << "Day " << (i + 1) << "\t$" << park[i] << endl;
	cout << "\nTaxi fees" << endl;
	for (int n = 0; n < len; n++)
		cout << "Day " << (n + 1) << "\t$" << taxi[n] << endl;
}

void displayExpenses(double total, double allow) {
	double excess = total - allow;
	double saved = allow - total;

	cout << fixed << showpoint << setprecision(2);
	cout << "\nTotal Expenses: $" << total << endl;
	cout << "Expenses allowed by company: $" << allow << endl;
	if (excess > 0)
		cout << "Excess expenses not paid by company: $" << excess << endl;
	else if (saved > 0)
		cout << "Money saved during trip: $" << saved << endl << endl;
}
