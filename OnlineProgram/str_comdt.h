typedef struct  T_SLOT_INF{
    BYTE        slot_status;                        /*# �X���b�g��� #*/
    BYTE        pkg_inf;                            /*# �}��PKG��� #*/
    BYTE        dpram_adr;                          /*# �擪�A�h���X    #*/
    BYTE        dpram_size;                         /*# DPRAM�T�C�Y     #*/
    BYTE        q_id;                               /*# ���b�Z�[�W�L���[ID #*/
    BYTE        door_scn;                           /*# �h�A�z���X�L�����v�� #*/
    BYTE        helilg_flg;                         /*# �w���X�`�F�b�N�C���[�K���t���O #*/
    BYTE        pkg_id;                             /*# �p�b�P�[�W��� #*/  // 99.11.01
    BYTE        pkg_ver;                            /*# �p�b�P�[�W�o�[�W���� #*/    // 99.11.01
    BYTE        slot_pre_status;                    /*# �O��X���b�g��� #*/            //LU-99.7.29
    WORD        door_pre_data;                      /*# �O��h�A�z���X�L������� #*/    //LU-99.7.29
    }SLOT_INF;

typedef struct  T_DOWN_LOAD{
    BYTE        crint;                              /*# �����K��������  #*/
    BYTE        law;                                /*# A�|Low/�ʁ|Low�w��  #*/
    BYTE        iptype;                             /*# IPv4/IPv6�w��   #*/
    BYTE        mult_ttl;                           /*# �����p�P�b�gTTL�l #*/
    DWORD       multi_addr[4];                      /*# �}���`�L���X�g�A�h���X  #*/
    WORD        port1;                              /*# �����p�P�b�g�p�|�[�g    #*/
    WORD        port2;                              /*# CR-INT�p�P�b�g�p�|�[�g  #*/
    WORD        u_port1;                            /*# ���j�L���X�g����CLKD�p�|�[�g�ԍ� #*/    // 2000.07  V2.1 �ǉ�
    WORD        u_port2;                            /*# ���j�L���X�g����CRINT�p�|�[�g�ԍ� #*/   // 2000.07  V2.1 �ǉ�
    DWORD       clk_addr[4];                        /*# �����p�P�b�g���M���A�h���X #*/
    BYTE        yuragi;                             /*# ��炬�ݒ�l(�P�`�T)(V1) #*/
    BYTE        buffer;                             /*# �o�b�t�@�ʐ��w��(�P�`�T)(V1) #*/
    BYTE        clkdsync;                           /*# ���������w�� #*/    // 2000.05.11  V1.2 �ǉ�
    BYTE        tftp;                               /*# �ۗ����^�g�[�L�[�_�E�����[�h���� #*/    //2001.02.28 V2.2�ǉ�
    WORD        ccuver;                             /*# �b�b�t�o�[�W���� #*/
    WORD        dmy1;                               /*#  #*/
    }LU_DWNLOAD;

typedef struct  T_DPDWNLOAD{
    BYTE        pkg_code;                           /*# PKG���     #*/
    BYTE        dpram_adr;                          /*# �擪�A�h���X    #*/
    BYTE        dpram_size;                         /*# DPRAM�T�C�Y     #*/
    BYTE        helilg_flg;                         /*# �w���X�`�F�b�N�C���[�K���t���O #*/
    }DP_DWNLOAD;

typedef struct  T_LU_NETWORK{                       /*# �l�b�g���[�N�f�[�^ #*/
    BYTE        macaddr[6];                         /*# LU MAC�A�h���X #*/
    DWORD       yiaddr;                             /*# LU IP�A�h���X #*/
    DWORD       siaddr;                             /*# CCU IP�A�h���X#*/
    DWORD       fiaddr;                             /*# FILE�T�[�o IP�A�h���X #*/   //2000.07 V2.1 �ǉ�
    DWORD       giaddr;                             /*# �Q�[�g�E�G�CIP�A�h���X #*/
    DWORD       subnet_msk;                         /*# �T�u�l�b�g�}�X�N #*/
    DWORD       tag_a;                              /*# �u�k�`�m�^�O�` #*/
    DWORD       tag_b;                              /*# �u�k�`�m�^�O�a #*/
    DWORD       tag_c;                              /*# �u�k�`�m�^�O�b #*/
    DWORD       tag_d;                              /*# �u�k�`�m�^�O�c #*/
    DWORD       ccu1_ip;                            /*# �b�b�t�P�A�h���X #*/                    // 2001.08 V2.3 �ǉ�
    DWORD       ccu2_ip;                            /*# �b�b�t�Q�A�h���X #*/                    // 2001.08 V2.3 �ǉ�
    DWORD       ccu3_ip;                            /*# �b�b�t�R�h���X #*/  //2004.08 SS9100�Ή��ǉ�
    DWORD       act_ccu;                            /*# �`�b�s �b�b�t�A�h���X #*/               // 2001.08 V2.3 �ǉ�
}LU_NETWORK;

