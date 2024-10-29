class Processor:
    processor_count = 0

    def __init__(self):
        self.task_list = []
        Processor.processor_count += 1
        self.id = Processor.processor_count

    def display_tasks(self, max_time):
        print(f"Processor{self.id}", end="  ")

        for time in range(max_time + 1):
            empty_slot = True

            for task in self.task_list:
                if task[1][0] <= time < task[1][1]:
                    print(f"t{task[0]}", end="  ")
                    empty_slot = False
            if empty_slot:
                print("  ", end="  ")

def schedule_jobs(jobs):
    active_processors = []
    max_end_time = 0
    jobs_sorted = sorted(jobs, key=lambda job: job[1][0])

    while jobs_sorted:
        current_task = jobs_sorted.pop(0)
        assigned = False

        if active_processors:
            for processor in active_processors:
                if processor.task_list[-1][1][1] <= current_task[1][0]:
                    processor.task_list.append(current_task)
                    assigned = True
                    break
                
        if not assigned:
            new_processor = Processor()
            new_processor.task_list.append(current_task)
            active_processors.append(new_processor)

        if max_end_time < current_task[1][1]:
            max_end_time = current_task[1][1]

    return max_end_time, active_processors

def main():
    job_data = [
        (1, [7, 8]),
        (2, [3, 7]),
        (3, [1, 5]),
        (4, [5, 9]),
        (5, [0, 2]),
        (6, [6, 8]),
        (7, [1, 6])
    ]

    max_time, processor_output = schedule_jobs(job_data)

    print("Time   ", end="  ")
    for time in range(max_time):
        print(f"{time:2d}", end="  ")
    print()
    
    for processor in processor_output:
        processor.display_tasks(max_time)
        print()
    
if __name__ == "__main__":
    main()
