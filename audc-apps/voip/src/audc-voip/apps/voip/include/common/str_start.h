typedef	struct	{										/*# �k�t�N�����\�� #*/
	DWORD	ipl_exec_flg;							/*# �h�o�k���s�ς݃t���O #*/
	DWORD	ipl_cnt;									  /*# �h�o�k���s�� #*/
	DWORD	main_cnt;									  /*# ���C�����s�� #*/
	DWORD	so_err_cnt;						/*# �\�P�b�g�C�j�V�����G���[�� #*/

	DWORD	tftp_err_cnt;							  /*# �s�e�s�o�G���[�� #*/
	DWORD	file_err_cnt;					/*# �v���O�����t�@�C���G���[�� #*/
	DWORD	sys_err_cnt;							  /*# �V�X�e���G���[�� #*/
	DWORD	sys_err_id;							/*# �V�X�e���G���[�^�X�N�h�c #*/

	DWORD	sys_err_inf;						  /*# �V�X�e���G���[�t����� #*/
	DWORD	doep_err_cnt;					/*# �b�b�t�I�[�_�w�b�_�G���[�� #*/
	DWORD	ccu_err_cnt;			  /*# �b�b�t�w���X�^�C���A�E�g�G���[�� #*/
	DWORD	mik_err_cnt;								/*# �l�h�j�G���[�� #*/

	DWORD	mik_err_inf;							/*# �l�h�j�G���[�t����� #*/
	DWORD	pool0_cnt;									/*# �o�n�n�k�O�c�萔 #*/
	DWORD	pool1_cnt;									/*# �o�n�n�k�P�c�萔 #*/
	DWORD	rsv3;												/*# ���U�[�u #*/

	DWORD	flash_rerr_cnt1;	/*# �h�o�k�|�e�k�`�r�g�ǂݍ��݃G���[�J�E���g #*/
	DWORD	flash_werr_cnt1;	/*# �h�o�k�|�e�k�`�r�g�������݃G���[�J�E���g #*/
	DWORD	flash_rerr_cnt2;	/*# �r�x�r�|�e�k�`�r�g�ǂݍ��݃G���[�J�E���g #*/
	DWORD	flash_werr_cnt2;	/*# �r�x�r�|�e�k�`�r�g�������݃G���[�J�E���g #*/

	DWORD	dmy5;														/*#  #*/
	DWORD	dmy6;														/*#  #*/
	DWORD	dmy7;														/*#  #*/
	DWORD	dmy8;														/*#  #*/

	DWORD	dmy9;														/*#  #*/
	DWORD	dmy10;														/*#  #*/
	DWORD	dmy11;														/*#  #*/
	DWORD	dmy12;														/*#  #*/

	DWORD	dmy13;														/*#  #*/
	DWORD	dmy14;														/*#  #*/
	DWORD	dmy15;														/*#  #*/
	DWORD	dmy16;														/*#  #*/

} LU_START;

