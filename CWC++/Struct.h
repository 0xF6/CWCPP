#pragma once
struct Menu_Global
{
public:
	int menu_ng();
	int test_menu_ng();
	int menu_ng_set();
	int test_menu_ng_set();
	int menu();
	int menu_game(), menu_game_set();
	int menu_util(), menu_util_set();
	int setting(), setting_set();
	int gaide(), gaide_set();
	int auz_menu(), auz_menu_set();
	int menu_admin();
	int cck_menu(), ccl_main();
	int apb_set(), ayb_set(), amb_set(), adb_set();
	int main_menu();
};
struct Audio
{
public:
	int menu_audio(), menu_audio_set(), menu_egoist(), menu_egoist_set();
	int ps_1(), ps_2(), ps_3(), ps_4(), ps_5(), ps_6();
	int ps_1_loop(), ps_2_loop(), ps_3_loop(), ps_4_loop(), ps_5_loop(), ps_6_loop();
	void Egoist_1(), Egoist_2(), Egoist_3();
	void Egoist_1_loop(), Egoist_2_loop(), Egoist_3_loop();
};
struct mlt_int
{
public:
	int num_set, num_set1, num_set2, num_set3;
	int numD, numD_1, numD_2, numD_3;
	int error_1(), error_2(), error_3(), error_4(), error_5(), error_6(), error_7(), error_8(), error_9(), error_10(), error_11();
	int sa();
	int d();
	int admins();
	int win_s();
	int nya();
	int ShellAdd();
	int look_api();
};
struct randoms
{
public:
	int random();
	int t_random();
	int random_val(int range_min, int range_max);
};
struct Game_snake
{
public:
	int snake_size, change_x, change_y, coordinates_x[1000], coordinates_y[1000], food_x, food_y;
	int game();
};
struct MyMain
{
	int main_ar(int argv, char* argc []);
	int main_vpn(int argc, char* argv []);
	int main_rand(int argc, char *argv []);
};
struct PassWord
{
public:
	bool pass();
	bool password();
	bool auz_pass_log();
	bool auz_pass();
	bool pass_geta(char true_pass[80]);
};
struct mlt_bool
{
public:
	bool autors();
	bool upload();
	bool error();
	bool cck_menu_2();
};
struct mlt_void
{
public:
	void play(int m);
	void apb();
	void amb();
	void adb();
	void ayb();
	void histori();
	void kbhit_aud();
	void kbhit_egoist();
	//void PrintTcpTable(PMIB_TCPTABLE pTcpTable);
};
struct mlt_const
{
public:
	int N;
	int M;
	int INTERVAL;
};
struct Game_char
{
	char symbol;
	char a[1000][1000];
};
struct Kill
{
public:
	int cursor();
	int mem_kill();
};
struct defcout
{
public:
	void fari();
	void byter();
};
struct WinApi_NetApi
{
public:
	int PlayMov();
	int ips();
	void ListIpAddresses();
};
