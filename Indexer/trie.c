#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


typedef struct filenode FileNode;
struct filenode{
	char* filename;
	int count;
	struct filenode* next;
};

typedef struct trienode {
	char letter;
    char* word;
    FileNode* head;
    struct trienode* list[36];
}TrieNode;


TrieNode*  create_trienode() {
	TrieNode *q = (TrieNode*)malloc(sizeof(TrieNode));
	int x;
	for(x = 0;x<36;x++)
	{
		q->list[x] = NULL;
	}
	q->head = NULL;
	q->word = "";
	return q;
}

FileNode* create_filenode(char* filename) {
	FileNode *q = (FileNode*)malloc(sizeof(FileNode));
	q->count = 0;
	q->next = NULL;
	q->filename = filename;
	
	return q;

}

void add_filenode(char* filename, TrieNode* tn){
//static int i = 0;
//printf("\t%s\n", filename);
//printf("here %d\n", ++i);
	FileNode* curr = tn->head;
	
	if(tn->head == NULL){
		tn->head = create_filenode(filename);
//printf("%s\n",tn->head->filename);
		tn->head->count = 1;
//printf("file: %s count: %d\n", filename, tn->head->count);
	}
	else{
		
		while(curr != NULL){
			if(strcmp(curr->filename,filename) == 0){
				curr->count++;
//printf("file %s new count %d\n",filename, curr->count);
//printf("%s\n",tn->head->filename);
				break;
			}
			else if(curr->next == NULL){
				curr->next = create_filenode(filename);
//printf("%s\n",tn->head->filename);
				curr->next->count = 1;
				break;
			}
			curr = curr->next;	
		}
		
	}
	
}


void add_token(char* token, char* filename, TrieNode* tn, int index) {
//printf("%c\n",tn->letter);
	int x = 0;
	token[index] = (char)tolower(token[index]);
//printf("Current letter: %c!\n",token[index]);

//	if(token[index] == '\0'){
//printf("Character is null byte.");
//	}

	if(token[index] == '\0'){

		if(strcmp(tn->word,"") == 0){
			tn->word = token;
//printf("hit %s %c\n",tn->word, tn->letter);
		}
		//FileNode* newFN = create_filenode(filename);
		add_filenode(filename, tn);
	}
//printf("%d %d %c~\n",index, strlen(token), token[index]);
	else //if(index < strlen(token))
	{
		if((int)token[index] < 58 && (int)token[index] > 47)
		{
			x = ((int)token[index]) - 22;
			//x = 26-35
		
		}
		else if((int)token[index] <123 && (int)token[index] >96)
		{
			x = ((int)token[index]) - 97;
			// x = 0-25
		
		}else{
			x = -1;
			printf("Invalid character found: %c\n", token[index]);
		}
		
		if(x != -1){	
			if(tn->list[x] == NULL)
			{
	//printf("index: %d char: %c\n",x, token[index]);
				tn->list[x] = create_trienode();
				tn->list[x]->letter = token[index];
			}
			
			//recursive call
			add_token(token, filename, tn->list[x],index + 1);
		}
	}
}

void print_filenames(TrieNode *tn, char* filename, FILE *out){
	if(tn->head == NULL){
		printf("you did something wrong");
	}
	int x = 0, tag = 0;
	int count = 0;
	FileNode* largest;
	FileNode* curr = tn->head;

	while(curr != NULL)
	{
//printf("%d\n",curr);
		if(count != 0 && curr->count == count){
			if (strcmp(largest->filename, curr->filename) > 0){
				largest = curr;
			}
		}
		if(curr->count > count)
		{
			count = curr->count;
			largest = curr;
		}
		
		curr = curr->next;
		
		if(curr == NULL && count !=0)
		{
			if(tag == 1){
				fputs(",\n", out);
			} else {tag = 1;}
			char tmp[1000];
//printf("%s\n", tn->head->filename);
			
			sprintf(tmp, "\t\t{\"%s\" : \"%d\"}",largest->filename,largest->count);
			fputs(tmp, out);
			largest->count = 0;
			count = 0;
			curr = tn->head;
			largest = NULL;
		}
		else if(curr == NULL && count == 0){
			fputs("\n", out);
			break;
		}
	}
	
}	
		
		
void print_trie(TrieNode *tn, char* filename, FILE *out)
{
	static int tag = 0;
		
	
//printf("letter = %c, word = %s\n",tn->letter, tn->word);
	if(strcmp(tn->word,"") != 0)
	{
		if(tag == 0)
			tag = 1;
		else {
			fputs(",\n", out);
		}
		char tmp[1000];
		sprintf(tmp,"\t{\"%s\" : [\n",tn->word);
		fputs(tmp, out);
//printf("\t%s\n",tn->head->filename);
		print_filenames(tn, filename, out);
		sprintf(tmp, "\t]}");
		fputs(tmp, out);
	}

	
	int i;
	for(i = 0; i<36; i++)
	{
		
//printf("%d\n",tn->list[i]);
		if(tn->list[i] != NULL)
		{
//printf("%c %d\n", tn->letter, i );
			//recursive call
			print_trie(tn->list[i], filename, out);
		}
	
	}
}
	
