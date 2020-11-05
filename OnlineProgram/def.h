/*###########################################################################*/
/*#                                                                         #*/
/*# ���W���[���� : LU                                                       #*/
/*#                                                                         #*/
/*#     ���{�ꖼ : LU���ʃf�t�@�C����`�t�@�C��REL                          #*/
/*#     �Q�Ɣԍ� : -                                                        #*/
/*#     �P�ʋ敪 : -                                                        #*/
/*#     �T    �v :                                                          #*/
/*#     ��    �� : 99/01/28 - �ђ�  �Ɩ�                                    #*/
/*#     �X    �V : 08/12/05 - T.Kitazawa                                    #*/
/*#                                                                         #*/
/*###########################################################################*/
//#include "../../mik/mik_armc.h"

/*---------------------------------------------------------------------------*/
/*       �쐬�t�@�C����ʗp                                                  */
/*---------------------------------------------------------------------------*/

/*--- �f�o�b�N�^�����Őؑ֕K�v�Ȓ�` ---*/

#ifndef V1_DEBUG                                                                                //makefile��
//#define   V1_DEBUG                                                                                //makefile��
#endif
//#define   V2_1_DBG                    /*# �f�o�b�Ndefine���X�C�b�` #*/    //V2.1LU�t�@�[���ǉ�    //makefile��

#ifdef      V1_DEBUG            /* �f�o�b�N����`�i�����ł͖���`�j */

//#define       NO_BOOTP_USE        /*# IP=10.11.1.(2+(BYTE)MAC)�ɂ��� #*///makefile�ɂđ���

//#define           LED_INT_TEST        // �H�ꎎ���p�k�d�c�L��     99.12.3
//#define           CLKD_SYNC_CHK       // �b�k�j�c�������킹��Ԃ��A���[�������v�ŕ\���iLED_INT_TEST����`����j   99.12.3
//#define           SYNC_PHS_CHK        // �t�F�[�Y�ύX�ʒm�p�P�b�g���o�i�n�[�h�f�o�b�O�p�j 2000.06.26
//#define           SYNC_CMP_DBG        /*# �������킹�����f�o�b�N�i����������`�ɂ���j #*/

#define         NON_HELCHK_TO       /*# ��`���b�b�t����̃w���X�`�F�b�N�s�D�n�Ď������i�������͏�ɂs�D�n�D�����j #*///2004.04 IP-CS debug

//#define       LU_TFTP_RETRY       //  �\�t�g�f�o�b�O����TFTP�𖳌����g���C����Ƃ��ɒ�`����
//#define       NON_MAC_ROM_CHECK   /*# �h�o�k�v���O�����`�F�b�N�L�����A�l�`�b�G���A��ΏۂƂ��Ȃ�����`�i�o�׎��ɖ���`�j #*/
//#define       FAULT_LED_INTER     /*# ��Q���k�d�c�C���^�[�L���i�����ł͂ɖ���`�ɂ���j #*/
//#define       WDT_ON              // �E�H�b�`�h�b�O�L���i�ʏ�̓��C�N�Œ�`���邪�f�o�b�O�����L���ɂ���ꍇ��`����j
//#define       WDT_STOP            // �E�H�b�`�h�b�O��~�i�v�c�s�L���ł��E�H�b�`�h�b�O���^���A�N�Z�X���Ȃ��B�o�׎�����`�Ƃ���j //2000.2.1�L���ɂ���
//#define       DBG_CACHE_ON        /*# �f�o�b�O���ł̖��߃L���b�V���L�� #*/
//#define       LU_DBG              /*# LU�f�o�b�N�t���O #*/
//#define       LU_DBG2             /*# DBG_SW���o�K�[�h #*/
//#define       LU_DBG3             /*# DBG_SW�s���� #*/
//#define       LU_DBG4             /*# DBG_SW���� #*/

//#define           EXEC_TIME_LED       // �������ԑ���p�i����������`�j
//#define           KM_RCV_TIME_LED     // �������ԑ���p�i����������`�j
//#define           MAC_RST_TIME_LED        // �������ԑ���p�i����������`�j
//#define           MAC_ISR_TIME_LED    // �������ԑ���p�i����������`�j
//#define       LED_CHK_NO0         /*# ���ԑ���LED�\���i�T���������j�L�� #*/
//#define       LED_CHK_NO1         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO2         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO3         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO4         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO5         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO6         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO7         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO8         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO9         /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO10        /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO11        /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO12        /*# ���ԑ���LED�\���L�� #*/
//#define       LED_CHK_NO98        /*# ���ԑ���LED�\���i�A�C�h���C���^�[�o���j�L�� #*/
//#define       LED_CHK_NO99        /*# ���ԑ���LED�\���i�x�[�X�j�L�� #*/

//#define       CLKD_DBG            // CLKD DBG
//#define       CLKD_LOG            // CLKD LOG���W���[�h�ڍהŁiCLKD_LOG2�Ɠ����ɐݒ肵�Ȃ����Ɓj
//#define       CLKD_LOG2           // CLKD LOG���W���[�h�ȗ��ŁiCLKD_LOG�Ɠ����ɐݒ肵�Ȃ����Ɓj
//#define       DPRAM_WT_TRC        /*# �c�o�q�`�l�������O�L�� #*/
//#define       DPRAM_RW_LOG            /*# �c�o�q�`�l�Ǐ����O�L�� #*/

//#define       OAK_EVT_DEBUG       // �n�`�j�C�x���g�ǂݎ�胍�O
//#define       OAK_ORD_DEBUG       // �n�`�j�ݒ�I�[�_���O
//#define           TIME_LOG_DEBUG      //�f�o�b�O���O�p�T�������ԏ��X�V
//#define       VOICE_SET_DEBUG     // �����ݒ�|�C���^���O
//#define       PB_NG_DEBUG         // �o�a��M�m�f�f�o�b�O���O�i�n�[�h�f�o�b�O�p�j
//#define       OAK_PB_DEBUG        // �n�`�j�o�a�������O�i�n�[�h�f�o�b�O�p�j
//#define       PB_ON_TIME_DBG      // �o�a�I���Ď����ԕύX�i�n�[�h�f�o�b�O�p�j
//#define       PB_EC_ON_DBG        // �o�a�Ď������G�R�L�����I���Ƃ���i�n�[�h�f�o�b�O�p�j

//#define       ECHO_CAN_STOP       // �G�R�[�L�����Z���펞��~�i�n�[�h�f�o�b�O�p�j�ʏ햢��`�Ƃ���
//#define LU_NUMSCN_DBG             // 2001.1.17 �k�t�ԍ��ʒm�X�L���������f�o�b�O�p�i�����ł͖���`�j
//#define       RATE_CPU_EXE        // �b�o�t�g�p�����肷��ꍇ�ɒ�`����j 99.11.17
#define FPGA_CHK_DBG    //�I�����C��DBG�p fpga�R���t�B�O���[�g�L�� IP-CS�Ή� 2004.04

#else       //V1_DEBUG          /* �f�o�b�N������`�i�����ł͒�`�j */

//#define       RATE_CPU_EXE        // �b�o�t�g�p�����肷��ꍇ�ɒ�`����j 99.11.17
#define         PROG_CHECK_EXE      /*# �v���O�����`�F�b�N��L���ɂ���i�f�o�b�O���͖���`�j #*/
#define         RAM_CHECK_EXE
#define         LU_PRG_CHECK        //  �_�E�����[�h�t�@�C���`�F�b�N���s���ꍇ�ɒ�`����i�����ł̎��͒�`���邱�Ɓj
//#define       LSW_START_WAIT      /*# �h�o�k�ŃR�A�X�C�b�`�k�r�v�̓���҂��^�C�~���O�L���i�����ł͒�`����j #*/
#define         CCU_HELCHK_START    // 2000.03.28 ������Ԃ���w���X�Ď����{�i�\�t�g��������m�F�p�j

#ifdef  MENTE
#define             NON_HELCHK_TO   /*# ��`���b�b�t����̃w���X�`�F�b�N�s�D�n�Ď������i�������͏�ɂs�D�n�D�����j #*/
#endif  //#ifdef    MENTE

#endif      //V1_DEBUG

#define     FAULT_LED_INTER     /*# ��Q���k�d�c�C���^�[�L���i�����ł͂ɖ���`�ɂ���j #*/

//#define           MAC_RESET_DBG       // �l�`�b���Z�b�g����f�o�b�O�p�i����������`�j
//#define           RTP_SND_DBG         // �q�s�o�p�P�b�g���������]���f�o�b�O�p�i����������`�j
//#define           RTP_SEQ_NG_DISP     // �q�s�o�p�P�b�g�����k�d�c�\���f�o�b�O�p�i����������`�j


/*--- �V�X�e���X�V�p�Œ��` ---*/

#ifdef      MAKE_IPL
//#ifdef            MAKE_IPL_ROM
#define             LU_TAG_BOOT         /*# LSW�����[�G�[�W�F���g��BOOTPreply�����p����Ȃ����Ƃ̑΍�i�����ł͒�`����j #*/
//#endif    //ifdef     MAKE_IPL_ROM
#endif  //ifdef     MAKE_IPL

//#define   TEST_TEST_TEST      // �H�ꎎ���I�[�_�L���i�H�ꎎ���p�̏ꍇ��`����j   99.10.26
#ifdef      TEST_TEST_TEST
#ifndef         NON_HELCHK_TO
#define             NON_HELCHK_TO           /*# ��`���b�b�t����̃w���X�`�F�b�N�s�D�n�Ď������i�������̓f�t�@�C�����Ȃ��j #*/
#endif          //NON_HELCHK_TO
#endif      //TEST_TEST_TEST

#define         V1_2_HARD           /*# �u�P�D�Q�n�[�h����p #*/    // 2000.05.11 �ǉ�
#define         REMAKE_PKG          /*# �č�n�[�h�����s�p #*/    // 99.10.15
#define         EMA_PORT_CHG        /*# ���ʐؑ�(EMA)�|�[�g�R�ɕύX�n�[�h���s�p #*/ // 99.12.17
#define         CCU_HELCHK_START    // 2000.03.28 ������Ԃ���w���X�Ď����{�i�\�t�g��������m�F�p�j
//#define       OLD_IPL_ROM         /*# ���łh�o�k�łl�`�b�A�h���X��0x1ff00�̎���`���� #*/ // 99.10.25

#define         LU_MACRO            /*# �}�N���L���t���O #*/    // ��ɒ�`�iPOOL�擾�m�f�ƂȂ�̂Łj
#define         LU_RTP_SEQ          /*# RTP���ݽ�����L���t���O #*/

#define         ALM_LMP     0x80    /*# ���ԑ���LED�`�k�l�����v #*/
#define         OP_LMP      0x40    /*# ���ԑ���LED�n�o�����v #*/

//�����ݒ��p�o�n�q�s�O�f�[�^�i�}�X�^�^�X���[�u���ʁj
//#define           ALM_LMP_ON  0x0042
//#define           ALM_LMP_OFF 0x00c2
#define         ALM_LMP_ON  0x004f  /*# �A���[�������v�n�m�A�o�o���Z�b�g�A�d�b�@�\�n�e�e #*/    //�h�o�k�ɂ͂d�b�֌W�Ȃ�������킵���Ȃ��悤��
#define         ALM_LMP_OFF 0x00cf  /*# �A���[�������v�n�e�e�A�o�o���Z�b�g�A�d�b�@�\�n�e�e #*/  //�h�o�k�ɂ͂d�b�֌W�Ȃ�������킵���Ȃ��悤��


#define         VO_FLOW             /*# �����t���[����L�� #*/
//#define       VO_FLOW_CNT     5   /*# �����t���[����L������臒l #*/
#define         VO_FLOW_STOP    5   /*# ������~����臒l #*/
#define         VO_FLOW_START   2   /*# �����J�n����臒l #*/
//#define           VO_SEQ_NG       16  /*# �V�[�P���X�m�f�͈̔͒l #*/
#define         VO_SEQ_NG       10  /*# �V�[�P���X�m�f�͈̔͒l #*/

/*# NON_DOEP_SEQCHK��NON_DOEP_SEQCHK_RETRY�͂ǂ��炩�P��L���ɂ��邱�� #*/
////#define     NON_DOEP_SEQCHK     /*# ��`���b�b�t�V�[�P���X�`�F�b�N���� #*/
#define     NON_DOEP_SEQCHK_RETRY   /*# ��`���b�b�t�C�x���g�����đ� #*/ // ���������f�t�@�C��

//#define       DSP_SEARCH_ORD

#ifndef     OK
#define         OK              0x00
#endif      //OK

#define     CLR                 0x00
/*
********  �ǉ��FV3-IPCS-001 2008.12.04  --- Minimam Delay Time ���ւ������ύX�ɔ���  *********
*/
#define		DELAY_TIME_FULL		0xFFFFFFFF      /* V03.03 �x�����ԍő�l               */

#define     NG                  0xFF
//#define     NULL                0x00
//#define       NULLP           (void*)(0)
#define     ON                  0x01
#define     OFF                 0x00
#define     CONT                0x02
#define     SYS_NG              0xFFFFFFFF

//���kLU�Ή��B2003.09.16
#define XC_ON   0x01
#define XC_OFF  0x00

#define XC_OK   0x00
#define XC_NG   0x01

/* �m�t�k�k��` */
#define         XC_NULLP        0          /* �m�t�k�k�|�C���^ */
#define         XC_NULL         0          /* �m�t�k�k */

#define     NON                 0xFF

typedef     unsigned char       BYTE;   /*# BYTE  = 8bit #*/
typedef     unsigned short      WORD;   /*# WORD  = 16bit #*/
typedef     unsigned long       DWORD;  /*# DWORD = 32bit #*/

/* ���kLU */
#ifndef LONG
#define LONG       long            /* 32�r�b�g�����L�萮��
                                                     -2147483648�`2147483647 */
#endif

#ifndef  ULONG
#define  ULONG     unsigned long   /* 32�r�b�g������������ 0�`4294967295 */
#endif
#ifndef  USHORT
#define  USHORT    unsigned short  /* 16�r�b�g������������ 0�`65535 */
#endif
#ifndef  UCHAR
#define  UCHAR     unsigned char   /* 8�r�b�g������������ 0�`255 */
#endif
#ifndef  CHAR
#define  CHAR      char            /* 8�r�b�g�����L�萮�� -128�`127 */
#endif

#ifndef  SHORT
#define  SHORT    short  /* 16�r�b�g������������ 0�`65535 */
#endif

#ifndef  INT
#define  INT       int             /* 32�r�b�g�����L�萮��
                                                     -2147483648�`2147483647 */
#endif
#ifndef  UINT
#define  UINT      unsigned int    /* 32�r�b�g������������ 0�`4294967295 */
#endif
#ifndef  BOOL
#define  BOOL      int             /* 32�r�b�g������������ 0�`4294967295 */
#endif
/* ���kLU */


// �r�k�b�o�O�΍�̂��߂r�k�b�p�b�P�[�W�F���p 2000.01.18 �ǉ�
#define     SLC_PKG_ID      0x12

/*---  V2.1LU�t�@�[���ǉ��iSTART�j  ---*/

//#define       V2_EC_FORCE_CUT                 /*# �G�R�[�L�����Z���ݒ苭���n�e�e #*/                              //2001.02.12 V2.2�ǉ�

#ifdef      MAKE_IPL
#define     V2_1_IPL_EXCLUSIVE              /*# IPL����V2.1��p�� #*/                                           //2000.09 V2.1�ǉ�
#define     IPL_FLASH_NOWRITE               /*# IPL(FROM)�����ł�FLASH��������(�R�}���h�܂�)�}�~ #*/            //2000.12.05 V2.1�ǉ�
#endif

#define     V2_1_IPL_SLVNODELAY             /*# IPL�����X���[�u�Ȉ�Ăa�n�n�s #*/                                           //2000.09 V2.1�ǉ�

#define     V2_1_PHY_LINKUPSCAN             /*# PHY�̊��荞�ݔ����s�ǑΉ��i�����N�A�b�v�X�L�����������荞�݁j #*/

//#define       BOOT_SAVE_ONLINE            /*# �I�����C���t�@�C�����ła�n�n�s�����Z�[�u���� #*/              //2000.07 V2.1�ǉ�
#define         BOOT_MAKE_ONLINE            /*# �I�����C���t�@�C�����ła�n�n�s�����쐬���� #*/                //2000.11.28 V2.1DEBUG
//#define       V2_1_IPL_BEFORE             /*# �u�Q�D�P�ȑO�̂h�o�k�g�p�i�u�Q�D�P�h�o�k�J���ȍ~�͍폜�j #*/    //2000.07 V2.1�ǉ�
//#define       V2_1_DSP2CH_INI4CH          /*# �Q�b�g�c�r�o�łS�b�g�������� #*/

//#define       V2_1_DBG_2CHDSPSTART            /*# �]������ROM�p�F�Q�����c�r�o�X�^�[�g #*/
//#define       V2_1_DBG_4CHDSPFIX              /*# �]������ROM�p�F�S�����c�r�o�Œ� #*/

//#define           RFCD_TRACE

//������M�G���[�m�F�p  2001.03.12
#define RTP_SEQ_ERR_LOG
#define RTP_BUF_ERR_LOG

