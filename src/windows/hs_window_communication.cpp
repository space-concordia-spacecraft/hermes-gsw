#include "windows/hs_window_communication.h"

namespace hermes {

    WindowCommunication::WindowCommunication() { this->connected = false; }

    WindowCommunication::~WindowCommunication() {}

    void WindowCommunication::Connect(const char* portName) {
        //Try to connect to the given port through CreateFile
        this->hSerial = CreateFile(portName,
                                   GENERIC_READ | GENERIC_WRITE,
                                   0,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);

        printf("Connection Initialized\n");

        //Check if the connection was successful
        if (this->hSerial == INVALID_HANDLE_VALUE) {
            //If not successful display an Error
            if (GetLastError() == ERROR_FILE_NOT_FOUND) {

                //Print Error if necessary
                printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);

            } else {
                printf("ERROR!!!");
            }
        } else {
            //If connected we try to set the comm parameters
            DCB dcbSerialParams = { 0 };

            //Try to get the current
            if (!GetCommState(this->hSerial, &dcbSerialParams)) {
                //If impossible, show an error
                printf("failed to get current serial parameters!");
            } else {
                //Define serial connection parameters for the arduino board
                dcbSerialParams.BaudRate = CBR_9600;
                dcbSerialParams.ByteSize = 8;
                dcbSerialParams.StopBits = ONESTOPBIT;
                dcbSerialParams.Parity = NOPARITY;
                //Setting the DTR to Control_Enable ensures that the Arduino is properly
                //reset upon establishing a connection
                dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

                //Set the parameters and check for their proper application
                if (!SetCommState(hSerial, &dcbSerialParams)) {
                    printf("ALERT: Could not set Serial Port parameters");
                } else {
                    //If everything went fine we're connected
                    this->connected = true;
                    //Flush any remaining characters in the buffers
                    PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                    //We wait 2s as the arduino board will be reseting
                    Sleep(2000);
                }
            }
        }
    }

    bool WindowCommunication::Disconnect() {
        //Check if we are connected before trying to disconnect
        if (this->connected) {
            //We're no longer connected
            this->connected = false;
            //Close the serial handler
            CloseHandle(this->hSerial);
            printf("Connection Terminated\n");
        }
        return true;
    }

    bool WindowCommunication::IsConnected() {
        //Simply return the connection status
        return this->connected;
    }

    int WindowCommunication::Read(char* buffer, unsigned int nbChar) {
        //Number of bytes we'll have read
        DWORD bytesRead;
        //Number of bytes we'll really ask to read
        unsigned int toRead;

        //Use the ClearCommError function to get status info on the Serial port
        ClearCommError(this->hSerial, &this->errors, &this->status);

        //Check if there is something to read
        if (this->status.cbInQue > 0) {
            //If there is we check if there is enough data to read the required number
            //of characters, if not we'll read only the available characters to prevent
            //locking of the application.
            if (this->status.cbInQue > nbChar) {
                toRead = nbChar;
            } else {
                toRead = this->status.cbInQue;
            }

            //Try to read the require number of chars, and return the number of read bytes on success
            if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL)) {
                return bytesRead;
            }
        }

        return 0;
    }


    bool WindowCommunication::Write(const char* buffer, unsigned int nbChar) {
        DWORD bytesSend;

        //Try to write the buffer on the Serial port
        if (!WriteFile(this->hSerial, (void*) buffer, nbChar, &bytesSend, 0)) {
            //In case it don't work get comm error and return false
            ClearCommError(this->hSerial, &this->errors, &this->status);

            return false;
        } else {
            return true;
        }
    }

    string WindowCommunication::GetName() const {
        return "Communication Terminal";
    }

    void WindowCommunication::RenderGUI() {
        static bool my_tool_active = true;
        static const char* items[]{ "Overview", "Health", "ADCS", "Thermal", "Battery", "Payload", "Commands" };
        static int selectedItem = 0;
        static bool* p_open;

        ImGui::Begin("Connection");

        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
        if (ImGui::Button("Connect")) {
            Connect("\\\\.\\COM6");
            if (IsConnected()) {
                printf("Connection Established\n");
            }
        };

        ImGui::PopStyleColor(3);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
        if (ImGui::Button("Disconnect")) {
            Disconnect();
        };

        ImGui::PopStyleColor(3);
        if (IsConnected()) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::Text("Connection Established");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("Disconnected");
            ImGui::PopStyleColor();
        }


        ImGui::BeginChild("Scrolling");

        static Logger log;
        ImGui::BeginChild("Logger");
        if (IsConnected()) {
            char incomingData[256] = "";            // don't forget to pre-allocate memory
            //printf("%s\n",incomingData);
            int dataLength = 255;
            int readResult = 0;
            readResult = Read(incomingData, dataLength);
            // printf("Bytes read: (0 means no data available) %i\n",readResult);
            incomingData[readResult] = 0;

            log.AddLog("%s", incomingData);

        }
        ImGui::EndChild();
        log.RenderGUI();


        static ComponentConsole console;
        console.RenderGUI();


        //ImGui::ShowDemoWindow();
        ImGui::EndChild();
        ImGui::End();

    }

}