void tokenize(char* FullString, char* filename, TrieNode* tn)
{
//printf("%s\n", filename);
	char *current = FullString;
	char *word;
	char *tok;
	int wordlength = 0;
	
	while((current[0] = (char)tolower(current[0])) != '\0') //Iterate through stream
	{
		if(isalpha(current[0]))
		{
//printf("%d\n",current[0]);
			wordlength = 1;
			word = current;
			current++;
			current[0] = (char)tolower(current[0]);
//printf("%d\n",current[0]);
			if(!(
				((int)current[0] > 47 && (int)current[0] < 58)
				|| ((int)current[0] > 96 && (int)current[0] < 123)
				))
			{
				tok = (char*)malloc(sizeof(char) * (wordlength + 1));
				strncpy(tok, word, wordlength);
				strncpy(tok + wordlength, "\0", 1);
//printf("word = %s %d\n",tok, wordlength);
				 add_token(tok, filename, tn, 0);
				 wordlength = 0;
				 continue;
			}
			
			while((current[0] = (char)tolower(current[0])) != '\0') //Iterate until end of token
			{
//printf("\t%d\n",current[0]);
				if(((int)current[0] > 47 && (int)current[0] < 58)
					|| ((int)current[0] > 96 && (int)current[0] < 123))
				{
					wordlength++;
					current++;
					continue;
				}
				else{
					tok = (char*)malloc(sizeof(char) * (wordlength + 1));
					strncpy(tok,word, wordlength);
					strncpy(tok + wordlength, "\0", 1);
//printf("word = %s %d\n",tok, wordlength);
					add_token(tok, filename, tn, 0);
					break;				 
				}
			
			}
			if(current[0] == '\0'){
				tok = (char*)malloc(sizeof(char) * (wordlength + 1));
				strncpy(tok,word, wordlength);
				strncpy(tok + wordlength, "\0", 1);
//printf("word = %s %d\n",tok, wordlength);
				add_token(tok, filename, tn, 0);
			}
		}
		current++;
		wordlength = 0;
	}	
}
	
void read_file(char* filename, TrieNode* tn){

	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Unable to open file: %*s", filename);
		exit(0);
	}
	char stream[1000];
	
	while(!feof(fp)){
		
		fgets(stream, 1000, fp);
		if(feof(fp)){
			break;
		}
//printf("\t%s\n", stream); // Replace with tokenizer
		tokenize(stream, filename, tn);
	}
	fclose(fp);
}
	
void trav_dir(char *dir, int depth, TrieNode* tn)
{
	static int tag = 0;
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

//	strcat(dir, "/testdir");
//	printf("pwd = %s\n", dir);
	
	if((dp = opendir(dir)) == NULL) {
		// Then assume it is a file
		printf("Directory could not be opened: %*s",dir);
		return;
	}
	
	chdir(dir);
	
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			/* Found a directory, but ignore . and .. */
				if(strcmp(".",entry->d_name) == 0 ||
					strcmp("..",entry->d_name) == 0) {
					continue;
				}
			//printf("%*s%s/\n",depth, " ",entry->d_name);
			/* Recurse at a new indent level*/
			trav_dir(entry->d_name,depth+4, tn);
		}
		else {
			//printf("%*s%s\n",depth, " ", entry->d_name);
			char* buf = (char*)malloc(sizeof(char)*PATH_MAX);
			realpath(entry->d_name, buf);
//printf("%s\n", buf);
			read_file(buf, tn);
		}
	}
	chdir("..");
	closedir(dp);
}
	
	
	
int main(int argc, char** argv){
	
	TrieNode *head = create_trienode();
	
	head->letter = '!';
	DIR *dp;
	if((dp = opendir(argv[2])) ==  NULL){
		printf("File was provided instead of directory. Indexing single file.\n");
		read_file(argv[2], head);
	}
	else trav_dir(argv[2], 0, head);
	
	FILE *out = fopen(argv[1], "w");
	if(out == NULL){
		exit(1);
	}
	
	//printf("\t\t\t%s\n", head->list[0]->list[17]->list[4]->head->filename);
	
	char tmp[1000];
	sprintf(tmp,"{\"list\" : [\n");
	fputs(tmp, out);
	print_trie(head, argv[1], out);
	sprintf(tmp, "\n]}\n");
	fputs(tmp, out);
	fclose(out);
}
