/*----------------------------------------------------------------------------
 * Cardpad ��ʾ�ű�
 * ---------------------------------------
 * ����ϵͳ���û��Զ�����.
 *
 * ���ƣ���֧�ֽṹ���������Ͱ�ֵ���ݡ�
 *
 *
 * by ������ (y.volta@mail.com)
 * rev 1.2.0, $$ 2011/09/12 06:13:36  $$
 *               2012/03/06 21:28:32 ��ӻص�ʵ��
 *---------------------------------------------------------------------------*/
 
//cls;

#define DLL_ENTRY_CNT      2

char *dll_names[DLL_ENTRY_CNT+1] = {
    "gen_totp",
    "clpbrd",
    0
};

char *dll_proto[DLL_ENTRY_CNT+1] = {
    "int oath_totp_generate (u8 *secret, u32 secret_length, time_t now, u16 time_step_size, time_t start_offset, u32 digits, u8 *output_otp);",
    "void set_clipboard_text (char *text);",
    0
};

////////////////////////////////////////////////////////////////////////////////
// ���ص���������
void load_dlls ()
{
    loadll ("user32", "msgbox1", "int MessageBoxA(int, char *, char *, int);");
    loadll ("user32", "EnumWindows", "int EnumWindows(callback enum_func, u32 param);");
    loadll ("user32", "GetWindowText", "int GetWindowTextA(u32 hwnd, char *buf, int len);");
    loadll ("user32", "SetWindowText", "int SetWindowTextA(u32 hwnd, char *buf);");
    loadll ("user32", "SendMessage", "int SendMessageA(u32 hwnd, int msg, char* w, char * l);");
    loadll ("Kernel32", "dprint", "void OutputDebugStringA(char *msg);");

    loadll ("user32", "GetFocus", "int GetFocus();" );
    loadll ("user32", "GetForegroundWindow", "int GetForegroundWindow();" );
    
    // oath.oath_totp_generate, oath.set_clipboard_text
    // loadll ("oath", "gen_totp", "int oath_totp_generate (char *secret, int secret_length, time_t now, unsigned short time_step_size, time_t start_offset, unsigned int digits, char *output_otp);");
    // loadll ("oath", "clpbrd", "void set_clipboard_text (char *text);");

    loadllx ( "oath", dll_names, dll_proto );
}

///////////////////////////////////////////////////////////////////////////////
// hex to asc: 0x22 -> "22"
void Hex2Asc(char *Dest,char *Src,int SrcLen)
{
	int i;
	for ( i = 0; i < SrcLen; i ++ )
	{
		sprintf(Dest + i * 2,"%02X",(unsigned char)Src[i]);
	}
	Dest[i * 2] = 0;
}

///////////////////////////////////////////////////////////////////////////////
// asc to hex: "22" -> 0x22
void Asc2Hex(char *Dest,char *Src,int SrcLen)
{
	int i;
	for ( i = 0; i < SrcLen / 2; i ++ )
	{
		sscanf(Src + i * 2,"%02X",(unsigned char *)&Dest[i]);
	}
}


//
// 1. ����ϵͳ�� Messagebox�� ����ʾһ����Ϣ
//
void test_msgbox ()
{
  int ret = msgbox1 (0, "���Ǵ� user32.dll ������Ϣ.", "title", 0);
}

//
// 2. ���� OATH �� TOTP
//
void test_totp ()
{
    int       rv = 0;
    char      *s = "3131DD7A3F0305A886BB37893655F3926D5C5632";
    byte      hSeed[64];
    //char      *secret = "\x99\x99\xDD\xDD\xFF\xFF\xDD\x33\x88\xBB\x77\x99\x66\x66\x33\x22\xDD\xCC\x33\x22";
    int       secret_len = 20;
    time_t     now        = time(NULL);
    unsigned  int digits = 6;
    unsigned  int time_step_size = 60;
    time_t     start_offset       = 0;
    char       otp[32];

    Asc2Hex (hSeed, s, strlen (s));
    
    /* ���ÿ��еĽӿڣ����� TOTP */
    int ret = gen_totp (hSeed,
                        secret_len,
                        now,
                        time_step_size,
                        start_offset, 
                        digits, 
                        otp);
              	
    /* ��ӡ��� */
    printf (" the OTP: %s", otp);
    
    /* �� OTP ���Ƶ������� */
    clpbrd (otp);
}

//
// 3. ����ö�ٻص�����ȡ Cardpad �Ĵ��ھ��
//
bool cb_enum_win (u32 hwnd, u32 param)   
{   
    char title[256]; 
    
    GetWindowText (hwnd, title, 256);
    // printf ("0x%08X: %s\n", hwnd, title);
    strlwr ( title );
    if ( strstr ( title, "cardpad" )!= NULL ) 
    {
        printf ( "- Found Cardpad - \n   Handle: 0x%08X\n  Caption: %s.\n", hwnd, title );
    }
    
    return true;
}

load_dlls ();

#define WM_SETTEXT 0x000C
//test_msgbox ();
//test_totp ();
//dprint ("Hello World!\n");
// sleep ( 900 );
printf ( "." );
char *msg="test";
SendMessage (0x002A16A2, 0x000C, 0, msg);
//EnumWindows ((callback)cb_enum_win, 0x9090);


/* --------------------------------------------- end-of-script ---------------*/
