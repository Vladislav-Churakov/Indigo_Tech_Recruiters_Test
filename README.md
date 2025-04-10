# Indigo_Tech_Recruiters_Test

TEST TASK:

You are given a locked container represented as a two-dimensional grid of boolean values (true = locked, false = unlocked). 
Your task is to write an algorithm that fully unlocks the box, i.e., 
transforms the entire matrix into all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
- Use the SecureBox public API (toggle, isLocked, getState).
- Strategically toggle cells to reach a state where all elements are false.
- Return true if the box remains locked, false if successfully unlocked.
You are not allowed to path or modify the SecureBox class.

Evaluation Criteria:
- Functional correctness
- Computational efficiency
- Code quality, structure, and comments
- Algorithmic insight and clarity
