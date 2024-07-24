#include <windows.h>  
#include <iostream>  

// 串口配置结构体  
DCB dcbSerialParams = { 0 };

// 初始化串口配置  
void InitializeSerialPort(HANDLE hSerial) {
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state" << std::endl;
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;         // 设置波特率为9600  
    dcbSerialParams.ByteSize = 8;                // 数据位为8  
    dcbSerialParams.StopBits = ONESTOPBIT;       // 停止位为1  
    dcbSerialParams.Parity = NOPARITY;           // 无奇偶校验位  

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state" << std::endl;
    }
}

int main() {

    SetConsoleOutputCP(CP_UTF8);

    HANDLE hSerial;
    char buff[256];
    DWORD bytesRead;

    // 打开串口  
    hSerial = CreateFile(
        L"COM2",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    // 初始化串口  
    InitializeSerialPort(hSerial);

    // 持续读取串口数据  
    while (true) {
        if (ReadFile(hSerial, buff, sizeof(buff) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buff[bytesRead] = '\0';
                std::cout << "Received: " << buff << std::endl;
            }
        }
        else {
            DWORD error = GetLastError();
            if (error != ERROR_IO_PENDING) {
                std::cerr << "Error reading from serial port" << std::endl;
                break;
            }
        }

        //添加一些延迟，以避免过度占用CPU资源  
        Sleep(10);
    }

    // 关闭串口  
    CloseHandle(hSerial);

    return 0;
}