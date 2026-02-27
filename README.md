# Win32 Inter-Process Communication: Morse Code Encoding System

## Project Overview
This project implements a synchronized dual-process system designed to demonstrate **Inter-Process Communication (IPC)** within the Windows environment. Utilizing the **Win32 API**, the system consists of two distinct applications—**Send** and **Receive**—that communicate using the `WM_COPYDATA` protocol to perform real-time text-to-binary Morse code encoding.



## System Architecture
The application is divided into two specialized modules that operate in tandem:

### 1. Sender Module (SendMessage)
* **User Interface**: Features a native Win32 GUI with an input field, a command button, and a multiline output display.
* **Input Validation**: Implements a rigorous filtering mechanism that permits only alphabetical characters and spaces, ensuring data integrity before transmission.
* **Process Discovery**: Utilizes `FindWindow` to locate the active "Receive" process within the operating system's window hierarchy.
* **Data Dispatch**: Encapsulates user input into a `COPYDATASTRUCT` and transmits it via `SendMessage`.

### 2. Receiver Module (ReceiveMessage)
* **Processing Engine**: Acts as the system's "backend," waiting for incoming data packets.
* **Morse Encoding Logic**: Implements a specialized binary representation of Morse code:
    * **Signal Logic**: "1" represents a signal, "0" represents a gap.
    * **Standard Gaps**: Uses "000" for letter separation and "00000" for word separation.
* **Bi-directional Feedback**: Once processing is complete, it identifies the sender's handle and transmits the encoded binary string back to the origin process for display.



## Technical Specification
The implementation relies on core Windows programming concepts:
* **Protocol**: `WM_COPYDATA` for safe memory sharing between independent process address spaces.
* **Window Management**: Use of `WNDCLASSEX`, `WndProc` (callback functions), and the Windows message loop (`GetMessage`/`DispatchMessage`).
* **Graphics & UI**: Native rendering via `HDC` (Hardware Device Context) and `DrawText` for real-time status updates in the client area.
* **Synchronization**: Synchronous messaging ensures that the sender waits for the receiver to acknowledge and process the data before updating the UI.

## Binary Morse Logic Examples
| Character | Binary Representation |
| :--- | :--- |
| **A** | `10111` |
| **E** | `1` |
| **S** | `10101` |
| **Word Space** | `00000` |

## Setup and Execution
To observe the IPC mechanism:
1. **Compile**: Build both source files using a C++ compiler (e.g., MinGW or Visual Studio).
2. **Launch Receiver**: Run the `Receive` executable first. It will enter a "Waiting for text..." state.
3. **Launch Sender**: Run the `Send` executable.
4. **Interaction**: Enter text into the sender's input field and press **OK**. The receiver will process the data, and the encoded binary string will appear in the sender's output box.

## Technical Stack
* **Language**: C++
* **Framework**: Win32 API (Native Windows Development)
* **Data Structures**: `std::map` for algorithmic mapping and `Set<Character>` logic.

---

## Author
* **Ciobanu Sorina**
