#include <tilck/common/basic_defs.h>
#include <tilck/kernel/syscalls.h>
#include <tilck/kernel/errno.h>
#include <tilck/kernel/fs/vfs.h>
#include <tilck/kernel/user.h>
#include <tilck/kernel/process.h>
#include <tilck/kernel/signal.h>
#include <tilck/kernel/timer.h>
#include <tilck/kernel/datetime.h>
#include <tilck/kernel/fs/vfs.h>
#include <tilck/kernel/hal.h>
#include <tilck/kernel/irq.h>
#include <tilck/kernel/debug_utils.h>
#include <tilck/kernel/fault_resumable.h>
#include <tilck/kernel/elf_utils.h>
#include <tilck/kernel/signal.h>
#include <tilck/mods/tracing.h>
#include <tilck/kernel/execve.h>
#include <tilck/kernel/poll.h>
#include <tilck/kernel/waitpid.h>
#include <tilck/kernel/fs/fs_syscalls.h>
#include <tilck/kernel/arch/x86_64/arch_syscalls_x64.h>

#include <tests/test.h>

void test_syscall_getuid(void)
{
   int uid = sys_getuid();
   ASSERT(uid == 0);
}

void test_syscall_getgid(void)
{
   int gid = sys_getgid();
   ASSERT(gid == 0);
}

void test_syscall_geteuid(void)
{
   int euid = sys_geteuid();
   ASSERT(euid == 0);
}

void test_syscall_getegid(void)
{
   int egid = sys_getegid();
   ASSERT(egid == 0);
}

void test_syscall_chown(void)
{
   int rc = sys_chown("/tmp/testfile", 1000, 1000);
   ASSERT(rc == 0 || rc == -EPERM);
}

void test_syscall_fchown(void)
{
   int fd = sys_open("/tmp/testfile", O_CREAT | O_WRONLY, 0644);
   ASSERT(fd >= 0);

   int rc = sys_fchown(fd, 1000, 1000);
   ASSERT(rc == 0 || rc == -EPERM);

   sys_close(fd);
}

void test_syscall_lchown(void)
{
   int rc = sys_lchown("/tmp/testfile", 1000, 1000);
   ASSERT(rc == 0 || rc == -EPERM);
}

void test_syscall_llseek(void)
{
   int fd = sys_open("/tmp/testfile", O_CREAT | O_WRONLY, 0644);
   ASSERT(fd >= 0);

   u64 result;
   int rc = sys_llseek(fd, 0, 0, &result, SEEK_SET);
   ASSERT(rc == 0);

   sys_close(fd);
}

void test_syscall_gettimeofday(void)
{
   struct k_timeval tv;
   int rc = sys_gettimeofday(&tv, NULL);
   ASSERT(rc == 0);
}

void test_syscall_fcntl(void)
{
   int fd = sys_open("/tmp/testfile", O_CREAT | O_WRONLY, 0644);
   ASSERT(fd >= 0);

   int flags = sys_fcntl(fd, F_GETFL, 0);
   ASSERT(flags >= 0);

   sys_close(fd);
}

void test_syscall_mmap(void)
{
   void *addr = sys_mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
   ASSERT(addr != MAP_FAILED);

   sys_munmap(addr, 4096);
}

void test_syscall_poll(void)
{
   struct pollfd fds[1];
   fds[0].fd = sys_open("/tmp/testfile", O_CREAT | O_WRONLY, 0644);
   ASSERT(fds[0].fd >= 0);

   fds[0].events = POLLIN;
   int rc = sys_poll(fds, 1, 1000);
   ASSERT(rc == 0);

   sys_close(fds[0].fd);
}

void test_syscall_waitpid(void)
{
   int pid = sys_fork();
   if (pid == 0) {
      sys_exit(0);
   } else {
      int status;
      int rc = sys_waitpid(pid, &status, 0);
      ASSERT(rc == pid);
      ASSERT(WIFEXITED(status));
      ASSERT(WEXITSTATUS(status) == 0);
   }
}

void run_syscall_tests(void)
{
   test_syscall_getuid();
   test_syscall_getgid();
   test_syscall_geteuid();
   test_syscall_getegid();
   test_syscall_chown();
   test_syscall_fchown();
   test_syscall_lchown();
   test_syscall_llseek();
   test_syscall_gettimeofday();
   test_syscall_fcntl();
   test_syscall_mmap();
   test_syscall_poll();
   test_syscall_waitpid();
}

int main(void)
{
   run_syscall_tests();
   return 0;
}
