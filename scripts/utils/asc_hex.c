
//
// generated by cardpad
//

void Make_Hex_Room (char *src)
{
    int len = strlen ( src );
    char *dst = malloc ( len * 2 + 1 );
    int j = 0;
    for ( int i=0; i<len; i ) 
    {
        dst[j++] = src[i++];
        dst[j++] = src[i++];
        dst[j++] = ' ';
    }
    
    ui_settext ( "txtHex", dst );
    
    free ( dst );
}

// btnHex Click event handler
void btnHex_Click()
{
    char *s = ui_gettext ( "txtAsc" );
    int   do_conv = ui_getint ( "chkAsUTF8" );
    
    if (strlen ( s ) == 0)  return;
    char *h = 0;
        
    if ( do_conv ) 
    {
        char *g;
        nfc_gbk_to_utf8 ( s, &g );
        h2a ( g, strlen ( g ), &h );
    }
    else
        h2a ( s, strlen ( s ), &h );

    int   do_mkroom = ui_getint ( "chkSpace" );    
    if (do_mkroom)
        Make_Hex_Room ( h );
    else
        ui_settext ( "txtHex", h );
}

void strNormal (char *str)
{
    int len = strlen (str);
    char *stmp = calloc ( len + 1, 1 );
    
    int j = 0;
    for ( int i=0; i<len; i++ ) 
    {
        if ( str[i] == ',' )    continue;
        if ( str[i] == ' ' )    continue;
        if ( str[i] == ';' )    continue;
        
        if ( (str[i] == '0') ) 
        {
            if ( (str[i+1] == 'x') || (str[i+1] == 'X') ) 
            {
                i ++;
                continue;
            }
        }
        
        stmp[j++] = str[i];
    }
    
    strcpy (str, stmp);
    free ( stmp );
}

// btnAsc Click event handler
void btnAsc_Click()
{
    char *h = ui_gettext ( "txtHex" );
    int   do_conv = ui_getint ( "chkAsUTF8" );
    
    if (strlen ( h ) == 0)  return;
    char *s = 0;
    strNormal ( h );
    a2h ( h, &s );
    
    if ( do_conv ) 
    {
        char *u;
        nfc_utf8_to_gbk ( s, &u );
        ui_settext ( "txtAsc", u );
    }
    else
    {
        ui_settext ( "txtAsc", s );
    }
}



// xml ui resource
char *convert_ui = [[
<?xml version="1.0" ?>
<ControlWnd Name="数据转换" LocationX="0" LocationY="0" SizeWidth="569" SizeHeight="460" >
  <ControlGroups>
    <ControlGroup Name="Group 1">
      <Controls>
            
        <TextBox Name="txtAsc" Visible="true" Enabled="true"
            LocationX="17" LocationY="19" SizeWidth="504" SizeHeight="158"
            Text="你好， NFCShare" MultiLine="1"></TextBox> 

        <Button Name="btnHex" Visible="true" Enabled="true"
            LocationX="169" LocationY="185" SizeWidth="68" SizeHeight="26"
            Text="∨ Hex"></Button> 

        <Button Name="btnAsc" Visible="true" Enabled="true"
            LocationX="296" LocationY="185" SizeWidth="68" SizeHeight="26"
            Text="∧ Asc"></Button> 
            
        <TextBox Name="txtHex" Visible="true" Enabled="true"
            LocationX="17" LocationY="220" SizeWidth="504" SizeHeight="158"
            Text="C4 E3 BA C3 A3 AC 20 4E 46 43 53 68 61 72 65 " MultiLine="1"></TextBox> 

        <CheckBox Name="chkAsUTF8" Visible="true" Enabled="true"
            LocationX="28" LocationY="382" SizeWidth="250" SizeHeight="22"
            Text="字符串自动 UTF8 编/解码"></CheckBox> 
            
        <CheckBox Name="chkSpace" Visible="true" Enabled="true"
            LocationX="306" LocationY="382" SizeWidth="210" SizeHeight="22"
            Text="Hex 每字节以空格分隔"></CheckBox> 
            
      </Controls>
    </ControlGroup>
  </ControlGroups>
</ControlWnd>
]];


// init xml ui, and show it.
ui_open (convert_ui, 1);

// close ui
// ui_close();

/*---------------------------------------------------- end of script ----------------------*/


