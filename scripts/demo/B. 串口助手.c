/*----------------------------------------------------------------------------
 * NFCShare ��ʾ�ű� | �������ֽű�
 * ---------------------------------------
 *
 * չʾ���豸 API �ġ��豸���������ָ��ʹ��
 *
 * by ������ (dotnfc@163.com)
 * rev 1.0.0, $$ 2016/07/20 14:24:55 $$
 *---------------------------------------------------------------------------*/
 

#define STATUS_NORMAL       1
#define STATUS_ERROR        2
#define STATUS_SUCCESS      3
#define STATUS_WARNING      4


int g_uart_handle = 0;        // ���ھ��
int g_proc_send = -1;
int g_proc_recv = -1;



/*==============================================================================
 * \brief ��־��Ϣ
 */
void logText (int mode, char *fmt, ...)
{
    va_list ap;
    char buffer[1024];
    
    va_start ( &ap, fmt );
    vsprintf ( buffer, fmt, ap );
    va_end ( ap );
    ui_settext ( "lblStatus", buffer );
    
    if ( mode == STATUS_ERROR ) {
        ui_setint ( "lblStatus", 0x0000ff );
    }
    else if ( mode == STATUS_SUCCESS ) {
        ui_setint ( "lblStatus", 0x009d00 );
    }
    else if ( mode == STATUS_WARNING ) {
        ui_setint ( "lblStatus", 0x0080ff );    // yellow
    }
    else
        ui_setint ( "lblStatus", 0x00 );        // black
}


/*==============================================================================
 * \brief ˢ���豸�б�
 */
void btnRefresh_Click()
{
    int count = uart_port_list (  );
    if ( count == 0 ) 
    {
        logText ( STATUS_ERROR, "δ���ִ����豸" );
        return;
    }
    
    logText ( STATUS_NORMAL, "���� %d �������豸", count );
    
    ui_clearitems ( "cbxDev" );
    for ( int i=0; i<count; i++ ) 
    {
        ui_additem ( "cbxDev", uart_port_getname ( i, UART_NAME_FRIENDLY) );
    }
    
    if ( count > 0 ) 
    {
        ui_setsel_inx ( "cbxDev", 0 );
    }    
}



/*==============================================================================
 * \brief ��ȡ��������Ϣ
 */
char *get_baud ()
{
    char *s = 0;
    int n = ui_getsel_inx ( "cbxBaud" );
    switch (n)
    {
        case 0: s = "baud=1200"; break;
        case 1: s = "baud=2400"; break;
        case 2: s = "baud=4800"; break;
        
        case 3: s = "baud=9600"; break;
        case 4: s = "baud=14400"; break;
        case 5: s = "baud=19200"; break;
        
        case 6: s = "baud=57600"; break;
        case 7: s = "baud=115200"; break;
        case 8: s = "baud=125000"; break;
        default: s = "baud=9600"; break;        
    }
    
    return s;
}

/*==============================================================================
 * \brief ��ȡλ��Ϣ
 */
char *get_bits ()
{
    char *s = 0;
    int n = ui_getsel_inx ( "cbxBits" );
    switch (n)
    {
        case 0: s = "data=5"; break;
        case 1: s = "data=6"; break;
        case 2: s = "data=7"; break;
        
        case 3: s = "data=8"; break;
        default: s = "data=8"; break;        
    }
    
    return s;
}


/*==============================================================================
 * \brief ��ȡ��ż��Ϣ
 */
char *get_parity ()
{
    char *s = 0;
    int n = ui_getsel_inx ( "cbxParity" );
    switch (n)
    {
        case 0: s = "parity=N"; break;      // None
        case 1: s = "parity=O"; break;      // Odd
        case 2: s = "parity=E"; break;      // Even
        
        case 3: s = "parity=M"; break;      // Mark
        case 4: s = "parity=S"; break;      // Space
        default: s = "parity=N"; break;        
    }
    
    return s;
}


/*==============================================================================
 * \brief ��ȡֹͣλ��Ϣ
 */
char *get_stop ()
{
    char *s = 0;
    int n = ui_getsel_inx ( "cbxStopBit" );
    switch (n)
    {
        case 0: s = "stop=1"; break;      // 1
        case 1: s = "stop=1.5"; break;      // 1.5
        case 2: s = "stop=2"; break;      // 2
        default: s = "stop=1"; break;        
    }
    
    return s;
}


/*==============================================================================
 * \brief ��ȡ��������Ϣ
 */
