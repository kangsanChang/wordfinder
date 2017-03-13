//hash.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 600011 //해싱 테이블의 크기 = 소수

char filename[10000]; // filename 전역변수

typedef struct ListNode{
	int line_num;
	struct ListNode *link;
}ListNode;

ListNode *hash_table[TABLE_SIZE]; // 전역변수 hash table!

int hash_function(char *key)
{
	unsigned long long int hash_index = 0;
	if (strlen(key)<3){
		return 0;
	}
	while (*key)
		hash_index = 31 * hash_index + *key++; // 연산자 우선순위에 의해 key++ (다음문자) 후 key 실행 ++은 후위연산이므로 처음은 그냥뽑아냄

	hash_index = hash_index % TABLE_SIZE;
	return (int)hash_index;
}

void hash_chaining(int hash_idx, int line_num){
	if (hash_idx == 0){
		return;
	}

	if (hash_table[hash_idx] != NULL){ //한개이상 연결 되어 있는 경우
		ListNode *temp = hash_table[hash_idx];
		ListNode *create_node;
		while (temp->link != NULL){
			temp = temp->link; // 찾은 idx 에서 제일 끝자리 찾아서 chaining 하러 올라감
		}

		create_node = (ListNode*)malloc(sizeof(ListNode)); //마지막 지점에서 malloc
		create_node->line_num = line_num;
		create_node->link = NULL;
		temp->link = create_node;
	}
	else{ // 첫 연결일 경우
		hash_table[hash_idx] = (ListNode*)malloc(sizeof(ListNode));
		hash_table[hash_idx]->line_num = line_num;
		hash_table[hash_idx]->link = NULL;
	}
}

void search_hash(char* string){
	int hash_idx = hash_function(string);
	ListNode *temp = hash_table[hash_idx];
	if (hash_table[hash_idx] == NULL){
		printf("문자열에 없는 단어입니다! \n");
		return;
	}
	while (temp->link != NULL){
		printf("%d \n", temp->line_num);
		temp = temp->link;
	}
	printf("%d \n", temp->line_num);
}

void readfile(char op){
	FILE* fp;
	char* string;
	char buf[256] = { 0 };
	int line_cnt = 1;
	char* ignorestr = (char*)malloc(sizeof(2)); //공간이 있어야 strcpy 가능..!

	if (op == 'y'){ // Case Sensitive
		fp = fopen(filename, "r");
		if (fp == NULL){
			printf("파일이 안보이네요!\n");
			system("pause");
			exit(1);
		}
		else{
			while (1){
				if (fgets(buf, 256, fp) == NULL){
					break;
				}
				string = strtok(buf, " ,./<>?`1234567890-=|~\n!@#$%%^&*()_+:;\"'{}[]\\"); // 1.맨처음것을 strok 으로 자르고
				while (string != NULL){
					hash_chaining(hash_function(string), line_cnt); // 2.체이닝
					string = strtok(NULL, " ,./<>?`1234567890-=|~\n!@#$%%^&*()_+:;\"'{}[]\\"); // 3.strok 다시 한 후 while loop으로..!
				}
				line_cnt++;
				printf("현재 %d 줄을 읽고 있습니다. \n", line_cnt);
			}
			fclose(fp);
		}

	}
	else if (op == 'n'){ // Case Ignore
		fp = fopen(filename, "r");
		if (fp == NULL){
			printf("파일이 안보이네요!\n");
			system("pause");
			exit(1);
		}
		else{
			while (1){
				if (fgets(buf, 256, fp) == NULL){
					break;
				}
				string = strtok(buf, " ,./<>?`1234567890-=|~\n!@#$%%^&*()_+:;\"'{}[]\\\""); // 1.맨처음것을 strok 으로 자르고

				while (string != NULL){
					strcpy(ignorestr, string);
					strcpy(ignorestr, strlwr(ignorestr));
					hash_chaining(hash_function(ignorestr), line_cnt); // 2.체이닝
					string = strtok(NULL, " ,./<>?`1234567890-=|~\n!@#$%%^&*()_+:;\"'{}[]\\\""); // 3.strok 다시 한 후 while loop으로..!


				}
				line_cnt++;
				printf("현재 %d 줄을 읽고 있습니다. \n", line_cnt);
			}
			fclose(fp);
		}
	}
}

char Case_sensitive(){
	char op;
	while (1){
		printf("대소문자 구별 여부(y/n) : ");
		op = getchar();
		if (op == 'y' || op == 'n'){
			return op;
		}
		printf("\n");
		fflush(stdin);
	}
}
void loopcase(char op){

	clock_t start_time, end_time;
	char word[50] = { 0 };

	if (op == 'y') {
		while (1){
			printf("검색 원하는 단어 입력 : ");
			scanf("%s", word);
			if (strlen(word)<3){
				printf("3글자 단어부터 검색됩니다.");
				continue;
			}
			start_time = clock();
			search_hash(word);
			end_time = clock();
			printf("Time : %lf\n",
				((double)(end_time - start_time)) / CLOCKS_PER_SEC);
			printf("\n");
		}
	}
	else if (op == 'n'){
		while (1){
			printf("검색 원하는 단어 입력 : ");
			scanf("%s", word);
			if (strlen(word)<3){
				printf("3글자 단어부터 검색됩니다.");
				continue;
			}
			start_time = clock();
			search_hash(strlwr(word));
			end_time = clock();
			printf("Time : %lf\n",
				((double)(end_time - start_time)) / CLOCKS_PER_SEC);
			printf("\n");
		}
	}
	else{
		printf("ERR!\n");
		system("pause");
		exit(1);
	}

}


int main(void){

	char op;
	printf("input filename (.txt 포함해서): " );
	scanf("%s",filename);
	fflush(stdin);
	op = Case_sensitive();
	readfile(op);
	loopcase(op);

	system("pause");
}
