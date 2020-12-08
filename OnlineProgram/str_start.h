typedef	struct	{										/*# ＬＵ起動情報構造 #*/
	DWORD	ipl_exec_flg;							/*# ＩＰＬ実行済みフラグ #*/
	DWORD	ipl_cnt;									  /*# ＩＰＬ実行回数 #*/
	DWORD	main_cnt;									  /*# メイン実行回数 #*/
	DWORD	so_err_cnt;						/*# ソケットイニシャルエラー回数 #*/

	DWORD	tftp_err_cnt;							  /*# ＴＦＴＰエラー回数 #*/
	DWORD	file_err_cnt;					/*# プログラムファイルエラー回数 #*/
	DWORD	sys_err_cnt;							  /*# システムエラー回数 #*/
	DWORD	sys_err_id;							/*# システムエラータスクＩＤ #*/

	DWORD	sys_err_inf;						  /*# システムエラー付加情報 #*/
	DWORD	doep_err_cnt;					/*# ＣＣＵオーダヘッダエラー回数 #*/
	DWORD	ccu_err_cnt;			  /*# ＣＣＵヘルスタイムアウトエラー回数 #*/
	DWORD	mik_err_cnt;								/*# ＭＩＫエラー回数 #*/

	DWORD	mik_err_inf;							/*# ＭＩＫエラー付加情報 #*/
	DWORD	pool0_cnt;									/*# ＰＯＯＬ０残り数 #*/
	DWORD	pool1_cnt;									/*# ＰＯＯＬ１残り数 #*/
	DWORD	rsv3;												/*# リザーブ #*/

	DWORD	flash_rerr_cnt1;	/*# ＩＰＬ－ＦＬＡＳＨ読み込みエラーカウント #*/
	DWORD	flash_werr_cnt1;	/*# ＩＰＬ－ＦＬＡＳＨ書き込みエラーカウント #*/
	DWORD	flash_rerr_cnt2;	/*# ＳＹＳ－ＦＬＡＳＨ読み込みエラーカウント #*/
	DWORD	flash_werr_cnt2;	/*# ＳＹＳ－ＦＬＡＳＨ書き込みエラーカウント #*/

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

