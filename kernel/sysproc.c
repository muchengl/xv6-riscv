#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  // 需要判断的起始地址
  uint64 buf;
  argaddr(0,&buf);
  // 页数
  int page_num;
  argint(1,&page_num);
  // 存答案的地方
  uint64 ans_p;
  argaddr(2,&ans_p);


  //printf("%p %d %p\n",buf,page_num,ans_p);
  //int ans[page_num/8+1];
  uint64 ans=0;
  struct proc *p = myproc();

  //vmprint(p->pagetable,0);

  // 进行 access 判断
  for(uint64 i=0;i<page_num;i++){

      pte_t *pte=walk(p->pagetable,buf,0);

      if(pte != 0 && (*pte & PTE_A)) {
          ans=ans|(1L << i);
          *pte = *pte & ~PTE_A;
      }
      buf+=PGSIZE;
      //printf("%p\n",buf);
  }

  //  copy_out();
  if(copyout(p->pagetable, ans_p, (char *)&ans,sizeof(ans))<0) return -1;


  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}




