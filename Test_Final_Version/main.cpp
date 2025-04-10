#include <iostream>
#include <vector>
#include <random>
#include <time.h>

/*
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
*/

class SecureBox
{
private:
   std::vector<std::vector<bool>> box;

   std::mt19937_64 rng;
   uint32_t ySize, xSize;

   //================================================================================
   // Method: shuffle
   // Description: Randomly toggles cells in the box to 
   // create an initial locked state.
   //================================================================================
   void shuffle()
   {
      for (uint32_t t = rng() % 1000; t > 0; t--)
         toggle(rng() % ySize, rng() % xSize);
   }

public:

   //================================================================================
   // Constructor: SecureBox
   // Description: Initializes the secure box with a given size and 
   //              shuffles its state using a pseudo-random number generator 
   //              seeded with current time.
   //================================================================================
   SecureBox(uint32_t y, uint32_t x): ySize(y), xSize(x)
   {
      rng.seed(time(0));
      box.resize(y);
      for (auto& it : box)
         it.resize(x);
      shuffle();
   }

   //================================================================================
   // Method: toggle
   // Description: Toggles the state at position (x, y) and also all cells in the
   //              same row above and the same column to the left of it.
   //================================================================================
   void toggle(uint32_t y, uint32_t x)
   {
      box[y][x] = !box[y][x];
      for (uint32_t i = 0; i < xSize; i++)
      {
         box[y][i] = !box[y][i];
      }
      for (uint32_t i = 0; i < ySize; i++)
      {
         box[i][x] = !box[i][x];
      }
   }

   //================================================================================
   // Method: isLocked
   // Description: Returns true if any cell 
   //              in the box is true (locked); false otherwise.
   //================================================================================
   bool isLocked()
   {
      for (uint32_t x = 0; x < xSize; x++)
         for (uint32_t y = 0; y < ySize; y++)
            if (box[y][x])
               return true;

      return false;
   }

   //================================================================================
   // Method: getState
   // Description: Returns a copy of the current state of the box.
   //================================================================================
   std::vector<std::vector<bool>> getState()
   {
      return box;
   }
};
//================================================================================
//Function:gaussGF2
//Description:This function implements the Gaussian elimination method to solve a system 
//            of linear equations A · x = b modulo 2 (in the field GF(2)).
//================================================================================
std::vector<uint32_t> gaussGF2(std::vector<std::vector<uint32_t>> A, std::vector<uint32_t> b)
{
    uint32_t n = A.size();
    for (uint32_t i = 0; i < n; i++)
    {
        A[i].push_back(b[i]);
    }
    
    uint32_t row = 0;
    for (uint32_t col = 0; col < n && row < n; col++)
    {
        uint32_t pivot = row;
        while (pivot < n && A[pivot][col] == 0)
            pivot++;
        if (pivot == n)
            continue;
        std::swap(A[row], A[pivot]);
        for (uint32_t i = 0; i < n; i++) {
            if (i != row && A[i][col] == 1) {
                for (uint32_t j = col; j <= n; j++) {
                    A[i][j] ^= A[row][j];
                }
            }
        }
        row++;
    }
    
    for (uint32_t i = row; i < n; i++) 
    {
        if (A[i][n] == 1)
        {
            return std::vector<uint32_t>();
        }
    }
    
    std::vector<uint32_t> x(n, 0);
    for (int i = n - 1; i >= 0; i--)
    {
        int pivotCol = -1;
        for (uint32_t j = 0; j < n; j++)
        {
            if (A[i][j] == 1)
            {
                pivotCol = j;
                break;
            }
        }
        if (pivotCol == -1)
            continue;
        uint32_t sum = A[i][n];
        for (uint32_t j = pivotCol + 1; j < n; j++)
        {
            sum ^= (A[i][j] & x[j]);
        }
        x[pivotCol] = sum;
    }
    
    return x;
}
//================================================================================
// Function:Matrixb
// Description:The function takes a two-dimensional bool vector (of size rows×columns) and
//             converts it into a one-dimensional uint32_t vector,
//             where true is replaced by 1 and false by 0, using the index i * columns + j.
//================================================================================
std::vector<uint32_t> Matrixb(const std::vector<std::vector<bool>> &state, uint32_t rows, uint32_t columns)
{
   uint32_t numberOfCells = columns * rows;

   std::vector<uint32_t> b(numberOfCells, 0);
  
   for (uint32_t i = 0; i < rows; i++)
   {
      for (uint32_t j = 0; j < columns; j++)
      {
         uint32_t idx = i * columns + j;
         b[idx] = state[i][j] ? 1 : 0;
      }
   }
   return b;
}

