/*
 * Process and Thread Environment Block support functions
 *
 * This file is part of System Informer.
 */

#ifndef _NTPEBTEB_H
#define _NTPEBTEB_H

#include "helper.h"

typedef struct _RTL_USER_PROCESS_PARAMETERS *PRTL_USER_PROCESS_PARAMETERS;
typedef struct _RTL_CRITICAL_SECTION *PRTL_CRITICAL_SECTION;
typedef struct _SILO_USER_SHARED_DATA *PSILO_USER_SHARED_DATA;
typedef struct _LEAP_SECOND_DATA *PLEAP_SECOND_DATA;

#include <ntsxs.h>

// private
#define KACF_OLDGETSHORTPATHNAME 0x00000001
#define KACF_VERSIONLIE_NOT_USED 0x00000002
#define KACF_GETDISKFREESPACE 0x00000008
#define KACF_FTMFROMCURRENTAPT 0x00000020
#define KACF_DISALLOWORBINDINGCHANGES 0x00000040
#define KACF_OLE32VALIDATEPTRS 0x00000080
#define KACF_DISABLECICERO 0x00000100
#define KACF_OLE32ENABLEASYNCDOCFILE 0x00000200
#define KACF_OLE32ENABLELEGACYEXCEPTIONHANDLING 0x00000400
#define KACF_RPCDISABLENDRCLIENTHARDENING 0x00000800
#define KACF_RPCDISABLENDRMAYBENULL_SIZEIS 0x00001000
#define KACF_DISABLEALLDDEHACK_NOT_USED 0x00002000
#define KACF_RPCDISABLENDR61_RANGE 0x00004000
#define KACF_RPC32ENABLELEGACYEXCEPTIONHANDLING 0x00008000
#define KACF_OLE32DOCFILEUSELEGACYNTFSFLAGS 0x00010000
#define KACF_RPCDISABLENDRCONSTIIDCHECK 0x00020000
#define KACF_USERDISABLEFORWARDERPATCH 0x00040000
#define KACF_OLE32DISABLENEW_WMPAINT_DISPATCH 0x00100000
#define KACF_ADDRESTRICTEDSIDINCOINITIALIZESECURITY 0x00200000
#define KACF_ALLOCDEBUGINFOFORCRITSECTIONS 0x00400000
#define KACF_OLEAUT32ENABLEUNSAFELOADTYPELIBRELATIVE 0x00800000
#define KACF_ALLOWMAXIMIZEDWINDOWGAMMA 0x01000000
#define KACF_DONOTADDTOCACHE 0x80000000

// private
typedef struct _API_SET_NAMESPACE
{
    ULONG Version;
    ULONG Size;
    ULONG Flags;
    ULONG Count;
    ULONG EntryOffset;
    ULONG HashOffset;
    ULONG HashFactor;
} API_SET_NAMESPACE, *PAPI_SET_NAMESPACE;

// private
typedef struct _API_SET_HASH_ENTRY
{
    ULONG Hash;
    ULONG Index;
} API_SET_HASH_ENTRY, *PAPI_SET_HASH_ENTRY;

// private
typedef struct _API_SET_NAMESPACE_ENTRY
{
    ULONG Flags;
    ULONG NameOffset;
    ULONG NameLength;
    ULONG HashedLength;
    ULONG ValueOffset;
    ULONG ValueCount;
} API_SET_NAMESPACE_ENTRY, *PAPI_SET_NAMESPACE_ENTRY;

// private
typedef struct _API_SET_VALUE_ENTRY
{
    ULONG Flags;
    ULONG NameOffset;
    ULONG NameLength;
    ULONG ValueOffset;
    ULONG ValueLength;
} API_SET_VALUE_ENTRY, *PAPI_SET_VALUE_ENTRY;

// private
typedef struct _TELEMETRY_COVERAGE_HEADER
{
    UCHAR MajorVersion;
    UCHAR MinorVersion;
    struct
    {
        USHORT TracingEnabled : 1;
        USHORT Reserved1 : 15;
    };
    ULONG HashTableEntries;
    ULONG HashIndexMask;
    ULONG TableUpdateVersion;
    ULONG TableSizeInBytes;
    ULONG LastResetTick;
    ULONG ResetRound;
    ULONG Reserved2;
    ULONG RecordedCount;
    ULONG Reserved3[4];
    ULONG HashTable[ANYSIZE_ARRAY];
} TELEMETRY_COVERAGE_HEADER, *PTELEMETRY_COVERAGE_HEADER;

