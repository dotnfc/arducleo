/*----------------------------------------------------------------------------
 * JavaCard ������ʾ�� - Calc Terminal
 * ---------------------------------------
 * 
 *   ���� res/calc.cap �����У��� jcop.exe�������ô˽ű����а�װ�󣬼���
 * ������Ӧ����Ϊ������ʹ�á�UI ��: 
 *
 *    RST  - Ϊ��λ��Ƭ��
 *    ON   - ѡ�� calc Ӧ�ã�
 *    inst - ���ء���װӦ��, 
 *    del  - ɾ��Ӧ�á�
 *
 *   UI ������
 *    1. ʹ�� RST �鿴 atr
 *    2. inst ���ء���װӦ��
 *    3. ʹ�ü���������
 *
 * by ������ (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-09-30 07:59:00 $$
 *---------------------------------------------------------------------------*/
#include "../libs/calc_ui.c"

cls;

#define AID_CALC_APP        "3B2963616C6301"
#define AID_CALC_PKG        "3B2963616C63"

#define MSG_ERROR           "    -- error --     "
#define MSG_READY           "    -- ready --     "
#define MSG_INVALID         " -- invalid card -- "

#define USE_JCOP_SIMUL      0      // 0 - ��Ƭ��ʽ, 1 - jcop simulator ��ʽ

#if USE_JCOP_SIMUL

// ���� JCOP ģ����
void start_jcop_simul ()
{
    if ( proc_isrunning ("jcop.exe") == 0 ) 
    {   // jcop.exe δ���У���ִ�� bin\ ��ʵ��
        char param[300];
        char jcopExe[260];
        
        strcpy (jcopExe, BIN_PATH);
        strcat (jcopExe, "jcop.exe");
        sprintf (param, "/k \"%s\"", jcopExe);
        // printf (param);

        // launch the jcop.exe simulator
        if(exec ("cmd.exe", param, 0, SW_MINIMIZE) == 0) 
        {
            printf ("failed to lauch the jcop.exe.\n");
            exit (1);
        }
        
        sleep (1200);        // wait for jcop.exe being ready
    }
}

start_jcop_simul ();
rd_open (RDR_JCOP, PROT_TX, 0);

#else
rd_open (-1, PROT_TX, 0);
#endif // USE_JCOP_SIMUL

rd_reset ();            // ��λ��Ƭ

////////////////////////////////////////////////////////////////////////////////
// ������ȫͨ��
void do_auth ()
{
    select -AID a000000003000000;
    //open_sc -security 0 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f ;
    auth (0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// Ӧ��ɾ��
void calc_del ()
{
    printf (" Remove the Calculator Applet from JCOP Card...\n\n");
    
    do_auth ();
    
    delete -AID %AID_CALC_APP;
    delete -AID %AID_CALC_PKG;
    
    get_status -element 40;
}

////////////////////////////////////////////////////////////////////////////////
// Ӧ�����ء���װ
void calc_inst ()
{
    printf (" Install the Calculator Applet to JCOP Card...\n\n");
    
    do_auth ();
    install -file ../res/calc.cap -sdAID a000000003000000 -priv 2;
    
    get_status -element 40;
    
    select -AID %AID_CALC_APP;
}

////////////////////////////////////////////////////////////////////////////////
// ���������� "22" -> 0x22
void asc2hex(char *Dest, char *Src, int SrcLen)
{
	int i;
	for ( i = 0; i < SrcLen / 2; i ++ )
	{
		sscanf(Src + i * 2,"%02X", &Dest[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// APDU ִ�лص���������ʾִ��ϸ��
void show_result ()
{
    short result = 0;
    char data[32];
    char msg[32];
    int len = strlen (_rapdu);
    
    printf (" => %s\n", apdu_fmt(_capdu));
    printf (" <= %s\n", apdu_fmt(_rapdu));
    printf ("  %04X\n\n", _sw);
        
    if ((_sw != 0x9000) || (len < 10))
    {   
        if ((_sw & 0xFF00) == 0x6100)
            return;
        ui_settext ("txtDisp", MSG_ERROR);
    } 
    else 
    {
        asc2hex (data, _rapdu, len);
        
        result = ((data[3] & 0xFF) << 8) | (data[4] & 0xFF);
        
        sprintf (msg, "%d", result);
        ui_settext ("txtDisp", msg);
        //setMemory(data[0] == 0x01);
    }    
}

////////////////////////////////////////////////////////////////////////////////
// ִ�м���
void do_calc (byte ins)
{
    $ 00 %02ins 00 00 00 /show_result();
}

// btnClear Click event handler
void btnClear_Click()
{
    do_calc ('C');
}

// btn0 Click event handler
void btn0_Click()
{
    do_calc ('0');
}

// btn1 Click event handler
void btn1_Click()
{
    do_calc ('1');
}

// btn2 Click event handler
void btn2_Click()
{
    do_calc ('2');
}

// btn3 Click event handler
void btn3_Click()
{
    do_calc ('3');
}

// btn4 Click event handler
void btn4_Click()
{
    do_calc ('4');
}

// btn5 Click event handler
void btn5_Click()
{
    do_calc ('5');
}

// btn6 Click event handler
void btn6_Click()
{
    do_calc ('6');
}

// btn7 Click event handler
void btn7_Click()
{
    do_calc ('7');
}

// btn8 Click event handler
void btn8_Click()
{
    do_calc ('8');
}

// btn9 Click event handler
void btn9_Click()
{
    do_calc ('9');
}

// btnDiv Click event handler
void btnDiv_Click()
{
    do_calc (':');
}

// btnST Click event handler
void btnST_Click()
{
    do_calc ('S');
}

// btnMul Click event handler
void btnMul_Click()
{
    do_calc ('x');
}

// btnRM Click event handler
void btnRM_Click()
{
    do_calc ('R');
}

// btnSUB Click event handler
void btnSUB_Click()
{
    do_calc ('-');
}

// btnMM Click event handler
void btnMM_Click()
{
    do_calc ('M');
}

// btnAdd Click event handler
void btnAdd_Click()
{
    do_calc ('+');
}

// btnEQ Click event handler
void btnEQ_Click()
{
    do_calc ('=');
}

// btnInst Click event handler
void btnInst_Click()
{
    calc_inst ();
}

// btnDel Click event handler
void btnDel_Click()
{
    calc_del ();
}

// btnReset Click event handler
void btnReset_Click()
{
    char *atr;
    
    reset;
    atr = rd_reset ();
    if (atr[0] != 0x0)
        ui_settext ("txtDisp", MSG_READY);
}

// btnOn Click event handler
void btnOn_Click()
{
    select -AID %AID_CALC_APP;
}

// init xml ui, and show it.
ui_open (calc_ui, 1);

// close ui
// ui_close();

/* --------------------------------------------- end-of-script ---------------*/