//#define   RTP_SEQ_ERR_TIME                    // �f�o�b�O�p   2001.04.26

//#define   SYSERR_PKT_SND                      /*# �V�X�G���[���A�}���`�L���X�g�p�P�b�g�ŏ�񑗐M�i�f�o�b�O�p�j #*/    // 2001.03.23 �ǉ�
//#define   MAC_RESET_LOG                       /*# �l�`�b�n���O���Z�b�g���O�i�ʏ햢��`�j #*/                          // 2001.04.16 �ǉ�
//#define   MAC_FIFO_LOG                        /*# �l�`�b�e�h�e�n���O�i�ʏ햢��`�j #*/                                // 2001.04.16 �ǉ�
//#define   KMAC_DBG_LOG                        /*# �l�`�b�f�o�b�O���O�i�ʏ햢��`�j #*/                                // 2001.06.13 �ǉ�


#ifdef      V2_1_DBG
#define     V2_DBG_POOL0_DET                /*# POOL0��͗p�\���̒�`���[�h #*/
//#define       V2_1_DBG_TFTPOVER
//#define       V2_1_DBG_IPADDR             /*# �f�o�b�N�p�F�}�V���h�o�A�h���X #*/
#define     V2_1_DBG_FANALM             /*# �f�o�b�N�p�F�t�@���A���[������ #*/
#define     V2_1_DBG_MACADDR            /*# �f�o�b�N�p�F�l�`�b�A�h���X #*/
//#define       V2_1_DBG_BUFFERCNT          /*# �f�o�b�N�p�F�o�b�t�@�ʎw�莞�Ԑ� #*/
//#define       V2_1_DBG_PGEINNFO           /*# �f�o�b�N�p�F�v�[���擾�G���[�ڍ׏����W #*/
//#define       V2_1_DBG_TIMEEXE            /*# �f�o�b�N�p�F�^�C�}���s���ԑ��� #*/
//#define       V2_1_DBG_TFTPSENDEXE            /*# �f�o�b�N�p�FTFTP�p�P�b�g���M���s���ԑ��� #*/
//#define       V2_1_DBG_DOEPEXE            /*# �f�o�b�N�p�F���s���ԑ��� #*/
//#define       V2_1_DBG_CRINTEXE           /*# �f�o�b�N�p�F���s���ԑ��� #*/
//#define       V2_1_DBG_FUSIONHEAPWAIT     /*# �f�o�b�N�p�FFUSION�q�[�v�擾�҂�(LED��ON���邱��) #*/
//#define       V2_1_DBG_CRINTCNT           /*# �f�o�b�N�p�FCRINT���s�� #*/
//#define       V2_1_DBG_CLKDCNT            /*# �f�o�b�N�p�FCRINT���s�� #*/

//#define       V2_1_DBG_INTPHY             /*# �f�o�b�N�p�F�o�g�x���荞�݋����R�[�� #*/
//#define       V2_1_DBG_INTMAC             /*# �f�o�b�N�p�F�l�`�b���荞�݋����R�[�� #*/
#ifdef  MAKE_IPL
//#define       V2_1_DBG_INTDSP             /*# �f�o�b�N�p�F�c�r�o���荞�݋����R�[�� #*/
#else
//#define           V2_1_DBG_INTDSP             /*# �f�o�b�N�p�F�c�r�o���荞�݋����R�[�� #*/
//#define       LSI_DOWN_CHECK                  /*#  #*/    //2001.02.26 V2.2�ǉ�
#endif
//#define           V2_1_DBG_SIOINTCHK          /*# �f�o�b�N�p�F�r�h�n���荞�݃`�F�b�N #*/
//#define       V2_1_DBG_LUNUM              /*# �f�o�b�N�p�F�k�t�ԍ������ݒ� #*/

//#define       V2_1_DBG_ECHOEX             /*# �f�o�b�N�p�F�d�b�g�n�L�����Z���{�[�h���� #*/
//#define       V2_1_DBG_DBGLED             /*# �f�o�b�N�p�F�c�a�f�|�k�d�c #*/
//#define       V2_1_DBG_IPL                /*# �f�o�b�N�p�F�h�o�k #*/
//#define       V2_1_DBG_SIONG              /*# �f�o�b�N�p�F�r�h�n�ʐM�m�f #*/
//#define       V2_1_DBG_UNISELFOK          /*# UNI CAST �����ւ̑��M�}�~��~ #*/
//#define       V2_1_DBG_LUMNGSYNCSND       /*# �f�o�b�N�p�FLUMNG SYNC SEND���� #*/
//#define       V2_1_DBG_UNIINTFULL         /*# �f�o�b�N�p�FLUMNG SYNC SEND���� #*/
//#define       V2_1_DBG_UNISELFSTOP        /*# �f�o�b�N�p�FUNI CAST �����ւ̑��M�}�~ #*/
//#define       V2_1_DBG_CRINTBZOFF         /*# �f�o�b�N�p�F�b�q�h�m�s�X�^�[�g���̃u�U�[�n�e�e #*/
//#define       V2_1_DBG_UNIFULL            /*# �f�o�b�N�p�F���j�L���X�g����e�t�k�k #*/
//#define       V2_1_DBG_TFTPSRVRTY         /*# �f�o�b�N�p�FTFTPSRV���M���g���C #*/
//#define       V2_1_DBG_TFTPSRV1PKTJ           /*# �f�o�b�N�p�FTFTPSRV�P�p�P�b�g512bytejust���M #*/
//#define       V2_1_DBG_TFTPSRV1PKTU           /*# �f�o�b�N�p�FTFTPSRV�P�p�P�b�g512byteunder���M #*/
//#define       V2_1_DBG_CRINTADDR          /*# �f�o�b�N�p�F�b�q�h�m�s�Ƃb�k�j�c�A�h���X�̐؂蕪����� #*/
//#define       V2_1_DBG_TFTPEX             /*# �f�o�b�N�p�F�s�e�s�o�p�P�b�g�O�o #*/
#endif

/*---  V2.1LU�t�@�[���ǉ��iEND�j  ---*/



/*###########################################################################*/
/*#                                                                         #*/
/*#                               �V�X�e���萔                              #*/
/*#                                                                         #*/
/*###########################################################################*/
#define     VO_1FRM_BYTE    40              /*# �����P�t���[���T�C�Y(Byte) #*/
#define     VO_1FRM_WORD    20              /*# �����P�t���[���T�C�Y(WORD) #*/
#define     VO_1FRM_DWORD   10              /*# �����P�t���[���T�C�Y(DWORD) #*/
#define     VO_RTP_FRM_MAX  38              /*# �q�s�o�����t���[����MAX(5ms�P��) #*/
//#define       VO_BUF_FRM_MAX  150             /*# ��M�o�b�t�@�����t���[����MAX(5ms�P��) #*/
//#define       VO_BUF_FRM_MAX  200             /*# ��M�o�b�t�@�����t���[����MAX(5ms�P��) 10ms100�� #*/    // 2000.01.16 �ύX
#define     VO_BUF_FRM_MAX  400             /*# ��M�o�b�t�@�����t���[����MAX(5ms�P��) 10ms200�� #*/    // 2000.01.16 �ύX      // 2000.09 V2.1�ύX

//���kLU�Ή��B����1�t���[���o�C�g���ǉ��B2003.09.07
#define G723L_1FRM_BYTE     20
#define G723H_1FRM_BYTE     24
#define G729_1FRM_BYTE      10
#define G711U_1FRM_BYTE     80
#define G711A_1FRM_BYTE     80



#define     VO_SLT_U        0xffffffff      /*# �����p�^�[���f�[�^�F���|������ #*/
#define     VO_SLT_A        0x55555555      /*# �����p�^�[���f�[�^�F�`�|������ #*/
#define     VO_SLTPTN_U     0               /*# �����p�^�[���w��F���|������ #*/
#define     VO_SLTPTN_A     1               /*# �����p�^�[���w��F�`�|������ #*/

#define     CODEC_U         0               /*# �b�n�c�d�b�F�ʁ|�������w�� #*/
#define     CODEC_A         1               /*# �b�n�c�d�b�F�`�|�������w�� #*/

//#define       CNMAX           8               /*# �ő�CN�� �i�u�P�́A�S�g�v�����j#*/
#define     CNMAX           4               /*# �ő�CN�� #*///2004.04 IP-CS�ύX
#define     CNMAX_IPCS          3               /*# IPCS���ő�CN�� #*///2004.10 SS9100 


#define     COM_NOMAL       0           /*# ���ϊ��]���iWORD�]�����Ɏg�p�j#*/
#define     COM_CHANGE      1           /*# �ϊ��]���iWORD�]�����Ɏg�p�j#*/

    /*# �^�X�N�l�`�w #*/
//#define       TCB_MAX         14          /*# �^�X�N(TCB)�� #*/
//#define       TCB_MAX         17          /*# �^�X�N(TCB)�� #*/   //2000.07 V2.1�ύX
//#define       TCB_MAX         18          /*# �^�X�N�� 0--17 #*/      // 2001.08 V2.3�ύX
//#define       TCB_MAX         38          /*# �^�X�N�� 0--37 #*/ /* ���kLU */
//#define       TCB_MAX         39          /*# �^�X�N�� 0--38 #*/ /* ���kLU */
//���kLU�Ή��BDSP�R�}���h�^�X�N2�ɂ����̂ŕύX�B2003.09.08
//#define       TCB_MAX         40              /*# �^�X�N�� 0--39 #*/ /* ���kLU */
#define     TCB_MAX         14              /*# �^�X�N�� 0--14 #*/ /* IP-CS *///2004.04

    /*# �v���C�I���e�B �l�`�w #*/
//#define       PRI_MAX         1           /*# �D��x�� #*/
//#define       PRI_MAX         2           /*# �D��x�� #*/    // 99.11.10
#define     PRI_MAX         1           /*# �D��x�� #*/    // 99.11.29
    /*# �d�b�a �l�`�w #*/
//#define       ECB_MAX         44          /*# �d�b�a�� #*/
//#define       ECB_MAX         47          /*# �d�b�a�� #*/        //2000.07 V2.1�ύX
//#define       ECB_MAX         49              /*# �d�b�a�� #*/        //2001.02.08 V2.2�ύXforMENTE
//#define       ECB_MAX         50              /*# �d�b�a��  0--49 #*/     //2001.08.08 V2.3�ύX forLULOG
//#define       ECB_MAX         70              /*# �d�b�a��  0--69 #*/ /* ���kLU */
//#define       ECB_MAX         72              /*# �d�b�a��  0--71 #*/ /* ���kLU */

//���kLU�Ή��BDSP�R�}���h�^�X�N2�ɂ����̂ŕύX�B2003.09.08
//#define       ECB_MAX         73              /*# �d�b�a��  0--72 #*/ /* ���kLU */
#define     ECB_MAX         97              /*# �d�b�a��  0--96 #*/ /* ���kLU *///2003.10 V3.1


    /*# �k�n�f �l�`�w #*/
#define     MIK_LOG_MAX     256         /*# �l�h�j���O�ʐ� #*/
    /*# �v�[���l�`�w #*/
//#define       POOL_MAX        16          /*# �v�[���h�c��(V1) #*/
//#define       POOL_MAX        25          /*# �v�[���h�c��(V1) #*///2000.07 V2.1�ύX
//#define       POOL_MAX        28          /*# �v�[���h�c�� #*/ /* ���kLU */

//���kLU�Ή��BDSP���A���^�C���R�}���h�p�ǉ������̂ŕύX�B2003.09.08
#define     POOL_MAX        32          /*# �v�[���h�c�� 0--31 #*/ /* ���kLU */

    /*# �^�X�N�p�X�^�b�N�T�C�Y��` #*/
#define     sup_Stack_size          2048        /*# sup�X�^�b�N�T�C�Y #*/
#define     dpisr_main_Stack_size   2048
#define     lumng_Stack_size        2048
#define     doep_Stack_size         2048
#define     h323_Stack_size         2048
#define     clkd_Stack_size         2048
#define     crintctl_Stack_size     2048
#define     tftp_Stack_size         2048
#define     tmr_Stack_size          2048
//#define       door_main_Stack_size    2048//IP-CS�Ή��폜 2004.04
#define     dspmng_Stack_size       2048
#define     fusion_rcv_Stack_size   2048        /*# fusion_rcv�X�^�b�N�T�C�Y #*/
#define     fusion_tmr_Stack_size   2048        /*# fusion_tmr�X�^�b�N�T�C�Y #*/
//#define       almmng_Stack_size       2048        /*# (V1) #*///IP-CS�Ή��폜 2004.04
#define     ipl_Stack_size          2048        /*# IPL�p(V1) #*/
//#define       bootpsrv_Stack_size     2048        /*# BOOTP�T�[�o�X�^�b�N�T�C�Y #*/   //2000.07 V2.1�ǉ�//IP-CS�Ή��폜 2004.04
//#define       tftpsrv_Stack_size      2048        /*# TFTP�T�[�o�X�^�b�N�T�C�Y #*/    //2000.07 V2.1�ǉ�//IP-CS�Ή��폜 2004.04
//#define       sioctl_Stack_size       2048        /*# SIO����X�^�b�N�T�C�Y #*/       //2000.07 V2.1�ǉ�//IP-CS�Ή��폜 2004.04
#define     mnt_Stack_size          2048        /*# �ێ�ݒ�X�^�b�N�T�C�Y #*/      //2001.02.07 V2.2�ǉ�
#define     lu_log_Stack_size       2048        /*# �k�t���O�X�^�b�N�T�C�Y #*/      //2001.08 V2.3�ǉ�
/* ���kLU */
//#define       fcl_mng_Stack_size      2048        /*# FAX�Ǘ��X�^�b�N�T�C�Y #*///IP-CS FAX�Ή��폜 2004.04
//#define       fcl_t30_Stack_size      2048        /*# T30 �X�^�b�N�T�C�Y #*/
//#define       fcl_t38_Stack_size      2048        /*# T38 �X�^�b�N�T�C�Y #*/
//#define       fcl_mdm_Stack_size      2048        /*# MDM �X�^�b�N�T�C�Y #*/
//#define       fcl_acc1_Stack_size     2048        /*# ACC1�X�^�b�N�T�C�Y #*/
//#define       fcl_acc2_Stack_size     2048        /*# ACC2�X�^�b�N�T�C�Y #*/
//#define       fcl_acc3_Stack_size     2048        /*# ACC3�X�^�b�N�T�C�Y #*/
//#define       fcl_acc4_Stack_size     2048        /*# ACC4�X�^�b�N�T�C�Y #*/
//#define       fcl_acc5_Stack_size     2048        /*# ACC5�X�^�b�N�T�C�Y #*/
//#define       fcl_acc6_Stack_size     2048        /*# ACC6�X�^�b�N�T�C�Y #*/
//#define       fcl_acc7_Stack_size     2048        /*# ACC7�X�^�b�N�T�C�Y #*/
//#define       fcl_acc8_Stack_size     2048        /*# ACC8�X�^�b�N�T�C�Y #*/
//#define       fcl_con1_Stack_size     2048        /*# CON1�X�^�b�N�T�C�Y #*/
//#define       fcl_con2_Stack_size     2048        /*# CON2�X�^�b�N�T�C�Y #*/
//#define       fcl_con3_Stack_size     2048        /*# CON3�X�^�b�N�T�C�Y #*/
//#define       fcl_con4_Stack_size     2048        /*# CON4�X�^�b�N�T�C�Y #*/
//#define       fcl_con5_Stack_size     2048        /*# CON5�X�^�b�N�T�C�Y #*/
//#define       fcl_con6_Stack_size     2048        /*# CON6�X�^�b�N�T�C�Y #*/
//#define       fcl_con7_Stack_size     2048        /*# CON7�X�^�b�N�T�C�Y #*/
//#define       fcl_con8_Stack_size     2048        /*# CON8�X�^�b�N�T�C�Y #*/

//���kLU�Ή� 2003.08.22
//#define       dspcommand_Stack_size   2048        /*# DSP�R�}���h�^�X�N�X�^�b�N�T�C�Y #*/
//���kLU�Ή� 2003.09.03
#define     dspcommand0_Stack_size  2048        /*# DSP�R�}���h�^�X�N0�X�^�b�N�T�C�Y #*/
//#define       dspcommand1_Stack_size  2048        /*# DSP�R�}���h�^�X�N1�X�^�b�N�T�C�Y #*///IP-CS�Ή��폜 2004.04


#ifdef  NO_BOOTP_USE
//#define   LU_IP                   0x02010b0a
//#define   CCU_IP                  0x01010b0a
//#define   DEFAULT_MASK            0x00ffffff
//#define   LU_IP                   0x0a0b0102      // 10.11.1.2
//#define   CCU_IP                  0x0a0b0101      // 10.11.1.1
#define     LU_IP                   0xc0a80164      // 192.168.1.100
//#define   LU_IP                   0xc0a8010a      // 192.168.1.10
//#define   CCU_IP                  0xc0a801dd      // 192.168.1.221
//#define   CCU_IP                  0xc0a801de      // 192.168.1.222
#define     CCU_IP                  0xc0a801e0      // 192.168.1.222
#define     DEFAULT_MASK            0xffffff00
//#define   CCU_IP1                 0xde01a8c0      // 192.168.1.222
//#define   CCU_IP2                 0xde01a8c0      // 192.168.1.222
#define     CCU_IP1                 0xe001a8c0      // 192.168.1.222
#define     CCU_IP2                 0xe101a8c0      // 192.168.1.222
#endif  // NO_BOOTP_USE

