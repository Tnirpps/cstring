# Contributing

We welcome contributions to the string library project! If you'd like to contribute, please adhere to the following coding conventions and practices to maintain code consistency and quality.

## Coding Style

  #### Use 4 spaces for indentation instead of tabs.
  #### Place opening curly braces on the same line as the function definition or control statement and place closing braces on their own line.
  ```
  // incorrect
  for (size_t i = 0; i < n; ++i)
  {
      ...
  }
  ```
  ```
  //correct
  for (size_t i = 0; i < n; ++i) {
      ...
  }
  ```
  
  #### Leave one space before the opening parenthesis of control statements (if, for, while, etc.) but no space between a function name and its calling parenthesis.

  ```
  // incorrect
  if(condition) {
      ...
  }

  // incorrect
  for(int i = 0; i < count; ++i) {
      ...
  }
  
  // incorrect
  while(condition) {
      ...
  }
  
  // incorrect
  int result = functionName (argument1, argument2);
  ```
  ```
  // correct
  if (condition) {
      ...
  }

  // correct
  for (int i = 0; i < count; ++i) {
      ...
  }

  // correct
  while (condition) {
      ...
  }

  // correct
  int result = functionName(argument1, argument2);
  ```

  #### Always declare and initialize each variable on a separate line. Avoid multiple declarations in a single statement.
  
  ```
  // incorrect
  int i = 0, j = 0;
  
  // correct
  int i = 0;
  int j = 0;
  ```

  #### Use prefix increment where it's possible (e.g., in for loops) and appropriate type for loop counter

  ```
  // incorrect
  for (size_t i = 0; i < count; i++) {
      ...
  }
  ```
  ```
  // correct
  for (size_t i = 0; i < count; ++i) {
      ...
  }
  ```
  #### Naming Conventions:
  - Use CamelCase.
  - ENUMs and #define constants should be written in UPPER_CASE with underscores (e.g., MAX_ERROR_MSG_LEN).
  - Prefix identifiers for internal functions with an underscore (e.g., _swapPtr).

## Error Handling

- Enumerate errors with the EErrorCode enum.
- Use the setError function to indicate errors, and clear errors with clearError.
- Check for possible errors diligently and handle them appropriately using the isError function.

## Comments

- Your comments should make the code easier to understand. Avoid stating the obvious, but do explain why certain decisions were made if they're not immediately clear. Good code should mostly speak for itself, so aim for clarity in your naming and structure to minimize the need for comments.

## Tests

- Include unit tests for new features and functions when possible. See [test](https://github.com/Tnirpps/cstring/blob/main/tests/main.c)
- Follow the existing testing style and patterns as shown in the testing part of the codebase.
- Ensure tests are comprehensive and cover success cases, failure cases, and edge cases where applicable.

## Thanks 

Remember to regularly pull and merge changes from the upstream master branch to keep your local repository up to date.

Thank you for taking the time to contribute!
