#pragma once
#include "hardware/types.hxx"

struct TaskState {
	SInterruptRegisters context;
	SCommonRegisters regs;

	void jump_into();
};

struct Task {
	TaskState state;
	Task *next;
};

struct Scheduler {
	static Scheduler *scheduler;
	Task *tasks;
	Task *current;

	Scheduler();

	void save_current(SInterruptRegisters const &, SCommonRegisters const &);
	void schedule_next();

	void create_task(void (*fn)(void *arg), void *arg);
	void delete_current_task();
};
