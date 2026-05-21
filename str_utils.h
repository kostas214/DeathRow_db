int is_digits(const char *buffer);

char** split_str(char *buffer,char split_char,int* split_len);

int count_char(const char* buffer,char target_char);

void free_str_arr(char** array,int length);