char *get_stream ()
{
    char *s = 0;
    int n = ui_getsel_inx ( "cbxStreamCtrl" );
    switch (n)
    {     
    }
    
    return "";
}


/*==============================================================================
 * \brief ��ȡ������Ϣ
 * ref: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363143(v=vs.85).aspx
 * COMx[:][baud=b][parity=p][data=d][stop=s][to={on|off}][xon={on|off}][odsr={on|off}][octs={on|off}][dtr={on|off|hs}][rts={on|off|hs|tg}][idsr={on|off}]
 *
 * baud=115200 data=8 parity=N stop=1
 */
char *get_conn_string ()
{
    char param[256];
    sprintf ( param, "%s %s %s %s",
            get_baud (),
            get_bits (),
            get_parity (),
            get_stop ()
             );
             
    return param;
}


/*==============================================================================
 * \brief ����ʱ����Ϣ
 */
char *gen_stime ()
{
    struct _timeb tb;
    char buffer[24];
    
    ftime ( &tb );
    strftime ( buffer, sizeof(buffer), "[%H:%m:%S,", localtime ( &tb.time ) );
    sprintf ( buffer + 10, "%03d] ", tb.millitm );
    return buffer;
}


/*==============================================================================
 * \brief �����߳�
 */
byte g_rev_data[1024];
int g_rev_len = 1024;
void proc_recv_data ()
{    
    g_rev_len = 1024;

    if (uart_recv ( g_uart_handle, g_rev_data, &g_rev_len ) != 0) {
        //fmt_sys_err ( get_last_error() );
        return;
    }
        
    if (g_rev_len == 0)
        return;
    
    int isHex = ui_getint ( "rbRecvHex" );
    update_rx_count (g_rev_len);
    if (isHex) {            // ʮ������ģʽ
        char *str;
        int i, j;
        h2a ( g_rev_data, g_rev_len, &str );
        char *sdat = malloc ( g_rev_len * 3 + 1);
        g_rev_len *= 2;
        for ( i=0; i<g_rev_len; ) {
            sdat[j ++] = str[i++];
            sdat[j ++] = str[i++];
            sdat[j ++] = ' ';
        }
        sdat[j] = '\0';
        
        int showtime = ui_getint ( "chkRecvShowTime" );
        if (showtime) {
            ui_appendtext ( "txtRecv", gen_stime () );
            ui_appendtext ( "txtRecv", sdat );
            ui_appendtext ( "txtRecv", "\r\n" );
        }
        else {
            ui_appendtext ( "txtRecv", str );
        }
    }
    else {                  // �ַ�ģʽ
        g_rev_data[g_rev_len] = 0;
        int showtime = ui_getint ( "chkRecvShowTime" );
        if (showtime) {
            ui_appendtext ( "txtRecv", gen_stime () );
            ui_appendtext ( "txtRecv", g_rev_data );
            ui_appendtext ( "txtRecv", "\r\n" );
        }
        else {
            char *s_normal = str_replace ( g_rev_data, "\n", "\r\n" );
            ui_appendtext ( "txtRecv", s_normal );
            //ui_appendtext ( "txtRecv", "\r\n" );
        }        
    }
}


/*==============================================================================
 * \brief �Զ������߳�
 */
void proc_send_data ()
{
    int bauto_send = ui_getint ( "chkAutoSend" );
    if (bauto_send) {
        btnSend_Click();
    }
}


/*==============================================================================
 * \brief ���豸
 */
void btnConn_Click()
{
    int inx = ui_getsel_inx ( "cbxDev" );
    char *sdev = uart_port_getname (inx, UART_NAME_DEVICE_PATH);
    char *param = get_conn_string ();
    printf ( "open %s, param: %s\n", sdev, param );
    g_uart_handle = uart_open ( sdev, param );
    if ( g_uart_handle == 0 ) {
        logText (STATUS_ERROR, "�򿪴���ʧ��\n" );
        fmt_sys_err ( uart_geterror() );
        return;
    }
    else {
        logText (STATUS_SUCCESS, "���� %s �Ѵ�\n", uart_port_getname (inx, UART_NAME_PORT ) );
    }
    
    // ������̨����
    g_proc_recv = timer_begin ( 200, "proc_recv_data" );
    
    char *s_interval = ui_gettext ( "txtSendInterval" );
    int inter = atoi (s_interval);
    if (inter > 200) {
        //printf ( "auto send every %d mills\n", inter );
        g_proc_send = timer_begin ( inter, "proc_send_data" );
    }
}

