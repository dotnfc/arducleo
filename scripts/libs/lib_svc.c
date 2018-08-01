
//
// Service Control Manager object specific access types
//
#define SC_MANAGER_CONNECT             0x0001
#define SC_MANAGER_CREATE_SERVICE      0x0002
#define SC_MANAGER_ENUMERATE_SERVICE   0x0004
#define SC_MANAGER_LOCK                0x0008
#define SC_MANAGER_QUERY_LOCK_STATUS   0x0010
#define SC_MANAGER_MODIFY_BOOT_CONFIG  0x0020

//
// Service object specific access type
//
#define SERVICE_QUERY_CONFIG           0x0001
#define SERVICE_CHANGE_CONFIG          0x0002
#define SERVICE_QUERY_STATUS           0x0004
#define SERVICE_ENUMERATE_DEPENDENTS   0x0008
#define SERVICE_START                  0x0010
#define SERVICE_STOP                   0x0020
#define SERVICE_PAUSE_CONTINUE         0x0040
#define SERVICE_INTERROGATE            0x0080
#define SERVICE_USER_DEFINED_CONTROL   0x0100

//
// Controls
//
#define SERVICE_CONTROL_START                  0x00000100
#define SERVICE_CONTROL_STOP                   0x00000001
#define SERVICE_CONTROL_PAUSE                  0x00000002
#define SERVICE_CONTROL_CONTINUE               0x00000003
#define SERVICE_CONTROL_INTERROGATE            0x00000004
#define SERVICE_CONTROL_SHUTDOWN               0x00000005
#define SERVICE_CONTROL_PARAMCHANGE            0x00000006
#define SERVICE_CONTROL_NETBINDADD             0x00000007
#define SERVICE_CONTROL_NETBINDREMOVE          0x00000008
#define SERVICE_CONTROL_NETBINDENABLE          0x00000009
#define SERVICE_CONTROL_NETBINDDISABLE         0x0000000A
#define SERVICE_CONTROL_DEVICEEVENT            0x0000000B
#define SERVICE_CONTROL_HARDWAREPROFILECHANGE  0x0000000C
#define SERVICE_CONTROL_POWEREVENT             0x0000000D
#define SERVICE_CONTROL_SESSIONCHANGE          0x0000000E
#define SERVICE_CONTROL_PRESHUTDOWN            0x0000000F
#define SERVICE_CONTROL_TIMECHANGE             0x00000010
#define SERVICE_CONTROL_TRIGGEREVENT           0x00000020


//
// Service State -- for CurrentState
//
#define SERVICE_STOPPED                        0x00000001
#define SERVICE_START_PENDING                  0x00000002
#define SERVICE_STOP_PENDING                   0x00000003
#define SERVICE_RUNNING                        0x00000004
#define SERVICE_CONTINUE_PENDING               0x00000005
#define SERVICE_PAUSE_PENDING                  0x00000006
#define SERVICE_PAUSED                         0x00000007

typedef struct _SERVICE_STATUS {
    DWORD   dwServiceType;
    DWORD   dwCurrentState;
    DWORD   dwControlsAccepted;
    DWORD   dwWin32ExitCode;
    DWORD   dwServiceSpecificExitCode;
    DWORD   dwCheckPoint;
    DWORD   dwWaitHint;
} SERVICE_STATUS;


loadll ( "Advapi32", "OpenSCManager",  "int OpenSCManagerA(char *s_machine_name, char s_db_name, UINT access);");
loadll ( "Advapi32", "OpenService",  "int OpenServiceA( int hSCManager, char *ServiceName, int dwDesiredAccess);");
loadll ( "Advapi32", "QueryServiceStatus",  "BOOL QueryServiceStatus (int hService, void* lpServiceStatus);");
loadll ( "Advapi32", "CloseServiceHandle",  "BOOL CloseServiceHandle (int hSCManager);");
loadll ( "Advapi32", "ControlService",  "BOOL ControlService (int hSvc, DWORD code, void* lpServiceStatus);");
loadll ( "Advapi32", "_StartService",  "BOOL StartServiceA(int hService, DWORD dwNumServiceArgs, char **lpServiceArgVectors);");

char *str_svc_status (DWORD status)
{
    switch (status)
    {
    case SERVICE_STOPPED:
        return "已停止"; // "SERVICE_STOPPED" );
        break;
    case SERVICE_START_PENDING:
        return "启动中"; // "SERVICE_START_PENDING" );
        break;
    case SERVICE_STOP_PENDING:
        return "停止中"; // "SERVICE_STOP_PENDING" );
        break;
    case SERVICE_RUNNING:
        return "正在运行"; // "SERVICE_RUNNING" );
        break;
    case SERVICE_CONTINUE_PENDING:
        return "继续中"; // "SERVICE_CONTINUE_PENDING" );
        break;
    case SERVICE_PAUSE_PENDING:
        return "暂停中"; // "SERVICE_PAUSE_PENDING" );
        break;
    case SERVICE_PAUSED:
        return "已中止"; // "SERVICE_PAUSED" );
        break;
    default:
        return "状态未知";
        break;
    }
    
    return "";
}

/*==============================================================================
 * 取指定服务的当前状态
 */
int get_service_status (char *svc_name)
{
    int hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE);
    if ( hSC == NULL ) 
    {
        printf ( "Failed to open sc manager\n" );
        exit ( 1 );
    }
    
    int hSvc = OpenService (hSC, svc_name, SERVICE_QUERY_STATUS);
    if ( hSvc == NULL ) 
    {
        CloseServiceHandle (hSC);
        printf ( "Failed to open service\n" );
        exit ( 1 );
    }

    SERVICE_STATUS svcStatus;
    QueryServiceStatus (hSvc, &svcStatus);
    char *s_stat = str_svc_status (svcStatus.dwCurrentState);

    printf ( "%s: %s\n", svc_name, s_stat );
    CloseServiceHandle (hSC);
    
    return svcStatus.dwCurrentState;
}

/*==============================================================================
 * 取指定服务的当前状态
 */
void control_service_status (char *svc_name, uint32_t ctrl_code)
{
    int hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE);
    if ( hSC == NULL ) 
    {
        printf ( "Failed to open sc manager\n" );
        exit ( 1 );
    }
    
    int hSvc = OpenService ( hSC, svc_name, SERVICE_START | SERVICE_STOP );
    if ( hSvc == NULL ) 
    {
        CloseServiceHandle (hSC);
        DWORD dwErr = GetLastError();
        printf ( "Failed to open service in 'Start & Stop' mode for the following reason\n  " );
        fmt_sys_err ( dwErr );
        
        if (dwErr == 0x5)
            printf ( "\n * You should run NFCShare.exe in Administrator mode *\n" );
        exit ( 1 );
    }
    
    if (ctrl_code == SERVICE_CONTROL_START)
    {
        if (!_StartService( hSvc, 0, 0))
        {
            printf ( "Failed to Start this service\n" );
            DWORD dwErr = GetLastError();
            fmt_sys_err ( dwErr );
        }
    }
    else 
    {
        SERVICE_STATUS svcStatus;
        BOOL ret = ControlService (hSvc, ctrl_code, &svcStatus);
        if (!ret)
        {
            DWORD dwErr = GetLastError();
            fmt_sys_err ( dwErr );
        }
    }
    
    CloseServiceHandle (hSC);
}

