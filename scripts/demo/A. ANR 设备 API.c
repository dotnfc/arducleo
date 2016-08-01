/*----------------------------------------------------------------------------
 * NFCShare 演示脚本 | Aero NFC Reader 设备 API
 * ---------------------------------------
 *
 * 展示了设备 API 的“设备操作”类的指令使用
 *
 * by 渝武陶 (dotnfc@mail.com)
 * rev 1.0.0, $$ 2014-02-17 12:50:00 $$
 *---------------------------------------------------------------------------*/
 

/*==============================================================================
 * \brief 提示信息封装
 */
void show_tip (char *msg)
{
    printf ( "\n %s: \n", msg );
}


/*==============================================================================
 * \brief GetData 操作
 */
void btnGetData_Click()
{
    int stat = ui_getint ( "rd_getdata_uid" );
    if (stat) {
        show_tip ( "Get UID" );
        $ FFCA 0000 00 ;
    } else {
        show_tip ( "Get ATS" );        
        $ FFCA 0100 00 ;
    }
}


/*==============================================================================
 * \brief 取版本操作
 */
void btnGetVer_Click()
{
    int stat = ui_getint ( "rd_getver_a" );
    if (stat) {
        show_tip ( "Get Version A" );
        $ FF00 4800 00 ;
    } else {
        show_tip ( "Get Aero NFC Reader Version" );
        $ FF00 4801 00 ;
    }
}


/*==============================================================================
 * \brief 更新版本
 */
void btnSetVer_Click()
{
    char *s = ui_gettext ( "txtVersionA" );
    if (strlen ( s ) > 30) {
        printf ( "version string too long ( <= 15 chars )\n" );
    } else {
        show_tip ( "Update Version" );
        int l = strlen (s) / 2;
                
        $ FF00 4802 %l %s;
    }
}

/*==============================================================================
 * \brief 蜂鸣器设置
 */
void btnSetBuzzer_Click()
{
    int stat = ui_getint ( "rd_buzzer_enable" );
    if (stat) {
        show_tip ( "Enable Buzzer" );
        $ FF00 52FF 00 ;
    } else {
        show_tip ( "Disable Buzzer" );
        $ FF00 5200 00 ;
    }
}


/*==============================================================================
 * \brief 取设备模式
 */
void btnGetMode_Click()
{
    $ FF00 0100 00 ;        // current mode
}


/*==============================================================================
 * \brief 设置特定设备模式: 读写器模式下，P2P 设备优先
 */
void btnSetModePesist_Click()
{
    $ FF00 0100 02 ;        // mode -> nvm
}


/*==============================================================================
 * \brief 设置设备模式
 */
void btnSetMode_Click()
{
    int a5 = ui_getint ( "rd_mode_a5" );
    int a6 = ui_getint ( "rd_mode_a6" );
    int a7 = ui_getint ( "rd_mode_a7" );
    int a8 = ui_getint ( "rd_mode_a8" );
    int a9 = ui_getint ( "rd_mode_a9" );
    int spec=ui_getint ( "chkModeSpec" );
    
    int mode = 0xFF;
    
    if (spec)
        spec = 0x02;
    else
        spec = 0x00;

    if (a5) {
        show_tip ( "Mode: FW update" );
        mode = 0xA5;
    }

    if (a6) {
        show_tip ( "Mode: Reader" );
        mode = 0xA6;
    }

    if (a7) {
        show_tip ( "Mode: Tag Emul" );
        mode = 0xA7;
    }

    if (a8) {
        show_tip ( "Mode: Host Emul" );
        mode = 0xA8;
    }

    if (a9) {
        show_tip ( "Mode: P2P" );
        mode = 0xA9;
    }
    
    if (mode == 0xFF)
        printf ( "Please specify a device mode\n" );
    else
        $ FF00 01 %spec 01 %mode ;
}


/*==============================================================================
 * \brief 设置读写器名称
 */