// btnDisConn Click event handler
void btnDisConn_Click()
{
    uart_close ( g_uart_handle );
    g_uart_handle = 0;
    logText (STATUS_NORMAL, "�ѹر�");

    if ( g_proc_recv != -1 ) {
        timer_end ( g_proc_recv );
        g_proc_recv = -1;
    }
    
    if ( g_proc_send != -1 ) {
        timer_end ( g_proc_send );
        g_proc_send = -1;
    }
}

/*==============================================================================
 * \brief ���������¼�
 */
void btnSend_Click()
{
    char *sIn = ui_gettext ( "txtSend" );

    byte *data;
    int len;
    int mode = ui_getint ( "rbSendHex" );
    if ( mode ) {       // ʮ������ģʽ
        str_normal ( sIn, " \r\n" );
        a2h ( sIn, &data );
        len = strlen ( sIn ) / 2;
    }
    else {              // �ַ�ģʽ
        len = strlen ( sIn );
        data = sIn;
    }
    
    if (uart_send ( g_uart_handle, data, len ) != 0) {
        logText (STATUS_ERROR, "����ʧ��");
        printf ( "����ʧ��\n %s\n", fmt_sys_err ( get_last_error() ) );
    }
    
    update_tx_count (len);
}

/*==============================================================================
 * \brief ��ս��������¼�
 */
void btnClear_Click()
{
    ui_settext ( "txtRecv", "" );
    update_tx_count (0);
    update_rx_count (0);
}

/*==============================================================================
 * \brief DTR ����Ϊ�ߵ�ƽ
 */
void btnDTRH_Click()
{
    uart_dtr_switch (g_uart_handle, 1);
}

/*==============================================================================
 * \brief DTR ����Ϊ�͵�ƽ
 */
void btnDTRL_Click()
{
    uart_dtr_switch (g_uart_handle, 0);
}


/*==============================================================================
 * \brief ���·������ݳ���
 */
int g_tx_count = 0;
void update_tx_count (int count)
{
    char buffer[24];
    if ( count == 0 ) 
        g_tx_count = 0;
    else
        g_tx_count += count;
        
    sprintf (buffer, "TX: %d", g_tx_count );
    ui_settext ( "lblTX", buffer );
}

/*==============================================================================
 * \brief ���½������ݳ���
 */
int g_rx_count = 0; 
void update_rx_count (int count)
{
    char buffer[24];
    if ( count == 0 ) 
        g_rx_count = 0;
    else
        g_rx_count += count;
        
    sprintf (buffer, "RX: %d", g_rx_count );
    ui_settext ( "lblRX", buffer );
}

// ui init call back.
void ui_init ()
{
    ui_clearitems ( "cbxDev" );
    
    ui_clearitems ( "cbxBaud" );
    ui_additem ( "cbxBaud", "1200" );
    ui_additem ( "cbxBaud", "2400" );
    ui_additem ( "cbxBaud", "4800" );
    ui_additem ( "cbxBaud", "9600" );
    ui_additem ( "cbxBaud", "14400" );
    ui_additem ( "cbxBaud", "19200" );
    ui_additem ( "cbxBaud", "57600" );
    ui_additem ( "cbxBaud", "115200" );
    ui_additem ( "cbxBaud", "125000" );
    ui_additem ( "cbxBaud", "128000" );
    ui_additem ( "cbxBaud", "256000" );
    ui_setsel_inx ( "cbxBaud", 3 );
    
    ui_clearitems ( "cbxBits" );
    ui_additem ( "cbxBits", "5" );          ui_additem ( "cbxBits", "6" );
    ui_additem ( "cbxBits", "7" );          ui_additem ( "cbxBits", "8" );
    ui_setsel_inx ( "cbxBits", 3 );
    
    ui_clearitems ( "cbxParity" );
    ui_additem ( "cbxParity", "None" );     ui_additem ( "cbxParity", "Odd" );
    ui_additem ( "cbxParity", "Even" );     ui_additem ( "cbxParity", "Mark" );
    ui_additem ( "cbxParity", "Space" );
    ui_setsel_inx ( "cbxParity", 0 );
    
    ui_clearitems ( "cbxStopBit" );
    ui_additem ( "cbxStopBit", "1" );       ui_additem ( "cbxStopBit", "1.5" );
    ui_additem ( "cbxStopBit", "2" );
    ui_setsel_inx ( "cbxStopBit", 0 );
    
    ui_clearitems ( "cbxStreamCtrl" );
    ui_additem ( "cbxStreamCtrl", "None" ); 
    ui_additem ( "cbxStreamCtrl", "DTR/DSR" );
    ui_additem ( "cbxStreamCtrl", "RTS/CTS" );    
    ui_setsel_inx ( "cbxStreamCtrl", 0 );
    
    ui_setint ( "rbRecvAscii", 0 );
    ui_setint ( "rbRecvHex", 1 );
    
    ui_setint ( "rbSendAscii", 0 );
    ui_setint ( "rbSendHex", 1 );
    
    btnRefresh_Click();
    update_tx_count (0);
    update_rx_count (0);
}

