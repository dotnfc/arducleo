/*----------------------------------------------------------------------------
 * Cardpad ��ʾ�ű�
 * ---------------------------------------
 *
 * 1. ѯ���û��Ƿ�ִ�в���.
 * 2. ͨ�����������ѭ��ִ�й̶�ָ��Ĵ���.
 *
 * by ������ (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-09-17 12:50:00 $$
 *---------------------------------------------------------------------------*/
 
cls;

#if 0

    reset;                      // �� ATR ����

#else
    char *atr = rd_reset ( );   // �� ATR ����

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

// ���庯��
void loop_test (int count)
{
    printf ( "%s ()++\n", __FUNC__ );
    
    int i = 0;

    for (i = 0; i < count; i ++)
    {
        $ 00a4 0400 00 ;        // ���͵��� APDU
        if (i == 10)
        {   // ���ִ�е��� 10 �Σ���ʾһ����Ϣ���ű��ļ�����ǰ�кţ����ж�ִ��
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