//#define           DOEP_CCU_PORT       49200
#define         DOEP_CCU_PORT       0x30c0          // 49200
//#define           DOEP_MY_PORT        49200
#define         DOEP_MY_PORT        0x30c0          // 49200

#define         LU_LOG_PORT     0x32c0          // 49202

/*# �}�W�b�N�N�b�L�[#*/
#define BOOTP_MAGIC     0x63825363
#define BOOTP_MAGIC_LE  0x63538263

/*# ����ʁi�u�P�j #*/
#define         LU_JAPAN        0x00
#define         LU_USA          0x01

//#define           LU_NUM_MAX      96              /*# �L���k�t�ԍ��l�`�w #*/
#define         LU_NUM_MAX      255             /*# �L���k�t�ԍ��l�`�w #*///2004.01 LU32B

#define         LU_NUM_DIAL_MAX 98              //2004.01 LU32B

#define     FAN_MSK 0x04                                    /*# ��Q���ڼ޽� bit2Ͻ��p  #*/
//#define       POW_MSK 0x18                                    /*# ��Q���ڼ޽� bit3,4Ͻ��p  #*/
//#define       UPS_MSK 0x60                                    /*# ��Q���ڼ޽� bit5,6Ͻ��p  #*/
#define     AC_MSK      0x18                                /*# ��Q��񃌃W�X�^ bit3,4�}�X�N�p  #*/
#define     AC_OK       0x18                                /*# �`�b���� #*/
#define     AC_OFF      0x00                                /*# ��d�`�b�I�t #*/
#define     AC_FAIL0    0x10                                /*# �O�n�ُ� #*/
#define     AC_FAIL1    0x08                                /*# �P�n�ُ� #*/

#define     EMA_OK      0x0001                              /*# �ʏ�i�^�p���j #*/
#define     EMA_NG      0x0000                              /*# ��Q�i���ʐؑցj #*/

// �n�[�h�E�F�A�o�[�W����
//#define       E_V1_2_HARD 0x13                                /*# V1.2�l+1�iV1.1�n�[�h��0xff�Ɍ����邽��+1����0�Ƃ���j #*/   // 2000.05.12 V1.2�ǉ�
//#define       E_V2_1_HARD 0x22                                /*# V2.1�l+1�iV1.1�n�[�h��0xff�Ɍ����邽��+1����0�Ƃ���j #*/   // 2000.07 V2.1�ǉ�
#define     E_V1_1_HARD 0x11                                /*# V1.1�l�iV1.1�n�[�h��0xff�Ɍ����邪�����ŋ����ϊ�����j #*/      // 2000.09 V2.1�ǉ�
#define     E_V1_2_HARD 0x12                                /*# V1.2�l #*/                                                      // 2000.09 V1.2�ύX
#define     E_V2_1_HARD 0x21                                /*# V2.1�l #*/                                                      // 2000.09 V2.1�ύX

#define     E_V1_0_KAPPA    0x0000                          /*# �Ȃu�P #*/  //2000.09 V2.1�ǉ�
#define     E_V2_0_KAPPA    0x0100                          /*# �Ȃu�Q #*/  //2000.09 V2.1�ǉ�

// �b�b�t�o�[�W����
//#define       E_V1_1_CCU  0x0101                              /*# �_�~�[�i���g�p�j#*/ // 2000.07 V2.1�ǉ�
//#define       E_V1_2_CCU  0x0102                              /*# �_�~�[�i���g�p�j#*/ // 2000.07 V2.1�ǉ�
//#define       E_V2_1_CCU  0x0201                              /*# CCU�o�[�W�����ԍ� #*/   // 2000.07 V2.1�ǉ�
#define     E_V2_1_CCU  0x3231                              /*# CCU�o�[�W�����ԍ� #*/   // 2000.07 V2.1�ǉ�     2001.08 �ύX
#define     E_V2_2_CCU  0x3232                              /*# CCU�o�[�W�����ԍ� #*/
#define     E_V2_3_CCU  0x3233                              /*# CCU�o�[�W�����ԍ� #*/

//  �������������` V1.2�ǉ�
#define     E_SYN_STOP  0                                   /*# �������䖢�m�� #*/  // 2000.05.22 V1.2�ǉ�
#define     E_MST_V1_1  1                                   /*# �}�X�^�k�t V1.1 #*/ // 2000.05.11 V1.2�ǉ�
#define     E_MST_V1_2  2                                   /*# �}�X�^�k�t V1.2 #*/ // 2000.05.11 V1.2�ǉ�
#define     E_BETUSEN1  3                                   /*# �ʐ������P #*/      // 2000.05.11 V1.2�ǉ�
#define     E_DOUKI1    4                                   /*# ��������P #*/      // 2000.05.11 V1.2�ǉ�
#define     E_PACKET1   5                                   /*# �p�P�b�g�����P #*/  // 2000.05.11 V1.2�ǉ�
#define     E_BETUSEN2  6                                   /*# �ʐ������Q #*/      // 2000.05.11 V1.2�ǉ�
#define     E_DOUKI2    7                                   /*# ��������Q #*/      // 2000.05.11 V1.2�ǉ�
#define     E_PACKET2   8                                   /*# �p�P�b�g�����Q #*/  // 2000.10.19 V2.1�ǉ�

// �e�k�`�r�g�f�[�^��� 2000.09 V2.1�ǉ�
enum {
    FLS_DATID_IPL,                  /*# �h�o�k�i�g�p�s�j #*/
    FLS_DATID_CMD,                  /*# �R�}���h���s #*/
    FLS_DATID_DAT,                  /*# �Ǘ��f�[�^ #*/
    FLS_DATID_HORY,                 /*# �ۗ��� #*/
    FLS_DATID_TALKY,                /*# �g�[�L�[ #*/
    FLS_DATID_SYS,                  /*# �I�����C���V�X�e���t�@�C�� #*/
    FLS_DATID_DSP,                  /*# �c�r�o�v���O���� #*/
    FLS_DATID_V3DSP,                /*# �O�t���c�r�o�v���O���� #*///2003.8�ǉ�
    FLS_DATID_CADATA,               /*# CA�؂�ւ����FIP-CS(SS9100_PH1�ĊJ) �ǉ� #*/
    FLS_DATID_ETC,                  /*# ���̑� #*/
    FLS_DATID_MAX                   /*# �f�[�^��ʍő�� #*/
};

/*# �r�h�n�ʐM�����f�[�^�t�H�[�}�b�g #*/
enum{
    SIOCTL_IN_VER,                  /*# �r�h�n�ʒm�o�[�W�����iRESERVE�j #*/     //LU32B
    SIOCTL_IN_LUNUM_H,              /*# �k�t�ԍ���� #*/    //LU32B
    SIOCTL_IN_LUNUM,                /*# �k�t�ԍ� #*/
    SIOCTL_IN_VLAN,                 /*# �u�k�`�m�|�h�c#*/
    SIOCTL_IN_CCUID,                /*# �b�b�t�|�h�c #*/
    SIOCTL_IN_DIPSW,                /*# �c�h�o�r�v��� #*/  //LU32B
    SIOCTL_IN_EXT,                  /*# �g����� #*/    //LU32B
    SIOCTL_IN_SUV,                  /*# �T�o�C�o����� #*/  //LU32B 
    SIOCTL_IN_DATAMAX               /*# �f�[�^���ő�l #*/
};
#define SIOCTL_IN_BUFMAX    1024

/*# �r�h�n�����ʐM�f�[�^�t�H�[�}�b�g #*/
enum{
    SIOCTL_OUT_STX,                 /*# �r�s�w #*/
    SIOCTL_OUT_VER_10,              /*# �r�h�n�ʒm�o�[�W�����i�P�O�̈ʁj�iRESERVE�j #*/     //LU32B
    SIOCTL_OUT_VER_1,               /*# �r�h�n�ʒm�o�[�W�����i�P�̈ʁj�iRESERVE�j #*/       //LU32B
    SIOCTL_OUT_LUNUM_1000,          /*# �k�t�ԍ��i�P�O�O�O�̈ʁj #*/        //LU32B
    SIOCTL_OUT_LUNUM_100,           /*# �k�t�ԍ��i�P�O�O�̈ʁj #*/          //LU32B
    SIOCTL_OUT_LUNUM_10,            /*# �k�t�ԍ��i�P�O�̈ʁj #*/
    SIOCTL_OUT_LUNUM_1,             /*# �k�t�ԍ��i�P�̈ʁj #*/
    SIOCTL_OUT_VLAN_10,             /*# �u�k�`�m�|�h�c�i�P�O�̈ʁj#*/
    SIOCTL_OUT_VLAN_1,              /*# �u�k�`�m�|�h�c�i�P�̈ʁj #*/
    SIOCTL_OUT_CCUID_10,            /*# �b�b�t�|�h�c�i�P�O�̈ʁj #*/
    SIOCTL_OUT_CCUID_1,             /*# �b�b�t�|�h�c�i�P�̈ʁj #*/
    SIOCTL_OUT_DIPSW_10,            /*# �c�h�o�r�v���i�P�O�̈ʁj #*/  //LU32B
    SIOCTL_OUT_DIPSW_1,             /*# �c�h�o�r�v���i�P�̈ʁj #*/    //LU32B
    
    SIOCTL_OUT_EXT_10,              /*# �g�����i�P�O�̈ʁj #*///LU32B 
    SIOCTL_OUT_EXT_1,               /*# �g�����i�P�̈ʁj #*///LU32B
    SIOCTL_OUT_SUV_10,              /*# �T�o�C�o������i�P�O�̈ʁj #*///LU32B   
    SIOCTL_OUT_SUV_1,               /*# �T�o�C�o������i�P�̈ʁj #*///LU32B 
    
    SIOCTL_OUT_SUM0,                /*# �T���l�F�O�o�C�g�� #*/
    SIOCTL_OUT_SUM1,                /*# �T���l�F�P�o�C�g�� #*/
    SIOCTL_OUT_ETX,                 /*# �d�s�w #*/
    SIOCTL_OUT_DATAMAX              /*# �f�[�^���ő�l #*/
};
#define SIOCTL_OUT_BUFMAX   (SIOCTL_IN_BUFMAX*2)

#define     EXT_D01     0x0001  //2004.01 LU32B

//SS9100 2004.11.12 EXT_IPstage�͍폜���Ĉȍ~�߂�
//#define       EXT_IPstage     0x0002  //2004.04 IP-CS
#define     EXT_SS9100      0x0002      //0x0004    //2004.08 SS9100
#define     EXT_OTHERS      0x0004      //0x0008    //2004.08 SS9100

// CA�؂�ւ����f�t�H���g�l�FIP-CS(SS9100_PH1�ĊJ) �ǉ�
#define     CA_DATA_SERV_ID         0x00            // �ڑ��掯�ʎq(�v���C�}��:0)
#define     CA_DATA_SURV_CHG_TIME   0x02            // �T�o�C�o���؂�ւ��^�C�}(120�b:2��)
#define     CA_DATA_SURV_RET_TIME   0x00            // �T�o�C�o���؂�߂��^�C�}(0�b)
#define     CA_DATA_CONNECT_TIME    0x00            // �R�l�N�V�����Đڑ��^�C�}(0�b)


/*###########################################################################*/
/*#                                                                         #*/
/*#                           ���b�Z�[�W�֘A                                #*/
/*#                                                                         #*/
/*###########################################################################*/
#ifdef  MAKE_IPL
#define     SUP_ID          0x00        /*# �T�|�[�g�^�X�N #*/
#define     DRIVER_ID       0x01        /*# �f�o�C�X�h���C�o�[��M�^�X�N #*/
#define     FUSION_ID       0x02        /*# FUSION�^�C�}�^�X�N #*/
#define     IPL_ID          0x03        /*# IPL�^�X�N #*/
//#define       SIOCTL_ID       0x04        /*# �r�h�n���� #*/      // 2000.07 V2.1//IP-CS�폜2004.04
#define     FLASH_ID        0x05        /*# �e�k�`�r�g���� #*/  // 2000.09 V2.1�ǉ�
#else//�I�����C��
#ifdef  MENTE
#define     SUP_ID          0x00        /*# �T�|�[�g�^�X�N #*/
#define     LUMNG_ID        0x01        /*# �k�t�Ǘ��^�X�N #*/
#define     DOEP_ID         0x02        /*# �c�n�d�o���� #*/
#define     TMR_ID          0x03        /*# �^�C�}�^�X�N #*/
#define     DRIVER_ID       0x04        /*# �f�o�C�X�h���C�o�[��M�^�X�N #*/
#define     FUSION_ID       0x05        /*# FUSION�^�C�}�^�X�N #*/
#define     MNT_ID          0x06        /*# �ێ�ݒ萧�� #*/    // 2001.02.07 V2.2�ǉ�
#define     DSPINT_ID       0x07        /*# �c�r�o���荞�� #*/      //2000.07.13 V2.1DEBUG
#define     FLASH_ID        0x08        /*# �e�k�`�r�g���� #*/  // 2000.09 V2.1�ǉ�

#define     COM_ID          0x80        /*# ���ʊ֐�(�^�X�N�ł͂Ȃ��I�I) #*/
#else   //#ifdef    MENTE
    /*# �^�X�NID(�^�X�N�A�^�b�`�p,���̑��^�X�N���ʗp) #*/
#define     SUP_ID          0x00        /*# �T�|�[�g�^�X�N #*/
#define     DPISR_ID        0x01        /*# �c�o�q�`�l����  #*/
#define     LUMNG_ID        0x02        /*# �k�t�Ǘ��^�X�N #*/
#define     DOEP_ID         0x03        /*# �c�n�d�o���� #*/
#define     H323_ID         0x04        /*# �g�R�Q�R���� #*/
#define     CLKD_ID         0x05        /*# �b�k�j�c���� #*/
#define     CRINTCTL_ID     0x06        /*# �b�q�h�m�s���� #*/
#define     TFTP_ID         0x07        /*# �s�e�s�o���� #*/
#define     TMR_ID          0x08        /*# �^�C�}�^�X�N #*/
//#define       DOOR_ID         0x09        /*# �h�A�z������ #*///IP-CS�폜2004.04
#define     DSPMNG_ID       0x09        /*# �c�r�o��ԊǗ� #*/
#define     DRIVER_ID       0x0A        /*# �f�o�C�X�h���C�o�[��M�^�X�N #*/
#define     FUSION_ID       0x0B        /*# FUSION�^�C�}�^�X�N #*/
//#define       ALMMNG_ID       0x0D        /*# �`�k�l�l�m�f #*///IP-CS�폜2004.04
//#define       BOOTPSRV_ID     0x0E        /*# �a�n�n�s�o�T�[�o #*/// 2000.07 V2.1�ǉ�//IP-CS�폜2004.04
//#define       TFTPSRV_ID      0x0F        /*# �s�e�s�o�T�[�o #*/  // 2000.07 V2.1�ǉ�//IP-CS�폜2004.04
//#define       SIOCTL_ID       0x10        /*# �r�h�n���� #*/      // 2000.07 V2.1�ǉ�//IP-CS�폜2004.04
#define     LULOG_ID        0x0C        /*# �k�t���O #*/        // 2001.08 V2.3�ǉ�
//
//#define       FAXMNG_ID       0x12        /*# FAX�Ǘ��^�X�N #*//* ���kLU *///IP-CS FAX�폜2004.04
//#define       T30_ID          0x13        /*# T30�^�X�N #*/
//#define       T38_ID          0x14        /*# T38�^�X�N #*/
//#define       MDM_ID          0x15        /*# MDM�^�X�N #*/
//#define       ACC1_ID         0x16        /*# accept(CH1)�^�X�N #*/
//#define       ACC2_ID         0x17        /*# accept(CH2)�^�X�N #*/
//#define       ACC3_ID         0x18        /*# accept(CH3)�^�X�N #*/
//#define       ACC4_ID         0x19        /*# accept(CH4)�^�X�N #*/
//#define       ACC5_ID         0x1A        /*# accept(CH5)�^�X�N #*/
//#define       ACC6_ID         0x1B        /*# accept(CH6)�^�X�N #*/
//#define       ACC7_ID         0x1C        /*# accept(CH7)�^�X�N #*/
//#define       ACC8_ID         0x1D        /*# accept(CH8)�^�X�N #*/
//#define       CON1_ID         0x1E        /*# connect(CH1)�^�X�N #*/
//#define       CON2_ID         0x1F        /*# connect(CH2)�^�X�N #*/
//#define       CON3_ID         0x20        /*# connect(CH3)�^�X�N #*/
//#define       CON4_ID         0x21        /*# connect(CH4)�^�X�N #*/
//#define       CON5_ID         0x22        /*# connect(CH5)�^�X�N #*/
//#define       CON6_ID         0x23        /*# connect(CH6)�^�X�N #*/
//#define       CON7_ID         0x24        /*# connect(CH7)�^�X�N #*/
//#define       CON8_ID         0x25        /*# connect(CH8)�^�X�N #*/
//���kLU�Ή��B�^�X�NID�ǉ� 2003.08.22
//���kLU�Ή��B�^�X�NID�ǉ� 2003.09.03

