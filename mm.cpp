#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

int newpid = 0;

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[123];

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法

//主函数
int main(){
	int op;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;){
		sleep(1);
		display_menu();
		fflush(stdin);
		scanf("%d", &op);
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ puts("No algorithm now!"); break; }
			case 3:{ create_new_process(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
    allocated_block *temp;
    temp = allocated_block_head;
    while(temp != NULL){
        if (id == temp->pid) {
            printf("Start Address: %d", temp->start_addr)
        }
        temp = temp -> next;
    }
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

void set_mem_size(){ //更改最大内存大小
    int memsize;
    if (flag == 1) {
        cout << "Please input the memory size: ";
        cin >> memsize;
        mem_size = memsize;
    }else {
        printf("The memory has been allocated. You can't change the memory size now!");
    }
    
}

int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
    
    //FirstFit
    /*free_block *check, *chpre;
    int count = 0
    rearrange();
    check = free_block_head;
    while (count = 0) {
        //头节点分配为内存
        if (check->size >= ab->size && check == free_block_head) {
            ab->start_addr = check->start_addr;
            check->start_addr = check->start_addr + ab->size;
            check->size = check->size - ab->size;
            if (check->size <= 1) {
                free_block_head = check->next;
            }
            count = 1;
        }
        //其它节点分配为内存
        else if (check->size >= ab->size && check != free_block_head) {
            ab->start_addr = check->start_addr;
            check->start_addr = check->start_addr + ab->size;
            check->size = check->size - ab->size;
            if (check->size <= 1) {
                chpre->next = check->next;
            }
            count = 1;
        }
        else if (check->next == NULL && check->size < ab->size) {
            printf("There isn't enough memory!");
            return mem_size;
        }
        else {
            chpre = check;
            check = check->next;
        }
    }
    
    //调整allocated链表
    ab->next = allocated_block_head;
    allocated_block_head->next = ab;
    
    return ab->start_addr;*/
    
    
    //BestFit
    free_block *check, *chpre;
    free_block *sort, *tsort;
    
    //按内存从小到大排序
    sort = free_block_head;
    while(sort != NULL){
        tsort = sort->next;
        while (tsort != NULL){
            if (tsort->size < sort->size){
                swap(&sort->start_addr, &tsort->start_addr);
                swap(&sort->size, &tsort->size);
            }
            tsort = tsort->next;
        }
        sort = sort->next;
    }
    
    //allocate memoty
    check = free_block_head;
    int test = 0;
    while (test = 0) {
        if (check->size >= ab->size && check = free_block_head) {
            ab->start_addr = check->start_addr;
            check->start_addr = check->start_addr + ab->size;
            check->size = check->size - ab->size;
            if (check->size <= 1) {
                free_block_head = check->next;
            }
            test = 1;
        }
        else if(check->size >= ab->size && check != free_block_head){
            ab->start_addr = check->start_addr;
            check->start_addr = check->start_addr + ab->size;
            check->size = check->size - ab->size;
            if (check->size <= 1) {
                chpre->next = check->next;
            }
            count = 1;
        
        }
        else if(check->next == NULL && check->size < ab->size){
            printf("There isn't enough memory!");
            return mem_size;
        }
        else {
            chpre = check;
            check = check->next;
        }
    }
    
    ab->next = allocated_block_head;
    allocated_block_head->next = ab;
    
    return ab->start_addr;
    
}

int create_new_process(){ //创建新进程
    flag = 1;
    allocated_block *np;
    newpid += 1;
    np->pid = newpid;
    printf("Please input the process name: ");
    scanf("%s", &(np->process_name));
    cout << "Please input the memory allocated size: ";
    cin >> np->size;
    np->start_addr = allocate_mem(*np);
    if (np->start_addr == mem_size) {
        cout << "Please input a smaller memory size: ";
        cin >> np->size;
        np->start_addr = allocate_mem(*np);
    }
    
}

void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange(){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (tmpx->start_addr < tmp->start_addr){
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}


int free_mem(allocated_block *ab){ //释放某一块的内存
    free_block frtmp, frtmpre;
    frtmp = free_block_head;
    if (frtmp->start_addr > ab->start_addr && frtmp == free_block_head) {
        tmpnode = allocated_block_head->next;
        allocated_block_head = ab;
        allocated_block_head->next = tmpnode;
        return 1
    }
    frtmpre = free_block_head;
    frtmp = free_block_head->next;
    while (frtmp != NULL) {
        if (frtmp->start_addr > ab->start_addr && frtmpre->start_addr < ab->start_addr) {
            ab->next = frtmp;
            frtmpre->next = ab;
        }
        frtmpre = frtmp;
        frtmp = frtmp->next;
    }
    if (frtmp == NULL && frtmpre->start_addr < ab->start_addr) {
        frtmpre->next = ab;
    }
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	allocated_block *ab;
	int pid;
	puts("Please input the pid of Killed process");
	scanf("%d", &pid);
	ab = find_process(pid);
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}
}



