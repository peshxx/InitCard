//#include "hsm_tcpsub.h"

int InitHsmDevice(char *tcpaddr, int port, int timeout);

int CloseHsmDevice(int comid);

int generate_root_key( int comid, char *msghd, char *mark_byte, int sk_index, int sk_group, char *gvi , char *key_attribute, int num, char *key_component, char *kcv );

int generate_transport_key( int comid, char *msghd, int group_index, char *mark_byte, int tk_index, \
				char *key_attribute, char *init_key, char *kcv );

int derivate_key( int comid, char *msghd, char *mark_byte, int sk_index, \
				char *source_gvi, \
				char *new_gvi, char *key_attribute, \
				int derivate_num, char *derivate_data, char *kcv );

int generate_random_key( int comid, char *msghd, char *gvi, \
				char *key_attribute, char *kcv );

// int export_key(
//                 int	comid,
//                 char    *msghd,
//                 char   	*protect_key_gvi,
//                 char   	*export_key_gvi,
//                 int    	derivate_num_p,
//                 char   	*derivate_data1,
//                 int    	derivate_num_e,
//                 char   	*derivate_data2,
//                 int    	key_head_len,
//                 char   	*key_head,
//                 int    	key_tail_len,
//                 char   	*key_tail,
//                 int    	mac_data_len,
//                 char   	*iv,
//                 char   	*mac_data,
//                 char   	*kcv,
//                 int	*chiper_data_len,
//                 char  	*chiper,
//                 char   	*mac );

// int import_key(
//                 int	comid,
//                 char    *msghd,
//                 char   	*protect_key_gvi,
//                 int     derivate_num_p,
//                 char   	*derivate_data,
//                 int     mac_data_len,
//                 char   	*iv,
//                 char   	*mac_data,
//                 int     chiper_len,
//                 char   	*chiper,
//                 char   	*mac,
//                 char    *kcv );

// int special_import_key(
//                 int	comid,
//                 char    *msghd,
//                 char   	*protect_key_gvi,
//                 char   	*import_key_gvi,
//                 char   	*import_key_attribute,
//                 int     offset,
//                 int     derivate_num_p,
//                 char   	*derivate_data,
//                 int     mac_data_len,
//                 char   	*iv,
//                 char   	*mac_data,
//                 int     chiper_len,
//                 char   	*chiper,
//                 char   	*mac,
//                 char   	*kcv );


int change_transport_key(
		int	comid, 
		char 	*msghd, 
		int	group_index,
		char	*mark_byte,
		int	tk_index,
		int 	key_type, 
		int 	key_head_len, 
		char 	*key_head, 
		int 	key_tail_len, 
		char 	*key_tail, 
		int 	mac_data_len, 
		char 	*iv, 
		char 	*mac_data, 
		char 	*kcv, 
		int	*chiper_data_len, 
		char 	*chiper, 
		char 	*mac );
	
int query_key_state(
                int	comid,
                char    *msghd,
		char	*mark_byte,
		int	tk_index,
		int	group_index,
                char   	*gvi,
                char   	*kcv,
                char   	*key_attribute );

int generate_random_data(
                int	comid,
                char    *msghd,
                int     length,
                int     output_mode,
                char   	*random_data );

int secure_msg(
                int	comid,
                char    *msghd,
                int     flag,
                char   	*gvi,
                int     derivate_num,
                char   	*derivate_data,
                int     session_key_mode,
               	char   	*random_data,
                int     mac_len,
                int     mac_data_len,
                char   	*iv,
                char   	*mac_data,
                int	msg_len,
                char   	*message,
                int 	*chiper_data_len,
                char   	*chiper,
                char   	*mac );
	
int generate_mac_tac(
		int	comid,
		char    *msghd,
                int     flag,
                char   	*gvi,
                int     derivate_num,
               	char   	*derivate_data,
                int     session_key_mode,
                char   	*random_data,
                int     mac_len,
                int	msg_len,
                char   	*iv,
                char   	*message,
               	char   	*mac);

int verify_and_generate_mac_tac(
                int			comid,
                char    *msghd,
				int			flag,
                char   	*gvi,
                int     derivate_num,
                char   	*derivate_data,
                int     session_key_mode,
                char   	*random_data,
                char   	*mac1,
                int			msg_len1,
                char   	*iv1,
                char   	*message1,
                int			msg_len2,
                char   	*iv2,
               	char   	*message2,
                char   	*mac2 );

int encrypted_and_decrypted(
                int			comid,
                char    *msghd,
                int     block_num,
                int     crypto_flag,
                int     algo_mode,
								char		*mark_byte,
								int			tk_index,
								int			group_index,
                char   	*gvi,
                int     derivate_num,
                char   	*derivate_data,
                int     session_key_mode,
                char   	*random_data,
                int     pad_mode,
                char   	*pad_char,
                int     pad_count_flag,
                char   	*iv,
                int			msg_len,
                char   	*message,
                int			*output_message_len,
                char   	*output_message,
                char   	*next_iv );



int verify_mac_tac(
		int	comid, 
		char	*msghd, 
		int	flag,
		char	*gvi,
		int	derivate_num,
		char	*derivate_data,
		int	session_key_mode,
		char 	*random_data,
		int 	mac_len,
		char 	*mac,
		int	msg_len,
		char 	*iv, 
		char	*message);
		

int erase_specified_key( int comid, char *msghd, char *mark_byte, int tk_index, int group_index, char *gvi );

int import_key_lmk_encrypt( int comid, char *msghd, char *mark_byte, int tk_index, int group_index, char *gvi, char *key_attribute, char *cipher, char *kcv );

int export_key_lmk_encrypt( int comid, char *msghd, char *mark_byte, int tk_index, int group_index, char *gvi, char *kcv, char *key_attribute, char *cipher );

int export_key1(
                int	comid,
                char    *msghd,
                char		*mark_byte,
								int			ek_index,
								int			ek_group,
								char   	*export_key_gvi,
                int    	derivate_num_p,
                char   	*derivate_data1,
                int    	derivate_num_e,
                char   	*derivate_data2,
                int    	key_head_len,
                char   	*key_head,
                int    	key_tail_len,
                char   	*key_tail,
                int    	mac_data_len,
                char   	*iv,
                char   	*mac_data,
                char   	*kcv,
                int			*chiper_data_len,
                char  	*chiper,
                char   	*mac );


int special_import_key1(
                int			comid,
                char    *msghd,
                char		*mark_byte,
								int			tk_index,
								int 		tk_group,
								int			ik_index,
								char   	*import_key_gvi,
                char   	*import_key_attribute,
                int     offset,
                int     derivate_num_p,
                char   	*derivate_data,
                int     mac_data_len,
                char   	*iv,
                char   	*mac_data,
                char   	*chiper,
                char   	*mac,
                char   	*kcv );

