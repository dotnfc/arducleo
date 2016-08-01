/**=============================================================================
 * \brief mbed build helper script
 * 
 * by .nfc, 2016/07/30 08:16:33
 */

// install telnet: pkgmgr /iu:"TelnetClient"

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
        ui_setenable ( "btnReset", 1 );
        //ui_setenable ( "btnErase", 1 );
        ui_setenable ( "btnDownload", 1 );
        ui_waitcursor ( 0 );
        g_exec_done = 0;
    }
    else {
        ui_setenable ( "btnReset", 0 );
        //ui_setenable ( "btnErase", 0 );
        ui_setenable ( "btnDownload", 1 );
        ui_waitcursor ( 1 );
        g_exec_done = 0;
    }
}

char sz_ocd_scr_path[260];
char sz_dap_cfg[260];
char sz_stm_cfg[260];
char sz_app_file[260];
char sz_ocd_param[2048];
void get_openocd_path_info ()
{
    char spath[260];
    char *s = mbed_openocd_bin_path (NULL);
    strcpy (spath, s);
    char *p = strstr ( spath, "\\bin" );
    if (p) *p = 0;
    char *sroot = str_replace ( spath, "\\", "/" );
    
    //strcpy (spath, "X:/Applet/arduino-1.7.8/hardware/tools/OpenOCD-0.9.0-arduino/bin");
    sprintf ( sz_ocd_scr_path, "%s/%s", sroot, "scripts" );
    sprintf ( sz_dap_cfg, "%s", "interface/cmsis-dap.cfg" );
    sprintf ( sz_stm_cfg, "%s", "target/stm32f1x.cfg" );
    strcpy (sz_app_file, mbed_get_target_path ());
    p = str_replace ( sz_app_file, "\\", "/" );
    strcpy (sz_app_file, p);
    sprintf ( sz_ocd_param, " -s %s -f %s -f %s ", sz_ocd_scr_path, sz_dap_cfg, sz_stm_cfg );
}


void dld_action (char *msg, char *cmdl, char *param)
{
    char *s = mbed_get_target_path (  );
    printf ( "\n\n=============================== %s =============================== \n", msg);
    printf ( "%s\n\n", s );
    UI_Enable (0);
    sleep ( 300 );
    
    exec_redirect_io ( cmdl, param, "exec_done_cb" );
}


// btnReset Click event handler
void btnReset_Click()
{
    get_openocd_path_info ();
    strcat (sz_ocd_param, " -c \"init\"  -c \"reset run\" -c \"exit\"");
    printf ( "%s\n", sz_ocd_param );
    
    dld_action ("复位芯片", "openocd", sz_ocd_param); 
}

// btnErase Click event handler
void btnErase_Click()
{
    //get_openocd_path_info ();
    //strcat (sz_ocd_param, "-c \"reset exit\"");
    //dld_action ("擦除芯片", "openocd", sz_ocd_param); 
}

// btnDownload Click event handler
void btnDownload_Click()
{
    char sz_prog[1024];
    get_openocd_path_info ();
    sprintf (sz_prog, " -c \"program %s reset exit 0x08000000\"", sz_app_file);
    strcat (sz_ocd_param, sz_prog);
    printf ( "%s\n", sz_ocd_param );
    
    dld_action ("固件下载", "openocd", sz_ocd_param); 
}

// ui init call back.
void ui_init ()
{
    mbed_openocd_bin_path ( "E:\\temp\\722\\openocd\\bin" );
    UI_Enable (1);
}


// xml ui resource
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="mbed 固件下载" LocationX="0" LocationY="0" SizeWidth="600" SizeHeight="156" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <Button Name="btnReset" Visible="true" Enabled="true"
            LocationX="71" LocationY="52" SizeWidth="120" SizeHeight="34"
            Text="复位目标芯片"></Button> 

        <Button Name="btnErase" Visible="true" Enabled="false"
            LocationX="225" LocationY="52" SizeWidth="120" SizeHeight="34"
            Text="擦除目标芯片"></Button> 

        <Button Name="btnDownload" Visible="true" Enabled="true"
            LocationX="381" LocationY="52" SizeWidth="120" SizeHeight="34"
            Text="下载固件"></Button> 

        <Label Name="lbl0" Visible="true" Enabled="true"
            LocationX="32" LocationY="12" SizeWidth="540" SizeHeight="28"
            Text="操作前，请链接 Arducleo 硬件。并注意用 mLink 对目标芯片供电。"></Label> 

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
    sleep ( 100 );
}

// close ui
ui_close();

/*---------------------------------------------------- end of script ----------------------*/