//================================================================================
//Function: generateInfluenceMatrix
//Description: This function takes the dimensions of the box (number of rows and columns) 
//             and returns a square matrix A
//             of size (rows × columns) × (rows × columns).
//             Each row of this matrix corresponds to the toggle operation for a specific cell of the box, 
//             and each column represents a target cell.
//             The entry A[i][j] = 1 if toggling the cell with index i changes the state of the cell with index j; 
//             this happens if the two cells are in the same row or in the same column. Otherwise, the value is 0.
//================================================================================
std::vector<std::vector<uint32_t>> generateInfluenceMatrix(uint32_t rows, uint32_t columns)
{
   uint32_t numberOfCells = columns * rows;

   std::vector<std::vector<uint32_t>> A(numberOfCells, std::vector<uint32_t>(numberOfCells, 0));


   for (uint32_t i = 0; i < rows; ++i)
   {
      for (uint32_t j = 0; j < columns; ++j)
      {
         uint32_t toggleIndex = i * columns + j;

         for (uint32_t i2 = 0; i2 < rows; ++i2)
         {
            for (uint32_t j2 = 0; j2 < columns; ++j2)
            {
               uint32_t cellIndex = i2 * columns + j2;
               if (i == i2 || j == j2)
                  A[toggleIndex][cellIndex] = 1;
               else
                  A[toggleIndex][cellIndex] = 0;
            }
         }
      }
   }

   return A;
}

//================================================================================
// Function: openBox
// Description: Your task is to implement this function to unlock the SecureBox.
//              Use only the public methods of SecureBox (toggle, getState, isLocked).
//              You must determine the correct sequence of toggle operations to make
//              all values in the box 'false'. The function should return false if
//              the box is successfully unlocked, or true if any cell remains locked.  
//================================================================================
bool openBox(uint32_t y, uint32_t x)
{
   SecureBox box(y, x);


   // PLEASE, PROVIDE YOUR SOLUTION HERE
   // ...
   

   std::vector<std::vector<bool>> nowState = box.getState();
   std::vector<uint32_t> martixb = Matrixb(nowState, y, x);
   std::vector<std::vector<uint32_t>> matrixA = generateInfluenceMatrix(y, x);


   // Solve the system of equations A * sol = b modulo 2"
   std::vector<uint32_t> sol = gaussGF2(matrixA, martixb);
   if(sol.empty())
   {
      // If there is no solution, return that the box remains locked.
      return true;
   }

   // Interpret the obtained solution: for each index i, if sol[i] == 1, call toggle.
   for (uint32_t i = 0; i < y * x; i++) 
   {
      if(sol[i] == 1)
      {
         uint32_t r = i / x;    
         uint32_t c = i % x;    
         box.toggle(r, c);
      }
   }

   return box.isLocked();
}


int main(int argc, char* argv[])
{
   uint32_t y = std::atol(argv[1]);
   uint32_t x = std::atol(argv[2]);
   bool state = openBox(y, x); // y, x

   if (state)
      std::cout << "BOX: LOCKED!" << std::endl;
   else
      std::cout << "BOX: OPENED!" << std::endl;

   return state; 
}