typedef struct  T_COM_DATA{
    /*# �}�X�^/�X���[�u���� #*/
    DWORD       lu_cabinet;                         /*# �k�t�L���r�l�b�g�^�C�v 0/1:BS/EX #*/
    DWORD       dpram_num;                          /*# DPRAM������  #*/
    BYTE        tag_ctl;                            /*# TAG�ݒ�(����F1�A�����F�O) #*/
    BYTE        clkd_ins;                           /*# CLKD-INS���(INS�F1�AOUS�F�O) #*/
    BYTE        lu_type;                            /*# LU�^�C�v (KP4.3�F�}�X�^=0�^�X���[�u=1 #*/
//  BYTE        lu_number;                          /*# �k�t���u�ԍ� #*/
    WORD        lu_number;                          /*# �k�t���u�ԍ� #*///SS9100�Ή��g�� 2004.08
    BYTE        clkd_type;                          /*# CLKD�^�C�v (�ۯ�Ͻ�=0�^�ۯ��ڰ��=1) #*/
    BYTE        slv_number;                         /*# �X���[�u�ԍ� (KP1.0�`3�F�}�X�^=0(?)�^�X���[�u=1(0)�`16(15)) #*/
    BYTE        ins_pkgnum;                         /*# ����PKG�� (PORT�X���b�g��) #*/
    BYTE        ins_chnum;                          /*# ����ch�� (KP1.4�F4(0)�^8(1)) �A��V1��4ch�Œ� #*/
    BYTE        chsta_num;                          /*# �g�p�擪CH�ԍ� (KP1.0�`3�F�X���[�u=0(0)�`15(15)) #*/
    BYTE        country;                            /*# �����(V1)(KP4.2:JAPAN(0)�^USA(1)) #*/
    BYTE        syn_ptn;                            /*# ��������p�^�[�� #*/                // 2000.05.12 V1.2 �ǉ�
    BYTE        hard_ver;                           /*# �n�[�h�E�F�A�o�[�W������� #*/      // 2000.05.12 V1.2 �ǉ�
    BYTE        crint_trg;                          /*# �b�q�h�m�s�������v�� #*/            // 2000.05.12 V1.2 �ǉ�
    BYTE        clkd_ins_chg;                       /*# CLKD�����ω���� #*/                // 2000.05.20 V1.2 �ǉ�
    BYTE        subnet_master;                      /*# �T�u�l�b�g�}�X�^�w�� #*/            // 2000.07 V2.1�ύX
    BYTE        remote;                             /*# ���u�ݒu��� #*/                    // 2000.12.26 V2.2�ǉ�
    BYTE        mnt;                                /*# �ێ烂�[�h��� #*/                  // 2000.12.26 V2.2�ǉ�
    BYTE        slv_mnt;                            /*# �X���[�u�ȕێ烂�[�h #*/            // LU32B 
    BYTE        ccu_id;                             /*# CCU���ʎq�iMST:DIPSW�ASLV:SIO�j #*/ // 2000.12.26 V2.2�ǉ�
    BYTE        tos_a;                              /*# TOS A #*/                           // 2000.12.27 V2.2�ǉ�
    BYTE        tos_b;                              /*# TOS B #*/                           // 2000.12.27 V2.2�ǉ�
    BYTE        tos_c;                              /*# TOS C #*/                           // 2000.12.27 V2.2�ǉ�
    BYTE        tos_d;                              /*# TOS D #*/                           // 2000.12.27 V2.2�ǉ�
    BYTE        dmy[1];                             /*#  #*/
    WORD        kappa_ver;                          /*# �Ȃk�r�h�o�[�W���� #*/              // 2000.09 V2.1�ǉ�
    WORD        hel_tim;                            /*# �w���X�`�F�b�N�^�C�}�J�E���g #*/    // 2000.12.27 V2.2�ǉ�
    LU_DWNLOAD  lu_download;                        /*# �_�E�����[�h�f�[�^ #*/          // 2000.07 V2.1�ύX
    LU_NETWORK  lu_network;                         /*# �l�b�g���[�N�f�[�^ #*/          // 2000.07 V2.1�ύX
//  DWORD       voice_start[4];                     /*# �����f�R�[�h�J�n�o�b�t�@�ʐ� #*/// 2000.01.16   //2000.07 V2.1�ύX
    DWORD       voice_start[CNMAX];                     /*# �����f�R�[�h�J�n�o�b�t�@�ʐ� #*/// 2000.01.16   //2000.07 V2.1�ύX
//  DWORD       voice_stop[4];                      /*# �����f�[�^�j���o�b�t�@�ʐ� #*/  // 2000.01.16   //2000.07 V2.1�ύX
    DWORD       voice_stop[CNMAX];                      /*# �����f�[�^�j���o�b�t�@�ʐ� #*/  // 2000.01.16   //2000.07 V2.1�ύX
    /*# LU�}�X�^��� #*/
    SLOT_INF    slot_inf[DPRAM_MAX];                /*# �X���b�g���(V1) #*/
    DP_DWNLOAD  dpram_data[256];                    /*# DPRAM�_�E�����[�h�f�[�^ #*/
    BYTE        clkd_noinsevt;                      /*# CLKD�������C�x���g�ʒm��� #*/                  // 2000.09 V2.1�ǉ�
    BYTE        dl_ord_bc;                          /*# �_�E�����[�h�I�[�_�T�C�Y�i�b��j #*/            // 2000.09 V2.1�ǉ�
    BYTE        flash_write;                        /*# �e�k�`�r�g���������� #*/                        // 2001.01.19 V2.2�ǉ�
    BYTE        obs_notify;                         /*# ��Q�ʒm�v�� #*/ //LU32B
    DWORD       lu_slv_restart;                     /*# �X���[�u���X�^�[�g�v��0/1�Ȃ��^���� #*/         // 2001.1.18 �ǉ�
    /*#LU�X���[�u��� #*/
//  BYTE        *horyud_ptr;                        /*# �Đ��������t�@�C���ۗ����f�[�^�L���ʃ|�C���^ #*/
//  BYTE        *horyud_ptr[4];                     /*# �Đ��������t�@�C���ۗ����f�[�^�L���ʃ|�C���^ #*///2001.01.17 V2.2�ύX
    BYTE        *horyud_ptr[CNMAX];                 /*# �Đ��������t�@�C���ۗ����f�[�^�L���ʃ|�C���^ #*///2004.01.19 �ύX
//  WORD        horyu_rdcnt;                        /*# �ۗ����f�[�^���[�h�J�E���^ #*/
//  WORD        horyu_rdcnt[4];                     /*# �ۗ����f�[�^���[�h�J�E���^ #*/                  //2001.01.17 V2.2�ύX
    WORD        horyu_rdcnt[CNMAX];                 /*# �ۗ����f�[�^���[�h�J�E���^ #*/              //2004.01.19 �ύX
//  WORD        horyu_rdmax;                        /*# �ۗ����f�[�^���[�h�l�`�w#*/
//  WORD        horyu_rdmax[4];                     /*# �ۗ����f�[�^���[�h�l�`�w#*/                     //2001.01.17 V2.2�ύX
    WORD        horyu_rdmax[CNMAX];                 /*# �ۗ����f�[�^���[�h�l�`�w#*/                     //2004.01.19 �ύX
//  DWORD       horyu_act;                          /*# �ۗ����f�[�^�A�N�g�� #*/                        //2001.01.17 V2.2�폜
//  DWORD       chunk_size;                         /*# �ۗ����f�[�^�T�C�Y #*/                          //2001.01.17 V2.2�폜
    DWORD       horyu_chunk_size;                   /*# �ۗ����f�[�^�T�C�Y #*/                          //2001.01.17 V2.2�ǉ�
    DWORD       talky_chunk_size;                   /*# �g�[�L�[�f�[�^�T�C�Y #*/                        //2001.01.17 V2.2�ǉ�
    DWORD       silent_data[VO_1FRM_DWORD];         /*# �����f�[�^ #*/
    BYTE        *horyu_data_top;                    /*# �ۗ����t�@�C���f�[�^�Đ��J�n�ʒu #*/            //2001.01.31 V2.2�ǉ�
    BYTE        *talky_data_top;                    /*# �g�[�L�[�t�@�C���f�[�^�Đ��J�n�ʒu #*/          //2001.01.31 V2.2�ǉ�
    BYTE        silent_ptn;                         /*# �����p�^�[�� #*/
    BYTE        dsp_ch;                             /*# DSP����PP�`�����l���� #*/                       // 2000.08 V2.1�ǉ�
    WORD        voice_acm_cnt[CNMAX];               /*# �����f�[�^�~�σJ�E���^ #*/
    DWORD       timer_cnt;                          /*# �l�h�j�p�^�C�}�J�E���g #*/
    WORD        sndok_flg;                          /*# ���MOK�t���O #*/
    WORD        snd_cnt;                            /*# DSP�������M�� #*/
    WORD        sndstop_flg;                        /*# DSP�����t���[���䒆�t���O 0/1:���M�^��~�� #*/
    WORD        io_bz_ctrl;                         /*# �޻ް���۰�ڼ޽����޴ر(V1) #*/
    WORD        io_port0;                           /*# �|�[�g�O�o�̓C���[�W #*/
//  WORD        dsp_rdng_cnt;                       // LU 99.8.9    2001.03.16���O�Ǘ��Ɉړ�
//  WORD        dsp_sndng_cnt;                      // LU 99.9.9    2001.03.16���O�Ǘ��Ɉړ�
    WORD        online;                             // LU 99.8.9
    WORD        rtp_sndseq[CNMAX];
    WORD        rtp_rcvseq[CNMAX];
    DWORD       rtp_time[CNMAX];
//  DWORD       rtp_rcvng1[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvng2[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvng3[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvng4[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvng5[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvng6[CNMAX];                  //  2001.03.16���O�Ǘ��Ɉړ�
//  DWORD       rtp_rcvok[CNMAX];                   //  2001.03.16���O�Ǘ��Ɉړ�
    volatile DWORD  time_log_cnt;                   /*# ���O���W�p�T�������ԏ�� #*/
//���kLU�Ή� 2003.12.12
    WORD        silent_set;                         /*# �����p�^�[���Z�b�g�t���O #*/
//IP-CS�Ή� 2004.04
    BYTE        led_flg;                        /*# ��Q��LED�_�Ńt���O #*/
    BYTE        tftp_ing;                           /*# TFTPH���쒆�t���O #*/
    BYTE        reset;                              /*# TFTP���쒆�̃��Z�b�g�w���ێ��G���A #*/
//SS9100(PH1�ĊJ)�Ή� 2006.06
    BYTE        ph1_flg;                    /*PH1�ĊJ���t���O*/
    BYTE		ph1_dpram_err;				/*PH1�ݒ背�W�X�^�������ُ݈�t���O*/

}COM_DATA;

