#include "basic.hxx"
#include "memory/kmalloc.hxx"

Scheduler *scheduler;
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