void btnSetName_Click()
{
    char *ven  = ui_gettext ( "txtDNVen" );
    char *prod = ui_gettext ( "txtDNIF" );
    
    int lven = strlen ( ven );
    int lprod= strlen ( prod );
    
    if (lven > 16) {
        printf ( "vender name should be less than 16 chars\n" );
        return;
    }
    
    if (lprod > 16) {
        printf ( "interface name should be less than 16 chars\n" );
        return;
    }
    
    int lc = 2 + lven + 2 + lprod;
    
    // $ FF00 4200 0D  A0 03 414353  A1 06 414352313232 ;
    char *s_ven;
    h2a ( ven, lven, &s_ven );
    
    char *s_prod;
    h2a ( prod, lprod, &s_prod );
    
    $ FF00 4200 %lc  A0 %lven %s_ven   A1 %lprod %s_prod;
    
    if ( _sw == 0x9000 ) 
    {
        printf ( "Please re-plug this NFC device to your system, and\n" );
        printf ( "do refresh from the Reader Toolbar to see the new reader.\n" );
        // FF00 4200 00 -> reset to the factory reader name.
    }
}


/*==============================================================================
 * \brief UI 初始化
 */
void ui_init ()
{
    //ui_setint ( "rd_getdata_uid", 1 );
    ui_setint ( "lblbuz", 0x707070 );
}


