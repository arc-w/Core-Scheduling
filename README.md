This project is homework from my university. The code is my solution. Here is the description:  
# Core Scheduling: Minimizing Energy with Deadlines and Dependency Cycles
Introduction
Modern computing systems often feature multiple processing cores with varying performance
characteristics (speed) and power consumption rates. Efficiently scheduling tasks across these
heterogeneous cores is crucial, especially in real-time systems where tasks must be completed by
specific deadlines. A primary goal in mobile and embedded systems is to minimize energy
consumption while guaranteeing that all tasks meet their timing constraints and respect any
dependencies between them.  
You are tasked with developing a scheduler for a set of computational tasks that need to be
executed on a system with multiple heterogeneous cores. Each task has a defined computational
workload, a deadline by which it must be completed, and potential dependencies on other tasks.
Each core type has a specific processing speed and a power consumption rate (energy consumed
per unit of time).  
The goal is to find a schedule (an assignment of each task to a core, along with a start and end
time) that minimizes the total energy consumed by all cores while ensuring:  
1. Dependency Constraints:
If Task A must complete before Task B can start (denoted A -> B), the scheduled end
time of Task A must be less than or equal to the scheduled start time of Task B. Tasks and
their dependencies can be modeled as a directed graph.  
2. Deadline Constraints:
Every task must be completed on or before its specified deadline.  
3. Core Constraints:
A single core can only execute one task at a time.  
Dependency Graph Analysis
Task dependencies should ideally form a Directed Acyclic Graph (DAG). However,
configuration errors or specific system models might introduce cycles. Before attempting
scheduling, you must analyze the task dependency graph to detect any cyclic dependencies.  
● Graph Traversal: Techniques that systematically traverse the graph are required to
identify strongly connected components or back-edges, which indicate cycles.  
● Direct Mutual Dependency (2-Cycle): If the analysis reveals exactly two tasks, say Task
A and Task B, forming a direct mutual dependency (A -> B and B -> A), this represents a
special constraint. These two tasks must be scheduled to start at the exact same time, but
on different cores. This parallel execution constraint overrides the standard finish-to-start
dependency between only these two tasks. All other dependencies involving Task A or
Task B must still be respected. If this parallel execution is impossible (e.g., not enough
cores, violates deadlines, or conflicts with other dependencies), then no valid schedule
exists.  
● Larger Cycles: If a cycle involves three or more tasks, a valid schedule satisfying all
constraints is considered impossible.  
Finding the Minimum Energy Schedule  
If the dependency graph is valid (either a DAG or contains only resolvable 2-cycles), you need to
find the schedule with the minimum total energy.  
● State-Space Exploration: Consider the process of building a schedule step-by-step. A
partial schedule can be thought of as a "state," perhaps defined by the set of completed
tasks and the times when each core becomes available next. Finding the optimal schedule
involves exploring this space of possible states.  
● Systematic Search: A systematic search strategy is needed to explore these states. Since
the goal is minimum energy, the search should prioritize exploring states that have
resulted in lower accumulated energy consumption so far.  
● Cost Tracking: Maintaining the minimum energy cost found to reach each distinct state
is crucial to avoid redundant work and ensure optimality.  
● Efficient Exploration: The number of possible states can be large. Employing a data
structure that allows efficient retrieval of the most promising (lowest-energy) state to
explore next is highly recommended for performance.  
Energy Calculation  
● The execution time of a task on a specific core is calculated as: Task Workload / Core
Speed  
● The energy consumed by a task on a specific core is: Execution Time * Core Power Rate  
● The total energy for a schedule is the sum of the energy consumed by all tasks  
Input Format  
Input will be provided via a text file named input.txt with the following structure:  
N # Number of tasks  
Task Info (N lines): TaskID Workload Deadline  
T1_ID T1_Workload T1_Deadline  
T2_ID T2_Workload T2_Deadline  
...  
TN_ID TN_Workload TN_Deadline  
M # Number of dependencies  
Dependency Info (M lines): From_TaskID To_TaskID  
Dep1_From Dep1_To  
Dep2_From Dep2_To  
...  
DepM_From DepM_To  
K # Number of core types  
Core Info (K lines): CoreSpeed CorePowerRate  
Core1_Speed Core1_PowerRate  
Core2_Speed Core2_PowerRate  
● N, M, K - positive integers  
● TaskIDs are positive integers (not necessarily sequential).  
● Workload, Deadline, CoreSpeed, CorePowerRate - positive floating-point numbers.  
● Dependencies are specified using the original TaskIDs.  
● Core types are implicitly assigned IDs 0, 1, ..., K-1 based on their order in the input.  
Output Format  
Your program should output the results to the standard output.  
Cycle Detection Output:  
● Before other output, print messages related to cycle detection as described above
(warnings for cycles, confirmation of DAG status). Use standard error for warnings and
standard output for the DAG confirmation message. Ensure output buffers are flushed
appropriately so these messages appear in the correct order relative to subsequent output.  
Example warning:  
 Warning: Cycle detected involving tasks (1-based IDs): { 2, 1 }  
● If cycles are detected, indicate the attempt to proceed:
 Attempting to schedule based on special cycle handling rules.  
Scheduling Results:  
● If a valid schedule is found:  
○ Print the minimum total energy: Minimum Total Energy Consumed: XXX.XX  
○ Print the schedule table and the details for each task (Task ID, Core ID, Start  
Time, End Time, Energy)
● If no valid schedule is found:  
 Scheduling failed. No solution found that meets all deadlines.
