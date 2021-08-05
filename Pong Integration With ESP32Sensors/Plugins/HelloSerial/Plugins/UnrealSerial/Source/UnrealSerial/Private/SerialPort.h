#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "UnrealSerialPrivatePCH.h"
#include "SerialPort.generated.h"

UENUM(BlueprintType, Category = "Unreal Serial")
enum class EBaudRate : uint8
{
	BR1200	UMETA(DisplayName = "1200"),
	BR2400	UMETA(DisplayName = "2400"),
	BR4800	UMETA(DisplayName = "4800"),
	BR9600	UMETA(DisplayName = "9600"),
	BR19200	UMETA(DisplayName = "19200"),
	BR38400	UMETA(DisplayName = "38400"),
	BR57600	UMETA(DisplayName = "57600"),
	BR115200	UMETA(DisplayName = "115200")

};

UCLASS(BlueprintType, Category = "Unreal Serial", meta = (Keywords = "serial port arduino"))
class USerialPort : public UObject
{
	GENERATED_BODY()	

public:
	USerialPort();
	~USerialPort();

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Open Serial Port"), Category = "Unreal Serial", meta = (Keywords = "open serial port"))
	bool Open(FString Port, EBaudRate BaudRate = EBaudRate::BR9600);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write Bytes"), Category = "Unreal Serial", meta = (Keywords = "write bytes"))
	void WriteBytes(TArray<uint8> BinaryData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write Text"), Category = "Unreal Serial", meta = (Keywords = "write text"))
	void WriteText(FString text);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Bytes"), Category = "Unreal Serial", meta = (Keywords = "read bytes"))
	TArray<uint8> ReadBytes();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Close Serial Port"), Category = "Unreal Serial", meta = (Keywords = "close serial port"))
	void Close();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is Serial Port Open"), Category = "Unreal Serial", meta = (Keywords = "is serial port open"))
	bool IsOpen();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "String From Bytes"), Category = "Unreal Serial", meta = (Keywords = "string from bytes"))
	static FString StringFromBytes(TArray<uint8> BinaryData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Float From Bytes"), Category = "Unreal Serial", meta = (Keywords = "float from bytes"))
	static float FloatFromBytes(TArray<uint8> BinaryData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Int From Bytes"), Category = "Unreal Serial", meta = (Keywords = "int from bytes"))
	static int32 IntFromBytes(TArray<uint8> BinaryData);

private:
#if PLATFORM_LINUX || PLATFORM_MAC
	int serial_port;
#else
	void * handler;
#endif
};