/*# FLASH���Ǘ��\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    BYTE        lu_num;             /*# �k�t�ԍ� #*/
    BYTE        lsi_num;            /*# �k�r�h�ԍ� #*/
    BYTE        dummy[2];           /*# �_�~�[ #*/
    DWORD       ip;                 /*# �h�o�A�h���X #*/
}LU_SIPINF; //8byte

/*# FLASH���F�Ǘ��f�[�^�\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    BYTE        vlan_id;                /*# �u�k�`�m�|�h�c #*/
    BYTE        class_a;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�` #*/
    BYTE        class_b;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�a #*/
    BYTE        class_c;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�b #*/
    BYTE        class_d;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�c #*/
    BYTE        ccu_id;                 /*# �b�b�t���ʎq #*/
    BYTE        ccu_ver[2];             /*# �b�b�t�o�[�W���� #*///�����܂�28�o�C�g
    DWORD       yiaddr;                 /*# ���h�o�A�h���X #*/
    DWORD       ipmask;                 /*# �T�u�l�b�g�}�X�N #*/
    DWORD       giaddr;                 /*# �Q�[�g�E�F�C�A�h���X #*/
    DWORD       siaddr;                 /*# �b�b�t�h�o�A�h���X #*/
    DWORD       siaddr2;                /*# �b�b�t�h�o�A�h���X�Q #*/
    LU_SIPINF   sip_inf[SLV_MAX];       /*# �X���[�u�Ȃh�o��� #*/  //8*16=128byte
    WORD        lu_number;          /*# LU�ԍ� #*/  //2004.01
    BYTE        dipsw;                  /*# DIPSW��� #*/   //IP-CS�Ή� 2004.04
    BYTE        extension;              /*# �g����� #*/    //IP-CS�Ή� 2004.04
    WORD        vlan_id_W;              /*# (WORD)�u�k�`�m�|�h�c #*///SS9100�Ή� 2004.08
