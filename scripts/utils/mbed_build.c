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


// btnMake Click event handler
void btnMake_Click()
{
    char *s = mbed_get_target_path (  );
    printf ( "\n\n=============================== Build =============================== \n");
    printf ( "%s\n\n", s );
    UI_Enable (0);
    sleep ( 300 );
    
    exec_redirect_io ( "make.exe", "", "exec_done_cb" );
}

// btnClean Click event handler
void btnClean_Click()
{
    char *s = mbed_get_target_path (  );
    printf ( "\n\n=============================== Clean =============================== \n");
    printf ( "%s\n\n", s );
    UI_Enable (0);
    sleep ( 300 );
    
    exec_redirect_io ( "make.exe", " clean", "exec_done_cb" );
}


// ui init call back.
void ui_init ()
{
    mbed_gcc_bin_path ( "E:\\temp\\722\\gcclite52q4\\bin" );
    UI_Enable (1);
}


// xml ui resource
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="mbed 固件生成" LocationX="0" LocationY="0" SizeWidth="600" SizeHeight="320" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <Label Name="lbl0" Visible="true" Enabled="true"
            LocationX="24" LocationY="10" SizeWidth="168" SizeHeight="22"
            Text="make 参数"></Label> 

        <TextBox Name="txtParam" Visible="true" Enabled="true"
            LocationX="24" LocationY="46" SizeWidth="537" SizeHeight="143"
            Text=""></TextBox> 

        <Button Name="btnMake" Visible="true" Enabled="true"
            LocationX="150" LocationY="211" SizeWidth="120" SizeHeight="34"
            Text="生成"></Button> 

        <Button Name="btnClean" Visible="true" Enabled="true"
            LocationX="304" LocationY="211" SizeWidth="120" SizeHeight="34"
            Text="清理"></Button> 

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



