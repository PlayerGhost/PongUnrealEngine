// Fill out your copyright notice in the Description page of Project Settings.

#include "SerialPort.h"
#include <stdio.h>
#include <string.h>

#if PLATFORM_LINUX || PLATFORM_MAC

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#else

#include <windows.h>

#endif

USerialPort::USerialPort()
{
    UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: SerialPort object created."));
}

USerialPort::~USerialPort()
{
    Close();
}

bool USerialPort::Open(FString Port, EBaudRate BaudRate)
{
    if (IsOpen())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Can't open. A port is already opened."));
        return false;
    }

    FString portn;

#if PLATFORM_LINUX || PLATFORM_MAC

    portn = Port;
    unsigned int baudr;
    switch (BaudRate)
    {
    case EBaudRate::BR1200:
        baudr = B1200;
        break;
    case EBaudRate::BR2400:
        baudr = B2400;
        break;
    case EBaudRate::BR4800:
        baudr = B4800;
        break;
    case EBaudRate::BR9600:
        baudr = B9600;
        break;
    case EBaudRate::BR19200:
        baudr = B19200;
        break;
    case EBaudRate::BR38400:
        baudr = B38400;
        break;
    case EBaudRate::BR57600:
        baudr = B57600;
        break;
    case EBaudRate::BR115200:
        baudr = B115200;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Invalid baudrate."));
        return false;
        break;
    }

#else

    portn = FString("\\\\.\\") + Port;
    unsigned long baudr;
    switch (BaudRate)
    {
    case EBaudRate::BR1200:
        baudr = CBR_1200;
        break;
    case EBaudRate::BR2400:
        baudr = CBR_2400;
        break;
    case EBaudRate::BR4800:
        baudr = CBR_4800;
        break;
    case EBaudRate::BR9600:
        baudr = CBR_9600;
        break;
    case EBaudRate::BR19200:
        baudr = CBR_19200;
        break;
    case EBaudRate::BR38400:
        baudr = CBR_38400;
        break;
    case EBaudRate::BR57600:
        baudr = CBR_57600;
        break;
    case EBaudRate::BR115200:
        baudr = CBR_115200;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Invalid baudrate."));
        return false;
        break;
    }

#endif

    UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Trying to open: %s."), *portn);

#if PLATFORM_LINUX || PLATFORM_MAC

    serial_port = open(TCHAR_TO_ANSI(*portn), O_RDWR | O_NOCTTY | O_NDELAY);
    struct termios tty;
    struct termios tty_old;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Failed to get current serial parameters."));
        return false;
    }
    tty_old = tty;
    cfsetospeed(&tty, (speed_t)baudr);
    cfsetispeed(&tty, (speed_t)baudr);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    cfmakeraw(&tty);
    tcflush(serial_port, TCIFLUSH);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Could not set serial port parameters."));

        return false;
    }
    fcntl(serial_port, F_SETFL, FNDELAY);

#else

    handler = CreateFileA(static_cast<LPCSTR>(TCHAR_TO_ANSI(*portn)),
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (handler == INVALID_HANDLE_VALUE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Handle was not attached. Reason: %s not available."), *portn);
        return false;
    }
    else
    {
        DCB dcbSerialParameters = {0};
        if (!GetCommState(handler, &dcbSerialParameters))
        {
            UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Failed to get current serial parameters."));
            return false;
        }
        else
        {
            dcbSerialParameters.BaudRate = baudr;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: could not set serial port parameters."));
                return false;
            }
            else
            {
                PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
            }
        }
    }

#endif

    UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: %s opened."), *portn);
    return true;
}

void USerialPort::WriteBytes(TArray<uint8> binaryData)
{

#if PLATFORM_LINUX || PLATFORM_MAC

    unsigned char *data = reinterpret_cast<unsigned char *>(binaryData.GetData());
    write(serial_port, data, binaryData.Num());

#else

    COMSTAT status;
    DWORD errors;
    DWORD bytesSend;
    unsigned char *data = reinterpret_cast<unsigned char *>(binaryData.GetData());
    if (!WriteFile(handler, (void *)data, binaryData.Num(), &bytesSend, 0))
    {
        ClearCommError(handler, &errors, &status);
    }

#endif

}
void USerialPort::WriteText(FString text)
{
    if (!IsOpen())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Can't write. Port not opened."));
        return;
    }

    FBufferArchive BinaryArrayArchive;
    BinaryArrayArchive << text;
    WriteBytes(BinaryArrayArchive);
}

TArray<uint8> USerialPort::ReadBytes()
{
    TArray<uint8> ReturnBytes;
    if (!IsOpen())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Can't read. Port not opened."));
        return ReturnBytes;
    }

#if PLATFORM_LINUX || PLATFORM_MAC

    unsigned char buf[4096];
    int32 n;
    n = read(serial_port, &buf, sizeof(buf));
    if (n > 0)
    {
        for (int32 x = 0; x < n; x++)
        {
            ReturnBytes.Add(buf[x]);
        }
    }

#else

    unsigned char buf[4096];
    int32 n;
    DWORD bytesRead;
    COMSTAT status;
    DWORD errors;
    ClearCommError(handler, &errors, &status);

    n = status.cbInQue;
    if (n > 0)
    {
        ReadFile(handler, buf, n, &bytesRead, NULL);
        for (int32 x = 0; x < n; x++)
        {
            ReturnBytes.Add(buf[x]);
        }
    }

#endif
    return ReturnBytes;
}

void USerialPort::Close()
{

#if PLATFORM_LINUX || PLATFORM_MAC

    close(serial_port);
    serial_port = 0;

#else

    CloseHandle(handler);
    handler = 0;

#endif

    UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Serial port closed."));
}

bool USerialPort::IsOpen()
{

#if PLATFORM_LINUX || PLATFORM_MAC

    if (serial_port != 0)
    {
        return true;
    }

#else

    if (handler != 0)
    {
        return true;
    }

#endif

    return false;
}

FString USerialPort::StringFromBytes(TArray<uint8> BinaryData)
{
    const std::string value(reinterpret_cast<const char *>(BinaryData.GetData()), BinaryData.Num());
    return FString(value.c_str());
}

float USerialPort::FloatFromBytes(TArray<uint8> BinaryData)
{
    if (BinaryData.Num() > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Cant convert Float from Bytes. It must have only 4 bytes."));
        return 0.0f;
    }

    const float value = *(reinterpret_cast<float *>(BinaryData.GetData()));
    return value;
}

int32 USerialPort::IntFromBytes(TArray<uint8> BinaryData)
{
    if (BinaryData.Num() > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unreal Serial: Cant convert Int from Bytes. It must have only 4 bytes."));
        return 0;
    }

    const int32 value = *(reinterpret_cast<int32 *>(BinaryData.GetData()));
    return value;
}