union PEB_BITFIELD_UNION
{
    BOOLEAN BitField;
    struct
    {
        BOOLEAN ImageUsesLargePages : 1;
        BOOLEAN IsProtectedProcess : 1;
        BOOLEAN IsImageDynamicallyRelocated : 1;
        BOOLEAN SkipPatchingUser32Forwarders : 1;
        BOOLEAN IsPackagedProcess : 1;
        BOOLEAN IsAppContainer : 1;
        BOOLEAN IsProtectedProcessLight : 1;
        BOOLEAN IsLongPathAwareProcess : 1;
    };
};

union PEB_CROSS_PROCESS_FLAGS_UNION
{
    ULONG CrossProcessFlags;
    struct
    {
        ULONG ProcessInJob : 1;
        ULONG ProcessInitializing : 1;
        ULONG ProcessUsingVEH : 1;
        ULONG ProcessUsingVCH : 1;
        ULONG ProcessUsingFTH : 1;
        ULONG ProcessPreviouslyThrottled : 1;
        ULONG ProcessCurrentlyThrottled : 1;
        ULONG ProcessImagesHotPatched : 1; // REDSTONE5
        ULONG ReservedBits0 : 24;
    };
};

union PEB_KERNEL_CALLBACK_TABLE_UNION
{
    PVOID KernelCallbackTable;
    PVOID UserSharedInfoPtr;
};

union PEB_CONTEXT_DATA_UNION
{
    PVOID pContextData; // WIN7
    PVOID pUnused; // WIN10
    PVOID EcCodeBitMap; // WIN11
};

union PEB_TRACING_FLAGS_UNION
{
    ULONG TracingFlags;
    struct
    {
        ULONG HeapTracingEnabled : 1;
        ULONG CritSecTracingEnabled : 1;
        ULONG LibLoaderTracingEnabled : 1;
        ULONG SpareTracingBits : 29;
    };
};

union PEB_LEAP_SECONDS_FLAG_UNION
{
    ULONG LeapSecondFlags;
    struct
    {
        ULONG SixtySecondEnabled : 1;
        ULONG Reserved : 31;
    };
};

