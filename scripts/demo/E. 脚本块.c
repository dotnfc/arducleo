/*----------------------------------------------------------------------------
 * NFCShare 演示脚本
 * ---------------------------------------
 *
 * 1. 执行批处理、Python 脚本块
 *
 * by 渝武陶 (dotnfc@163.com)
 * rev 1.0.0, $$ 2018-08-01 12:50:00 $$
 *---------------------------------------------------------------------------*/
 

char bat[]=[[
//.bat
ping www.baidu.com
]];

char py[]=[[
//.py
print ("Hello World!")
print ("I'm in Python")
]];

// btnSayHello Click event handler
void btnSayHello_Click()
{
    printf ( "\n\n----------------------- python ----------------------- \n" );
    exec_block ( py );
}

// btnBatPing Click event handler
void btnBatPing_Click()
{
    printf ( "\n\n----------------------- batch ----------------------- \n" );
    exec_block ( bat );
}


// ui init call back.
void ui_init ()
{
}

// xml ui resource
char *nfcshare_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="脚本块" LocationX="0" LocationY="0" SizeWidth="487" SizeHeight="144" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <Button Name="btnSayHello" Visible="true" Enabled="true"
            LocationX="269" LocationY="36" SizeWidth="168" SizeHeight="49"
            Text="Python Hello World"></Button> 

        <Button Name="btnBatPing" Visible="true" Enabled="true"
            LocationX="46" LocationY="36" SizeWidth="168" SizeHeight="49"
            Text="Batch Ping CCTV"></Button> 

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