//  WORD        lu_number_W;            /*# (WORD)LU�ԍ� #*///SS9100�Ή� 2004.08
    WORD        reserve;            /*#  lu_number_W�p�~ #*///SS9100�Ή� 2004.11.09
    DWORD       siaddr3;                /*# �b�b�t�h�o�A�h���X3 #*///SS9100�Ή� 2004.08
    BYTE        dummy[88];              /*# �_�~�[ #*/
}COM_FLS_DATA;  //28+128+100=256

/*# FLASH���F�Ǘ��f�[�^�ݒ��ԍ\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    BYTE        vlan_id;                /*# �u�k�`�m�|�h�c #*/
    BYTE        class_a;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�` #*/
    BYTE        class_b;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�a #*/
    BYTE        class_c;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�b #*/
    BYTE        class_d;                /*# �u�k�`�m�v���C�I���e�B�b�k�`�r�r�|�c #*/
    BYTE        ccu_id;                 /*# �b�b�t���ʎq #*/
    BYTE        ccu_ver;                /*# �b�b�t�o�[�W���� #*/
    BYTE        yiaddr;                 /*# ���h�o�A�h���X #*/
    BYTE        ipmask;                 /*# �T�u�l�b�g�}�X�N #*/
    BYTE        giaddr;                 /*# �Q�[�g�E�F�C�A�h���X #*/
    BYTE        siaddr;                 /*# �b�b�t�h�o�A�h���X #*/
    BYTE        siaddr2;                /*# �b�b�t�h�o�A�h���X�Q #*/
    BYTE        sip_inf[SLV_MAX];       /*# �X���[�u�Ȃh�o��� #*/  //16byte
    BYTE        lu_number;          /*# LU�ԍ� #*/      //2004.01
    BYTE        dipsw;                  /*# DIPSW��� #*/   //IP-CS�Ή� 2004.04
    BYTE        extension;              /*# �g����� #*/    //IP-CS�Ή� 2004.04
    BYTE        vlan_id_W;              /*# (WORD)�u�k�`�m�|�h�c #*///SS9100�Ή� 2004.08