#define     DSPCOMMAND0_ID  0x0D        /*# DSP0�R�}���h�^�X�N #*/
//#define       DSPCOMMAND1_ID  0x0E        /*# DSP1�R�}���h�^�X�N #*/

#define     CRINT_ID        0x0E        /*# �b�q�h�m�s���荞�� #*/  //2000.07.13 V2.1DEBUG
#define     DSPINT_ID       0x0F        /*# �c�r�o���荞�� #*/      //2000.07.13 V2.1DEBUG
#define     DSPINT2_ID      0x10        /*# �c�r�o���荞��2 #*/ // 2003.08 V3.1�ǉ�
#define     FLASH_ID        0x11        /*# �e�k�`�r�g���� #*/  // 2000.09 V2.1�ǉ�


#define     COM_ID          0x80        /*# ���ʊ֐�(�^�X�N�ł͂Ȃ��I�I) #*/
#endif  //#ifdef    MENTE

#endif  // MAKE_IPL

    /*# �^�X�NECB(SND_MSG�p) #*/
#define     SUP_ECB         0x00        /*# �T�|�[�g�^�X�N #*/
#define     DPISR_ECB       0x01        /*# �c�o�q�`�l����  #*/
#define     LUMNG_ECB       0x02        /*# �k�t�Ǘ��^�X�N #*/
#define     DOEP_ECB        0x03        /*# �c�n�d�o���� #*/
#define     H323_ECB        0x04        /*# �g�R�Q�R���� #*/
#define     CLKD_ECB        0x05        /*# �b�k�j�c���� #*/
#define     CRINTCTL_ECB    0x06        /*# �b�q�h�m�s���� #*/
#define     TFTP_ECB        0x07        /*# �s�e�s�o���� #*/
#define     TMR_ECB         0x08        /*# �^�C�}�^�X�N #*/
#define     CRINT_ECB       0x09        /*# �b�q�h�m�s���荞�� #*/
#define     DOOR_ECB        0x0A        /*# �h�A�z������ #*/
#define     DSPMNG_ECB      0x0B        /*# �c�r�o��ԊǗ� #*/
#define     DSPINT_ECB      0x0C        /*# �c�r�o���荞�� #*/
#define     ALMMNG_ECB      0x0D        /*# �`�k�l�l�m�f #*/
#define     BOOTPSRV_ECB    0x0E        /*# �a�n�n�s�o�T�[�o #*/                // 2000.07 V2.1�ǉ�
#define     TFTPSRV_ECB     0x0F        /*# �s�e�s�o�T�[�o #*/                  // 2000.07 V2.1�ǉ�
#define     SIOCTL_ECB      0x10        /*# �r�h�n���� #*/                      // 2000.07 V2.1�ǉ�
//#define       DRIVER_ECB      0x0E        /*# �f�o�C�X�h���C�o�[��M�^�X�N #*/
#define     DRIVER_ECB      0x11        /*# �f�o�C�X�h���C�o�[��M�^�X�N #*/    // 2000.07 V2.1�ύX
//#define       FUSION_ECB1     0x0F        /*# FUSION�pECB1 #*/
#define     FUSION_ECB1     0x12        /*# FUSION�pECB1 #*/                    // 2000.07 V2.1�ύX
//#define       FUSION_ECB29    0x2F    /*# FUSION�pECB29�iRESERVE�����W�j #*/  // 2000.07 V2.1�ύX
//#define       FUSION_ECB53    0x47    /*# FUSION�pECB53�iRESERVE�����W�j #*/  // 2003.10 V3.1�ύX

#define     MNT_ECB         0x48        /*# �ێ�p�C���^�t�F�[�X #*/            // 2001.02.07 V2.2�ǉ�
#define     LULOG_ECB       0x49        /*# �k�t���O #*/                        // 2001.08.08 V2.3�ǉ�

#define     FAXMNG_ECB      0x4A        /*# FAX�Ǘ��^�X�N #*//* ���kLU */
#define     T30_ECB         0x4B        /*# T30�^�X�N #*/
#define     T38_ECB         0x4C        /*# T38�^�X�N #*/
#define     MDM_ECB         0x4D        /*# MDM�^�X�N #*/
#define     ACC1_ECB        0x4E        /*# accept(CH1)�^�X�N #*/
#define     ACC2_ECB        0x4F        /*# accept(CH2)�^�X�N #*/
#define     ACC3_ECB        0x50        /*# accept(CH3)�^�X�N #*/
#define     ACC4_ECB        0x51        /*# accept(CH4)�^�X�N #*/
#define     ACC5_ECB        0x52        /*# accept(CH5)�^�X�N #*/
#define     ACC6_ECB        0x53        /*# accept(CH6)�^�X�N #*/
#define     ACC7_ECB        0x54        /*# accept(CH7)�^�X�N #*/
#define     ACC8_ECB        0x55        /*# accept(CH8)�^�X�N #*/
#define     CON1_ECB        0x56        /*# connect(CH1)�^�X�N #*/
#define     CON2_ECB        0x57        /*# connect(CH2)�^�X�N #*/
#define     CON3_ECB        0x58        /*# connect(CH3)�^�X�N #*/
#define     CON4_ECB        0x59        /*# connect(CH4)�^�X�N #*/
#define     CON5_ECB        0x5A        /*# connect(CH5)�^�X�N #*/
#define     CON6_ECB        0x5B        /*# connect(CH6)�^�X�N #*/
#define     CON7_ECB        0x5C        /*# connect(CH7)�^�X�N #*/
#define     CON8_ECB        0x5D        /*# connect(CH8)�^�X�N #*/

//���kLU�Ή��BECB�ǉ�
#define     DSPCOMMAND0_ECB 0x5E        /*# DSP�R�}���h�^�X�N0 #*/
#define     DSPCOMMAND1_ECB 0x5F        /*# DSP�R�}���h�^�X�N1 #*/
#define     DSPINT2_ECB     0x60        /*# DSP�����ݏ���2 #*/



    /*# ���b�Z�[�W�敪 #*/
#define     IN_MSG          0x01        /*# �������b�Z�[�W #*/
#define     LU_ODR          0x02        /*# �k�t�I�[�_ #*/
#define     LU_EVT          0x03        /*# �k�t�C�x���g #*/
#define     TIM_OUT         0x04        /*# �^�C���A�E�g���b�Z�[�W #*/

    /*# ���b�Z�[�W��ʁF�������b�Z�[�W #*/

#define     I_VOSTART       0x01        /*# �����ʐM�X�^�[�g #*/
#define     I_VOSTOP        0x02        /*# �����ʐM�X�g�b�v #*/
#define     I_VOPAUSE       0x03        /*# �����ʐM�ꎞ��~ #*/
#define     I_VORESTART     0x04        /*# �����ʐM�ĊJ #*/
#define     I_VODISKREP     0x05        /*# �����ʐM�ؒf�ʒm #*/
#define     I_VOSTOPRSP     0x06        /*# �����ʐM�X�g�b�v���X�|���X #*/
#define     I_CRINT         0x07        /*# CRINT�ʒm #*/
#define     I_PLSEND        0x08        /*# �p���X���o���� #*/
#define     I_TONESTART     0x09        /*# �g�[�����o�J�n #*/
#define     I_TONESTOP      0x0A        /*# �g�[�����o��~ #*/
#define     I_PBSNDSTART    0x0B        /*# PB���o�J�n #*/
#define     I_PBSNDSTOP     0x0C        /*# PB���o��~ #*/
#define     I_BTDETSTART    0x0D        /*# BT�Ď��J�n #*/
#define     I_BTDETSTOP     0x0E        /*# BT�Ď���~ #*/
#define     I_DTDETSTART    0x0F        /*# DT�Ď��J�n #*/
#define     I_DTDETSTOP     0x10        /*# DT�Ď���~ #*/
#define     I_PBDETSTART    0x11        /*# PB�Ď��J�n #*/
#define     I_PBDETSTOP     0x12        /*# PB�Ď���~ #*/
#define     I_CHSWCHG       0x13        /*# CHSW�؂�ւ� #*/
#define     I_DSPCTL        0x14        /*# DSP���� #*/
#define     I_MUONPTNSET    0x15        /*# �����p�^�[���w�� #*/
#define     I_CODECCHG      0x16        /*# CODEC�؂�ւ� #*/
#define     I_PADCTL        0x17        /*# PAD/EC���� #*/

//���kLU�Ή��BDSP�nMSG�B2003.09.08
#define     I_DSPCMDSND             0x18    
#define     I_HPI_ECHO_REPLY        0x19    /* HPI�G�R�[������M */
#define     I_DSP_REALTIME_COMMAND  0x1a
#define     I_DSP_MODECHG           0x1b    /* DSP��FAX�ؑ֒ʒm */
#define     I_DSPSTART              0x1c    /* DSP�J�n�ʒm */
#define     I_PASSCHGEND            0x1d    /* �p�X�ؑ֊��� */
#define     I_DSPERROR              0x1e    /* �p�X�ؑ֊��� */
//2003.10.02�ǉ�
#define     I_DTMFCOMPLETE          0x1f    /* DTMF���o���� */


#define     I_HELCHKREQ     0x20        /*# �w���X�`�F�b�N�v�� #*/
#define     I_HELCHKRSP     0x21        /*# �w���X�`�F�b�N���� #*/
#define     I_LINKUP        0x22        /*# DOEP�����N���� #*/
#define     I_DOEPRCV       0x23        /*# DOEP��M #*/

#define     I_LINKUPCMP     0x40        /*# DOEP�����N���䌋�� #*/
#define     I_CLKDOBS       0x41        /*# CLKD��Q�ʒm�v�� #*/
#define     I_FANOBS        0x42        /*# FAN��Q�ʒm�v�� #*/
#define     I_SYNCMP        0x43        /*# CLKD�������킹�����ʒm�v�� #*/  // 2000.05.13 �ǉ�

//#define       I_DPRAMRD       0x50        /*# DPRAM�f�[�^�ǂݏo���v�� #*/
#define     I_PGDLCMP       0x51        /*# �v���O�����_�E�����[�h���� #*///2004.04 IP-CS �ǉ�

#define     I_CRINTSRT      0x60        /*# CRINT�N���v�� #*/

#define     I_CLKDSRT       0x70        /*# CLKD�N���v�� #*/

#define     I_VOICESND      0x80        /*# �������M�v�� #*/
#define     I_SDESRCV       0x81        /*# SDES��M #*/
#define     I_BYERCV        0x82        /*# BYE��M #*/

#define     I_BOOTPREQRCV   0x90        /*# BOOTPreq��M #*/    //2000.07 V2.1�ǉ�

#define     I_TFTPDATRCV    0xa0        /*# TFTP�f�[�^��M #*/  //2000.07 V2.1�ǉ�

#define     I_SIODATRCV     0xb0        /*# SIO�f�[�^��M #*/   //2000.07 V2.1�ǉ�

/* FAX�Ǘ��^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�`    */
#define     I_FAXTONE       0xc0        /*# FAX�g�[�����o #*/
#define     I_FAXEND        0xc1        /*# FAX�菇�I�� #*/
#define     I_T38STATUS     0xc2        /*# T38��Ԓʒm #*/
#define     I_FAXKILL       0xc3        /*# FAX������~�w�� #*/
#define     I_FAXTOVOICE        0xc4        /*# FAXtoVOICE�I�� #*/
#define     I_FAXPASS       0xc5        /*# FAX�p�X�ؒf�w�� #*/ //2003.11.10�ǉ�

/* T30�^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�`    */
#define     I_T30REQ        0xc8        /*# T30�����v�� #*/

/* MDM�^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�`    */
#define     I_FAXMDMREQ     0xcc        /*# fax���f�������v�� #*/

/* T38�^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�`    */
#define     I_T38REQ        0xd0        /*# T38�����v�� #*/
#define     I_ACCEPTRECV        0xd1        /*# T38�f�[�^��M(�A�N�Z�v�g) #*/
#define     I_CONNECTRECV       0xd2        /*# T38�f�[�^��M(�R�l�N�g) #*/
#define     I_UDPRECV       0xd3        /*# T38�f�[�^��M(UDP) #*/
#define     I_CONNECTRES        0xd4        /*# �R�l�N�g���ʒʒm #*/
#define     I_ACCEPTRES     0xd5        /*# �A�N�Z�v�g�ʒm #*/
#define     I_SENDERR       0xd6        /*# ���M�G���[�ʒm #*/

/* �A�N�Z�v�g�^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�` */
#define     I_ACCEPTREQ     0xd8        /*# �A�N�Z�v�g�w�� #*/

/* �R�l�N�g�^�X�N�ɂĎ�M���郁�b�Z�[�W�̒�`   */
#define     I_CONNECTREQ        0xd9        /*# �R�l�N�g�w�� #*/
#define     I_T38SENDREQ        0xda        /*# T38���M�w�� #*/

//SS9100(PH1�ĊJ)�Ή� 2006.06
#define     I_PH1ORDEND     0xe0        /*# PH1�ĊJ���I�[�_�I�� #*/

#define     I_LOGSTART      0xf0        /*# ���O�I�[�v�� #*/    // 2001.08 V2.3�ǉ�
#define     I_LOGCMD        0xf1        /*# ���O�R�}���h #*/    // 2001.08 V2.3�ǉ�
#define     I_LOGTRACE      0xf2        /*# ���O�g���[�X #*/    // 2003.09 �ǉ�


    /*# ���b�Z�[�W��ʁF�k�t�I�[�_ #*/
#define     O_DOWNLOAD      0x01        /*# �_�E�����[�h #*/
#define     O_DPRAMID       0x02        /*# DPRAM��ʃ_�E�����[�h #*/
#define     O_HORYDOWN      0x03        /*# �ۗ����f�[�^�_�E�����[�h�v�� #*/
#define     O_PKGINITREQ    0x04        /*# ��PKG�����ݒ�v�� #*/
#define     O_LUHELREQ      0x05        /*# LU�w���X�`�F�b�N�v�� #*/
#define     O_PKGHELREQ     0x06        /*# PKG�w���X�`�F�b�N�v�� #*/
#define     O_PASSCTLREQ    0x07        /*# �p�X����v�� #*/
#define     O_TONECTLREQ    0x08        /*# TONE����v�� #*/
#define     O_HORYCTLREQ    0x09        /*# �ۗ�������v�� #*/
#define     O_PBDETSTART    0x0A        /*# PB�M���Ď��J�n�v�� #*/
#define     O_PBDETSTOP     0x0B        /*# PB�M���Ď���~�v�� #*/
#define     O_BTDETSTART    0x0C        /*# BT�Ď��v�� #*/
#define     O_DTDETSTART    0x0D        /*# DT�Ď��v�� #*/
#define     O_BTDETSTOP     0x0E        /*# BT�Ď���~�v�� #*/
#define     O_DTDETSTOP     0x0F        /*# DT�Ď���~�v�� #*/
#define     O_PBSNDSTART    0x10        /*# PB���M�v�� #*/
#define     O_PBSNDSTOP     0x11        /*# PB�M�����M��~�v�� #*/
#define     O_DOORHACT      0x12        /*# �h�A�z������ #*/
#define     O_GHORYCTL      0x13        /*# �O���ۗ������� #*/
#define     O_BROADCTL      0x14        /*# �������u���� #*/
#define     O_MUONPTNSET    0x15        /*# �����p�^�[���w�� #*/
#define     O_DOORHSCNREQ   0x16        /*# �h�A�z���X�L�����v�� #*/
#define     O_PKGHELILG     0x17        /*# PKG�w���X�`�F�b�N�C���[�K��PKG��ʃ_�E�����[�h #*/
#define     O_MEMDMPREQ     0x18        /*# ������DUMP�v��#*/ // 99-5-18
#define     O_SYNCSNDCTL    0x19        /*# ����/CRINT�p�P�b�g���M���� #*/  //2000.07 V2.1�ǉ�
#define     O_SYNCRCVCTL    0x1a        /*# ����/CRINT�p�P�b�g��M���� #*/  //2000.07 V2.1�ǉ�
#define     O_DSPCHCTL      0x1b        /*# DSPch���� #*/                   //2000.08 V2.1�ǉ�

#define     O_FAXPASSCTLREQ     0x1C        /*# FAX�p�X����v�� #*//* ���kLU */
#define     O_T38LISTENREQ      0x1E        /*# T38Listen�v�� #*/
#define     O_FAXDOWNLOAD       0x1D        /*# FAX�_�E�����[�h #*/

#define     O_TEST_ORD_TYPE 0xf8        /*# �H�ꎎ���p���� #*/
#define     O_TST_DRAM_CHK  0xf8        /*# �H�ꎎ���p �c�q�`�l�`�F�b�N #*/
#define     O_TST_MEM_CHK   0xf9        /*# �H�ꎎ���p �������`�F�b�N #*/
#define     O_TST_LED_CTL   0xfa        /*# �H�ꎎ���p �k�d�c�\�� #*/
#define     O_TST_MEM_RD    0xfb        /*# �H�ꎎ���p ���������[�h #*/
#define     O_TST_MEM_WT    0xfc        /*# �H�ꎎ���p ���������C�g #*/
#define     O_TST_DSP_CK    0xfd        /*# �H�ꎎ���p �c�r�o�������`�F�b�N #*/ // 2001.05.22 �ǉ�
#define     O_TEST_ORD_TEND 0xff        /*# �H�ꎎ���p���ʏI�� #*/          //2000.07 V2.1�ǉ�

