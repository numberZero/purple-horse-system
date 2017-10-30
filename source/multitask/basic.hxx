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
	Task *tasks;
	Task *current;

	Scheduler();

	void save_current(SInterruptRegisters const &, SCommonRegisters const &);
	void schedule_next();
};

extern Scheduler *scheduler;