/*==============================================================================
 * \brief GUI description
 */
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="Aero NFC Reader 设备 API" LocationX="0" LocationY="0" SizeWidth="600" SizeHeight="630" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <!--------------------------- getdata group ---------------------------->
        
        <GroupBox Name="gpGetData" Visible="true" Enabled="true"
            LocationX="25" LocationY="25" SizeWidth="491" SizeHeight="60"
            Text=" Get Data"></GroupBox> 

        <RadioButton Name="rd_getdata_uid" Visible="true" Enabled="true"
            LocationX="66" LocationY="52" SizeWidth="117" SizeHeight="22"
            Text="UID"></RadioButton> 

        <RadioButton Name="rd_getdata_ats" Visible="true" Enabled="true"
            LocationX="215" LocationY="52" SizeWidth="117" SizeHeight="22"
            Text="ATS" RadioManager="rd_getdata_uid"></RadioButton> 

        <Button Name="btnGetData" Visible="true" Enabled="true"
            LocationX="398" LocationY="46" SizeWidth="101" SizeHeight="29"
            Text="Get"></Button> 


        <!--------------------------- version group ---------------------------->
        
        <GroupBox Name="gpGetVer" Visible="true" Enabled="true"
            LocationX="25" LocationY="92" SizeWidth="491" SizeHeight="104"
            Text=" Get Version"></GroupBox> 

        <RadioButton Name="rd_getver_a" Visible="true" Enabled="true"
            LocationX="66" LocationY="130" SizeWidth="117" SizeHeight="22"
            Text="Version A"></RadioButton> 

        <RadioButton Name="rd_getver_b" Visible="true" Enabled="true"
            LocationX="215" LocationY="130" SizeWidth="177" SizeHeight="22"
            Text="Version B (fixed)" RadioManager="rd_getver_a"></RadioButton> 

        <Button Name="btnGetVer" Visible="true" Enabled="true"
            LocationX="398" LocationY="122" SizeWidth="101" SizeHeight="29"
            Text="Get"></Button> 

        <Label Name="lblVerA" Visible="true" Enabled="true"
            LocationX="74" LocationY="160" SizeWidth="79" SizeHeight="24"
            Text="Version A"></Label> 

        <TextBox Name="txtVersionA" Visible="true" Enabled="true"
            LocationX="162" LocationY="156" SizeWidth="170" SizeHeight="22"
            Text=""></TextBox> 

        <Button Name="btnSetVer" Visible="true" Enabled="true"
            LocationX="398" LocationY="160" SizeWidth="101" SizeHeight="29"
            Text="Set"></Button> 


        <!---------------------------- buzzer group ---------------------------->

        <GroupBox Name="gpBuzzer" Visible="true" Enabled="true"
            LocationX="24" LocationY="204" SizeWidth="491" SizeHeight="79"
            Text=" Buzzer "></GroupBox> 

        <RadioButton Name="rd_buzzer_enable" Visible="true" Enabled="true"
            LocationX="66" LocationY="229" SizeWidth="117" SizeHeight="22"
            Text="Enable"></RadioButton> 

        <RadioButton Name="rd_buzzer_disable" Visible="true" Enabled="true"
            LocationX="215" LocationY="229" SizeWidth="117" SizeHeight="22"
            Text="Disable" RadioManager="rd_buzzer_enable"></RadioButton> 

        <Button Name="btnSetBuzzer" Visible="true" Enabled="true"
            LocationX="397" LocationY="234" SizeWidth="101" SizeHeight="29"
            Text="Set"></Button> 

        <Label Name="lblbuz" Visible="true" Enabled="true"
            LocationX="78" LocationY="254" SizeWidth="249" SizeHeight="19"
            Text="* Beep when detecting a card"></Label> 
            
            
        <!-------------------------- reader mode group ------------------------->

        <GroupBox Name="gpMode" Visible="true" Enabled="true"
            LocationX="23" LocationY="290" SizeWidth="491" SizeHeight="134"
            Text=" Reader Mode "></GroupBox> 

        <RadioButton Name="rd_mode_a5" Visible="true" Enabled="true"
            LocationX="66" LocationY="317" SizeWidth="140" SizeHeight="22"
            Text="FW update (A5)"></RadioButton> 

        <RadioButton Name="rd_mode_a6" Visible="true" Enabled="true"
            LocationX="215" LocationY="317" SizeWidth="142" SizeHeight="22"
            Text="NFC Reader (A6)" RadioManager="rd_mode_a5"></RadioButton> 

        <RadioButton Name="rd_mode_a7" Visible="true" Enabled="true"
            LocationX="66" LocationY="342" SizeWidth="140" SizeHeight="22"
            Text="Tag Emul (A7)" RadioManager="rd_mode_a5"></RadioButton> 

        <RadioButton Name="rd_mode_a8" Visible="true" Enabled="true"
            LocationX="215" LocationY="342" SizeWidth="142" SizeHeight="22"
            Text="Host Emul (A8)" RadioManager="rd_mode_a5"></RadioButton> 

        <RadioButton Name="rd_mode_a9" Visible="true" Enabled="true"
            LocationX="66" LocationY="366" SizeWidth="140" SizeHeight="22"
            Text="P2P (A9)" RadioManager="rd_mode_a5"></RadioButton> 

        <CheckBox Name="chkModeSpec" Visible="true" Enabled="true"
            LocationX="66" LocationY="396" SizeWidth="321" SizeHeight="18"
            Text="Special P2P mode (see api manual)"></CheckBox> 

        <Button Name="btnGetMode" Visible="true" Enabled="true"
            LocationX="396" LocationY="310" SizeWidth="101" SizeHeight="29"
            Text="Get"></Button> 
            
        <Button Name="btnSetMode" Visible="true" Enabled="true"
            LocationX="396" LocationY="346" SizeWidth="101" SizeHeight="29"
            Text="Set"></Button> 

        <Button Name="btnSetModePesist" Visible="true" Enabled="true"
            LocationX="396" LocationY="382" SizeWidth="101" SizeHeight="29"
            Text="Persist"></Button> 

        <!---------------------------- IF name group --------------------------->
        
        <GroupBox Name="gpSetIF" Visible="true" Enabled="true"
            LocationX="22" LocationY="431" SizeWidth="491" SizeHeight="90"
            Text=" Device Name "></GroupBox> 

        <Button Name="btnSetName" Visible="true" Enabled="true"
            LocationX="395" LocationY="452" SizeWidth="101" SizeHeight="29"
            Text="Set"></Button> 

        <Label Name="lbldn1" Visible="true" Enabled="true"
            LocationX="72" LocationY="462" SizeWidth="80" SizeHeight="15"
            Text="Vender"></Label> 

        <TextBox Name="txtDNVen" Visible="true" Enabled="true"
            LocationX="162" LocationY="460" SizeWidth="170" SizeHeight="22"
            Text="Aero"></TextBox> 

        <Label Name="lbldn2" Visible="true" Enabled="true"
            LocationX="72" LocationY="490" SizeWidth="80" SizeHeight="15"
            Text="Interface"></Label> 

        <TextBox Name="txtDNIF" Visible="true" Enabled="true"
            LocationX="162" LocationY="488" SizeWidth="170" SizeHeight="22"
            Text="NFC Reader"></TextBox> 
        
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