#define     O_FLASHTOOL     0x2F        /*# �t���b�V���c�[���v�� #*/        //2000.12.04 V2.1FLASH_TOOL

#define     O_BUFFVALUE     0x30        /*# �o�b�t�@�ʎw�� #*/              //2000.07 V2.1�ǉ�
#define     O_LUCHGBAK      0x31        /*# �؂�߂� #*/                //2004.02 LU32B�ǉ�
#define     O_OBSNTFYREQ    0x32        /*# ��Q���ʒm�v�� #*/        //2004.02 LU32B
#define     O_PGDOWNLOAD    0x33        /*# �v���O�����_�E�����[�h�v�� #*/      //2004.04 IP-CS
#define     O_AQACT         0x35        /*# �������ʒm�N��/��~�v�� #*/       //2004.10 SS9100

//SS9100(PH1�ĊJ)�Ή� 2006.06
#define     O_PH1ENDREQ    0x37         /*PH1�I���v��*/
#define     O_RESTARTREQ   0x38         /*�ĊJ�����v��*/

#define     O_PKGORDER     0x80         /*# ����PKG�I�[�_ #*/
//SS9100(PH1�ĊJ)�Ή� 2006.06
#define     O_PH1ORDER     0x81         /*PH1�ĊJ�I�[�_*/

#define     O_MNT_ORD_START     0xf0    /*# �����e�i���X�p�I�[�_�J�n�l #*/  //2001.01.11 V2.2 �ǉ�
#define     O_MNT_ORD_END       0xff    /*# �����e�i���X�p�I�[�_�I���l #*/  //2001.01.11 V2.2 �ǉ�

//���kLU�Ή��BDSP�����p�I�[�_�ǉ��B2003.11.13
//DSP�����p�I�[�_�R�[�h start and end
#define O_DSP_ORD_TYPE      0xe0
#define O_DSP_ORD_TEND      0xef

//DSP�����p�I�[�_�R�[�h
#define     O_DSP_GET_SYSTEMSTATUS  0xe0
#define     O_DSP_GET_CHANNELSTATUS 0xe1
#define     O_DSP_REQUEST_PARAM     0xe2

    /*# ���b�Z�[�W��ʁF�k�t�C�x���g #*/
#define     E_LUONLINE      0x01        /*# LU�I�����C���ʒm #*/
#define     E_PKGINITRSP    0x02        /*# ��PKG�����ݒ艞�� #*/
#define     E_LUHELRSP      0x03        /*# LU�w���X�`�F�b�N���� #*/
#define     E_PKGHELRSP     0x04        /*# PKG�w���X�`�F�b�N���� #*/
#define     E_PKGSTATE      0x05        /*# PKG��Ԓʒm #*/
#define     E_PBDET         0x06        /*# PB�M����M #*/
#define     E_BTDET         0x07        /*# BT��M #*/
#define     E_DTDET         0x08        /*# DT��M #*/
#define     E_PBSNDEND      0x09        /*# PB���M���� #*/
#define     E_DOORHSCNRSP   0x0A        /*# �h�A�z���X�L���� #*/
#define     E_PKGTROUBLE    0x0B        /*# PKG��Q���o #*/
#define     E_CLKDTROUBLE   0x0C        /*# CLKD��Q���o #*/
#define     E_CLKDRESTORE   0x0D        /*# CLKD��Q�������o #*/
#define     E_FANTROUBLE    0x0E        /*# FAN��Q���o #*/
#define     E_MEMDMPRSP     0x0F        /*# ������DUMP���� #*/ // 99-5-18
#define     E_SYNC_COMP     0x10        /*# �b�k�j�c�������킹�����ʒm�i����␳���ɒʒm�j #*/  // 2000.05.11 V1.2 �ǉ�
#define     E_HORYDLEND     0x11        /*# �ۗ����_�E�����[�h�����ʒm #*/                      //2000.07 V2.1�ǉ�
#define     E_RELDET        0x12        /*# �`���l������ʒm #*/                                //2001.08 V2.3�ǉ�

#define     E_FAXERROR  0x16        /*# FAN��Q���o #*//* ���kLU */
#define     E_FAXPASSCHG    0x17        /*# �Ď�ʐ؂�ւ� #*/

/*���kLU�Ή�*/
#define     E_DSP_ERROR_DETECT  0x18
#define     E_DSP_REPAIR_DETECT 0x19


#define     E_PGDLEND           0x1A//2004.04 IP-CS
#define     E_AQNTC         0x1B//2004.10 SS9100


#define     E_DSP_SYSTEMSTATUS  0x1E//2004.04 IP-CS 
#define     E_DSP_CHANNELSTATUS 0x1F//2004.04 IP-CS 

//#define       E_DSP_SYSTEMSTATUS  0x1b//2004.04 IP-CS 
//#define       E_DSP_CHANNELSTATUS 0x1c//2004.04 IP-CS 

//#define       E_DSP_SYSTEMSTATUS  0x1a
//#define       E_DSP_CHANNELSTATUS 0x1b


// �H�ꎎ���p
#define     E_TST_DRAM_CHK  0xf8        /*# �H�ꎎ���p �c�q�`�l�`�F�b�N #*/
#define     E_TST_MEM_CHK   0xf9        /*# �H�ꎎ���p �������`�F�b�N #*/
#define     E_TST_LED_CTL   0xfa        /*# �H�ꎎ���p �k�d�c�\�� #*/
#define     E_TST_MEM_RD    0xfb        /*# �H�ꎎ���p ���������[�h #*/
#define     E_TST_MEM_WT    0xfc        /*# �H�ꎎ���p ���������C�g #*/
#define     E_TST_DSP_CK    0xfd        /*# �H�ꎎ���p �c�r�o�������`�F�b�N #*/ // 2001.05.22 �ǉ�
// �H�ꎎ���p

#define     E_PKGEVENT      0x80        /*# ����PKG�C�x���g #*/


/*# �I�[�_/�C�x���g�f�[�^ #*/
#define     NUM_OF_MSG      0x0FF       /*# ���b�Z�[�W���MAX�l #*/
#define     ORD_MIN         2           /*# �~�j�}���T�C�Y #*/

/*# LU�L���r�l�b�g�^�C�v #*/
#define     LU_BS_LU        0           /*# BS-LU #*/
#define     LU_EX_LU        1           /*# EX-LU #*/

/*# LU�^�C�v #*/
#define     LU_MASTER       0           /*# LU�}�X�^ #*/
#define     LU_SLAVE        1           /*# LU�X���[�u #*/

/*# CLKD�^�C�v #*/
#define     CLKD_MASTER     0           /*# �N���b�N�}�X�^ #*/
#define     CLKD_SLAVE      1           /*# �N���b�N�X���[�u #*/

/*# ����MAX�l #*/
//#define       SLOT_MAX        4           /*# �ő�����\�X���b�g�� #*/
#define     DPRAM_MAX       8           /*# �ő�DPRAM�� #*/

//#define       LU_MAX          96          /*# �ő�LU��(V1) #*/
#define     LU_MAX          255         /*# �ő�LU��(V4) #*///2004.01 LU32B
#define     SLV_MAX         16          /*# �ő�SLV��(V1) #*/

/*# ���荞�� #*/
#define     IRQ_ENABLE      1           /*# IRQ���荞�݂̂݋��� #*/

/*# �X���b�g������� #*/
#define     INS_SLOT    0               /*# PKG������ #*/
#define     OUS_SLOT    1               /*# PKG������ #*/

/*# ���M���^���M�掯�ʎq #*/
#define     SNDID_LU    0               /*# LU�I�[�_�C�x���g #*/
#define     SNDID_PKG   1               /*# PKG�I�[�_�C�x���g #*/

/*# ���M��PKG�l #*///IP-CS�Ή�
#define     FROM_SLAVE  1               /*# ���X���[�u���M���C�x���gPKG�ԍ��G���A�l #*/


/*# �������ݎ�� #*/
#define PKG_ORDER           1           /*# ��ʃI�[�_ #*/
#define HELTH_CHECK         2           /*# �w���X�`�F�b�N�I�[�_ #*/
#define DOOR_CTL            3           /*# �h�A�z������v�� #*/
//SS9100(PH1�ĊJ)�Ή� 2006.06
#define PH1_ORDER           4           /*# PH1�ĊJ�I�[�_ #*/

/*# BOOTP #*/ 
#define VEND_LU_TAG         128         /*# �x���_�f�[�^�^�C�v #*/
#define VEND_LU_SIZE        7           /*# �k�t�x���_���T�C�Y #*/ // 99.05.21 �ύX
#define VEND_VLAN_TAG       131         /*# �u�k�`�m�^�O #*/ // 99.08.18
#define VEND_VER_TAG        132         /*# IPL�o�[�W�����^�O #*/               // 2000.06.20 V1.2�ǉ�
#define VEND_VER_SIZE       4           /*# �k�t�o�[�W�����T�C�Y #*/            // 2000.06.20 V1.2�ǉ�
#define VEND_CCUID_TAG      133         /*# CCU ID�^�O #*/                      // 2000.07 V2.1�ǉ�
#define VEND_CCUID_SIZE     1           /*# CCU ID�T�C�Y #*/                    // 2000.07 V2.1�ǉ�
#define VEND_CCUIP_TAG      134         /*# CCU IP�A�h���X�^�O #*/              // 2000.07 V2.1�ǉ�
#define VEND_CCUIP_SIZE     4           /*# CCU IP�A�h���X�A�h���X #*/          // 2000.07 V2.1�ǉ�
#define VEND_CCUVER_TAG     135         /*# CCU�o�[�W�����^�O #*/               // 2000.07 V2.1�ǉ�
#define VEND_CCUVER_SIZE    2           /*# CCU�o�[�W�����T�C�Y #*/             // 2000.07 V2.1�ǉ�
#define VEND_EOL_TAG        255         /*# �G���h�I�u���X�g #*/ // 99.05.21 �ǉ�
#define VEND_SUBNET_SIZE    4           /*# �T�u�l�b�g�}�X�N #*/                // 2000.09 V2.1�ǉ�
#define VEND_GATEWAY_SIZE   4           /*# �Q�[�g�E�F�CIP�A�h���X #*/          // 2000.09 V2.1�ǉ�
#define VEND_VLAN_SIZE      5           /*# �T�u�l�b�g�}�X�N #*/                // 2000.09 V2.1�ǉ�
#define VEND_LUTYPE_TAG     136         /*# �k�t�^�C�v�^�O #*/                  // 2001.07.05 �ǉ�
#define VEND_LUTYPE_SIZE    1           /*# �k�t�^�C�v�T�C�Y #*/                // 2001.07.05 �ǉ�
#define VEND_LUTYPE_TYPE    9           /*# �k�t�^�C�v�iIP-CS�j #*/             // 2004.04 �ǉ�
#define VEND_CCU2_TAG       137         /*# CCU2 IP�A�h���X�^�O #*/             // 2001.08 V2.3 �ǉ�
#define VEND_CCU2_SIZE      4           /*# CCU2 IP�A�h���X�A�h���X #*/         // 2001.08 V2.3 �ǉ�


/*# �_�E�����[�h�I�[�_ #*/
/*# �_�E�����[�h�I�[�_�T�C�Y #*/
#define     V1_1_DL_SIZE    45                  /*# V1.1CCU�̃_�E�����[�h�I�[�_�T�C�Y #*/       // 2000.05.12 V1.2�ǉ�
#define     V2_1_DL_SIZE    51                  /*# V2.1CCU�̃_�E�����[�h�I�[�_�T�C�Y #*/       // 2000.07 V2.1�ǉ�
#define     V2_1_DL_SIZEV2  91                  /*# V2.1CCU�̃_�E�����[�h�I�[�_�T�C�YVer.2# */  // 2000.09 V2.1�ǉ�
//#define       V2_2_DL_SIZE    97                  /*# V2.2CCU�̃_�E�����[�h�I�[�_�T�C�Y# */       // 2000.12.27 V2.2�ǉ�
#define     V2_2_DL_SIZE    98                  /*# V2.2CCU�̃_�E�����[�h�I�[�_�T�C�Y# */       // 2001.03.08 �ύX
#define     V2_3_DL_SIZE    100                 /*# V2.3CCU�̃_�E�����[�h�I�[�_�T�C�Y# */       // 2001.08 �ǉ�

//  ��炬�ő�l��` 2000.09 V2.1�ǉ�
#define     V2_1_CCU_YURAGIMAX  11                          /*# V2.1 CCU�w���炬�ő�l #*/
#define     V1_1_CCU_YURAGIMAX  5                           /*# V1.1 CCU�w���炬�ő�l #*/

//  �o�b�t�@�ʎw��ő�l��` 2000.09 V2.1�ǉ�
#define     V2_1_CCU_BUFFNUMMAX 10                          /*# V2.1 CCU�w��o�b�t�@�ʎw��ő�l #*/
#define     V1_1_CCU_BUFFNUMMAX 5                           /*# V1.1 CCU�w��o�b�t�@�ʎw��ő�l #*/

//  D01�T�o�C�o���Ή� 2004.02 LU32B�ǉ�
#define     SURV_SWITCH_TIM     1800                    /*# PowerOn���̃f�t�H���gCCU�؊��^�C�}�l(�b) #*/

//SS9100�Ή��ǉ� 2004.11.01
#define     SURV_SWITCH_SS9100_TIM  120                 /*# SS9100���̃f�t�H���gCCU�؊��^�C�}�l(�b) #*/

/****************************************************************************/
/* FAX�g�[�����o���                                                        */
/****************************************************************************/
enum
{
    FAXTONE_CNG         ,       /* CNG      */
    FAXTONE_CED         ,       /* CED          */
    FAXTONE_PRI             /* �v���A���u�� */
} ;


/***********************************************************************/
/* DSP���                                 */
/***********************************************************************/
enum
{
    DSP_0         ,       /* ch0-ch3      */
    DSP_1                 /* ch4-ch7      */
} ;

/***********************************************************************/
/* ������~���                                */
/***********************************************************************/
enum
{
    STOP_DSP         ,      /* DSP    */
    STOP_CH                 /* CH         */
} ;



/*###########################################################################*/
/*#                                                                         #*/
/*#                               BOOTP�֘A                                 #*/
/*#                                                                         #*/
/*###########################################################################*/
#define     BOOTP_RESP_AERA 0x60000         /*# BOOTP RESP �������ݐ擪�A�h���X(���j #*/

/*###########################################################################*/
/*#                                                                         #*/
/*#                               �ۗ����֘A                                #*/
/*#                                                                         #*/
/*###########################################################################*/
//#define       HORYSIZE        (256*1024+40)               /*# �ۗ����T�C�Y #*/
#define     HORYSIZE        (256*1024)                  /*# �ۗ����T�C�Y #*/
#define     HORYSAVESIZE    (256*1024)                  /*# �ۗ����t���b�V���ۑ��T�C�Y #*/  //2001.01.16 V2.2�ǉ�
//#define       TALKYSIZE       (128*1024+40)               /*# �g�[�L�[�T�C�Y #*/
#define     TALKYSIZE       (128*1024)                  /*# �g�[�L�[�T�C�Y #*/
#define     TALKYSAVESIZE   (128*1024)                  /*# �g�[�L�[�t���b�V���ۑ��T�C�Y #*/    //2001.01.16 V2.2�ǉ�
//#define       HORY1_ADR       0x2130000               /*# �ۗ����P�擪�A�h���X(���j #*/
//#define       HORY2_ADR       HORY1_ADR+HORYSIZE      /*# �ۗ����Q�擪�A�h���X #*/
//#define       HORY_RDMAX      0x1770      /*# �ۗ����f�[�^�ǂݍ��ݍő�l(40BYTE�P��) #*/

