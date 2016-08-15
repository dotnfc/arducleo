/*----------------------------------------------------------------------------
 * NFCShare startup script sample
 * ---------------------------------------
 *
 * - 添加 Utilities 子菜单
 * - 添加 snippet 代码
 *
 * by 渝武陶 (dotnfc@163.com)
 * rev 1.0.0, $$ 2011/10/09 20:57:02 $$
 *---------------------------------------------------------------------------*/
 
char *xml_menu=[[
<?xml version="1.0" ?>
<UtilItems>

  
  <Item Name="字符转换" 
        Cmdl="this"
        Param="utils\\asc_hex.c"  
        Tooltip="run startup" 
  />
  
</UtilItems>
]];

//printf ("hello, i'm in startup.c\n");

// 加载 Utilities 子菜单
load_menu (xml_menu);
set_editor_font ("Consolas", 9);

// 添加 snippet 代码
add_snippet ("while () ...", "while (true)\n{\n    $end$\n}\n");

// 添加 doxygen 注释块
add_shell_cmd ( "doxlic", "utils/dox.c", "dox_lic();", 1 );     // 文件头
add_shell_cmd ( "doxfunc", "utils/dox.c", "dox_func();", 1 );   // 函数体模板
add_shell_cmd ( "doxfile", "utils/dox.c", "dox_file();", 1 );   // 文件、作者


/* --------------------------------------------- end-of-script ---------------*/
