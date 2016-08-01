/*----------------------------------------------------------------------------
 * unzip 库实例
 * ---------------------------------------
 *
 * 解压缩　cap 文件，显示其中文件的信息
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-11-29 08:32:00 $$
 *---------------------------------------------------------------------------*/
 
/**
 * 全局变量
 */
char comment[200] = {0};

cls;

int zh = unz_open ( "..\\res\\oath.cap" );
int cnt = unz_get_count ( zh );                     // 取 zip 包文件数目
int rv  = unz_get_comment ( zh, comment, 200 );     // 取 zip 包注释
printf ( "cnt: %d\n%s\n", cnt, comment );

// 枚举 zip 包中的文件
rv = unz_ffirst (zh);
while (rv == UNZ_OK)
{
    char fn[260];
    int sz, zsiz, uzsiz;
    if (unz_fget_info (zh, fn, &zsiz, &uzsiz) != UNZ_OK)
    {
        printf ("无法获取文件信息.\n");
        exit (1);
    }
    
    printf ( "\n文件 %s.\n压缩大小: %d 字节, 原始大小: %d 字节\n", fn, zsiz, uzsiz );
    if (unz_fopen ( zh ) != UNZ_OK)
    {
        printf ("failed to open file.\n");
        exit (1);
    }
    
    unz_fclose ( zh );    
    rv = unz_fnext ( zh );
}

// 关闭包
unz_close ( zh );

/* --------------------------------------------- end-of-script ---------------*/