/*###########################################################################*/
/*#                                                                         #*/
/*#                           ���ʊ֐��p�f�t�@�C��                          #*/
/*#                                                                         #*/
/*###########################################################################*/
#define     POOL0           0x00        /*# ��ʃ��b�Z�[�W�p        #*//*# DOEP�ɂ�USER-QUE�g�p #*/
#define     POOL1           0x01        /*# �񎟃��b�Z�[�W�p        #*//*# LU�Ǘ��ɂ�USER-QUE�g�p #*/
#define     POOL2           0x02        /*# DSP����f�[�^�p     #*//*# DSP�ɂ�USER-QUE�g�p #*/
#define     POOL3           0x03        /*# ������M�p #*/
#define     POOL4           0x04        /*# �������M�p #*/
#define     POOL5           0x05        /*# UDP���M�p #*/
#define     POOL6           0x06        /*# FUSION�p #*/
#define     POOL7           0x07        /*# TAG�p #*/
#define     POOL8           0x08        /*# DMY #*//*# DPRAM����CN0�ɂ�USER-QUE�g�p #*/
#define     POOL9           0x09        /*# DMY #*//*# DPRAM����CN1�ɂ�USER-QUE�g�p #*/
#define     POOLA           0x0A        /*# DMY #*//*# DPRAM����CN2�ɂ�USER-QUE�g�p #*/
#define     POOLB           0x0B        /*# DMY #*//*# DPRAM����CN3�ɂ�USER-QUE�g�p #*/
#define     POOLC           0x0C        /*# DMY #*//*# DPRAM����CN4�ɂ�USER-QUE�g�p #*/
#define     POOLD           0x0D        /*# DMY #*//*# DPRAM����CN5�ɂ�USER-QUE�g�p #*/
#define     POOLE           0x0E        /*# DMY #*//*# DPRAM����CN6�ɂ�USER-QUE�g�p #*/
#define     POOLF           0x0F        /*# DMY #*//*# DPRAM����CN7�ɂ�USER-QUE�g�p #*/
#define     POOL10          0x10        /*# DMY #*//*# DPRAM����CN8�ɂ�USER-QUE�g�p #*/ //2000.07 V2.1�ǉ�
#define     POOL11          0x11        /*# DMY #*//*# DPRAM����CN9�ɂ�USER-QUE�g�p #*/ //2000.07 V2.1�ǉ�
#define     POOL12          0x12        /*# DMY #*//*# DPRAM����CN10�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL13          0x13        /*# DMY #*//*# DPRAM����CN11�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL14          0x14        /*# DMY #*//*# DPRAM����CN12�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL15          0x15        /*# DMY #*//*# DPRAM����CN13�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL16          0x16        /*# DMY #*//*# DPRAM����CN14�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL17          0x17        /*# DMY #*//*# DPRAM����CN15�ɂ�USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL18          0x18        /*# TFTP�T�[�o�p #*/                            //2000.07 V2.1�ǉ�
#define     POOL19          0x19        /*# T38�p�P�b�g�p  #*//* ���kLU */                          //2003.08 �ǉ�
#define     POOL1A          0x1A        /*# FAX�f�[�^�p    #*/                          //2003.08 �ǉ�
#define     POOL1B          0x1B        /*# FAX�f�[�^�Ǘ��p#*/                          //2003.08 �ǉ�
//���kLU�Ή��BDSP���A���^�C���R�}���h�p�B2003.09.03
#define     POOL1C          0x1C        /*# DSP���A���^�C���R�}���h�p#*/
#define     POOL1D          0x1D        /*# �g���[�X���p#*/                       //2003.09
#define     POOL1E          0x1E        /*# DSP�������p#*/                      //2003.09
#define     POOL1F          0x1F        /*# DSP�������p#*/                      //2003.09


    /*# �v�[���T�C�Y #*/
//#define       POOL0_SIZE      40          /*# ��ʃ��b�Z�[�W�p [12(os)+28(����msg)] #*/
#define     POOL0_SIZE      56          /*# ��ʃ��b�Z�[�W�p [12(os)+44(����msg)] #*/ //2003.10.23
#define     POOL1_SIZE      280         /*# �񎟃��b�Z�[�W�p [12(os)+12(LUͯ��)+256(����/����ď��)] #*/
#define     POOL2_SIZE      80          /*# DSP����f�[�^�p [12(os)+4(SystemSTS)+64] #*/
#define     POOL3_SIZE      1540        /*# ������M���b�Z�[�W�p [12(os)+1528(Max LAN Rcv Size)] #*/
#define     POOL4_SIZE      984         /*# �������M���b�Z�[�W�p [12(os)+12(rtp)+960(����120ms)) #*/
#define     POOL5_SIZE      140         /*# UDP���M�p [12(os)+128(RTCP�ACLKD�ACRINT)]#*/
#define     POOL6_SIZE      20          /*# FUSION�p [12(os)+4(SystemSTS)+4(Dmy)] #*/
#define     POOL7_SIZE      28          /*# TAG�p [12(os)+16(TAG)] #*/
#define     POOL8_SIZE      1048        /*# EX���b�Z�[�W�p [12(os)+12(LUͯ��)+1024(����/����ď��)]  #*/
#define     POOL9_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLA_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLB_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLC_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLD_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLE_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOLF_SIZE      16          /*# DMY USER-QUE�g�p #*/
#define     POOL10_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL11_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL12_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL13_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL14_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL15_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL16_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL17_SIZE     16          /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL18_SIZE     528         /*# TFTP�T�[�o�p[12(os)+2(op)+2(blk)+512(DUP�p�P�b�g���ő�)] #*///2000.07 V2.1�ǉ�
#define     POOL19_SIZE     (12+1500)       /*# T38�p�P�b�g�p  #*//* ���kLU */                          //2000.07 V2.1�ǉ�
#define     POOL1A_SIZE     (12+400)        /*# FAX�f�[�^�p    #*/                          //2000.07 V2.1�ǉ�
#define     POOL1B_SIZE     (12+16)     /*# FAX�f�[�^�Ǘ��p#*/                          //2000.07 V2.1�ǉ�
//���kLU�Ή��BDSP���A���^�C���R�}���h�p�B2003.09.03
#define     POOL1C_SIZE     (12+2048)       /*# DSP���A���^�C���R�}���h�p�@#*/
#define     POOL1D_SIZE     (12+256)        /*# �g���[�X���p�@#*/                         //2003.09
#define     POOL1E_SIZE     (12+4096+4)     /*# DSP�p�@#*/                          //2003.09
#define     POOL1F_SIZE     (12+4096+4)     /*# DSP�p�@#*/                          //2003.09


    /*# �v�[���ʐ� #*/
#define     POOL0_CNT       512         /*# ��ʃ��b�Z�[�W�p #*/
#define     POOL1_CNT       256         /*# �񎟃��b�Z�[�W�p #*/
#define     POOL2_CNT       32          /*# DSP����f�[�^�p #*/
#define     POOL3_CNT       1           /*# ������M�p #*/
#define     POOL4_CNT       512         /*# �������M�p #*/
#define     POOL5_CNT       32          /*# UDP���M�p #*/
#define     POOL6_CNT       256         /*# FUSION�p #*/

//#define       POOL7_CNT       32          /*# TAG�p #*/
#define     POOL7_CNT       224         /*# TAG�p[96(uni_CLKD)+96(uni_CRINT)+32(���̐�)] #*/ // 2002.03.28�ύX

#define     POOL8_CNT       10          /*# EX���b�Z�[�W�p #*/
#define     POOL9_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLA_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLB_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLC_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLD_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLE_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOLF_CNT       1           /*# DMY USER-QUE�g�p #*/
#define     POOL10_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL11_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL12_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL13_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL14_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL15_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL16_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL17_CNT      1           /*# DMY USER-QUE�g�p #*///2000.07 V2.1�ǉ�
#define     POOL18_CNT      (32*2)      /*# TFTP&BOOTP�T�[�o�p #*/  //2000.07 V2.1�ǉ�
#define     POOL19_CNT      (32)        /*# T38�p�P�b�g�p   #*//* ���kLU */
#define     POOL1A_CNT      (240)       /*# FAX�f�[�^�p     #*/
#define     POOL1B_CNT      (250)       /*# FAX�f�[�^�Ǘ��p #*/
//���kLU�Ή��BDSP���A���^�C���R�}���h�p�B2003.09.03
#define     POOL1C_CNT      16          /*# DSP���A���^�C���R�}���h�p�@#*/
#define     POOL1D_CNT      50          /*# �g���[�X���p�@#*/             //2003.09
#define     POOL1E_CNT      8           /*# �g���[�X���p�@#*/             //2003.09
#define     POOL1F_CNT      8           /*# �g���[�X���p�@#*/             //2003.09



    /*# ���[�U�L���[ID #*/
#define     DOEP_QUEID      POOL0       /*# DOEP�p�L���[ID #*/
#define     LUMNG_DPQID     POOL1       /*# LU�Ǘ��p�L���[ID #*/
#define     DSPMNG_QUEID    POOL2       /*# DSP����f�[�^�p #*/
#define     H323_CN0QUEID   POOL4       /*# ��M��������CN0�ɂ�USER-QUE�g�p #*/
#define     H323_CN1QUEID   POOL4+1     /*# ��M��������CN1�ɂ�USER-QUE�g�p #*/
#define     H323_CN2QUEID   POOL4+2     /*# ��M��������CN2�ɂ�USER-QUE�g�p #*/
#define     H323_CN3QUEID   POOL4+3     /*# ��M��������CN3�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL0QUEID  POOL8       /*# DPRAM����CN0�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL1QUEID  POOL8+1     /*# DPRAM����CN1�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL2QUEID  POOL8+2     /*# DPRAM����CN2�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL3QUEID  POOL8+3     /*# DPRAM����CN3�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL4QUEID  POOL8+4     /*# DPRAM����CN4�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL5QUEID  POOL8+5     /*# DPRAM����CN5�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL6QUEID  POOL8+6     /*# DPRAM����CN6�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL7QUEID  POOL8+7     /*# DPRAM����CN7�ɂ�USER-QUE�g�p #*/
#define     DPISR_SL8QUEID  POOL8+8     /*# DPRAM����CN8�ɂ�USER-QUE�g�p #*/    //2000.07 V2.1�ǉ�
#define     DPISR_SL9QUEID  POOL8+9     /*# DPRAM����CN9�ɂ�USER-QUE�g�p #*/    //2000.07 V2.1�ǉ�
#define     DPISR_SL10QUEID POOL8+10    /*# DPRAM����CN10�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     DPISR_SL11QUEID POOL8+11    /*# DPRAM����CN11�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     DPISR_SL12QUEID POOL8+12    /*# DPRAM����CN12�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     DPISR_SL13QUEID POOL8+13    /*# DPRAM����CN13�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     DPISR_SL14QUEID POOL8+14    /*# DPRAM����CN14�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     DPISR_SL15QUEID POOL8+15    /*# DPRAM����CN15�ɂ�USER-QUE�g�p #*/   //2000.07 V2.1�ǉ�
#define     TFTPSRV_QUEID   POOL18      /*# TFTP�T�[�o�p�L���[ID #*/            //2000.07 V2.1�ǉ�

#define     T38_QUEID   POOL19      /*# T38PKT��M�p�L���[ID #*//* ���kLU */
#define     FAX_DATQUEID    POOL1A      /*# FAX�f�[�^�p�L���[ID #*//* ���kLU */
#define     FAX_LINKQUEID   POOL1B      /*# FAX�f�[�^�����N�p�L���[ID #*//* ���kLU */
//���kLU�Ή��BDSP���A���^�C���R�}���h�p�B2003.09.03
#define     DSP_REALQUEID   POOL1C          /*# DSP���A���^�C���R�}���h�p�@#*/
#define     LOG_TRACEQUEID  POOL1D          /*# �g���[�X���p�@#*/         //2003.09

    /*# ���[�U�L���[�ڑ���� #*/
#define     QUE_TOP             0
#define     QUE_TAIL            1

    /*# ���O���W�ʐ� #*/
#define     MSG_LOG_SIZE        1024

//2000.10 V2.1�ǉ�(start)
#define     SYNC_1S_5MSBASE     200                     /* 5ms�x�[�X�ł�1s���̃J�E���g */
#define     SYNC_CNT_MAX        (15*SYNC_1S_5MSBASE)    /*# �␳�J�E���^�J�E���g�ő�l�i5ms�F15�b�j #*/
//2000.10 V2.1�ǉ�(end)

#define     DOEP_HELCHK_TO      30                      /*# �w���X�`�F�b�N�^�C�}�J�E���g�f�t�H���g�l�i�b�j #*/  //2000.12.27 V2.2�ǉ��i���̕ύX���ċ��ʉ��j

//SS9100�Ή��ǉ� 2004.11.01
#define     DOEP_HELCHK_SS9100_TO   60                      /*# SS9100�w���X�`�F�b�N�^�C�}�J�E���g�f�t�H���g�l�i�b�j #*/

/*#########################################*/
/*#            �}�N����`                 #*/
/*#########################################*/

#ifdef LU_MACRO
#if 0 /* Ozawa */
#define     com_poolget(a)          XGETPB_R(a);
#define     com_ipoolget(a)         XSGETPB_R(a);/* com_poolget�Ɠ����ɂ��� */
#define     com_poolput(a,b)        XPUTPB_R(a,b)
#define     com_ipoolput(a,b)       XSPUTPB_R(a,b)
#define     com_iptdis(void)        pri = XIPTMR(void);\
                                    XIPTMW(3);
#define     com_iptena(a)           XIPTMW(a)
#define     com_sndmsg(a,b)         XPOSTM(a,b)
#define     com_isndmsg(a,b)        XSPOSTM(a,b)
#define     com_rcvmsg(a,b)         XWTMR1(a,b)
#define     com_getmsg(a)           XGETEV(a)
#define     com_igetmsg(a)          XSGETEV(a)
#define     com_queput(a,b,c)       XPUTUL_R(a,b,c)
#define     com_iqueput(a,b,c)      XSPUTUL_R(a,b,c)
#define     com_queget(a,b)         XGETUL_R(a,b)
#define     com_iqueget(a,b)        XSGETUL_R(a,b)
#define     com_quesns(a)           XULSNS_R(a)
#define     com_iquesns(a)          XSULSNS_R(a)
#endif /* Ozawa */
#define     com_chgword(C)          (WORD)((((C)>>8)&0xFF)|(((C)<<8)&0xFF00))
#define     com_chgdword(C)         ((DWORD)(((((DWORD)C)>>24)&0xFF)|((((DWORD)C)>>8)&0xFF00)|((((DWORD)C)<<8)&0xFF0000)|((((DWORD)C)<<24)&0xFF000000)))
#define com_10dwordcopy(a,b)        *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a = *b;
//���kLU�Ή��B�Z�}�t�H�p�}�N���ǉ��B2003.08.27
#define     com_getsemp(a,b,c)      XGETSEM(a,b,c)
#define     com_putsemp(a)          XPUTSEM(a)
#define     com_initsemp(a,b)       XINITSEM(a,b)
#define     com_sempsns(a)          XSEMSNS(a)
#endif //LU_MACRO

#ifndef TMRIPT_START                                        // LU 99.7.14
    #define     TMRIPT_START    (fusion_ipt_flg |= 0x02)
#endif
#ifndef TMRIPT_END
    #define     TMRIPT_END      (fusion_ipt_flg &= ~0x02)
#endif
#ifndef LANCIPT_START
    #define     LANCIPT_START   (fusion_ipt_flg |= 0x01)
#endif
#ifndef LANCIPT_END
    #define     LANCIPT_END     (fusion_ipt_flg &= ~0x01)
#endif

/*###########################################################################*/
/*#                                                                         #*/
/*#                           �b�o�t���ӃA�h���X                            #*/
/*#                                                                         #*/
/*###########################################################################*/
/*##################################################################*/
/*#           �ȁ|�k�r�h�����f�o�C�X                               #*/
/*##################################################################*/
/*########################################################*/
/*#           �v���Z�b�T��                               #*/
/*########################################################*/
#define     IO_RESET_VECT       0x00000000          /*# ���Z�b�g�x�N�^�A�h���X #*/
#define     IO_ROM_BASE         0x00000000          /*# �q�n�l�擪�A�h���X #*/
#define     IO_INRAM_BASE       0x10000000          /*# �����q�`�l�擪�A�h���X #*/
#define     IO_INRAM_LIMIT      0x10002000          /*# �����q�`�l�I���A�h���X #*/

/*########################################################*/
/*#           �`�r�a�f�o�C�X�^�`�g�a�f�o�C�X             #*/
/*########################################################*/
#define     IO_ICE              0x3fffffa0          /*# ���߃L���b�V�����䃌�W�X�^ #*/
#define     IO_WBE              0x3fffffc0          /*# ���C�g�o�b�t�@���䃌�W�X�^ #*/

/*#########################################*/
/*#           �l�`�b                      #*/
/*#########################################*/
// �f�o�C�X�h���C�o���Œ�`�ς�

/*#########################################*/
/*#           �o�h�b                      #*/
/*#########################################*/
//���kLU�Ή��BV1�p���W�X�^�R�����g�A�E�g�B2003.08.25
//�폜9���ȍ~
#define     IO_IRQ              0x78000090          /*# �h�q�p�����v�� #*/
#define     IO_IRQ_SOFT         0x78000010          /*# �h�q�p�\�t�g�X�e�[�^�X #*/
#define     IO_FIQ              0x78000100          /*# �e�h�p�����v�� #*/
#define     IO_FIQRAW           0x78000104          /*# �e�h�p �q�`�v�X�e�[�^�X #*/
#define     IO_FIQEN            0x7800010c          /*# �e�h�p�C�l�[�u�� #*/
#define     IO_IRNR             0x78000080          /*# �h�q�p�ԍ� #*/
#define     IO_CILR             0x78000084          /*# ���h�q�p�ԍ� #*/
#define     IO_IRL              0x78000088          /*# �h�q�p���x�� #*/
#define     IO_IREL             0x7800008c          /*# �h�q�p�G�b�W�^���x������ #*/
#define     IO_IRCL             0x78000090          /*# �h�q�p�X�e�[�^�X�N���A #*/
#define     IO_ILCR0            0x780000a0          /*# �h�q�p���x������O #*/
#define     IO_ILCR1            0x780000a4          /*# �h�q�p���x������P #*/
//�u�Q�n�[�h�p�i�h�m�s�q�b�j    //2000.09 V2.1�ǉ�
#define     IO_V2_IRQ           0x78000000          /*# �h�q�p�����v�� #*/
#define     IO_V2_IRQ_SOFT      0x78000004          /*# �h�q�p�\�t�g�X�e�[�^�X #*/
#define     IO_V2_FIQ           0x78000008          /*# �e�h�p�����v�� #*/
#define     IO_V2_FIQRAW        0x7800000c          /*# �e�h�p �q�`�v�X�e�[�^�X #*/
#define     IO_V2_FIQEN         0x78000010          /*# �e�h�p�C�l�[�u�� #*/
#define     IO_V2_IRNR          0x78000014          /*# �h�q�p�ԍ� #*/
#define     IO_V2_CILR          0x78000018          /*# ���h�q�p�ԍ� #*/
#define     IO_V2_IRL           0x7800001c          /*# �h�q�p���x�� #*/
//#define   IO_V2_IREL          0x7800008c          /*# �h�q�p�G�b�W�^���x������ #*/
//#define   IO_V2_IRCL          0x78000090          /*# �h�q�p�X�e�[�^�X�N���A #*/
#define     IO_V2_ILCR0         0x78000020          /*# �h�q�p���x������O #*/
#define     IO_V2_ILCR1         0x78000024          /*# �h�q�p���x������P #*/

