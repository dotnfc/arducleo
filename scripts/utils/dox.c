/*----------------------------------------------------------------------------
 * DoxyGen 注释示例
 * ---------------------------------------
 *
 * - 文件头
 * - 函数体模板
 * - 文件、作者
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2012/01/14 07:35:02  $$
 *---------------------------------------------------------------------------*/

void dox_lic()
{
    char *s_block=[[/**
 * Copyright (C) 1998-2012 admin All Rights Reserved.
 * 
 */
]];

    edt_insert_text ( s_block );
}

void dox_file()
{
    char s_txt[1024];
    char tmpbuf[128];
    
    time_t ltime = time ( 0 );
    struct tm *today = localtime( &ltime );
    
    strftime( tmpbuf, 128, "%Y-%m-%d", today );
    
    char *s_fmt = [[
/**
 * @file test.c
 * @brief 
 * @author admin
 * @version 1.0.0
 * @date %s
 */
]];

    sprintf ( s_txt, s_fmt, tmpbuf );
    edt_insert_text ( s_txt );
}

void dox_func()
{
    char *s_block=[[
/**
 * @brief 
 *
 * @param 
 * @param 
 *
 * @return 
 */
]];

    edt_insert_text ( s_block );
}

/* --------------------------------------------- end-of-script ---------------*/