/*==============================================================================
 * \brief ui closing
 */
void ui_exit ()
{
    btnDisConn_Click();
}


// xml ui resource
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="���ڹ���" LocationX="0" LocationY="0" SizeWidth="884" SizeHeight="610" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>
      
<!--------------------------------------------- �豸 ---------------------------------------------------->
        <ComboBox Name="cbxDev" Visible="true" Enabled="true"
            LocationX="19" LocationY="28" SizeWidth="238" SizeHeight="20"
            Text="cbxDev"></ComboBox> 
            
        <Button Name="btnRefresh" Visible="true" Enabled="true"
            LocationX="278" LocationY="28" SizeWidth="60" SizeHeight="29"
            Text="ˢ��"></Button> 
            
        <Button Name="btnConn" Visible="true" Enabled="true"
            LocationX="348" LocationY="28" SizeWidth="60" SizeHeight="29"
            Text="����"></Button> 

        <Button Name="btnDisConn" Visible="true" Enabled="true"
            LocationX="418" LocationY="28" SizeWidth="60" SizeHeight="29"
            Text="�ر�"></Button> 

        <Button Name="btnDTRH" Visible="true" Enabled="true"
            LocationX="534" LocationY="28" SizeWidth="66" SizeHeight="29"
            Text="dtrHigh"></Button> 

        <Button Name="btnDTRL" Visible="true" Enabled="true"
            LocationX="604" LocationY="28" SizeWidth="66" SizeHeight="29"
            Text="dtrLow"></Button> 
            
        <Button Name="btnClear" Visible="true" Enabled="true"
            LocationX="744" LocationY="28" SizeWidth="80" SizeHeight="29"
            Text="��ս���"></Button> 
            
<!------------------------------------------- ��������-------------------------------------------------->
        <GroupBox Name="grp1" Visible="true" Enabled="true"
            LocationX="17" LocationY="61" SizeWidth="240" SizeHeight="191"
            Text=" �������� "></GroupBox> 
            
        <Label Name="lbl1" Visible="true" Enabled="true"
            LocationX="26" LocationY="94" SizeWidth="62" SizeHeight="20"
            Text="������"></Label> 

        <ComboBox Name="cbxBaud" Visible="true" Enabled="true"
            LocationX="90" LocationY="91" SizeWidth="155" SizeHeight="20"
            Text=""></ComboBox> 

        <Label Name="lbl2" Visible="true" Enabled="true"
            LocationX="26" LocationY="123" SizeWidth="62" SizeHeight="20"
            Text="����λ"></Label> 

        <ComboBox Name="cbxBits" Visible="true" Enabled="true"
            LocationX="90" LocationY="120" SizeWidth="155" SizeHeight="20"
            Text="Combobox"></ComboBox> 

        <Label Name="lbl3" Visible="true" Enabled="true"
            LocationX="26" LocationY="151" SizeWidth="62" SizeHeight="20"
            Text="У��λ"></Label> 

        <ComboBox Name="cbxParity" Visible="true" Enabled="true"
            LocationX="90" LocationY="149" SizeWidth="155" SizeHeight="20"
            Text="Combobox"></ComboBox> 

        <Label Name="lbl4" Visible="true" Enabled="true"
            LocationX="26" LocationY="181" SizeWidth="62" SizeHeight="20"
            Text="ֹͣλ"></Label> 

        <ComboBox Name="cbxStopBit" Visible="true" Enabled="true"
            LocationX="90" LocationY="178" SizeWidth="155" SizeHeight="20"
            Text="Combobox"></ComboBox> 

        <Label Name="lbl5" Visible="true" Enabled="true"
            LocationX="26" LocationY="210" SizeWidth="62" SizeHeight="20"
            Text="������"></Label> 

        <ComboBox Name="cbxStreamCtrl" Visible="true" Enabled="true"
            LocationX="90" LocationY="207" SizeWidth="155" SizeHeight="20"
            Text="Combobox"></ComboBox> 
            
