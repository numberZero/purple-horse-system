#include "basic.hxx"
#include "memory/kmalloc.hxx"

Scheduler *Scheduler::scheduler;
extern "C" void set_context(SInterruptRegisters const &, SCommonRegisters const &);

void TaskState::jump_into()
{
	set_context(context, regs);
}

Scheduler::Scheduler()
{
	current = new (undeletable) Task;
	current->next = nullptr;
	tasks = current;
}

void Scheduler::save_current(SInterruptRegisters const & context, SCommonRegisters const &regs)
{
	current->state.context = context;
	current->state.regs = regs;
}

void Scheduler::schedule_next()
{
	current = current->next;
	if (!current)
		current = tasks;
	current->state.jump_into();
}

extern "C" void task_wrapper();
extern "C" void task_wrapper_2(void (*fn)(void *), void *arg)
{
	fn(arg);
	Scheduler::scheduler->delete_current_task();
}

void Scheduler::create_task(void (*fn)(void *), void *arg)
{
	Task *task = new (undeletable) Task;
	void *stack = KAllocator::allocator->alloc(65536, 10);
	task->state.context = { reinterpret_cast<uintptr_t>(task_wrapper), 0x08, 0x206 };
	task->state.regs = { 0, 0, 0, reinterpret_cast<uintptr_t>(stack) + 65500,
		0, reinterpret_cast<uintptr_t>(fn), 0, reinterpret_cast<uintptr_t>(arg) };
	task->next = tasks;
	tasks = task;
}

void Scheduler::delete_current_task()
{
	Task **ptask = &tasks;
	while (*ptask != current)
		ptask = &(*ptask)->next;
	*ptask = current->next;
	schedule_next();
}