//  BYTE        lu_number_W;            /*# (WORD)LU�ԍ� #*///SS9100�Ή� 2004.08
    BYTE        reserve;            /*#  lu_number_W�p�~ #*///SS9100�Ή� 2004.11.09
    BYTE        siaddr3;                /*# �b�b�t�h�o�A�h���X3 #*///SS9100�Ή� 2004.08
    BYTE        dummy[30];              /*# �_�~�[ #*/
}COM_FLS_INFO;  //12+16+36=64byte

/*# FLASH�t�@�C���Ǘ��\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    DWORD       lu_size;                /*# �s�e�s�o��M�k�t�t�@�C���T�C�Y #*/
    BYTE        lu_fn[256];             /*# �k�t�I�����C���t�@�C���� #*/                //2001.01.17 V2.2�ǉ�
    BYTE        hold_fn[256];           /*# �ۗ����t�@�C���� #*/                        //2001.01.17 V2.2�ǉ�
    BYTE        talky_fn[256];          /*# �g�[�L�[�t�@�C���� #*/                      //2001.01.17 V2.2�ǉ�
}COM_FLS_FMNG;

/* FLASH�_�E�����[�h�t�@�C���Ǘ��\����V3�p #*/
typedef struct {    //2002.10 V3�ǉ�
    BYTE        fpga_fn[256];           /*# FPGA�t�@�C���� #*/
    BYTE        tone_fn[256];           /*# �g�[��ROM�t�@�C���� #*/
    BYTE        inter_fn[256];          /*# �C���^ROM�t�@�C���� #*/
    BYTE        fpga_ver[4];            /*# FPGA�t�@�C���o�[�W������� #*/
    BYTE        tone_ver[4];            /*# �g�[��ROM�t�@�C���o�[�W������� #*/
    BYTE        inter_ver[4];           /*# �C���^ROM�t�@�C���o�[�W������� #*/
    DWORD       fpga_sum;               /*# FPGA�t�@�C��SUM�l #*/
    DWORD       tone_sum;               /*# �g�[��ROM�t�@�C��SUM�l #*/
    DWORD       inter_sum;              /*# �C���^ROM�t�@�C��SUM�l #*/
    
}COM_FLS_V3FMNG;

