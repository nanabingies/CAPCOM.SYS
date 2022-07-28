#include <intrin.h>
#include <ntddk.h>

extern "C"{
    DRIVER_DISPATCH sub_104E4;
    DRIVER_DIAPATCH sub_10590;
    DRIVER_UNLOAD   sub_1047C;
}

using pRoutineAddress = PVOID(NTAPI*)(_In_ PUNICODE_STRING);
VOID sub_107A0(_In_ unsigned __int64*);
VOID sub_10788(_In_ unsigned __int64*);
INT sub_10524(_In_ VOID*)


extern "C" NTSTATUS DrverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {

	char unk_10880[128] = 0;
	BYTE* __ImageBase_774 = '\\Device\\';
	memcpy(&unk_10880, &__ImageBase, strlen(__ImageBase));

	UNICODE_STRING DosName{};
	RtlInitUnicodeString(&DosName, L"\\Device\\Htsysm72FB");
	auto ns = IoCreateDevice(DriverObject, 0, &DosName, 0xAA01, 0, FALSE, &arg_10);
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

VOID sub_1047C(_In_ PDRIVER_OBJECT DriverObject){
    UNICODE_STRING DosDeviceName {};
    RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\Htsysm72FB");
    IoDeleteSymbolicLink(&DosDeviceName);
    IoDeleteDevice(DriverObject->DeviceName);
}


NTSTATUS sub_104E4(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIPR Irp){
    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    
    auto CurrentLocation = IoGetCurrentIrpStackLocation(Irp); 
    if (CurrentLocation.MajorFunction != 0 || CurrentLocation.MajorFunction != 2){
        Irp->IoStatus.Status = STATUS_NOT_IMPLEMENTED;
    }

	IofCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}


NTSTATUS sub_10590(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIPR Irp){
    
    auto CurrentLocation = IoGetCurrentIrpStackLocation(Irp);
    auto SystemBuffer = Irp->AssociatedIrp.SystemBuffer;

    ULONG Ioctl32 = 0AA012044;
    ULONG Ioctl64 = 0AA013044;
    ULONG inBufferSize = 0;
    ULONG ouBufferSize = 0;

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    if (CurrentLocation.MajorFunction != 0xE) {
	    Irp->IoStatus.Status = STATUS_NOT_IMPLEMENTED; 
	    goto loc_10626;
    }

    if (CurrentLocation->DeviceIoControl.IoControlCode == Ioctl32) {
	    outBufferSize = 4;
	    inBufferSize = 4;
    }
    else if (CurrentLocation->DeviceIoControl.IoControlCode == Ioctl64){
	    outBufferSize = 4;
	    inBufferSize = 8;
    }

    if (CurrentLocation->DeviceIoControl.InBufferLength != inBufferSize) {
	    Irp.IoStatus.Status = STATUS_INVALID_PARAMETER;
		goto loc_10626;
    }

    if (CuurentLocation->DeviceIoControl.OutputBufferLength != outBufferSize) {
	    Irp.IoStatus.Status = STATUS_INVALID_PARAMETER;
	    goto loc_10626;
    }

    INT out = sub_10524(SystemBuffer);
    *SystemBuffer = out;
    Irp->IoStatus.Information = out;

loc_10626:
    IofCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}




INT sub_10524(VOID * SystemBuffer){
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
}
    

VOID sub_10788(unsigned __int64* var_1){
    _disable();
    auto cr4 = __readcr4();
    *var_1 = cr4;
    cr4 &= 0FFFFFFFFFFEFFFFF;
    __writecr4(cr4);
}
	

VOID sub_107A0(unsigned __int64* var_1){
    auto cr4 = *var_1;
    __writecr4(cr4);
    _enable();
}
