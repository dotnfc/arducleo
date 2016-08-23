/**=============================================================================
 * \brief mbed build helper script
 * by .nfc, 2016/07/30 08:16:33
 */

/*==============================================================================
 * \brief background action has been done?
 */
int g_exec_done = 0;
void exec_done_cb()
{
    g_exec_done = 1;
}

/*==============================================================================
 * \brief button enable
 */
void UI_Enable (int enable)
{
    if (enable) {
        ui_setenable ( "btnMake", 1 );
        ui_setenable ( "btnClean", 1 );
        ui_waitcursor ( 0 );
        g_exec_done = 0;
    }
    else {
        ui_setenable ( "btnMake", 0 );
        ui_setenable ( "btnClean", 0 );
        ui_waitcursor ( 1 );
        g_exec_done = 0;
    }
}


/*==============================================================================
 * \brief button make
 */
char *get_target_param (char *buf)
{
    int n = ui_getint ( "rF072" );
    if (n)  { strcat (buf, " F072RB=1 ");  printf ("Target STM32F072RB\n");   return buf; }
    
    n = ui_getint ( "rF103" );
    if (n)  { strcat (buf, " F013RB=1 ");  printf ("Target STM32F013RB\n");   return buf; }
    
    n = ui_getint ( "rF401" );
    if (n)  { strcat (buf, " F401RE=1 ");  printf ("Target STM32F401RE\n");   return buf; }
    
    n = ui_getint ( "rF405" );
    if (n)  { strcat (buf, " F405RG=1 ");  printf ("Target STM32F405RG\n");   return buf; }
    
    printf ( "Unknown Target\n" );
}


/*==============================================================================
 * \brief button make
 */
void btnMake_Click()
{
    char sparam[256];
    printf ( "\n\n=============================== Build =============================== \n");
    
    get_target_param (sparam);
    char *s = mbed_get_target_path (  );
    
    printf ( "%s\n\n", s );
    UI_Enable (0);
    sleep ( 300 );
    
    exec_redirect_io ( "make.exe", sparam, "exec_done_cb" );
}


/*==============================================================================
 * \brief button clean
 */
void btnClean_Click()
{
    char *s = mbed_get_target_path (  );
    printf ( "\n\n=============================== Clean =============================== \n");
    printf ( "%s\n\n", s );
    UI_Enable (0);
    sleep ( 300 );
    
    exec_redirect_io ( "make.exe", " clean", "exec_done_cb" );
}


/*==============================================================================
 * \brief set f103 as default target
 */
void reset_target_radio ()
{
    ui_setint ( "rF072", 0 );
    ui_setint ( "rF103", 1 );
    ui_setint ( "rF401", 0 );
    ui_setint ( "rF405", 0 );
}


char sz_gcc_bin[260];
// ui init call back.
void ui_init ()
{
    reset_target_radio ();
    sprintf ( sz_gcc_bin, "%s\\arm_gcc\\bin", BIN_PATH );
    mbed_gcc_bin_path ( sz_gcc_bin );
    UI_Enable (1);
}


// xml ui resource
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="mbed 固件生成" LocationX="0" LocationY="0" SizeWidth="600" SizeHeight="320" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <GroupBox Name="gp1" Visible="true" Enabled="true"
            LocationX="17" LocationY="12" SizeWidth="545" SizeHeight="185"
            Text=" 参数 "></GroupBox> 

        <TextBox Name="txtParam" Visible="true" Enabled="true"
            LocationX="34" LocationY="46" SizeWidth="512" SizeHeight="46"
            Text=""></TextBox> 

        <Button Name="btnMake" Visible="true" Enabled="true"
            LocationX="164" LocationY="224" SizeWidth="120" SizeHeight="34"
            Text="生成"></Button> 

        <Button Name="btnClean" Visible="true" Enabled="true"
            LocationX="318" LocationY="224" SizeWidth="120" SizeHeight="34"
            Text="清理"></Button> 

        <RadioButton Name="rF103" Visible="true" Enabled="true"
            LocationX="45" LocationY="114" SizeWidth="159" SizeHeight="22"
            Text="STM32F103Rx"></RadioButton> 

        <RadioButton Name="rF401" Visible="true" Enabled="true"
            LocationX="45" LocationY="152" SizeWidth="159" SizeHeight="22"
            Text="STM32F401RE" RadioManager="rF103"></RadioButton> 

        <RadioButton Name="rF072" Visible="true" Enabled="true"
            LocationX="219" LocationY="114" SizeWidth="159" SizeHeight="22"
            Text="STM32F072RB" RadioManager="rF103"></RadioButton> 

        <RadioButton Name="rF405" Visible="true" Enabled="true"
            LocationX="219" LocationY="152" SizeWidth="159" SizeHeight="22"
            Text="STM32F405RG" RadioManager="rF103"></RadioButton> 
            
      </Controls>
    </ControlGroup>
  </ControlGroups>
</ControlWnd>
]];

// init xml ui, and show it.
ui_open (nfcshare_ui, 0);

while (1) {
    ui_peek_msg (  );
    
    if (g_exec_done) {
        UI_Enable (1);
    }
    sleep ( 10 );
}

// close ui
ui_close();

/*---------------------------------------------------- end of script ----------------------*/



