/*----------------------------------------------------------------------------
 * unzip ��ʵ��
 * ---------------------------------------
 *
 * ��ѹ����cap �ļ�����ʾ�����ļ�����Ϣ
 *
 * by ������ (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-11-29 08:32:00 $$
 *---------------------------------------------------------------------------*/
 
/**
 * ȫ�ֱ���
 */
char comment[200] = {0};

cls;

int zh = unz_open ( "..\\res\\oath.cap" );
int cnt = unz_get_count ( zh );                     // ȡ zip ���ļ���Ŀ
int rv  = unz_get_comment ( zh, comment, 200 );     // ȡ zip ��ע��
printf ( "cnt: %d\n%s\n", cnt, comment );

// ö�� zip ���е��ļ�
rv = unz_ffirst (zh);
while (rv == UNZ_OK)
{
    char fn[260];
    int sz, zsiz, uzsiz;
    if (unz_fget_info (zh, fn, &zsiz, &uzsiz) != UNZ_OK)
    {
        printf ("�޷���ȡ�ļ���Ϣ.\n");
        exit (1);
    }
    
    printf ( "\n�ļ� %s.\nѹ����С: %d �ֽ�, ԭʼ��С: %d �ֽ�\n", fn, zsiz, uzsiz );
    if (unz_fopen ( zh ) != UNZ_OK)
    {
        printf ("failed to open file.\n");
        exit (1);
    }
    
    unz_fclose ( zh );    
    rv = unz_fnext ( zh );
}

// �رհ�
unz_close ( zh );

/* --------------------------------------------- end-of-script ---------------*/
