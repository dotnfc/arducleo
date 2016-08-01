/*----------------------------------------------------------------------------
 * JavaCard OATH HOTP - HOTP Terminal
 * ---------------------------------------
 * 
 *   下载 res/calc.cap 到卡中（或 jcop.exe），利用此脚本进行安装后，即可
 *
 * 注： 6 位 OTP 计算量大，较慢；在实卡上考虑使用 8 位 OTP。
 *
 * by 渝武陶 (y.volta@mail.com)
 * rev 1.0.0, $$ 2011-09-29 12:50:00 $$
 *---------------------------------------------------------------------------*/
 

cls;

#define AID_OATH_APP        "46544f41544831312E302E31"
#define AID_OATH_PKG        "46544f41544831"

#define USE_JCOP_SIMUL      1       // 0 - 卡片方式, 1 - jcop simulator 方式

#if USE_JCOP_SIMUL

// 启动 JCOP 模拟器
void start_jcop_simul ()
{
    if ( proc_isrunning ("jcop.exe") == 0 ) 
    {   // jcop.exe 未运行，则执行 bin\ 的实例
        char param[300];
        char jcopExe[260];
        
        strcpy (jcopExe, BIN_PATH);
        strcat (jcopExe, "jcop.exe");
        sprintf (param, "/k \"%s\"", jcopExe);
        // printf (param);

        // launch the jcop.exe simulator
        if(exec ("cmd.exe", param, 0, SW_SHOW) == 0) 
        {
            printf ("failed to lauch the jcop.exe.\n");
            exit (1);
        }
        
        sleep (600);        // wait for jcop.exe being ready
    }
}

start_jcop_simul ();
rd_open (RDR_JCOP, PROT_TX, 0);

#else
rd_open (-1, PROT_TX, 0);
#endif // USE_JCOP_SIMUL

rd_reset ();            // 复位卡片


////////////////////////////////////////////////////////////////////////////////
// 建立安全通道 
void do_auth ()
{
    select -AID a000000003000000;
    auth ();
}

////////////////////////////////////////////////////////////////////////////////
// 应用删除
void oath_del ()
{
    printf (" Remove the Calculator Applet from JCOP Card...\n\n");
    
    do_auth ();
    
    gpdelete -AID %AID_OATH_APP;
    gpdelete -AID %AID_OATH_PKG;
    
    get_status -element 40;
}

////////////////////////////////////////////////////////////////////////////////
// 应用下载、安装
void oath_inst ()
{
    printf (" Install the Calculator Applet to JCOP Card...\n\n");
    
    do_auth ();
    install -file ../res/oath.cap -sdAID a000000003000000 -priv 2;
    
    get_status -element 40;
    
    select -AID %AID_OATH_APP;
}

oath_inst ();
//oath_del();

do_auth ();
select -AID %AID_OATH_APP;

// SET new OATH counter value
$ 0010 0000 08 00000000 00000000;

// GET current OATH counter value 
$ 0011 0000 00;

// SET the number of digits for the OTP
int otpLen = 8;
$ 0012 %02otpLen 00 14 3132333435363738393031323334353637383930;
//      ^ - 8: 8 位 OTP，6: 6 位 OTP

int i = 0;
int otp = 0;
int r_otp6[10] = {755224, 287082, 359152, 969429, 338314, 254676, 287922, 162583, 399871, 520489};
int r_otp8[10] = {84755224, 94287082, 37359152, 26969429, 40338314, 68254676, 18287922, 82162583, 73399871, 45520489};

for ( i = 0; i < 10; i++) 
{
    $ 0010 0000 08 %016i;       // SET new OATH counter value
    $ 0013 0000 00;             // Gen HOTP
    
    if ( otpLen == 6 ) 
        printf (" card OTP: %s, exp: %06X\n", _rapdu, r_otp6[i]);
    else
        printf (" card OTP: %s, exp: %08X\n", _rapdu, r_otp8[i]);
    
}


/* --------------------------------------------- end-of-script ---------------*/


/*
        Appendix D - HOTP Algorithm: Test Values
    ------------------------------------------------
          http://www.ietf.org/rfc/rfc4226.txt

   The following test data uses the ASCII string
   "12345678901234567890" for the secret:

   Secret = 0x3132333435363738393031323334353637383930

   Table 1 details for each count, the intermediate HMAC value.

   Count    Hexadecimal HMAC-SHA-1(secret, count)
   0        cc93cf18508d94934c64b65d8ba7667fb7cde4b0
   1        75a48a19d4cbe100644e8ac1397eea747a2d33ab
   2        0bacb7fa082fef30782211938bc1c5e70416ff44
   3        66c28227d03a2d5529262ff016a1e6ef76557ece
   4        a904c900a64b35909874b33e61c5938a8e15ed1c
   5        a37e783d7b7233c083d4f62926c7a25f238d0316
   6        bc9cd28561042c83f219324d3c607256c03272ae
   7        a4fb960c0bc06e1eabb804e5b397cdc4b45596fa
   8        1b3c89f65e6c9e883012052823443f048b4332db
   9        1637409809a679dc698207310c8c7fc07290d9e5

   Table 2 details for each count the truncated values (both in
   hexadecimal and decimal) and then the HOTP value.

                     Truncated
   Count    Hexadecimal    Decimal        HOTP
   0        4c93cf18       1284755224     755224
   1        41397eea       1094287082     287082
   2         82fef30        137359152     359152
   3        66ef7655       1726969429     969429
   4        61c5938a       1640338314     338314
   5        33c083d4        868254676     254676
   6        7256c032       1918287922     287922
   7         4e5b397         82162583     162583
   8        2823443f        673399871     399871
   9        2679dc69        645520489     520489

 */

