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

        static ComponentLogger log;
        ImGui::BeginChild("ComponentLogger");
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

        //CONSOLE EXAMPLE
        struct ExampleAppConsole {
            char InputBuf[256];
            ImVector<char*> Items;
            ImVector<const char*> Commands;
            ImVector<char*> History;
            int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
            ImGuiTextFilter Filter;
            bool AutoScroll;
            bool ScrollToBottom;

            ExampleAppConsole() {
                ClearLog();
                memset(InputBuf, 0, sizeof(InputBuf));
                HistoryPos = -1;

                // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
                Commands.push_back("HELP");
                Commands.push_back("HISTORY");
                Commands.push_back("CLEAR");
                Commands.push_back("CLASSIFY");
                AutoScroll = true;
                ScrollToBottom = false;
                AddLog("Welcome to Dear ImGui!");
            }

            ~ExampleAppConsole() {
                ClearLog();
                for (int i = 0; i < History.Size; i++)
                    free(History[i]);
            }

            // Portable helpers
            static int Stricmp(const char* s1, const char* s2) {
                int d;
                while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
                    s1++;
                    s2++;
                }
                return d;
            }

            static int Strnicmp(const char* s1, const char* s2, int n) {
                int d = 0;
                while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
                    s1++;
                    s2++;
                    n--;
                }
                return d;
            }

            static char* Strdup(const char* s) {
                IM_ASSERT(s);
                size_t len = strlen(s) + 1;
                void* buf = malloc(len);
                IM_ASSERT(buf);
                return (char*) memcpy(buf, (const void*) s, len);
            }

            static void Strtrim(char* s) {
                char* str_end = s + strlen(s);
                while (str_end > s && str_end[-1] == ' ') str_end--;
                *str_end = 0;
            }

            void ClearLog() {
                for (int i = 0; i < Items.Size; i++)
                    free(Items[i]);
                Items.clear();
            }

            void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
                // FIXME-OPT
                char buf[1024];
                va_list args;
                        va_start(args, fmt);
                vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
                buf[IM_ARRAYSIZE(buf) - 1] = 0;
                        va_end(args);
                Items.push_back(Strdup(buf));
            }

            void Draw(const char* title, bool* p_open) {
                ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
                if (!ImGui::Begin(title, p_open)) {
                    ImGui::End();
                    return;
                }

                // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
                // So e.g. IsItemHovered() will return true when hovering the title bar.
                // Here we create a context menu only available from the title bar.
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Close Console"))
                        *p_open = false;
                    ImGui::EndPopup();
                }

                ImGui::TextWrapped("Enter 'HELP' for help.");

                if (ImGui::SmallButton("Add Debug Text")) {
                    AddLog("%d some text", Items.Size);
                    AddLog("some more text");
                    AddLog("display very important message here!");
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }
                ImGui::SameLine();
                if (ImGui::SmallButton("Clear")) { ClearLog(); }
                ImGui::SameLine();
                bool copy_to_clipboard = ImGui::SmallButton("Copy");
                //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

                ImGui::Separator();

                // Options menu
                if (ImGui::BeginPopup("Options")) {
                    ImGui::Checkbox("Auto-scroll", &AutoScroll);
                    ImGui::EndPopup();
                }

                // Options, Filter
                if (ImGui::Button("Options"))
                    ImGui::OpenPopup("Options");
                ImGui::SameLine();
                Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
                ImGui::Separator();

                // Reserve enough left-over height for 1 separator + 1 input text
                const float footer_height_to_reserve =
                        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
                                  ImGuiWindowFlags_HorizontalScrollbar);
                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::Selectable("Clear")) ClearLog();
                    ImGui::EndPopup();
                }


                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
                if (copy_to_clipboard)
                    ImGui::LogToClipboard();
                for (int i = 0; i < Items.Size; i++) {
                    const char* item = Items[i];
                    if (!Filter.PassFilter(item))
                        continue;

                    // Normally you would store more information in your item than just a string.
                    // (e.g. make Items[] an array of structure, store color/type etc.)
                    ImVec4 color;
                    bool has_color = false;
                    if (strstr(item, "[error]")) {
                        color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                        has_color = true;
                    }
                    else if (strncmp(item, "# ", 2) == 0) {
                        color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                        has_color = true;
                    }
                    if (has_color)
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(item);
                    if (has_color)
                        ImGui::PopStyleColor();
                }
                if (copy_to_clipboard)
                    ImGui::LogFinish();

                if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                    ImGui::SetScrollHereY(1.0f);
                ScrollToBottom = false;

                ImGui::PopStyleVar();
                ImGui::EndChild();
                ImGui::Separator();

                // Command-line
                bool reclaim_focus = false;
                ImGuiInputTextFlags input_text_flags =
                        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                        ImGuiInputTextFlags_CallbackHistory;
                if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub,
                                     (void*) this)) {
                    char* s = InputBuf;
                    Strtrim(s);
                    if (s[0])
                        ExecCommand(s);
                    strcpy(s, "");
                    reclaim_focus = true;
                }

                // Auto-focus on window apparition
                ImGui::SetItemDefaultFocus();
                if (reclaim_focus)
                    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

                ImGui::End();
            }

            void ExecCommand(const char* command_line) {
                AddLog("# %s\n", command_line);

                // Insert into history. First find match and delete it so it can be pushed to the back.
                // This isn't trying to be smart or optimal.
                HistoryPos = -1;
                for (int i = History.Size - 1; i >= 0; i--)
                    if (Stricmp(History[i], command_line) == 0) {
                        free(History[i]);
                        History.erase(History.begin() + i);
                        break;
                    }
                History.push_back(Strdup(command_line));

                // Process command
                if (Stricmp(command_line, "CLEAR") == 0) {
                    ClearLog();
                } else if (Stricmp(command_line, "HELP") == 0) {
                    AddLog("Commands:");
                    for (int i = 0; i < Commands.Size; i++)
                        AddLog("- %s", Commands[i]);
                } else if (Stricmp(command_line, "HISTORY") == 0) {
                    int first = History.Size - 10;
                    for (int i = first > 0 ? first : 0; i < History.Size; i++)
                        AddLog("%3d: %s\n", i, History[i]);
                } else {
                    AddLog("Unknown command: '%s'\n", command_line);
                }

                // On command input, we scroll to bottom even if AutoScroll==false
                ScrollToBottom = true;
            }

            // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
            static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
                ExampleAppConsole* console = (ExampleAppConsole*) data->UserData;
                return console->TextEditCallback(data);
            }

            int TextEditCallback(ImGuiInputTextCallbackData* data) {
                //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
                switch (data->EventFlag) {
                    case ImGuiInputTextFlags_CallbackCompletion: {
                        // Example of TEXT COMPLETION

                        // Locate beginning of current word
                        const char* word_end = data->Buf + data->CursorPos;
                        const char* word_start = word_end;
                        while (word_start > data->Buf) {
                            const char c = word_start[-1];
                            if (c == ' ' || c == '\t' || c == ',' || c == ';')
                                break;
                            word_start--;
                        }

                        // Build a list of candidates
                        ImVector<const char*> candidates;
                        for (int i = 0; i < Commands.Size; i++)
                            if (Strnicmp(Commands[i], word_start, (int) (word_end - word_start)) == 0)
                                candidates.push_back(Commands[i]);

                        if (candidates.Size == 0) {
                            // No match
                            AddLog("No match for \"%.*s\"!\n", (int) (word_end - word_start), word_start);
                        } else if (candidates.Size == 1) {
                            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                            data->DeleteChars((int) (word_start - data->Buf), (int) (word_end - word_start));
                            data->InsertChars(data->CursorPos, candidates[0]);
                            data->InsertChars(data->CursorPos, " ");
                        } else {
                            // Multiple matches. Complete as much as we can..
                            // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                            int match_len = (int) (word_end - word_start);
                            for (;;) {
                                int c = 0;
                                bool all_candidates_matches = true;
                                for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                                    if (i == 0)
                                        c = toupper(candidates[i][match_len]);
                                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                                        all_candidates_matches = false;
                                if (!all_candidates_matches)
                                    break;
                                match_len++;
                            }

                            if (match_len > 0) {
                                data->DeleteChars((int) (word_start - data->Buf), (int) (word_end - word_start));
                                data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                            }

                            // List matches
                            AddLog("Possible matches:\n");
                            for (int i = 0; i < candidates.Size; i++)
                                AddLog("- %s\n", candidates[i]);
                        }

                        break;
                    }
                    case ImGuiInputTextFlags_CallbackHistory: {
                        // Example of HISTORY
                        const int prev_history_pos = HistoryPos;
                        if (data->EventKey == ImGuiKey_UpArrow) {
                            if (HistoryPos == -1)
                                HistoryPos = History.Size - 1;
                            else if (HistoryPos > 0)
                                HistoryPos--;
                        } else if (data->EventKey == ImGuiKey_DownArrow) {
                            if (HistoryPos != -1)
                                if (++HistoryPos >= History.Size)
                                    HistoryPos = -1;
                        }

                        // A better implementation would preserve the data on the current input line along with cursor position.
                        if (prev_history_pos != HistoryPos) {
                            const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, history_str);
                        }
                    }
                }
                return 0;
            }
        };
        static ExampleAppConsole console;
        console.Draw("Console", p_open);


        //ImGui::ShowDemoWindow();
        ImGui::EndChild();
        ImGui::End();

    }

}

