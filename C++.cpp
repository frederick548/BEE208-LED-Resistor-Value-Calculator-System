#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Standard class structure as recommended by the project guidelines [cite: 58, 59]
class LEDCircuit {
private:
    string ledName;
    double supplyVoltage;
    double forwardVoltage;
    double ledCurrentMilliAmps;
    double ledCurrentAmps;
    double resistorValue;
    double resistorPower;
    string designStatus;

public:
    // Constructor initialization
    LEDCircuit() {
        ledName = "";
        supplyVoltage = 0.0;
        forwardVoltage = 0.0;
        ledCurrentMilliAmps = 0.0;
        ledCurrentAmps = 0.0;
        resistorValue = 0.0;
        resistorPower = 0.0;
        designStatus = "Unknown";
    }

    // Step 4: Method to collect inputs from user [cite: 60]
    void setCircuitDetails() {
        cout << "\nEnter LED Name/Colour: ";
        cin.ignore(); 
        getline(cin, ledName);

        cout << "Enter Supply Voltage (V): ";
        cin >> supplyVoltage;

        cout << "Enter LED Forward Voltage (V): ";
        cin >> forwardVoltage;

        cout << "Enter Desired LED Current (mA): ";
        cin >> ledCurrentMilliAmps;
    }

    // Step 5: Validation function [cite: 60]
    bool validateDetails() {
        // Rejects negative values, zero current, or when Vs <= Vf [cite: 34, 44, 45]
        if (supplyVoltage <= forwardVoltage || ledCurrentMilliAmps <= 0 || supplyVoltage < 0 || forwardVoltage < 0) {
            return false;
        }
        return true;
    }

    // Step 6a: Set status for invalid entry branch
    void setInvalidStatus() {
        designStatus = "Invalid Design";
    }

    // Step 6b: Sequential technical calculations [cite: 60]
    void calculateResistor() {
        // Step 6b.1: Convert mA to Amperes [cite: 42, 77]
        ledCurrentAmps = ledCurrentMilliAmps / 1000.0;

        // Step 6b.2: Ohm's Law Resistor calculation [cite: 23, 42]
        resistorValue = (supplyVoltage - forwardVoltage) / ledCurrentAmps;

        // Step 6b.3: Calculate Resistor power dissipation [cite: 24, 42]
        resistorPower = (ledCurrentAmps * ledCurrentAmps) * resistorValue;
    }

    // Step 7 & 8: Safety thresholds evaluation [cite: 46, 47]
    void evaluateSafety() {
        if (ledCurrentMilliAmps > 30.0) {
            designStatus = "Warning: LED current may be too high for a standard LED.";
        } else {
            designStatus = "Design Accepted";
        }
    }

    // Step 9: Display console report for valid circuits [cite: 60]
    void displayCircuitReport() {
        cout << "--------------------------------------------------\n";
        cout << "LED Name: " << ledName << "\n";
        cout << "Supply Voltage: " << supplyVoltage << "\n";
        cout << "Forward Voltage: " << forwardVoltage << "\n";
        cout << "Desired Current: " << ledCurrentMilliAmps << "\n";
        cout << "Recommended Resistor: " << resistorValue << " ohms\n";
        cout << "Minimum Resistor Power: " << resistorPower << " W\n";
        cout << "Design Status: " << designStatus << "\n";
    }

    // Method to return formatted data for external file printing [cite: 60]
    string getReportString(int circuitIndex) {
        string out = "Circuit #" + to_string(circuitIndex) + "\n";
        out += "LED Name: " + ledName + "\n";
        out += "Supply Voltage: " + to_string(supplyVoltage) + "\n";
        out += "Forward Voltage: " + to_string(forwardVoltage) + "\n";
        
        if (designStatus == "Invalid Design") {
            out += "Design Status: " + designStatus + "\n--------------------------------------------------\n";
        } else {
            out += "Desired Current: " + to_string(ledCurrentMilliAmps) + "\n";
            out += "Recommended Resistor: " + to_string(resistorValue) + " ohms\n";
            out += "Minimum Resistor Power: " + to_string(resistorPower) + " W\n";
            out += "Design Status: " + designStatus + "\n--------------------------------------------------\n";
        }
        return out;
    }
};

int main() {
    // Step 1 & 2: Welcome message title [cite: 29, 72, 73]
    cout << "==================================================\n";
    cout << "       LED RESISTOR VALUE CALCULATOR SYSTEM       \n";
    cout << "==================================================\n";

    // Step 3: Loop preparation [cite: 38, 74]
    int numberOfCircuits;
    cout << "Enter number of LED circuits to calculate: ";
    cin >> numberOfCircuits;

    // Vector initialization to track history for file generation [cite: 38]
    vector<LEDCircuit> groupCircuitList;

    // The Main Loop block (Executes from 1 up to numberOfCircuits) [cite: 53, 75]
    for (int i = 0; i < numberOfCircuits; i++) {
        LEDCircuit currentCircuit;
        
        // Step 4: Take user inputs [cite: 75]
        currentCircuit.setCircuitDetails();

        // Step 5: Core Decision Check
        if (currentCircuit.validateDetails() == false) {
            // Step 6a: Invalid route (Sets error flag and jumps straight to Step 10 via loop advancement)
            currentCircuit.setInvalidStatus();
            cout << "Design Status: Invalid Design\n"; // Matches exam example layout 
        } 
        else {
            // Step 6b: Sequential Calculation Block
            currentCircuit.calculateResistor();
            
            // Step 7 & 8: Safety Verification
            currentCircuit.evaluateSafety();
            
            // Step 9: Print Report to Screen
            currentCircuit.displayCircuitReport();
        }

        // Keep a record of the object instance regardless of validity [cite: 67]
        groupCircuitList.push_back(currentCircuit);
        
        // Step 10: Automatic Loop Evaluator check happens right here at the brace loop end!
    }

    // Step 11: Save group compilation report into external plaintext file [cite: 39, 56, 80]
    ofstream groupReportFile("led_resistor_report.txt");
    if (groupReportFile.is_open()) {
        groupReportFile << "==================================================\n";
        groupReportFile << "       FINAL LED RESISTOR CALCULATOR REPORT        \n";
        groupReportFile << "==================================================\n";
        groupReportFile << "Total Circuits Processed: " << numberOfCircuits << "\n\n";

        for (int k = 0; k < numberOfCircuits; k++) {
            groupReportFile << groupCircuitList[k].getReportString(k + 1);
        }
        groupReportFile.close();
        cout << "\n[SUCCESS] Final report successfully saved to 'led_resistor_report.txt'.\n";
    } else {
        cout << "\n[ERROR] Problem creating output text file.\n";
    }

    // Step 12: End [cite: 81]
    cout << "\nThank you for using the system. Exiting...\n";
    return 0;
}