/*#########################################*/
/*#           �w�l�b                      #*/
/*#########################################*/
//���kLU�Ή��BV1�p���W�X�^�R�����g�A�E�g�B2003.08.25
#define     IO_BWCON            0x78100000          /*# �o�X���R���g���[�����W�X�^ #*/
#define     IO_OTCON            0x78100004          /*# �I�t�^�C���R���g���[�����W�X�^ #*/
#define     IO_PWCON0           0x78100008          /*# �v���O���}�u���E�F�C�g�R���g���[�����W�X�^�O #*/
#define     IO_PWCON1           0x7810000c          /*# �v���O���}�u���E�F�C�g�R���g���[�����W�X�^�P #*/
#define     IO_PWCON2           0x78100010          /*# �v���O���}�u���E�F�C�g�R���g���[�����W�X�^�Q #*/
#define     IO_DRCON            0x78100014          /*# �c�q�`�l�R���g���[�����W�X�^ #*/
#define     IO_RFTCN            0x78100018          /*# ���t���b�V���^�C�}�J�E���^���W�X�^ #*/
#define     IO_RCCON            0x7810001c          /*# ���t���b�V���T�C�N���R���g���[�����W�X�^ #*/
#define     IO_RFCON            0x78100024          /*# ���t���b�V���R���g���[�����W�X�^ #*/
#define     IO_ATCON0           0x78100028          /*# �A�N�Z�X�^�C�~���O�R���g���[�����W�X�^�O #*/
#define     IO_ATCON1           0x7810002c          /*# �A�N�Z�X�^�C�~���O�R���g���[�����W�X�^�P #*/
#define     IO_ATCON2           0x78100030          /*# �A�N�Z�X�^�C�~���O�R���g���[�����W�X�^�Q #*/
//�u�Q�n�[�h�p�_�~�[�i�{��`�̓V�X�e���N�������j//2000.09 V2.1�ǉ�
#define     IO_V2_XMC_BWC       0x78100000          /*# �o�X���R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_BAC       0x78100004          /*# �o�X�A�N�Z�X�R���g�[�����W�X�^ #*/
#define     IO_V2_XMC_DRMC      0x78100008          /*# �c�q�`�l�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_DRAC      0x7810000c          /*# �d�c�n�|�c�q�`�l�A�N�Z�X�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_SDACL1    0x78100010          /*# �r�c�q�`�l�A�N�Z�X�R���g���[�����W�X�^�P #*/
#define     IO_V2_XMC_SDACL2    0x78100014          /*# �r�c�q�`�l�A�N�Z�X�R���g���[�����W�X�^�Q #*/
#define     IO_V2_XMC_SDMD1     0x78100018          /*# �r�c�q�`�l���[�h���W�X�^�P #*/
#define     IO_V2_XMC_SDMD2     0x7810001c          /*# �r�c�q�`�l���[�h���W�X�^�Q #*/
#define     IO_V2_XMC_RFCNT     0x78100020          /*# �c�q�`�l���t���b�V���^�C�}�J�E���^���W�X�^ #*/
#define     IO_V2_XMC_RFSH      0x78100024          /*# �c�q�`�l���t���b�V���T�C�N���R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_ROMAC     0x78100028          /*# �q�n�l�A�N�Z�X�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_ROMPC     0x7810002c          /*# �q�n�l�y�[�W���[�h�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_SRMAC     0x78100030          /*# �r�q�`�l�A�N�Z�X�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_SRMPC     0x78100034          /*# �r�q�`�l�y�[�W���[�h�R���g���[�����W�X�^ #*/
#define     IO_V2_XMC_IOAC1     0x78100038          /*# �h�n�A�N�Z�X�R���g���[�����W�X�^�P #*/
#define     IO_V2_XMC_IOAC2     0x7810003c          /*# �h�n�A�N�Z�X�R���g���[�����W�X�^�Q #*/

/*#########################################*/
/*#           �c�r�o�i�n�`�j�j            #*/
/*#########################################*/
#define     IO_PRAM             0x7e800000          /*# �n�`�j �o�q�`�l�擪�A�h���X #*/

#define     IO_CNT              0x7e800004          /*# �n�[�h�E�F�A�R���g���[�����W�X�^ #*/
#define     IO_ST0              0x7e800008          /*# �n�[�h�E�F�A�X�e�[�^�X���W�X�^�O #*/
#define     IO_ST1              0x7e80000c          /*# �n�[�h�E�F�A�X�e�[�^�X���W�X�^�P #*/
#define     IO_WORD             0x7e800010          /*# �I�[�_�����W�X�^ #*/
#define     IO_ORDER            0x7e800014          /*# �I�[�_���W�X�^ #*/
#define     IO_EVENT            0x7e800018          /*# �C�x���g���W�X�^ #*/
#define     IO_V2_INTENA        0x7e800024          /*# �h�m�s�C�l�[�u�����W�X�^ #*/
#define     IO_V2_INTCLR        0x7e800028          /*# �h�m�s�C�N���A���W�X�^ #*/
#define     IO_RTFIFO           0x7e80001c          /*# �����f�[�^�e�h�e�n #*/
#define     IO_OAK_RST          0x7e80fffc          /*# ���Z�b�g #*/

/*#########################################*/
/*#           �b�`�b�g�d                  #*/       //2000.09 V2.1�ǉ�
/*#########################################*/
#define     IO_V2_CACHE_CON     0x78200004          /*# �L���b�V���R���g���[�����W�X�^ #*/
#define     IO_V2_CACHE_CACHE   0x78200008          /*# �L���b�V���u�����W�X�^ #*/
#define     IO_V2_CACHE_ABORT   0x78200010          /*# �A�{�[�g�A�h���X���W�X�^ #*/
#define     IO_V2_CACHE_FLUSH   0x7820001c          /*# �t���b�V�����W�X�^ #*/

/*########################################################*/
/*#           �`�o�a�f�o�C�X                             #*/
/*########################################################*/
/*#########################################*/
/*#           �q�d�r�d�s                  #*/
/*#########################################*/
#define     IO_WDT_CON          0xb0000000          /*# �E�H�b�`�h�b�O�^�C�}�R���g���[�� #*/
#define     IO_RST_REG          0xb0000004          /*# ���Z�b�g #*/
#define     IO_WDT_CDL          0xb0000008          /*# �E�H�b�`�h�b�O�^�C�}�J�E���^���� #*/
#define     IO_WDT_CDU          0xb000000c          /*# �E�H�b�`�h�b�O�^�C�}�J�E���^��� #*/

/*#########################################*/
/*#           ��������                    #*/
/*#########################################*/
#define     IO_CST              0xb0001000          /*# count start reg #*/
#define     IO_COF              0xb0001004          /*# count of reg #*/
#define     IO_PHS              0xb0001008          /*# phase reg #*/
#define     IO_COU              0xb000100c          /*# count reg #*/
#define     IO_SFT              0xb0001010          /*# shift reg #*/
#define     IO_SEL1             0xb0001014          /*# select1 reg #*/
#define     IO_SEL2             0xb0001018          /*# select2 reg #*/

/*#########################################*/
/*#           �d�b�@�h�^�n                #*/
/*#########################################*/
// �k�t�ł͔ėp�|�[�g�̂ݎg�p
#define     IO_PORT0            0xb0002004          /*# �|�[�g�O #*/
#define     IO_PORT1            0xb0002008          /*# �|�[�g�P #*/
#define     IO_PORT2            0xb000200c          /*# �|�[�g�Q #*/
#define     IO_PORT3            0xb0002010          /*# �|�[�g�R #*/
#define     IO_PORT4            0xb0002014          /*# �|�[�g�S #*/
#define     IO_P4CON            0xb0002018          /*# �|�[�g�S�R���g���[�����W�X�^ #*/

/*#########################################*/
/*#           �r�h�n                      #*/
/*#########################################*/
#define     IO_SIO_LC           0xb000400c          /*# ���C���R���g���[�� #*/
#define     IO_SIO_MC           0xb0004010          /*# ���f���R���g���[�� #*/
#define     IO_SIO_LST          0xb0004014          /*# ���C���X�e�[�^�X #*/
#define     IO_SIO_MST          0xb0004018          /*# ���f���X�e�[�^�X #*/
//2000.09 V2.1�ǉ��istart�j
#define     IO_V2_SIO_BUF       0xb8002000          /*# ����M�o�b�t�@���W�X�^ #*/
#define     IO_V2_SIO_STA       0xb8002004          /*# �X�e�[�^�X���W�X�^ #*/
#define     IO_V2_SIO_CON       0xb8002008          /*# �R���g���[�����W�X�^ #*/
#define     IO_V2_SIO_BCN       0xb800200c          /*# �{�[���[�g�R���g���[�����W�X�^ #*/
#define     IO_V2_SIO_BTC       0xb8002010          /*# �{�[���[�g�^�C�}�J�E���^���W�X�^ #*/
#define     IO_V2_SIO_BT        0xb8002014          /*# �{�[���[�g�^�C�}���W�X�^ #*/
#define     IO_V2_SIO_TCN       0xb8002018          /*# �e�X�g�R���g���[�����W�X�^ #*/
//2000.09 V2.1�ǉ��iend�j

/*#########################################*/
/*#           �s�h�l�d�q�P�^�Q�^�R        #*/
/*#########################################*/
//���kLU�Ή��BV1�p���W�X�^�R�����g�A�E�g�B2003.08.25
#define     IO_TMCON1           0xb8001000          /*# �^�C�}�R���g���[�����W�X�^�P #*/
#define     IO_TMEN1            0xb8001004          /*# �^�C�}�C�l�[�u�����W�X�^�P #*/
#define     IO_TMRLR1           0xb8001008          /*# �^�C�}�����[�h���W�X�^�P #*/
#define     IO_TMC1             0xb800100c          /*# �^�C�}�J�E���^�P #*/
#define     IO_TMOVFR1          0xb8001010          /*# �I�[�o�[�t���[���W�X�^�P #*/
#define     IO_TMCON2           0xb0005000          /*# �^�C�}�R���g���[�����W�X�^�Q #*/
#define     IO_TMEN2            0xb0005004          /*# �^�C�}�C�l�[�u�����W�X�^�Q #*/
#define     IO_TMRLR2           0xb0005008          /*# �^�C�}�����[�h���W�X�^�Q #*/
#define     IO_TMC2             0xb000500c          /*# �^�C�}�J�E���^�Q #*/
#define     IO_TMOVFR2          0xb0005010          /*# �I�[�o�[�t���[���W�X�^�Q #*/
//�u�Q�n�[�h�p�i�s�h�l�d�q�P�͋��ʁA�s�h�l�d�q�Q�^�R�͎��ۂ͖��g�p�j//2000.09 V2.1�ǉ�
//#define       IO_V2_TMCON2        0xb8001020          /*# �^�C�}�R���g���[�����W�X�^�Q #*/
//#define       IO_V2_TMEN2         0xb8001024          /*# �^�C�}�C�l�[�u�����W�X�^�Q #*/
//#define       IO_V2_TMRLR2        0xb8001028          /*# �^�C�}�����[�h���W�X�^�Q #*/
//#define       IO_V2_TMC2          0xb800102c          /*# �^�C�}�J�E���^�Q #*/
//#define       IO_V2_TMOVFR2       0xb8001030          /*# �I�[�o�[�t���[���W�X�^�Q #*/
//#define       IO_V2_TMCON3        0xb8001040          /*# �^�C�}�R���g���[�����W�X�^�R #*/
//#define       IO_V2_TMEN3         0xb8001044          /*# �^�C�}�C�l�[�u�����W�X�^�R #*/
//#define       IO_V2_TMRLR3        0xb8001048          /*# �^�C�}�����[�h���W�X�^�R #*/
//#define       IO_V2_TMC3          0xb800104c          /*# �^�C�}�J�E���^�R #*/
//#define       IO_V2_TMOVFR3       0xb8001050          /*# �I�[�o�[�t���[���W�X�^�R #*/

#define     IO_V2_TMCON1        0xb0005000          /*# �^�C�}�R���g���[�����W�X�^�P #*/
#define     IO_V2_TMEN1         0xb0005004          /*# �^�C�}�C�l�[�u�����W�X�^�P #*/
#define     IO_V2_TMRLR1        0xb0005008          /*# �^�C�}�����[�h���W�X�^�P #*/
#define     IO_V2_TMC1          0xb000500c          /*# �^�C�}�J�E���^�P #*/
#define     IO_V2_TMOVFR1       0xb0005010          /*# �I�[�o�[�t���[���W�X�^�P #*/
#define     IO_V2_TMCON2        0xb0005020          /*# �^�C�}�R���g���[�����W�X�^�Q #*/
#define     IO_V2_TMEN2         0xb0005024          /*# �^�C�}�C�l�[�u�����W�X�^�Q #*/
#define     IO_V2_TMRLR2        0xb0005028          /*# �^�C�}�����[�h���W�X�^�Q #*/
#define     IO_V2_TMC2          0xb000502c          /*# �^�C�}�J�E���^�Q #*/
#define     IO_V2_TMOVFR2       0xb0005030          /*# �I�[�o�[�t���[���W�X�^�Q #*/
#define     IO_V2_TMCON3        0xb0005040          /*# �^�C�}�R���g���[�����W�X�^�R #*/
#define     IO_V2_TMEN3         0xb0005044          /*# �^�C�}�C�l�[�u�����W�X�^�R #*/
#define     IO_V2_TMRLR3        0xb0005048          /*# �^�C�}�����[�h���W�X�^�R #*/
#define     IO_V2_TMC3          0xb000504c          /*# �^�C�}�J�E���^�R #*/
#define     IO_V2_TMOVFR3       0xb0005050          /*# �I�[�o�[�t���[���W�X�^�R #*/

/*#########################################*/
/*#           ���̑�                      #*/
/*#########################################*/
#define     IO_LSIVER           0xb0003000          /*# �k�r�h�o�[�W�������W�X�^ #*/    //2000.09 V2.1�ǉ�
#define     IO_PINCNT           0xb0003004          /*# �k�r�h�[�q�ݒ背�W�X�^ #*/
#define     IO_CLK_PINCNT       0xb000300c          /*# �N���b�N�[�q�ݒ背�W�X�^ #*/
#define     IO_V2_INTRESET      0xb0003020          /*# ���荞�ݗv�����W�X�^ #*/        //2000.10.31 V2.1DEBUG

/*#########################################*/
/*#           �q�d�l�`�o                  #*/
/*#########################################*/
#define     IO_REMAP            0xb8000020          /*# ���}�b�v����A�h���X #*/

/*##################################################################*/
/*#           �ȁ|�k�r�h�O���f�o�C�X                               #*/
/*##################################################################*/
/*########################################################*/
/*#           �O���f�o�C�X                               #*/
/*########################################################*/

#define     IO_DRAM_BASE        0xc0000000          /*# �c�q�`�l�擪�A�h���X #*/
//���kLU�Ή��BDRAM�G���h�ύX�B2003.08.25
//#define       IO_DRAM_LIMIT       0xc0400000          /*# �c�q�`�l�I���A�h���X #*/
#define     IO_DRAM_LIMIT       0xc0a00000          /*# �c�q�`�l�I���A�h���X #*/
#define     IO_RE_DRAM_BASE     0x00000000          /*# ���}�b�v��c�q�`�l�擪�A�h���X #*/

//���kLU�Ή��BDRAM�G���h�ύX�B2003.08.25
//#define       IO_RE_DRAM_LIMIT    0x00400000          /*# ���}�b�v��c�q�`�l�I���A�h���X #*/
#define     IO_RE_DRAM_LIMIT    0x00a00000          /*# ���}�b�v��c�q�`�l�I���A�h���X #*/
#define     IO_MACROM_ADR_V2    0xc803fffc          /*# �l�`�b�A�h���X #*///2003.8�ύX

#define     IO_IPLROM_ADR       0xc8000000          /*# IPL-ROM�A�h���X #*/ //2000.07 V2.1�ǉ�

/*#########################################*/
/*#           �e�k�`�r�g                  #*/       //2000.09 V2.1�ǉ�
/*#########################################*/

#define IO_FLS_BASE     0xc8000000
#define IO_FLS_CMDBASE  (IO_FLS_BASE+0x00040000)//2003.8�ύX

