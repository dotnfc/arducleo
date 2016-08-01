/*----------------------------------------------------------------------------
 * Cardpad 演示脚本
 * ---------------------------------------
 *
 * 1. 显示字符串中的 html 文本到 webbrowser 控件中
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2011/12/24 14:52:57 $$
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
// xml ui resource
char *cardpad_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="My Cardpad Window" LocationX="0" LocationY="0" SizeWidth="650" SizeHeight="530" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>

        <WebBrowser Name="web" Visible="true" Enabled="true"
            LocationX="16" LocationY="23" SizeWidth="594" SizeHeight="390"
            ></WebBrowser> 

        <Button Name="button" Visible="true" Enabled="true"
            LocationX="250" LocationY="431" SizeWidth="124" SizeHeight="29"
            Text="显示"></Button> 
            
      </Controls>
    </ControlGroup>
  </ControlGroups>
</ControlWnd>
]];

////////////////////////////////////////////////////////////////////////////////
// html 文本演示
char *s_html=[[
<html>
<body>
<table border="1">
  <tr>
    <th>Month</th>
    <th>Savings</th>
  </tr>
  <tr>
    <td>January</td>
    <td>$100</td>
  </tr>
</table>
</body>
</html>
]];

////////////////////////////////////////////////////////////////////////////////
// button Click event handler
void button_Click()
{
    ui_settext ( "web", s_html );
}

////////////////////////////////////////////////////////////////////////////////
// init xml ui, and show it.
ui_open (cardpad_ui, 1);

/* --------------------------------------------- end-of-script ---------------*/