// symbols
typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    PEB_BITFIELD_UNION BitField;

    HANDLE Mutant;

    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PRTL_CRITICAL_SECTION FastPebLock;
    PSLIST_HEADER AtlThunkSListPtr;
    PVOID IFEOKey;
    PEB_CROSS_PROCESS_FLAGS_UNION CrossProcessFlags;
    PEB_KERNEL_CALLBACK_TABLE_UNION KernelCallbackTable;

    ULONG SystemReserved;
    ULONG AtlThunkSListPtr32;
    PAPI_SET_NAMESPACE ApiSetMap;
    ULONG TlsExpansionCounter;
    PRTL_BITMAP TlsBitmap;

    ARRAY_CONTAINER<ULONG, 2> TlsBitmapBits; // TLS_MINIMUM_AVAILABLE
    PVOID ReadOnlySharedMemoryBase;
    PSILO_USER_SHARED_DATA SharedData; // HotpatchInformation
    PVOID *ReadOnlyStaticServerData;

    PVOID AnsiCodePageData; // PCPTABLEINFO
    PVOID OemCodePageData; // PCPTABLEINFO
    PVOID UnicodeCaseTableData; // PNLSTABLEINFO

    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;

    ULARGE_INTEGER CriticalSectionTimeout;
    SIZE_T HeapSegmentReserve;
    SIZE_T HeapSegmentCommit;
    SIZE_T HeapDeCommitTotalFreeThreshold;
    SIZE_T HeapDeCommitFreeBlockThreshold;

    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PVOID *ProcessHeaps; // PHEAP

    PVOID GdiSharedHandleTable; // PGDI_SHARED_MEMORY
    PVOID ProcessStarterHelper;
    ULONG GdiDCAttributeList;

    PRTL_CRITICAL_SECTION LoaderLock;

    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
    ULONG ImageSubsystem;
    ULONG ImageSubsystemMajorVersion;
    ULONG ImageSubsystemMinorVersion;
    KAFFINITY ActiveProcessAffinityMask;
    ARRAY_CONTAINER<ULONG, GDI_HANDLE_BUFFER_SIZE> GdiHandleBuffer;
    PVOID PostProcessInitRoutine;

    PRTL_BITMAP TlsExpansionBitmap;
    ARRAY_CONTAINER<ULONG, 32> TlsExpansionBitmapBits; // TLS_EXPANSION_SLOTS

    ULONG SessionId;

    ULARGE_INTEGER AppCompatFlags; // KACF_*
    ULARGE_INTEGER AppCompatFlagsUser;
    PVOID pShimData;
    PVOID AppCompatInfo; // APPCOMPAT_EXE_DATA

    UNICODE_STRING CSDVersion;

    PACTIVATION_CONTEXT_DATA ActivationContextData;
    PASSEMBLY_STORAGE_MAP ProcessAssemblyStorageMap;
    PACTIVATION_CONTEXT_DATA SystemDefaultActivationContextData;
    PASSEMBLY_STORAGE_MAP SystemAssemblyStorageMap;

    SIZE_T MinimumStackCommit;

    ARRAY_CONTAINER<PVOID, 2> SparePointers; // 19H1 (previously FlsCallback to FlsHighIndex)
    PVOID PatchLoaderData;
    PVOID ChpeV2ProcessInfo; // _CHPEV2_PROCESS_INFO

    ULONG AppModelFeatureState;
    ARRAY_CONTAINER<ULONG, 2> SpareUlongs;

    USHORT ActiveCodePage;
    USHORT OemCodePage;
    USHORT UseCaseMapping;
    USHORT UnusedNlsField;

    PVOID WerRegistrationData;
    PVOID WerShipAssertPtr;

    PEB_CONTEXT_DATA_UNION ContextData;

    PVOID pImageHeaderHash;
    PEB_TRACING_FLAGS_UNION TracingFlags;

    ULONGLONG CsrServerReadOnlySharedMemoryBase;
    PRTL_CRITICAL_SECTION TppWorkerpListLock;
    LIST_ENTRY TppWorkerpList;
    ARRAY_CONTAINER<PVOID, 128> WaitOnAddressHashTable;
    PTELEMETRY_COVERAGE_HEADER TelemetryCoverageHeader; // REDSTONE3
    ULONG CloudFileFlags;
    ULONG CloudFileDiagFlags; // REDSTONE4
    CHAR PlaceholderCompatibilityMode;
    ARRAY_CONTAINER<CHAR, 7> PlaceholderCompatibilityModeReserved;
    PLEAP_SECOND_DATA LeapSecondData; // REDSTONE5
    PEB_LEAP_SECONDS_FLAG_UNION LeapSecondFlags;

    ULONG NtGlobalFlag2;
    ULONGLONG ExtendedFeatureDisableMask; // since WIN11
} PEB, *PPEB;

#ifdef _WIN64
C_ASSERT(FIELD_OFFSET(PEB, SessionId) == 0x2C0);
//C_ASSERT(sizeof(PEB) == 0x7B0); // REDSTONE3
//C_ASSERT(sizeof(PEB) == 0x7B8); // REDSTONE4
//C_ASSERT(sizeof(PEB) == 0x7C8); // REDSTONE5 // 19H1
C_ASSERT(sizeof(PEB) == 0x7d0); // WIN11
#else
C_ASSERT(FIELD_OFFSET(PEB, SessionId) == 0x1D4);
//C_ASSERT(sizeof(PEB) == 0x468); // REDSTONE3
//C_ASSERT(sizeof(PEB) == 0x470); // REDSTONE4
//C_ASSERT(sizeof(PEB) == 0x480); // REDSTONE5 // 19H1
C_ASSERT(sizeof(PEB) == 0x488); // WIN11
#endif
#define GDI_BATCH_BUFFER_SIZE 310