#define IO_FLS_SR       (WORD *)(IO_FLS_CMDBASE+0x00000000)
#define IO_FLS_CR       (WORD *)(IO_FLS_CMDBASE+0x0000fffe)

#define IO_FLS_MAKER    (WORD *)(IO_FLS_CMDBASE+0x00000000)
#define IO_FLS_CMDADRS1 (WORD *)(IO_FLS_CMDBASE+0x0000aaaa)
#define IO_FLS_CMDADRS2 (WORD *)(IO_FLS_CMDBASE+0x00005554)

//���kLU�Ή��BFLASH�A�h���X�ύX�B2003.08.25
#define IO_FLS_HOLDDATA (WORD *)(IO_FLS_BASE+0x00050000)    /*# �ۗ����f�[�^ #*/
//#define   IO_FLS_FPGADATA (WORD *)(IO_FLS_BASE+0x00090000)    /*# FPGA�R���t�B�O�f�[�^ #*/
#define IO_FLS_FPGADATA (WORD *)(IO_FLS_BASE+0x000b0000+0x00060000) /*# FPGA�R���t�B�O�f�[�^ #*///2004.04 IP-CS



/*#########################################*/
/*#           �c�o�q�`�l�i�}�X�^�k�t�j    #*/
/*#########################################*/
#define     IO_PDRAM_PP0        0xf0000000          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP0        0xf0000000          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP0        0xf0000100          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP0       0xf0000102          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP0       0xf0000104          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP0       0xf0000106          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP0       0xf0000108          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP0       0xf000010C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP1        0xf0000200          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP1        0xf0000200          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP1        0xf0000300          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP1       0xf0000302          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP1       0xf0000304          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP1       0xf0000306          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP1       0xf0000308          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP1       0xf000030C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP2        0xf0000400          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP2        0xf0000400          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP2        0xf0000500          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP2       0xf0000502          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP2       0xf0000504          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP2       0xf0000506          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP2       0xf0000508          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP2       0xf000050C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP3        0xf0000600          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP3        0xf0000600          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP3        0xf0000700          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP3       0xf0000702          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP3       0xf0000704          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP3       0xf0000706          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP3       0xf0000708          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP3       0xf000070C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP4        0xf0000800          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP4        0xf0000800          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP4        0xf0000900          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP4       0xf0000902          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP4       0xf0000904          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP4       0xf0000906          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP4       0xf0000908          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP4       0xf000090C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP5        0xf0000a00          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP5        0xf0000a00          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP5        0xf0000b00          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP5       0xf0000b02          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP5       0xf0000b04          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP5       0xf0000b06          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP5       0xf0000b08          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP5       0xf0000b0C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP6        0xf0000c00          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP6        0xf0000c00          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP6        0xf0000d00          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP6       0xf0000d02          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP6       0xf0000d04          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP6       0xf0000d06          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP6       0xf0000d08          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP6       0xf0000d0C          /*# ���Z�b�g�i���C�g�j #*/

#define     IO_PDRAM_PP7        0xf0000e00          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP7        0xf0000e00          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP7        0xf0000f00          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP7       0xf0000f02          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP7       0xf0000f04          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP7       0xf0000f06          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP7       0xf0000f08          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP7       0xf0000f0C          /*# ���Z�b�g�i���C�g�j #*/

//2001.02.20 V2.2DEBUG�istart�j
#define     IO_PDRAM_PP8        0xf0001000          /*# �C�x���g�i���[�h�j #*/
#define     IO_MDRAM_PP8        0xf0001000          /*# �I�[�_�i���C�g�j #*/
#define     IO_PEREG_PP8        0xf0001100          /*# �o�j�f�������i���[�h�j #*/
#define     IO_PCLREG_PP8       0xf0001102          /*# �o�j�f�N���X�i���[�h�j #*/
#define     IO_PPVREG_PP8       0xf0001104          /*# �o�o�o�[�W�����i���[�h�j #*/
#define     IO_MRQREG_PP8       0xf0001106          /*# �I�[�_�Ǐo�v���i���C�g�j #*/
#define     IO_PRQREG_PP8       0xf0001108          /*# �C�x���g�������i���[�h�j #*/
#define     IO_RSTREG_PP8       0xf000110C          /*# ���Z�b�g�i���C�g�j #*/
//2001.02.20 V2.2DEBUG�iend�j

//#define       IO_DMY_WRITE        0xf7f00000          /*# DPRAM�A�N�Z�X�^�C�~���O�p�_�~�[�A�h���X #*/ // 99.10.4
//#define       IO_DMY_WRITE        0xc9000000          /*# DPRAM�A�N�Z�X�^�C�~���O�p�_�~�[�A�h���X #*/ // PRIT�w���XNG�΍� 99.10.4
//#define       IO_DMY_WRITE        0xd0000000          /*# DPRAM�A�N�Z�X�^�C�~���O�p�_�~�[�A�h���X #*/ //2000.09 V2.1�ύX
#define     IO_DMY_WRITE        0xd00000a4          /*# DPRAM�A�N�Z�X�^�C�~���O�p�_�~�[�A�h���X #*/ //2004.04 IP-CS

/*#########################################*/
/*#           ���̑��i�}�X�^�k�t�j        #*/
/*#########################################*/
#define     IO_ALM_INF          0xf0001800          /*# ��Q��񃌃W�X�^ #*/
#define     IO_CLK_SEL          0xf0001800          /*# �b�k�j�ؑփ��W�X�^ #*/
#define     IO_LUNUM01          0xf0002000          /*# �k�t�ԍ��i�P�ʁj���W�X�^ #*/
#define     IO_LUNUM10          0xf0002800          /*# �k�t�ԍ��i�P�O�ʁj���W�X�^ #*/
#define     IO_SW_INF           0xf0003000          /*# �r�v��񃌃W�X�^ #*/
#define     IO_BZ_CTRL          0xf0003000          /*# �u�U�[�R���g���[�����W�X�^ #*/
#define     IO_RST_SLV          0xf0003800          /*# ���Z�b�g���W�X�^ out #*/            // 99.10.15
#define     IO_MODE             0xf0003800          /*# ���[�h���W�X�^ in #*/               // 2000.06.14 V1.2 �ǉ�
#define     IO_WRT_PRO          0xf0004000          /*# ���C�g�v���e�N�g���W�X�^ out #*/    // 99.10.15
#define     IO_VLAN_ID          0xf0004000          /*# �u�k�`�m�|�h�c���W�X�^ in #*/       // 2000.06.14 V1.2 �ǉ�
#define     IO_HARD_VER         0xf0004800          /*# �n�[�h�E�F�A�o�[�W�������W�X�^ #*/  // 2000.05.11 V1.2 �ǉ�

//�悭�킩��Ȃ��A�h���X
#define     IO_SIOSEL           0xf0005800          /*# �r�h�n�Z���N�^���W�X�^ #*/          // 2000.09 V2.1�ǉ�
//���kLU�Ή��BI/O�}�b�v�ύX�B�ȉ��폜�B2003.08.25
//#define       IO_FROM_INI         0xf0006000          /*# FROM���W�X�^ #*/                    // 2000.09 V2.1�ǉ�


#define     IO_LEDSEL       0xd000009c          /*# LED�\���ؑփ��W�X�^ #*/ //IP-CS�Ή��ǉ� 2004.04
#define     IO_RSTHLD       0xd00000a0          /*# ���Z�b�g�ێ����W�X�^ #*/    //IP-CS�Ή��ǉ� 2004.04

//�r�b�g�A�T�C��
#define     LU_LED_SEL          0x0001          /*# LU_LED�\��(1)/CS_LED�\��(0) #*/ //2004.04 IP-CS�ǉ�
#define     RST_HLD         0x0001              /*# �ێ烂�[�h�N��(1)/�ʏ�N��(0)(�����l) #*///2004.04 IP-CS�ǉ�
#define     IO_INITSW       0x0008          /*# �ێ�SW ON(1)/OFF(0)(�ʏ탂�[�h) #*/     //2004.04 IP-CS�ǉ�



/*# ���[�h���W�X�^�r�b�g�A�T�C�� #*/
#define     IO_MODE_VLAN        0x0001              /*# VLAN�ݒ� #*/    //�l�ύX�s���n�[�h�d�l�ύX�s��
#define     IO_MODE_REMOTE      0x0002              /*# ���u�ݒ� #*/
#define     IO_MODE_AUTONEGO    0x0004              /*# AUTO-NEGO�ݒ� #*/
#define     IO_MODE_NEGOMODE    0x0018              /*# �����l�S�V�G�[�V�������[�h�ݒ� #*/
#define     IO_MODE_CCUID       0x0060              /*# CCU���ʎq�ݒ� #*/
#define     IO_MODE_MNT         0x0080              /*# �ێ烂�[�h�ݒ� #*/

/*# �}�X�N�f�[�^ #*/
//#define       LU_IAM_BIT          0x0002              /*# �h�`�l���ڏ�� PORT4.1 #*/      //2001.02.13 V2.2DEBUG
#ifdef      V2_1_DBG_ECHOEX
#define     LU_ECHO_HWSEL       0x0000              /*# �q�b�|�o�j�f����(0)/������(1)��� �X���[�uPORT4.1#*/    //�f�o�b�N�p
#else   //V2_1_DBG_ECHOEX
#define     LU_ECHO_HWSEL       0x0002              /*# �q�b�|�o�j�f����(0)/������(1)��� �X���[�uPORT4.1#*/    //2000.12.08 V2.2�ǉ�
#endif  //V2_1_DBG_ECHOEX
//#define       LU_JPUS_BIT         0x0040              /*# �k�t����� PORT4.2 #*/
#define     LU_JPUS_BIT         0x0004              /*# �k�t����� PORT4.2 #*/              //2000.09.06 �C��
#define     LU_TYPE_MSK         0x0008              /*# �k�t�}�X�^�E�X���[�u PORT4.3 #*/

#define     LU_CHD_BIT          0x0020              /*# �`���l�����䐔 PORT1.5 #*/
#define     LU_PBREC_BIT        0x0010              /*# �o�a�Ď����Ԏw�� PORT1.4 #*/
//#define       LU_PKG_MSK          0x000f              /*# �X���b�g�� PORT1.0_3 #*/
#define     LU_PKG_MSK          0x0007              /*# �X���b�g�� PORT1.0_3 #*///IP-CS 2004.04
#define     LU_SLAVE_MASK       0x000f              /*# �X���[�u�ԍ� PORT1.0_3 #*/
#define     LU_CHD_MSK          0x0010              /*# �`���l�����䐔 PORT1.4 #*/

#define     LU_UD_MSK           0x000f              /*# �k�t�ԍ��i�P�O�ʁj #*/
#define     LU_LD_MSK           0x000f              /*# �k�t�ԍ��i�P�ʁj #*/

//���kLU�Ή��BI/O�}�b�v�ύX�B�ȉ��폜�B2003.08.25
#define     IO_LSIVER_MASK      0x00000fff          /*# �ȃo�[�W�����}�X�N #*/              //2000.09 V1.2�ǉ�



/*#########################################*/
/*#           �t�@�C���֘A                #*/
/*#########################################*/
#define     BOOT_DATA_SIZE      1024                /*# �h�o�k�u�[�g�f�[�^�̈�T�C�Y #*/
#define     LU_F_TYP_OFST       0x00000100          /*# �t�@�C�����ʏ��I�t�Z�b�g #*/
#define     LU_F_END_OFST       0x00000110          /*# �t�@�C���I�����I�t�Z�b�g #*/
#define     LU_F_EXE_OFST       0x00000118          /*# �t�@�C�����s�Ԓn�I�t�Z�b�g�i�ύX�s�j #*/
#define     LU_FILE_SIZE        0x00100000          /*# �ő�_�E�����[�h�k�t�t�@�C���T�C�Y�i�P�l�܂Łj #*/
#define     LU_OAK_SIZE         0x00008000          /*# �n�`�j�t�@�C���T�C�Y #*/                    //2000.08 V2.1�ǉ�
//#define       LU_V3OAK_SIZE       0x00080000          /*# �O�t���n�`�j�t�@�C���T�C�Y #*/              //2003.08�ǉ�
#define     LU_V3OAK_SIZE       0x00078000          /*# �O�t���n�`�j�t�@�C���T�C�Y #*/              //2004.05 IP-CS �ύX
                                                    /*# �_�E�����[�h�k�t�t�@�C���T�C�Y�iLU+OAK�j #*///2000.07 V2.1����  //2000.09 V2.1�ύX
#define     LU_DL_SIZE          (0x00060000+0x00020000+LU_OAK_SIZE+LU_V3OAK_SIZE)//2004.05 IP-CS �ύX

//���kLU�Ή��B�����N�t�@�C���ύX�ɂ��킹�ĕۗ����̐擪�A�h���X�ύX�B2003.08.25
//#define       LU_CLR_LIMIT        0x00330000          /*# �O�N���A�Ώۃ��������~�b�g�i�����N�t�@�C���ƈ�v������I�j #*/  // 2001.08.07
#define     LU_CLR_LIMIT        0x00830000          /*# �O�N���A�Ώۃ��������~�b�g�i�����N�t�@�C���ƈ�v������I�j #*/  // 2001.08.07

#define     LU_V2_CPUSTACK_SIZE 0x00002000          /*# �b�o�t�g�p�X�^�b�N�T�C�Y #*/                //2001.02.19 V2.2DEBUG

#ifdef      MAKE_IPL

#ifdef      MAKE_IPL_ROM
#define     LU_FILE_TOP         IO_DRAM_BASE        /*# �t�@�C���_�E�����[�h�擪�A�h���X #*/
#else
#define     LU_FILE_TOP         0xc0350000          /*# �h�b�d�f�o�b�O���A�h���X #*/
#endif      // MAKE_IPL

#else       // MAKE_IPL_ROM
#define     LU_FILE_TOP         IO_RE_DRAM_BASE     /*# �t�@�C���擪�A�h���X #*/
#endif      // MAKE_IPL_ROM

#define     LU_FILE_TYPE        (LU_FILE_TOP + LU_F_TYP_OFST)   /*# �t�@�C�����ʏ��A�h���X #*/
#define     LU_FILE_END         (LU_FILE_TOP + LU_F_END_OFST)   /*# �t�@�C���I�����A�h���X #*/
#define     LU_FILE_EXE         (LU_FILE_TOP + LU_F_EXE_OFST)   /*# �t�@�C�����s�Ԓn���A�h���X�i�ύX�s�j #*/

#define LU_VER_ADDR         0x180                               /*# �h�o�k�o�[�W�����i�[�̈�A�h���X #*/    //2000.07 V2.1�ǉ�
#define LU_DATE_ADDR        0x190                               /*# ���t�i�[�̈�A�h���X #*/                //2001.01.29 V2.2�ǉ�
#define IPL_VER_ADDR        (LU_VER_ADDR+IO_IPLROM_ADR)         /*# �h�o�k�o�[�W�����i�[���A�h���X #*/      //2000.07 V2.1�ǉ�
#define ONL_VER_ADDR        (LU_VER_ADDR+IO_DRAM_BASE)          /*# �I�����C���o�[�W�����i�[���A�h���X #*/  //2000.07 V2.1�ǉ�

// �����q�`�l
#ifdef  WDT_STOP
//#define       WDT_DMY_ADR         0x10001fa0          // �E�H�b�`�h�b�O�_�~�[�A�N�Z�X�i�f�o�b�O�p�j
//#define       WDT_DMY_ADR         0x10001f60          // �E�H�b�`�h�b�O�_�~�[�A�N�Z�X�i�f�o�b�O�p�j   //2000.09 V2.1�ύX
//#define       WDT_DMY_ADR_V2      0x00101f60          // �E�H�b�`�h�b�O�_�~�[�A�N�Z�X�i�f�o�b�O�p�j   //2000.09 V2.1�ǉ�
#define     WDT_DMY_ADR         0xd0000000          // �E�H�b�`�h�b�O�_�~�[�A�N�Z�X�i�f�o�b�O�p�j   //2000.09 V2.1�ύX
#define     WDT_DMY_ADR_V2      0xd0000000          // �E�H�b�`�h�b�O�_�~�[�A�N�Z�X�i�f�o�b�O�p�j   //2000.09 V2.1�ǉ�
#endif
//#define       LU_EXEC_DATA        0x10001fb0          /*# �k�t�N�����A�h���X�i64bytes�j #*/
#define     LU_EXEC_DATA        0x10001f70          /*# �k�t�N�����A�h���X�i128bytes�j #*/        //2000.09 V2.1�ύX
#define     LU_EXEC_DATA_V2     0x00101f70          /*# �k�t�N�����A�h���X�i128bytes�j #*/        //2000.09 V2.1�ǉ�
// �H�ꎎ���p
#define     TP_DRAM_CHK         0x10001ff0          /*# �H�ꎎ��DRAM�`�F�b�N���ʃA�h���X #*/        //2000.09 V2.1�ύX
#define     TP_DRAM_CHK_V2      0x00101ff0          /*# �H�ꎎ��DRAM�`�F�b�N���ʃA�h���X #*/        //2000.09 V2.1�ǉ�


/*        Copyright (C) 1999 Oki Electric Industry Co.,Ltd.        */

