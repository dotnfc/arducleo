/*----------------------------------------------------------------------------
 * 文件对话框实例
 * ---------------------------------------
 *
 * int get_filename (char *filename, char *filter, int isOpen);
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-09-29 12:50:00 $$
 *---------------------------------------------------------------------------*/
 

cls;


char fn[260];
char *filter = "configure file (*.dat;*.fdp)|*.dat;*.fdp|All files (*.*)|*.*|";

// 打开文件
if ( dlg_selfile ( fn, filter, OFN_MODE_OPEN) == 1 ) 
{
    printf ( "you choosed: \n    %s.\n", fn );
}
else
{
    printf ( "nothing.\n" );
}


// 保存文件
if ( dlg_selfile (fn, filter, OFN_MODE_SAVE) == 1 ) 
{
    printf ( "you choosed: \n    %s.\n", fn );
}
else
{
    printf ( "nothing.\n" );
}

/* --------------------------------------------- end-of-script ---------------*/
