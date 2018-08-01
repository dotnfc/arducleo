/**
 * Windows 系统服务实用工具
 *
 * by dotNFC (dotnfc@gmail.com)
 *    2013/06/01 07:00:45
 *
 */

#include "../libs/lib_svc.c"


/*=============================================================================
 * \brief 查看指定服务的运行状态
 */
void btnView_Click()
{
    char *svc_name = ui_gettext ( "txtSvc" );
    if ( svc_name ) 
    {
        get_service_status (svc_name);
    } 
}


/*=============================================================================
 * \brief 操作封装
 */
void button_command (int swit)
{
    if ( !is_process_elevated ( ) ) 
    {
        char s_param[256];
        sprintf ( s_param, "/x %s", __FILE__ );
        exec_elevate ( "nfcshare.exe", s_param, NULL, SW_SHOW );
        exit ( 1 );
    }
    else
    {
        char *svc_name = ui_gettext ( "txtSvc" );
        if ( svc_name ) 
        {
            printf ( "\n" );
            if (swit)
                control_service_status (svc_name, SERVICE_CONTROL_START);
            else
                control_service_status (svc_name, SERVICE_CONTROL_STOP);
            
            sleep ( 200 );
            get_service_status (svc_name);
        }
    }
}


/*=============================================================================
 * \brief 打开服务
 */
void btnOn_Click()
{
    button_command (1);
}


/*=============================================================================
 * \brief 关闭服务
 */
void btnOff_Click()
{
    button_command (0);
}



/*=============================================================================
 * \brief 界面程序入口
 */
void ui_init ()
{
    int stat = is_process_elevated (  );
    if (stat == 0) {
        ui_setint ( "btnOn", 1 );
        ui_setint ( "btnOff", 1 );
    }
}


////////////////////////////////////////////////////////////////////////////////
// ui 资源
//
char *cardpad_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="系统服务实用工具" LocationX="0" LocationY="0" SizeWidth="398" SizeHeight="169" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <Label Name="label" Visible="true" Enabled="true"
            LocationX="26" LocationY="28" SizeWidth="70" SizeHeight="23"
            Text="服务名:"></Label> 

        <TextBox Name="txtSvc" Visible="true" Enabled="true"
            LocationX="106" LocationY="23" SizeWidth="226" SizeHeight="27"
            Text="CertPropSvc"></TextBox> 

        <Button Name="btnView" Visible="true" Enabled="true"
            LocationX="105" LocationY="80" SizeWidth="80" SizeHeight="31"
            Text="当前状态"></Button> 

        <Button Name="btnOn" Visible="true" Enabled="true"
            LocationX="198" LocationY="80" SizeWidth="64" SizeHeight="32"
            Text="启动"></Button> 

        <Button Name="btnOff" Visible="true" Enabled="true"
            LocationX="268" LocationY="80" SizeWidth="64" SizeHeight="32"
            Text="停止"></Button> 

      </Controls>
    </ControlGroup>
  </ControlGroups>
</ControlWnd>
]];


// init xml ui, and show it.
ui_open (cardpad_ui, 1);

// close ui
// ui_close();

/*---------------------------------------------------- end of script ----------------------*/