/*# FLASH���\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    COM_FLS_DATA    data;               /*# �f�[�^�� #*/
    COM_FLS_INFO    info;               /*# ��� #*/
    COM_FLS_FMNG    fmng;               /*# �t�@�C���Ǘ� #*/
    BYTE            comment[128];       /*# �R�����g #*/
    COM_FLS_V3FMNG  v3fmng;             /*# V3�t�@�C���Ǘ� #*/  //2002.10 V3�ǉ�
}COM_FLSDATA;

/*# FLASH����ԍ\���� #*/
typedef struct {    //2000.09 V2.1�ǉ�
    BYTE        data;                   /*# �Ǘ��f�[�^��� #*/
    BYTE        sys;                    /*# �V�X�e���t�@�C����ԁi�c�r�o�v���O�����܂ށj #*/
    BYTE        hory;                   /*# �ۗ������ #*/
//  BYTE        dmy;                    /*#  #*/
    BYTE        talky;                  /*# �g�[�L�[ #*/
    BYTE        cadata;                 /*# CA�؂�ւ�����ԁFIP-CS(SS9100_PH1�ĊJ) �ǉ� #*/
}COM_FLSDATA_STS;

/*# TOS�ݒ�p�\�P�b�g�f�B�X�N���v�^�\���� #*/
typedef struct {    //2001.01.16 V2.2�ǉ�
    DWORD       tftpsrv;                /*# �s�e�s�o�T�[�o #*/
    DWORD       bootpsrv;               /*# �a�n�n�s�o�T�[�o #*/
    DWORD       clkd_mlt;               /*# �b�k�j�c�}���`�L���X�g #*/
    DWORD       clkd_uni;               /*# �b�k�j�c���j�L���X�g #*/
    DWORD       clkd_phs;               /*# �b�k�j�c �o�g�r #*/
    DWORD       doep;                   /*# �c�n�d�o #*/
    DWORD       crint_mlt;              /*# �b�q�h�m�s�}���`�L���X�g #*/
    DWORD       crint_uni;              /*# �b�q�h�m�s���j�L���X�g #*/
    DWORD       lulog;                  /*# �k�t���O #*/
}COM_TOS_SD;

/*# FLASH���\���́FCA�؂�ւ���� #*/
typedef struct {  // IP-CS(SS9100_PH1�ĊJ) �ǉ�
    BYTE        serv_id;                /*# �ڑ��掯�ʎq #*/
    BYTE        surv_chg_time;          /*# �T�o�C�o���؂�ւ��^�C�} #*/
    BYTE        surv_ret_time;          /*# �T�o�C�o���؂�߂��^�C�} #*/
    BYTE        connect_time;           /*# �R�l�N�V�����Đڑ��^�C�} #*/
    BYTE        reserve[12];            /*# �\�� #*/
}FLS_CA_DATA;
