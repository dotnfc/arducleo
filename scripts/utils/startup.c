/*----------------------------------------------------------------------------
 * NFCShare startup script sample
 * ---------------------------------------
 *
 * - ��� Utilities �Ӳ˵�
 * - ��� snippet ����
 *
 * by ������ (dotnfc@163.com)
 * rev 1.0.0, $$ 2011/10/09 20:57:02 $$
 *---------------------------------------------------------------------------*/
 
char *xml_menu=[[
<?xml version="1.0" ?>
<UtilItems>

  
  <Item Name="�ַ�ת��" 
        Cmdl="this"
        Param="utils\\asc_hex.c"  
        Tooltip="run startup" 
  />
  
</UtilItems>
]];

//printf ("hello, i'm in startup.c\n");

// ���� Utilities �Ӳ˵�
load_menu (xml_menu);
set_editor_font ("Consolas", 9);

// ��� snippet ����
add_snippet ("while () ...", "while (true)\n{\n    $end$\n}\n");

// ��� doxygen ע�Ϳ�
add_shell_cmd ( "doxlic", "utils/dox.c", "dox_lic();", 1 );     // �ļ�ͷ
add_shell_cmd ( "doxfunc", "utils/dox.c", "dox_func();", 1 );   // ������ģ��
add_shell_cmd ( "doxfile", "utils/dox.c", "dox_file();", 1 );   // �ļ�������


/* --------------------------------------------- end-of-script ---------------*/
