#include <intrin.h>
#include <ntddk.h>

extern "C" NTSTATUS DrverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {

	char unk_10880[128] = 0;
	BYTE* __ImageBase_774 = '\\Device\\';
	memcpy(&unk_10880, &__ImageBase, strlen(__ImageBase));

	UNICODE_STRING DosName{};
	RtlInitUnicodeString(&DosName, L"\\Device\\Htsysm72FB");
	auto ns = IoCreateDevice(DriverObject, 0, DosName, 0xAA01, 0, FALSE, &arg_10);
	if (!NT_SUCCESS(ns)) {
		return ns;
	}

	UNICODE_STRING DosDeviceName{};
	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\Htsysm72FB");
	ns = IoCreateSymbolicLink(&DosDeviceName, &DosName);
	if (!NT_SUCCESS(ns)) {
		IoDeleteDevice(arg_10);
		return ns;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = sub_104E4;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = sub_104E4;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = sub_10590;
	DriverObject->DriverUnload = sub_1047C;

	return ns;
}
---------------------------------------------------------- -
sub_1047C() -> DriverUnload
UNICODE_STRING DosDeviceName {};
RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\Htb");
IoDeleteSymbolicLink(&DosDeviceName);
IoDeleteDevice(DriverObject->DeviceName);

---------------------------------------------------------- -
sub_104E4()->IRP
Irp->IoStatus.Information = 0;
Irp->IoStatus.Status = STATUS_SUCCESS;
auto CurrentLocation = IoGetCurrentIrpStackLocation(Irp);
if (*CurrentLocation == 0 || *(CurrentLocation + 0x2) == 2)
goto loc_1050D;
Irp->IoStatus.Status = 0C0000002; // STATUS_UNSUCCESSFUL

// OR ????
switch (CurrentLocation) ? ? ? ?

loc_1050D :
	IofCompleteRequest(Irp, IO_NO_INCREMENT);
return Irp->IoStatus.Status;

------------------------------------------------------------------ -
sub_10590()->Irp
auto CurrentLocation = IoGetCurrentIrpStackLocation(Irp);
auto SystemBuffer = Irp->AssociatedIrp.SystemBuffer;
ULONG Ioctl32 = 0AA012044;
ULONG Ioctl64 = 0AA013044;
ULONG inBufferSize = 0;
ULONG ouBufferSize = 0;

Irp->IoStatus.Status = 0
Irp->IoStatus.Information = 0;

if (*CurrentLocation != 0xE) {
	Irp->IoStatus.Status = 0C0000002; // STATUS_UNSUCCESSFUL
	goto loc_10626;
}

if (CurrentLocation->DeviceIoControl.IoControlCode == Ioctl32) {
	outBufferSize = 4;
	inBufferSize = 4;
}
else if (// == Ioctl64){
	outBufferSize = 4;
	inBufferSize = 8;
}

if (CuurentLocation->DeviceIoControl.InBufferLength != inBufferSize) {
	Irp.IoStatus.Status = 0C000000D;
		goto loc_10626;
}

if (CuurentLocation->DeviceIoControl.OutputBufferLength != outBufferSize) {
	Irp.IoStatus.Status = 0C000000D;
	goto loc_10626;
}

INT out = sub_10524(SystemBuffer);
*SystemBuffer = out;
Irp->IoStatus.Information = out;

loc_10626:
IofCompleteRequest(Irp, IO_NO_INCREMENT);
return Irp->IoStatus.Status;

-------------------------------------------------------------------------------------------- -
using pRoutineAddress = PVOID(NTAPI*)(_In_ PUNICODE_STRING);

sub_10524(VOID * SystemBuffer)
auto Buffer = SystemBuffer;
if (*(Buffer - 0x8) != Buffer) {
	return 0;
}

ULONG var_1 = 0;
PVOID SystemRoutineAddress = MmGetSystemRoutineAddress;
sub_10788(&var_1);
Buffer(SystemRoutineAddress);
sub_107A0(&var_1);
return 1;

----------------------------------------------------------------------------------------------
sub_10788(unsigned __int64* var_1) < -SMEP_DISABLE
	_disable();
auto cr4 = __readcr4();
*var_1 = cr4;
cr4 &= 0FFFFFFFFFFEFFFFF;
__writecr4(cr4);

---------------------------------------------------------------------------------------------- -
sub_107A0(unsigned __int64* var_1) < -SMEP_ENABLE
	auto cr4 = *var_1;
__writecr4(cr4);
_enable();

------------------------------------------------------------------------------
