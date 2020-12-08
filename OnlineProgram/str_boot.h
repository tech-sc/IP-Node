/*######## �u�[�g��� ############################*/

typedef	struct {	//2000.09 V2.1�ύX�i�؂�o���j
	DWORD   v_magic;					/*# �}�W�b�N�N�b�L�[ #*/
	BYTE    v_fields[60];				/*# �x���_�̈� #*/
} BOOTP_VEND;

typedef	struct	{	//2000.09 V2.1�ύX�i�؂�o���j
	BYTE		btp_op;						/*# �I�y���[�V���� #*/
	BYTE		btp_htype;					/*# �n�[�h�E�F�A�A�h���X�^�C�v #*/
	BYTE		btp_hlen;					/*# �n�[�h�E�F�A�A�h���X�� #*/
	BYTE		btp_hops;					/*# �z�b�v�� #*/
	DWORD		btp_xid;					/*# �g�����U�N�V�����h�c #*/
	WORD		btp_secs;					/*# �b�� #*/
	WORD		btp_unused;					/*# ���g�p #*/
	DWORD		btp_ciaddr;					/*# �N���C�A���g�h�o�A�h���X #*/
	DWORD		btp_yiaddr;					/*# Your�h�o�A�h���X #*/
	DWORD		btp_siaddr;					/*# �T�[�o �h�o�A�h���X #*/
	DWORD		btp_giaddr;					/*# �Q�[�g�E�F�C�h�o�A�h���X #*/
	BYTE		btp_chaddr[16];				/*# �N���C�A���g�n�[�h�A�h���X #*/
	BYTE		btp_sname[64];				/*# �T�[�o�� #*/
	BYTE		btp_file[128];				/*# �N���t�@�C���� #*/
	BOOTP_VEND	btp_vend;					/*# �x���_�̈� #*/
} BOOTP_PKT;

typedef	struct	{
	BOOTP_PKT	btp_rep;						/*# �a�n�n�s�o��������� #*/
	DWORD		sys_flag;						/*# �V�X�e�����s�ς݃t���O #*/
	DWORD		lu_size;						/*# �s�e�s�o��M�k�t�t�@�C���T�C�Y #*/
	DWORD		siaddr;							/*# �b�b�t �A�h���X #*/
	DWORD		yiaddr;							/*# �k�t�A�h���X #*/
	DWORD		ipmask;							/*# �T�u�l�b�g�}�X�N #*/
	DWORD		giaddr;							/*# �Q�[�g�E�G�C�i���[�^�j #*/
	BYTE		vlan_id;						/*# �u�k�`�m�p�h�c���ʂW�r�b�g #*/
	BYTE		class_a;						/*# �u�k�`�m�v���C�I���e�B�` #*/
	BYTE		class_b;						/*# �u�k�`�m�v���C�I���e�B�a #*/
	BYTE		class_c;						/*# �u�k�`�m�v���C�I���e�B�b #*/
	BYTE		class_d;						/*# �u�k�`�m�v���C�I���e�B�c #*/
	BYTE		lu_type;						/*# �}�X�^�^�X���[�u #*/
	BYTE		lu_number;						/*# �k�t�ԍ� #*/
	BYTE		slv_number;						/*# �k�r�h�ԍ� #*/
	BYTE		macadr[6];						/*# �l�`�b�A�h���X #*/
	BYTE		pad[2];							/*# LONG���E�̖����I�Ƀp�f�B���O��` #*/	// 2000.07 V2.1 �C��
	DWORD		garp_vlan_id;					/*# GARP��MVLAN ID #*/
	BYTE		garp_sw_addr[6];				/*# GARP���M��SW�A�h���X #*/
	WORD		ccu_ver;						/*# CCU�o�[�W���� #*/						// 2000.07 V2.1 �ǉ�
	DWORD		fiaddr;							/*# FILE�T�[�o�A�h���X #*/					// 2000.07 V2.1 �ǉ�
	DWORD		ccu1_ip;						/*# �b�b�t�P�A�h���X #*/					// 2001.08 V2.3 �ǉ�
	DWORD		ccu2_ip;						/*# �b�b�t�Q�A�h���X #*/					// 2001.08 V2.3 �ǉ�
	DWORD		hold_dl;						/*# �ۗ����c�k�σt���O #*/					// 2001.08 V2.3 �ǉ�
	DWORD		tone_dl;						/*# �ۗ����c�k�σt���O #*/					// 2001.08 V2.3 �ǉ�
	DWORD		onl_restart;					/*# �I�����C���ナ�X�^�[�g�t���O #*/		// 2001.08 V2.3 �ǉ�
	BYTE		*horyu_data_top;				/*# �ۗ����擪�A�h���X�ۑ� #*/				// 2001.08 V2.3 �ǉ�
	DWORD		horyu_chunk_size;				/*# �ۗ������f�[�^�T�C�Y�ۑ� #*/			// 2001.08 V2.3 �ǉ�
	BYTE		*talky_data_top;				/*# �g�[�L�[�擪�A�h���X�ۑ� #*/			// 2001.08 V2.3 �ǉ�
	DWORD		talky_chunk_size;				/*# �ۗ������f�[�^�T�C�Y�ۑ� #*/			// 2001.08 V2.3 �ǉ�
	DWORD		restart_cnt;					/*# ���X�^�[�g�� #*/						// 2001.08 V2.3 �ǉ�
	BYTE		extension;					/*# �g���t���O #*/	//2004.01 �ύX
	BYTE		padpad;						/*# �p�f�B���O #*/
	WORD		survtim;					/*# D01�T�o�C�o���Ή�  CCU�؊��^�C�}�l #*/	//2004.02 LU32B 
//	BYTE		rsv[631];						/*# ���U�[�u #*/							// 2000.07 V2.1 �ύX 	// 2001.08 V2.3 �ύX
	BYTE		rsv[624];						/*# ���U�[�u #*/							//2004.02 LU32B �ύX	
// 2004.01 �ύX
} LU_BOOT_DAT;

