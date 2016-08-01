/*----------------------------------------------------------------------------
 * Cardpad 演示脚本
 * ---------------------------------------
 *
 * 1. 询问用户是否执行测试.
 * 2. 通过输入框输入循环执行固定指令的次数.
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-09-17 12:50:00 $$
 *---------------------------------------------------------------------------*/
 
cls;

#if 0

    reset;                      // 带 ATR 解析

#else
    char *atr = rd_reset ( );   // 无 ATR 解析

    if ( strlen (atr) == 0 ) 
    {
        printf ( "failed to reset your card.\n" );
    }
    else
    {
        printf ( "atr: %s.\n", atr );
    }
#endif 


printf ("test begin\n");

// 主体函数
void loop_test (int count)
{
    printf ( "%s ()++\n", __FUNC__ );
    
    int i = 0;

    for (i = 0; i < count; i ++)
    {
        $ 00a4 0400 00 ;        // 发送单条 APDU
        if (i == 10)
        {   // 如果执行到第 10 次，显示一条信息（脚本文件及当前行号）后，中断执行
            printf ("breakpoint: %s(%d).\n", __FILE__, __LINE__);
            dbgbreak ();
        }
    }
}

// prompt user do or not do this test.
int rv = msgbox ("Do you want to do this test? ", 
			MB_YESNO | MB_ICONINFORMATION);

if ( rv == IDYES)
{
	char szBuf[100];

	rv = inputbox ("Cardpad", "How many times, do you want to do? ", 2, szBuf, 100);
	int tim = atoi (szBuf);

	printf (" You want to try %d times, now begin...\n\n", tim);

	loop_test (tim);
}
else
	printf ("\n You've skiped this test.\n\n");

printf ("test end\n");

/* --------------------------------------------- end-of-script ---------------*/
