/**
 * Windows ϵͳ����ʵ�ù���
 *
 * by dotNFC (dotnfc@gmail.com)
 *    2013/06/01 07:00:45
 *
 */

#include "../libs/lib_svc.c"


/*=============================================================================
 * \brief �鿴ָ�����������״̬
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
 * \brief ������װ
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
 * \brief �򿪷���
 */
void btnOn_Click()
{
    button_command (1);
}


/*=============================================================================
 * \brief �رշ���
 */
void btnOff_Click()
{
    button_command (0);
}



/*=============================================================================
 * \brief ����������
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
// ui ��Դ
//
char *cardpad_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="ϵͳ����ʵ�ù���" LocationX="0" LocationY="0" SizeWidth="398" SizeHeight="169" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <Label Name="label" Visible="true" Enabled="true"
            LocationX="26" LocationY="28" SizeWidth="70" SizeHeight="23"
            Text="������:"></Label> 

        <TextBox Name="txtSvc" Visible="true" Enabled="true"
            LocationX="106" LocationY="23" SizeWidth="226" SizeHeight="27"
            Text="CertPropSvc"></TextBox> 

        <Button Name="btnView" Visible="true" Enabled="true"
            LocationX="105" LocationY="80" SizeWidth="80" SizeHeight="31"
            Text="��ǰ״̬"></Button> 

        <Button Name="btnOn" Visible="true" Enabled="true"
            LocationX="198" LocationY="80" SizeWidth="64" SizeHeight="32"
            Text="����"></Button> 

        <Button Name="btnOff" Visible="true" Enabled="true"
            LocationX="268" LocationY="80" SizeWidth="64" SizeHeight="32"
            Text="ֹͣ"></Button> 

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