typedef struct _GDI_TEB_BATCH
{
    ULONG Offset;
    ULONG_PTR HDC;
    ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT
{
    ULONG Flags;
    PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT_EX
{
    TEB_ACTIVE_FRAME_CONTEXT BasicContext;
    PSTR SourceLocation;
} TEB_ACTIVE_FRAME_CONTEXT_EX, *PTEB_ACTIVE_FRAME_CONTEXT_EX;

typedef struct _TEB_ACTIVE_FRAME
{
    ULONG Flags;
    struct _TEB_ACTIVE_FRAME *Previous;
    PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _TEB_ACTIVE_FRAME_EX
{
    TEB_ACTIVE_FRAME BasicFrame;
    PVOID ExtensionIdentifier;
} TEB_ACTIVE_FRAME_EX, *PTEB_ACTIVE_FRAME_EX;

#define STATIC_UNICODE_BUFFER_LENGTH 261
#define WIN32_CLIENT_INFO_LENGTH 62

union TEB_CURRENT_IDEAL_PROCESSOR_UNION
{
    PROCESSOR_NUMBER CurrentIdealProcessor;
    ULONG IdealProcessorValue;
    struct
    {
        UCHAR ReservedPad0;
        UCHAR ReservedPad1;
        UCHAR ReservedPad2;
        UCHAR IdealProcessor;
    };
};

union TEB_CROSS_TEB_FLAGS_UNION
{
    USHORT CrossTebFlags;
    USHORT SpareCrossTebBits : 16;
};

union TEB_SAME_TEB_FLAGS_UNION
{
    USHORT SameTebFlags;
    struct
    {
        USHORT SafeThunkCall : 1;
        USHORT InDebugPrint : 1;
        USHORT HasFiberData : 1;
        USHORT SkipThreadAttach : 1;
        USHORT WerInShipAssertCode : 1;
        USHORT RanProcessInit : 1;
        USHORT ClonedThread : 1;
        USHORT SuppressDebugMsg : 1;
        USHORT DisableUserStackWalk : 1;
        USHORT RtlExceptionAttached : 1;
        USHORT InitialThread : 1;
        USHORT SessionAware : 1;
        USHORT LoadOwner : 1;
        USHORT LoaderWorker : 1;
        USHORT SkipLoaderInit : 1;
        USHORT SkipFileAPIBrokering : 1;
    };
};

typedef struct _TEB
{
    NT_TIB NtTib;

    PVOID EnvironmentPointer;
    CLIENT_ID ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    PPEB ProcessEnvironmentBlock;

    ULONG LastErrorValue;
    ULONG CountOfOwnedCriticalSections;
    PVOID CsrClientThread;
    PVOID Win32ThreadInfo;
    ARRAY_CONTAINER<ULONG, 26> User32Reserved;
    ARRAY_CONTAINER<ULONG, 5> UserReserved;
    PVOID WOW32Reserved;
    LCID CurrentLocale;
    ULONG FpSoftwareStatusRegister;
    ARRAY_CONTAINER<PVOID, 16> ReservedForDebuggerInstrumentation;
#ifdef _WIN64
    ARRAY_CONTAINER<PVOID, 25> SystemReserved1;

    PVOID HeapFlsData;

    ARRAY_CONTAINER<ULONG_PTR, 4> RngState;
#else
    ARRAY_CONTAINER<PVOID, 26> SystemReserved1;
#endif

    CHAR PlaceholderCompatibilityMode;
    BOOLEAN PlaceholderHydrationAlwaysExplicit;
    ARRAY_CONTAINER<CHAR, 10> PlaceholderReserved;

    ULONG ProxiedProcessId;
    ACTIVATION_CONTEXT_STACK ActivationStack;

    ARRAY_CONTAINER<UCHAR, 8> WorkingOnBehalfTicket;

    NTSTATUS ExceptionCode;

    PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
    ULONG_PTR InstrumentationCallbackSp;
    ULONG_PTR InstrumentationCallbackPreviousPc;
    ULONG_PTR InstrumentationCallbackPreviousSp;
#ifdef _WIN64
    ULONG TxFsContext;
#endif

    BOOLEAN InstrumentationCallbackDisabled;
#ifdef _WIN64
    BOOLEAN UnalignedLoadStoreExceptions;
#endif
#ifndef _WIN64
    ARRAY_CONTAINER<UCHAR, 23> SpareBytes;
    ULONG TxFsContext;
#endif
    GDI_TEB_BATCH GdiTebBatch;
    CLIENT_ID RealClientId;
    HANDLE GdiCachedProcessHandle;
    ULONG GdiClientPID;
    ULONG GdiClientTID;
    PVOID GdiThreadLocalInfo;
    ARRAY_CONTAINER<ULONG_PTR, WIN32_CLIENT_INFO_LENGTH>  Win32ClientInfo;

    ARRAY_CONTAINER<PVOID, 233> glDispatchTable;
    ARRAY_CONTAINER<ULONG_PTR, 29> glReserved1;
    PVOID glReserved2;
    PVOID glSectionInfo;
    PVOID glSection;
    PVOID glTable;
    PVOID glCurrentRC;
    PVOID glContext;

    NTSTATUS LastStatusValue;

    UNICODE_STRING StaticUnicodeString;
    ARRAY_CONTAINER<WCHAR, STATIC_UNICODE_BUFFER_LENGTH> StaticUnicodeBuffer;

    PVOID DeallocationStack;

    ARRAY_CONTAINER<PVOID, TLS_MINIMUM_AVAILABLE> TlsSlots;
    LIST_ENTRY TlsLinks;

    PVOID Vdm;
    PVOID ReservedForNtRpc;
    ARRAY_CONTAINER<PVOID, 2> DbgSsReserved;

    ULONG HardErrorMode;
#ifdef _WIN64
    ARRAY_CONTAINER<PVOID, 11> Instrumentation;
#else
    ARRAY_CONTAINER<PVOID, 9> Instrumentation;
#endif
    GUID ActivityId;

    PVOID SubProcessTag;
    PVOID PerflibData;
    PVOID EtwTraceData;
    PVOID WinSockData;
    ULONG GdiBatchCount;

    TEB_CURRENT_IDEAL_PROCESSOR_UNION CurrentIdealProcessor;

    ULONG GuaranteedStackBytes;
    PVOID ReservedForPerf;
    PVOID ReservedForOle; // tagSOleTlsData
    ULONG WaitingOnLoaderLock;
    PVOID SavedPriorityState;
    ULONG_PTR ReservedForCodeCoverage;
    PVOID ThreadPoolData;
    PVOID *TlsExpansionSlots;
#ifdef _WIN64
    PVOID ChpeV2CpuAreaInfo; // CHPEV2_CPUAREA_INFO // previously DeallocationBStore
    PVOID Unused; // previously BStoreLimit
#endif
    ULONG MuiGeneration;
    ULONG IsImpersonating;
    PVOID NlsCache;
    PVOID pShimData;
    ULONG HeapData;
    HANDLE CurrentTransactionHandle;
    PTEB_ACTIVE_FRAME ActiveFrame;
    PVOID FlsData;

    PVOID PreferredLanguages;
    PVOID UserPrefLanguages;
    PVOID MergedPrefLanguages;
    ULONG MuiImpersonation;

    TEB_CROSS_TEB_FLAGS_UNION CrossTebFlags;
    TEB_SAME_TEB_FLAGS_UNION SameTebFlags;

    PVOID TxnScopeEnterCallback;
    PVOID TxnScopeExitCallback;
    PVOID TxnScopeContext;
    ULONG LockCount;
    LONG WowTebOffset;
    PVOID ResourceRetValue;
    PVOID ReservedForWdf;
    ULONGLONG ReservedForCrt;
    GUID EffectiveContainerId;
    ULONGLONG LastSleepCounter; // Win11
    ULONG SpinCallCount;
    ULONGLONG ExtendedFeatureDisableMask;
    PVOID SchedulerSharedDataSlot; // 24H2
    PVOID HeapWalkContext;
    GROUP_AFFINITY PrimaryGroupAffinity;
    ARRAY_CONTAINER<ULONG, 2> Rcu;
} TEB, *PTEB;
/*
#ifdef _WIN64
//C_ASSERT(sizeof(TEB) == 0x1850); // WIN11
C_ASSERT(sizeof(TEB) == 0x1878); // 24H2
#else
//C_ASSERT(sizeof(TEB) == 0x1018); // WIN11
C_ASSERT(sizeof(TEB) == 0x1038); // 24H2
#endif
*/
#endif
