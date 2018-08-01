/*----------------------------------------------------------------------------
 * Cardpad ◊ ‘¥Ω≈±æ
 * ---------------------------------------
 * 
 * by ”ÂŒ‰Ã’ (y.volta@mail.com)
 * rev 1.0.0, $$ 2011/09/12 06:19:36  $$
 *---------------------------------------------------------------------------*/

char *ui_demo_welcome = [[
<?xml version="1.0" ?>
<ControlWnd Name="Welcome" LocationX="0" LocationY="0" SizeWidth="300" SizeHeight="200">
  <ControlGroups>
    <ControlGroup Name="Welcome!">
      <Controls>
        <Label Name="lblTip1" Visible="true" Enabled="true" 
            LocationX="10" LocationY="12" SizeWidth="50" SizeHeight="20"
            Text="Name:"></Label>
        <TextBox Name="txtName" Visible="true" Enabled="true" ReadOnly="false"
            LocationX="60" LocationY="10" SizeWidth="200" SizeHeight="24"
            Text=""></TextBox>      
        <Label Name="lblTip2" Visible="true" Enabled="true" 
            LocationX="10" LocationY="36" SizeWidth="50" SizeHeight="20"
            Text="Sex :"></Label>
        <RadioButton Name="rbMan" Visible="true" Enabled="true" 
            LocationX="60" LocationY="36" SizeWidth="80" SizeHeight="20"
            Text=" Male" Checked="true"></RadioButton>
        <RadioButton Name="rbWoman" Visible="true" Enabled="true" 
            LocationX="140" LocationY="36" SizeWidth="120" SizeHeight="20"
            Text=" Female" Checked="false" RadioManager="rbMan"></RadioButton>
        <Button Name="btnOK" Visible="true" Enabled="true" 
            LocationX="100" LocationY="70" SizeWidth="80" SizeHeight="24"
            Text="OK"></Button>             
      </Controls>      
    </ControlGroup>
  </ControlGroups>
</ControlWnd>            
]];


/* --------------------------------------------- end-of-script ---------------*/