<!------------------------------------------- ��������-------------------------------------------------->
        <GroupBox Name="grp2" Visible="true" Enabled="true"
            LocationX="17" LocationY="271" SizeWidth="240" SizeHeight="125"
            Text="��������"></GroupBox> 

        <RadioButton Name="rbRecvAscii" Visible="true" Enabled="true"
            LocationX="35" LocationY="302" SizeWidth="93" SizeHeight="20"
            Text="�ַ�"></RadioButton> 

        <RadioButton Name="rbRecvHex" Visible="true" Enabled="true"
            LocationX="154" LocationY="302" SizeWidth="93" SizeHeight="20"
            Text="ʮ������" RadioManager="rbRecvAscii"></RadioButton> 

        <CheckBox Name="chkAutoCR" Visible="true" Enabled="true"
            LocationX="43" LocationY="333" SizeWidth="190" SizeHeight="21"
            Text="�Զ�����"></CheckBox> 

        <CheckBox Name="chkRecvShowTime" Visible="true" Enabled="true"
            LocationX="43" LocationY="361" SizeWidth="190" SizeHeight="21"
            Text="��ʾʱ��"></CheckBox> 

<!------------------------------------------- ��������-------------------------------------------------->               
        <GroupBox Name="grp3" Visible="true" Enabled="true"
            LocationX="17" LocationY="415" SizeWidth="240" SizeHeight="99"
            Text="��������"></GroupBox> 

        <RadioButton Name="rbSendAscii" Visible="true" Enabled="true"
            LocationX="35" LocationY="446" SizeWidth="93" SizeHeight="20"
            Text="�ַ�"></RadioButton> 

        <RadioButton Name="rbSendHex" Visible="true" Enabled="true"
            LocationX="154" LocationY="446" SizeWidth="93" SizeHeight="20"
            Text="ʮ������" RadioManager="rbSendAscii"></RadioButton> 

        <CheckBox Name="chkAutoSend" Visible="true" Enabled="true"
            LocationX="38" LocationY="479" SizeWidth="98" SizeHeight="21"
            Text="�ظ�����"></CheckBox> 

        <TextBox Name="txtSendInterval" Visible="true" Enabled="true"
            LocationX="144" LocationY="476" SizeWidth="71" SizeHeight="21"
            Text="1000"></TextBox> 

        <Label Name="lblSend0" Visible="true" Enabled="true"
            LocationX="221" LocationY="475" SizeWidth="26" SizeHeight="21"
            Text="ms"></Label> 

<!------------------------------------------- ��ʾ�ؼ�-------------------------------------------------->
        <TextBox Name="txtRecv" Visible="true" Enabled="true"
            LocationX="278" LocationY="67" SizeWidth="547" SizeHeight="328"
            MultiLine="1" ReadOnly="true" Text=""></TextBox> 

        <TextBox Name="txtSend" Visible="true" Enabled="true"
            LocationX="279" LocationY="424" SizeWidth="547" SizeHeight="90"
            MultiLine="1" Text=""></TextBox> 

<!-------------------------------------------- ״̬�� --------------------------------------------------->
        <Label Name="lblStatus" Visible="true" Enabled="true"
            LocationX="19" LocationY="528" SizeWidth="411" SizeHeight="19"
            Text="����"></Label> 

        <Label Name="lblRX" Visible="true" Enabled="true"
            LocationX="462" LocationY="528" SizeWidth="94" SizeHeight="21"
            Text="RX: 0"></Label> 

        <Label Name="lblTX" Visible="true" Enabled="true"
            LocationX="577" LocationY="528" SizeWidth="94" SizeHeight="21"
            Text="TX: 0"></Label> 

        <Button Name="btnSend" Visible="true" Enabled="true"
            LocationX="756" LocationY="527" SizeWidth="64" SizeHeight="27"
            Text="����"></Button> 

      </Controls>
    </ControlGroup>
  </ControlGroups>
</ControlWnd>
]];


// init xml ui, and show it.
ui_open (nfcshare_ui, 1);

// close ui
// ui_close();

/*---------------------------------------------------- end of script ----------------------*/


